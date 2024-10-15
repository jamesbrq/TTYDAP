#include <cstdint>
#include <cstdio>
#include <ttyd/itemdrv.h>
#include <ttyd/item_data.h>
#include <ttyd/common_types.h>
#include <gc/OSLink.h>
#include <OWR_STATE.h>
#include <ttyd/swdrv.h>
#include <ttyd/mario_party.h>
#include <ttyd/party.h>
#include <ttyd/fontmgr.h>
#include <ttyd/seqdrv.h>
#include <ttyd/event.h>
#include <ttyd/string.h>
#include <AP/gor_00.h>
#include <AP/gor_01.h>
#include <AP/gor_02.h>

#include "common.h"
#include "OWR.h"
#include "patch.h"

using ::ttyd::common::ItemData;
using gc::OSLink::OSModuleInfo;
using ::ttyd::common::ShopItemData;
using namespace ::ttyd::common;
using ::ttyd::seqdrv::SeqIndex;

namespace ItemId = ::ttyd::common::ItemId;
namespace ModuleId = ::ttyd::common::ModuleId;

const uint32_t kShopOffsets[] = {
	0x36c5c, 0x57b70,  0xdd98,  0x3c14, 0x40fa8,
	0x1aed4, 0x15be8, 0x21c18, 0x23770, 0x11174
};

char* strcpy_String(char* dest, const char* src)
{
	return ttyd::string::strcpy(dest, src);
}

uint16_t GSWF_ARR[] =
{
	//Any of these being enabled will disable them
	//Shop Tutorial
	0,

	//Email Explanations
	37,38,

	//Save block and heart block tutorials
	233,234,

	//Zess T. blocking gate enable
	1187,

	//Contact Lens Ordered and in shop
	1189, 1191,

	//Skip Zess T. Explain Dialogue
	1197,

	//Attempt to enter ch.4 pipe early
	1341
};
uint16_t GSWF_ARR_SIZE = sizeof(GSWF_ARR) / sizeof(GSWF_ARR[0]);

namespace mod::owr
{
	ItemData* item_db = common::kItemDataArr;
	OWR* gSelf = nullptr;

	void* (*g_itemEntry_trampoline)(const char*, uint32_t, uint32_t, int32_t, void*, float, float, float) = nullptr;
	bool (*g_OSLink_trampoline)(OSModuleInfo*, void*) = nullptr;
	void (*g_stg0_00_init_trampoline)() = nullptr;

	OWR::OWR() {}

	void OWR::SequenceInit()
	{
		uint32_t SequencePosition = ttyd::swdrv::swByteGet(0);
		if (SequencePosition != 0)
			return;

		ttyd::swdrv::swByteSet(0, 3);
		ttyd::swdrv::swByteSet(1700, 7);
		ttyd::swdrv::swByteSet(1705, 1);

		uint8_t goombella = static_cast<uint8_t>(ttyd::party::PartyMembers::Goombella);

		ttyd::mario_party::partyJoin(goombella);

		ttyd::mario_party::marioPartyHello(goombella);

		uint16_t size = GSWF_ARR_SIZE;
		for (int i = 0; i < size; i++)
		{
			ttyd::swdrv::swSet(GSWF_ARR[i]);
		}
		ttyd::swdrv::swSet(1215);
		ttyd::swdrv::swSet(1216);
	}

	void OWR::LZTest()
	{
		uint32_t aaa_00_Address = 0x802EDE78;
		SeqIndex NextSeq = ttyd::seqdrv::seqGetNextSeq();
		SeqIndex Load = ttyd::seqdrv::SeqIndex::kLoad;

		if (NextSeq == Load)
		{
			strcpy_String(reinterpret_cast<char*>(aaa_00_Address), "gor_01");
		}
	}

	 void OWR::Init()
	 {
		gSelf = this;

		/* g_itemEntry_trampoline = patch::HookFunction(
			ttyd::itemdrv::itemEntry, [](const char* name, uint32_t id, uint32_t mode, int32_t collection_expr, void* script, float x, float y, float z) 
			{
				return g_itemEntry_trampoline(name, ItemId::ULTRA_HAMMER, mode, collection_expr, script, x, y, z);
			}); */

		g_OSLink_trampoline = patch::hookFunction(
			gc::OSLink::OSLink, [](OSModuleInfo* new_module, void* bss) {
				bool result = g_OSLink_trampoline(new_module, bss);
				if (new_module != nullptr && result) {
					gSelf->OnModuleLoaded(new_module);
				}
				return result;
			});

		g_stg0_00_init_trampoline = patch::hookFunction(
			ttyd::event::stg0_00_init, []() {
				gSelf->NewFileInit();
				g_stg0_00_init_trampoline();
			});

		uint32_t* kSkipUHCutsceneOpcode = reinterpret_cast<uint32_t*>(0x800abcd8);
		const uint32_t skip_cutscene_opcode = 0x48000030;     // b 0x0030
		mod::patch::writePatch(
			kSkipUHCutsceneOpcode, &skip_cutscene_opcode, sizeof(uint32_t));

		uint16_t size = GSWF_ARR_SIZE;
		for (int i = 0; i < size; i++)
		{
			ttyd::swdrv::swSet(GSWF_ARR[i]);
		}
	 }

	 void OWR::Update()
	 {
		 SequenceInit();
		 LZTest();
	 }

	 void OWR::Draw()
	 {
		 //char* buffer = new char[256];
		 //sprintf(buffer, "seq: %lu", ttyd::swdrv::swByteGet(0));
		 //DrawString(buffer, -232, -120,-1U);
	 }

	 void OWR::NewFileInit()
	 {

	 }

	void OWR::OnModuleLoaded(OSModuleInfo* module_info)
	{
		if (module_info == nullptr) return;
		uintptr_t module_ptr = reinterpret_cast<uintptr_t>(module_info);
		if (module_info->id != ModuleId::GOR) return;
		ApplyGor00Patches(module_info);
		ApplyGor01Patches(module_info);
		ApplyGor02Patches(module_info);
		ShopItemData* item_data = reinterpret_cast<ShopItemData*>(module_ptr + kShopOffsets[0]);
		for (int32_t copy = 0; copy < 7; ++copy) {
			// Skip first item slot on additional copies.
			if (copy > 0) ++item_data;
			for (int32_t i = copy > 0 ? 1 : 0; i < 6; ++i) {
				ItemPrice item_price = ItemPrice();
				if (i == 2)
					item_price.item = ItemId::CARD_KEY_001D;
				else
					item_price.item = ItemId::ULTRA_BOOTS;
				item_price.price = 1;
				item_data->item_id = item_price.item;
				item_data->buy_price = item_price.price;
				++item_data;
			}
		}
	}

	void OWR::DrawString(const char* data, int32_t x, int32_t y, uint32_t color, float scale) {
		if (!data) return;
		ttyd::fontmgr::FontDrawStart();
		ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t*>(&color));
		ttyd::fontmgr::FontDrawEdge();
		ttyd::fontmgr::FontDrawScale(scale);
		ttyd::fontmgr::FontDrawString(x, y, data);
	}

}