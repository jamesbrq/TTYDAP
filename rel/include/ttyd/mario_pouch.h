#pragma once

#include <cstdint>

namespace ttyd::mario_pouch
{
    extern "C"
    {
        enum class PouchHammerLevel : int32_t
        {
            HAMMER_LEVEL_NONE = 0, // The player has no Hammer
            HAMMER_LEVEL_NORMAL,   // The player has the Normal Hammer
            HAMMER_LEVEL_SUPER,    // The player has the Super Hammer
            HAMMER_LEVEL_ULTRA,    // The player has the Ultra Hammer
        };

        enum class PouchJumpLevel : int32_t
        {
            JUMP_LEVEL_NONE = 0, // The player has no Boots
            JUMP_LEVEL_NORMAL,   // The player has the Normal Boots
            JUMP_LEVEL_SUPER,    // The player has the Super Boots
            JUMP_LEVEL_ULTRA,    // The player has the Ultra Boots
        };

        struct PouchPartyData
        {
            uint16_t flags;
            int16_t max_hp;
            int16_t base_max_hp;
            int16_t current_hp;
            int16_t hp_level;
            int16_t attack_level;
            int16_t tech_level;
        };

        struct PouchData
        {
            PouchPartyData party_data[8];
            int16_t current_hp;
            int16_t max_hp;
            int16_t current_fp;
            int16_t max_fp;
            int16_t coins;
            int16_t current_sp;
            int16_t max_sp;
            int8_t unk_07e[6];
            float audience_level;
            int16_t rank;
            int16_t level;
            uint16_t star_powers_obtained; // Bitfield
            int16_t base_max_hp;
            int16_t base_max_fp;
            int16_t unallocated_bp;
            int16_t total_bp;
            int16_t star_points;
            int8_t jump_level;
            int8_t hammer_level;
            int16_t star_pieces;
            int16_t shine_sprites;
            int16_t power_bounce_record;
            int16_t key_items[121];
            int16_t items[20];
            int16_t stored_items[32];
            int16_t badges[200];
            int16_t equipped_badges[200];
            int8_t email_ids[100];
            int8_t pad_57e[2];
            uint32_t email_received[4]; // Bitfield
            uint32_t email_read[4];     // Bitfield
            int8_t unk_5a0[0x18];
            int8_t merlee_curse_uses_remaining;
            int8_t turns_until_merlee_activation;
            int8_t next_merlee_curse_type;
            int8_t super_bowser_coins;
            int32_t super_bowser_score;
            char yoshi_name[16];
            int8_t unk_5d0[4];
        };

        static_assert(sizeof(PouchPartyData) == 0xe);
        static_assert(sizeof(PouchData) == 0x5d4);

        const char *pouchGetYoshiName();
        void pouchSetYoshiName(const char *name);
        void pouchSetPartyColor(int32_t party_member, int32_t color); // 4 = Yoshi
        int32_t pouchGetPartyColor(int32_t party_member);
        // pouchCheckMail
        // pouchOpenMail
        // pouchReceiveMail
        // pouchReceiveMailCount
        void pouchGetStarStone(int32_t stone);
        // pouchAddKpaScore
        // pouchGetKpaScore
        // pouchAddKpaCoin
        // pouchGetKpaCoin
        // pouchMajinaiInit
        // pouchArriveBadge
        // N_pouchUnEquipBadgeID
        int32_t pouchEquipBadgeID(int16_t badge_id);
        // pouchEquipCheckBadgeIndex
        int32_t pouchEquipCheckBadge(int16_t badge_id);
        // pouchUnEquipBadgeIndex
        int32_t pouchEquipBadgeIndex(int32_t index);
        // pouchGetStarPoint
        void pouchRevisePartyParam();
        void pouchReviseMarioParam();
        // pouchRemoveKeepItem
        int32_t pouchAddKeepItem(int32_t item_type);
        // pouchGetPartyAttackLv
        PouchHammerLevel pouchGetHammerLv();
        PouchJumpLevel pouchGetJumpLv();
        // pouchSetAudienceNum
        // pouchGetAudienceNum
        int32_t pouchGetMaxAP();
        // pouchSetAP
        void pouchAddAP(int32_t star_power);
        int32_t pouchGetAP();
        void pouchSetMaxFP(int16_t max_fp);
        void pouchSetFP(int16_t fp);
        int16_t pouchGetMaxFP();
        int16_t pouchGetFP();
        void pouchSetPartyHP(int32_t partner_idx, int16_t hp);
        int16_t pouchGetPartyHP(int32_t partner_idx);
        void pouchSetMaxHP(int16_t max_hp);
        void pouchSetHP(int16_t hp);
        int16_t pouchGetMaxHP();
        int16_t pouchGetHP();
        // pouchAddHP
        int16_t pouchAddStarPiece(int16_t star_pieces);
        // pouchGetStarPiece
        // pouchSetSuperCoin
        // pouchGetSuperCoin
        int16_t pouchSetCoin(int16_t coins);
        int16_t pouchAddCoin(int16_t coins);
        // pouchGetCoin
        void pouchSortItem(int32_t sort_type);
        // comp_kind_r
        // comp_aiueo_r
        int32_t comp_kind(int16_t *lhs, int16_t *rhs);
        // comp_aiueo
        // unk_800d48b0
        int32_t pouchRemoveItemIndex(int32_t item_type, int32_t index);
        int32_t pouchRemoveItem(int32_t item_type);
        int32_t pouchCheckItem(int32_t item_type);
        uint32_t pouchGetItem(int32_t item_type);
        // pouchGetEmptyKeepItemCnt
        // pouchGetEmptyHaveItemCnt
        // pouchGetEquipBadgeCnt
        int32_t pouchGetHaveBadgeCnt();
        // pouchGetKeepItemCnt
        int32_t pouchGetHaveItemCnt();
        // L_pouchEquipBadge
        // pouchHaveBadge
        // pouchKeepItem
        int32_t pouchHaveItem(int32_t index);
        // pouchKeyItem
        void pouchInit();
        PouchData *pouchGetPtr();

        // Rank 0, 1, 2, max HP per party member.
        extern int16_t _party_max_hp_table[32];
    }
} // namespace ttyd::mario_pouch
