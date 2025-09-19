#pragma once

class Koopa;

class KoopaSensorCtrl {
public:
    /// @brief Creates a new `KoopaSensorCtrl`.
    /// @param pHost The pointer to the owning Bowser actor instance.
    KoopaSensorCtrl(Koopa* pHost);

    void update(HitSensor* pSensor);
    void initSensor();

private:
    /// @brief The pointer to the owning Bowser actor instance.
    /* 0x0 */ Koopa* mHost;
};
