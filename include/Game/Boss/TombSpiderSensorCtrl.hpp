#pragma once

class TombSpider;
class HitSensor;

class TombSpiderSensorCtrl {
public:
    TombSpiderSensorCtrl(TombSpider*);

    bool update(HitSensor*);
    bool updateMainParts(HitSensor*);
    bool updateGland(HitSensor*);
    bool updateVitalSpot(HitSensor*);
    bool updateAttackSensor(HitSensor*);
    void initSensor();

    /* 0x00 */ TombSpider* mParent;
};
