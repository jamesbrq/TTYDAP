#pragma once

#include "gc/os.h"
#include "ttyd/npcdrv.h"

#include <cstdint>

#define ERROR_TEXT_DEFAULT_POS_Y 120.f

class NpcNameToPtrErrorInfo
{
   public:
    NpcNameToPtrErrorInfo() {}

    ~NpcNameToPtrErrorInfo() {}

    uint32_t getCounter() const { return this->counter; }

    void incrementCounter() { this->counter++; }

    void resetCounter() { this->counter = 0; }

    uint32_t getTimer() const { return this->timer; }

    void setTimer(uint32_t time) { this->timer = static_cast<uint16_t>(time); }

    void drawErrorMessage();

   private:
    uint16_t counter;
    uint16_t timer;
};

class AnimPoseMainErrorInfo
{
   public:
    AnimPoseMainErrorInfo() {}

    ~AnimPoseMainErrorInfo() {}

    uint32_t getCounter() const { return this->counter; }

    void incrementCounter() { this->counter++; }

    void resetCounter() { this->counter = 0; }

    uint32_t getTimer() const { return this->timer; }

    void setTimer(uint32_t time) { this->timer = static_cast<uint16_t>(time); }

    void drawErrorMessage();

   private:
    uint16_t counter;
    uint16_t timer;
};

class HeapCorruptionInfo
{
   public:
    HeapCorruptionInfo()
    {
        int32_t totalLines = gc::os::OSAlloc_NumHeaps + 2;
#ifdef TTYD_JP
        totalLines += 1;
#endif
        this->bufferSize = static_cast<uint16_t>((totalLines * 2) * 64);
    }

    void freeBuffer()
    {
        char *buf = this->buffer;
        if (buf)
        {
            delete[] buf;
            this->buffer = nullptr;
        }
    }

    ~HeapCorruptionInfo() { this->freeBuffer(); }

    char *getBufferPtr() { return this->buffer; }

    bool shouldDrawBuffer() const { return this->buffer != nullptr; }

    char *initBuffer()
    {
        char *buf = this->buffer;
        if (!buf)
        {
            buf = new char[this->bufferSize];
            this->buffer = buf;
        }

        return buf;
    }

    char *initBufferEntry(uint32_t index)
    {
        char *buf = this->initBuffer();
        return &buf[index];
    }

    int32_t getBufferIndex() const { return this->heapIndex; }

    void setBufferIndex(int32_t index) { this->heapIndex = static_cast<int16_t>(index); }

    bool verifyBufferIndex() const
    {
        const int32_t index = this->heapIndex;
        return (index >= 0) && (index < (64 * this->bufferSize));
    }

    uint32_t getBufferSize() const { return this->bufferSize; }

    void drawBuffer();

   private:
    char *buffer;
    int16_t heapIndex;
    uint16_t bufferSize;
};

extern NpcEntry *(*g_npcNameToPtr_trampoline)(const char *name);
extern void (*g_animPoseMain_trampoline)(int32_t poseId);

void checkHeaps();
void drawErrorMessages();

NpcEntry *checkForNpcNameToPtrError(const char *name);
void preventAnimPoseMainCrash(int32_t poseId);
