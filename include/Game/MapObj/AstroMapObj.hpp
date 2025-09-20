#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/MapObj/MapObjActor.hpp"
#include "Game/Screen/GalaxyNamePlate.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/StringUtil.hpp"
#include "revolution/types.h"

class AstroMapObj : public MapObjActor {

    public:
        AstroMapObj(const char*);
        ~AstroMapObj();
        virtual void init(const JMapInfoIter&);
        virtual void control();
        virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

        void exeOpen();
        void exeDead();
        void exeWait();
        void exeAlive();
        void exeRevival();

        void setStateAlive();
        void setStateDead();
        bool isPlayMachineSE() const;
        bool tryStartAllAnimAndEffect(const char*);
        bool isEndRevival() const;
        void setStateDoorOpenOrClose(); // unknown return

        inline bool checkStrings(const char* arg1) {
            return MR::isEqualString(arg1, "AstroDomeEntrance") || MR::isEqualString(arg1, "AstroLibrary");
        }

        // Unused?
        virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&);
        virtual void initCaseUseSwitchB(const MapObjActorInitInfo&);
        virtual void initCaseNoUseSwitchA(const MapObjActorInitInfo&);
        virtual void initCaseUseSwitchA(const MapObjActorInitInfo&);


        JMapInfo* _C4;
        GalaxyNamePlate* _C8;
        const char* _CC; // Uhh...
        CollisionParts* _D0;
        CollisionParts* _D4;
        s32 _D8;
};

class AstroSimpleObj : public MapObjActor {
    public:
        AstroSimpleObj(const char*);
        ~AstroSimpleObj();


        s32 _C4;
        s32 _C8;
};
