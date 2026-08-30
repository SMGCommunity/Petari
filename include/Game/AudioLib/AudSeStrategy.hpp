#pragma once

#include <JSystem/JAudio2/JAISound.hpp>
#include <JSystem/JAudio2/JASHeapCtrl.hpp>

class JAISe;
class JASSoundParams;

class AudSeStrategy : public JAISoundStrategy< JAISe >, public JASPoolAllocObject< AudSeStrategy > {
public:
    AudSeStrategy(){};

    virtual void calc(JAISe*);
    virtual void mix(JAISe*, JASSoundParams*);
};

class AudSeStrategyMgr : public JAISoundStrategyMgr< JAISe > {
public:
    AudSeStrategyMgr(){};

    virtual JAISoundStrategy< JAISe >* newStrategy(JAISoundID);
    virtual bool deleteStrategy(JAISoundStrategy< JAISe >*);
};
