#pragma once

class JAISoundID;

class AudSoundNameConverter {
public:
    JAISoundID getSoundID(const char*) const;
    JAISoundID getSoundID(const char*, u32) const;
};
