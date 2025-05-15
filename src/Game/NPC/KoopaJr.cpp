#include "Game/NPC/KoopaJr.hpp"

namespace NrvKoopaJr {
    NEW_NERVE(HostTypeWait, KoopaJr, Wait);
    NEW_NERVE(HostTypeReaction, KoopaJr, Reaction);
    NEW_NERVE(HostTypeReactionEnd, KoopaJr, ReactionEnd);
    NEW_NERVE(HostTypeShipBattleAppear, KoopaJr, ShipBattleAppear);
    NEW_NERVE(HostTypeShipBattlePowerUp, KoopaJr, ShipBattlePowerUp);
    NEW_NERVE(HostTypeShipBattleEscape, KoopaJr, ShipBattleEscape);
    NEW_NERVE(HostTypeShipBattleShipDamage, KoopaJr, ShipBattleShipDamage);
    NEW_NERVE(HostTypeShipBattleDemoTalkStart, KoopaJr, ShipBattleDemoTalkStart);
    NEW_NERVE(HostTypeShipBattleDemoTalkWait, KoopaJr, ShipBattleDemoTalkWait);
};
