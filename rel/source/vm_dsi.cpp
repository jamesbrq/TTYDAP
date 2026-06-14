#include "vm_ttyd.h"
#include "gc/OSContext.h"

#include <cstdint>

// Combined DSI + ISI demand-paging net.
//
// Registered via __OSSetExceptionHandler on both kDsiException (2) and
// kIsiException (3) by VM_EnableDemandPaging(). The SDK low-level dispatcher
// saves the interrupted state into an OSContext and calls this handler with:
//
//   r3 = exception number, r4 = OSContext*  (translation on, interrupts off,
//   running on the interrupted stack).
//
// DSI: a data access to a non-resident window page. Fault address is DAR.
// ISI: an instruction fetch from a non-resident window code page. The fault
//      address is the faulting PC, which is in SRR0 (DAR/DSISR are not valid
//      for an ISI), so we read ctx->srr0 instead.
//
// If the fault is a window page-not-found we map the page and resume via
// OSLoadContext (restores the context and rfi's, so the faulting access
// re-executes against the freshly mapped page). Anything else chains to the
// handler we replaced so real bugs still surface as before.

namespace
{
    // Net diagnostics, read at 0x80003B60:
    //   ND0 total   ND1 handled   ND2 chained   ND3 last addr   ND4 last DSISR
    //   ND5 last handled addr
    volatile uint32_t *const ND = reinterpret_cast<volatile uint32_t *>(0x80003B60);
} // namespace

extern "C"
{
    void OSLoadContext(OSContext *ctx);  // 0x802971b4 (does not return)
    void OSReport(const char *fmt, ...); // 0x80297814 (-> Dolphin OSREPORT log)

    int vm_fault_c(uint32_t dsisr, uint32_t dar); // vm_ttyd.cpp (DSI)
    int vm_ifault_c(uint32_t addr);               // vm_ttyd.cpp (ISI)
    extern void *vm_saved_dsi;                    // prior DSI handler
    extern void *vm_saved_isi;                    // prior ISI handler

    void vm_fault_dispatch_c(uint32_t exc, OSContext *ctx)
    {
        // Keep the page-in atomic vs interrupts. A reschedule here would save
        // the running state over the current OSContext (== ctx) and undo the
        // full-register save done in the asm entry stub. Clear EE and never
        // restore it; OSLoadContext rfi's from ctx->srr1 and sets the final MSR.
        {
            uint32_t msr;
            asm volatile("mfmsr %0" : "=r"(msr));
            asm volatile("mtmsr %0" ::"r"(msr & ~0x8000u));
            asm volatile("isync" ::: "memory");
        }

        ND[0] = ND[0] + 1;

        if (exc == 3) // ISI: fault PC is in SRR0
        {
            uint32_t addr = ctx->srr0;
            ND[3] = addr;
            ND[4] = 0;

            if (vm_ifault_c(addr))
            {
                ND[1] = ND[1] + 1;
                ND[5] = addr;
                OSLoadContext(ctx); // mapped: resume the faulting fetch (no log: hot path)
            }

            ND[2] = ND[2] + 1;
            OSReport("VMNET IUNHANDLED exc=%d addr=%08x lr=%08x\n", exc, addr, ctx->lr);
            OSReport("  r0=%08x sp=%08x r2=%08x r3=%08x r4=%08x r5=%08x\n",
                     ctx->gpr[0],
                     ctx->gpr[1],
                     ctx->gpr[2],
                     ctx->gpr[3],
                     ctx->gpr[4],
                     ctx->gpr[5]);
            OSReport("  r6=%08x r7=%08x r8=%08x r9=%08x r10=%08x r11=%08x\n",
                     ctx->gpr[6],
                     ctx->gpr[7],
                     ctx->gpr[8],
                     ctx->gpr[9],
                     ctx->gpr[10],
                     ctx->gpr[11]);
            OSReport("  r12=%08x r13=%08x r14=%08x r15=%08x r16=%08x r17=%08x\n",
                     ctx->gpr[12],
                     ctx->gpr[13],
                     ctx->gpr[14],
                     ctx->gpr[15],
                     ctx->gpr[16],
                     ctx->gpr[17]);
            OSReport("  r24=%08x r25=%08x r26=%08x r27=%08x r28=%08x\n",
                     ctx->gpr[24],
                     ctx->gpr[25],
                     ctx->gpr[26],
                     ctx->gpr[27],
                     ctx->gpr[28]);
            OSReport("  r29=%08x r30=%08x r31=%08x srr0=%08x\n", ctx->gpr[29], ctx->gpr[30], ctx->gpr[31], ctx->srr0);

            if (vm_saved_isi)
                reinterpret_cast<void (*)(uint32_t, OSContext *)>(vm_saved_isi)(exc, ctx);

            OSLoadContext(ctx);
        }

        // DSI path
        uint32_t dsisr, dar;
        asm volatile("mfspr %0, 18" : "=r"(dsisr)); // DSISR
        asm volatile("mfspr %0, 19" : "=r"(dar));   // DAR

        ND[3] = dar;
        ND[4] = dsisr;

        if (vm_fault_c(dsisr, dar))
        {
            ND[1] = ND[1] + 1;
            ND[5] = dar;
            OSLoadContext(ctx); // mapped: resume the faulting instruction (no log: spammy)
        }

        ND[2] = ND[2] + 1;
        OSReport("VMNET UNHANDLED exc=%d dar=%08x dsisr=%08x srr0=%08x lr=%08x r3=%08x r4=%08x\n",
                 exc,
                 dar,
                 dsisr,
                 ctx->srr0,
                 ctx->lr,
                 ctx->gpr[3],
                 ctx->gpr[4]);

        if (vm_saved_dsi)
            reinterpret_cast<void (*)(uint32_t, OSContext *)>(vm_saved_dsi)(exc, ctx);

        OSLoadContext(ctx);
    }
}

// Registered exception entry (DSI + ISI), installed via __OSSetExceptionHandler.
//
// The SDK vector (OSExceptionVector) saves only r3/r4/r5, cr/lr/ctr/xer,
// srr0/srr1, and sets the OS_CONTEXT_STATE_EXC (0x2) bit. It leaves
// r0,r1,r2,r6-r31 and GQR1-7 for the handler to save -- OSDefaultException-
// Handler does this as its first act. A plain C handler does NOT (its prologue
// clobbers those regs before any C runs), so OSLoadContext later restores stale
// garbage for r6-r31 and the stack pointer, resuming the faulting instruction
// with wild registers -> branch through a bogus ptr.
//
// Defined as file-scope asm (not a naked function: the naked attribute is not
// reliably honored on PowerPC GCC, and an unwanted prologue would clobber the
// faulting r0/r1 before the save). On entry r3 = exception number, r4 = ctx.
// We replicate the default handler's full save into the context, then tail-call
// the C dispatcher on the faulting stack. Bare register numbers (GNU as PPC).
asm(
    ".globl vm_dsi_stub          \n"
    "vm_dsi_stub:                 \n"
    "   stw   0, 0(4)            \n"
    "   stw   1, 4(4)            \n"
    "   stw   2, 8(4)            \n"
    "   stmw  6, 0x18(4)         \n"
    "   mfspr 0, 913             \n"
    "   stw   0, 0x1a8(4)        \n"
    "   mfspr 0, 914             \n"
    "   stw   0, 0x1ac(4)        \n"
    "   mfspr 0, 915             \n"
    "   stw   0, 0x1b0(4)        \n"
    "   mfspr 0, 916             \n"
    "   stw   0, 0x1b4(4)        \n"
    "   mfspr 0, 917             \n"
    "   stw   0, 0x1b8(4)        \n"
    "   mfspr 0, 918             \n"
    "   stw   0, 0x1bc(4)        \n"
    "   mfspr 0, 919             \n"
    "   stw   0, 0x1c0(4)        \n"
    "   b     vm_fault_dispatch_c\n");