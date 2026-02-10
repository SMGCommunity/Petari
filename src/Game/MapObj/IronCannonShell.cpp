#include "Game/MapObj/IronCannonShell.hpp"

IronCannonLauncher::IronCannonLauncher(const char* pName) : IronCannonLauncherPoint(pName) {}
IronCannonLauncher::~IronCannonLauncher() {}



IronCannonLauncherPoint::IronCannonLauncherPoint(const char* pName) : LiveActor(pName) {}
IronCannonLauncherPoint::~IronCannonLauncherPoint() {}
