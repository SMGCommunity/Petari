#include "Game/Player/Mario.hpp"
#include <cstring>

struct SoundList {
	const char *name;
	u32 _4;

	u32 _8;
	u32 _C;
	
	u32 _10;
	u32 _14;
	//SoundList(const char *name, const char* offset) : name(name), _4(offset), _8(0), _C(0), _10(0), _14(0) {}
};

static SoundList soundlist[0x3];/* = {
	SoundList("声小ジャンプ", (const char*)0x10000),
	SoundList("声中ジャンプ", (const char*)0x10001),
	SoundList("声大ジャンプ", (const char*)0x10002),
};*/

struct SoundSwapList {
    const char *name;

    u32 offset1;
    u32 offset2;
    u32 offset3;
};

static SoundSwapList soundswaplist[1] = {
    {"", 0, 0, 0}
};

void Mario::initSound() {
	unsigned long length = initSoundTable(soundlist, 0);
	_96C = new HashSortTable(length);
	for(unsigned long i = 0; i < length; i++) {
		_96C -> add(soundlist[i].name, i, false);
	}
	_96C -> sort();
	_970 = 0;
}

struct OffsetView {
    u32 offset;
    u8 padding[12];
};

u32 Mario::initSoundTable(SoundList *list, u32 globalTablePosition) {
	const OffsetView *currswap = (const OffsetView *)((const char *)soundswaplist + globalTablePosition);
	SoundList *pos = list;
	int i = 0;
	while(true) {
		pos = list + i;
		if(pos -> name[0] == '\0') break;
		pos -> _10 = 0;
		pos -> _14 = pos -> _4;
		if(globalTablePosition > 0) {
			int j = 0;
			while(true) {
				if(soundswaplist[j].name[0] == '\0') break;
				if(strcmp(pos -> name, soundswaplist[j].name) == 0) {
					if(currswap[j].offset != 0) pos -> _14 = currswap[j].offset;
					break;
				}
				j++;
			}
		}
		i++;
	}
	return i;
}
