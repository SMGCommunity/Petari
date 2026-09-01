#include "JSystem/JAudio2/JASHeapCtrl.hpp"
#pragma onee

#include <JSystem/JAudio2/JAIAudible.hpp>

class JAUDopplerAudibleChannel;
class JAUDopplerAudibleAbsPos;

template < int SIZE, class T, class U >
class AudGenericAudible_ : public JAIAudible {
public:
    AudGenericAudible_() : JAIAudible(){};

    // todo: finish, and properly classify template parameters
    virtual ~AudGenericAudible_();

    virtual JASSoundParams* getOuterParams(int);
    virtual void calc();
};

template < int SIZE >
class AudDopplerAudible : public AudGenericAudible_< SIZE, JAUDopplerAudibleChannel, JAUDopplerAudibleAbsPos >,
                          public JASPoolAllocObject< AudDopplerAudible< SIZE > > {
public:
    AudDopplerAudible() : AudGenericAudible_< SIZE, JAUDopplerAudibleChannel, JAUDopplerAudibleAbsPos >(){};

    virtual ~AudDopplerAudible(){};

    // size 11c
};
