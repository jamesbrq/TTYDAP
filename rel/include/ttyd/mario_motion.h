#pragma once

namespace ttyd::mario_motion
{
    enum class MarioMotion : uint16_t
    {
        kStay = 0,
        kWalk,
        kDash,
        kJump,
        kJumpNPC,
        kJumpSw,
        kJumpStand,
        kJump2,
        kJump3,
        kJumpSmall,
        kFall,
        kFall2,
        kUpstairs,
        kLand,
        kTalk,
        kGetItem,
        kHip,
        kHip2,
        kHammer,
        kHammer2,
        kJabara,
        kSlit,
        kRoll,
        kKaze,
        kPlane,
        kShip,
        kYoshi,
        kCloud,
        kVivian,
        kDokan,
        kGrasp,
        kDamage,
        kDamageToge,
        kBottomless,
        kForceReset,
        kShadow,
        kPartyUse, // Dummied out
        kKpaSwim,
        kKpaPowUp,
        kKpaPowDown,
        kDummy, // Dummied out
    };

    extern "C"
    {
        void marioChgMoveMotion();
        void marioChgStayMotion();
        void marioChgTalkMotion();
        void marioChgGetItemMotion();
        void marioChgShipMotion();
        void marioChgRollMotion();
        void marioChgJumpStandMotion(float direction);
        void marioChgSmallJumpMotion();

        bool marioChkItemGetMotion();
        bool marioChkTalkable();

        // marioBoots
        // marioMotion

        void marioChgMot(MarioMotion motion);
        void marioChgMotSub(int motionId, bool unk);
        void marioChgMot2(int motionId);

        void marioClearJumpPara();
        void marioSetJumpPara();
        void marioSetFallPara();
        float marioGetFallSpd();
        float marioMakeJumpPara();

        // marioJump
        // marioFall
        // marioLandOn

        bool marioChkJump();
        bool marioChkTransform();
        bool marioChkItemMotion();
        // L_marioChkRub

        int marioRollChgChk();

        void marioChgMotJump2();
    }
} // namespace ttyd::mario_motion
