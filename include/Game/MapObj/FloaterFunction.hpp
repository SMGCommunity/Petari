#pragma once

class FloaterFunction {
public:
    static const char* getSeGroundOn(const char *);
    static const char* getSeMoveDown(const char *);
    static const char* getSeMoveUp(const char *);
    static bool isExistImpactOnRide(const char *);
    static bool isExistSpeedUpLanding(const char *);
};