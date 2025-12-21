#pragma once

class AudFader
{
    public:
        AudFader();
        void set(float floatVar, long longVar);
        void update();

        float mFloatVar;
        float mFloatVar2;
};
