/*#include "Game/Player/Mario.h"
#include <cstring>

struct SoundList {
	const char *name;
	const char* _4;

	u32 _8;
	u32 _C;
	
	u32 _10;
	const char* _14;
	SoundList(const char *name, const char* offset) : name(name), _4(offset), _8(0), _C(0), _10(0), _14(0) {}
};

static SoundList soundlist[0x3] = {
	SoundList("声小ジャンプ", (const char*)0x10000),
	SoundList("声中ジャンプ", (const char*)0x10001),
	SoundList("声大ジャンプ", (const char*)0x10002),
};
static const char* soundswaplist[4] = {"", NULL, NULL, NULL};

void Mario::initSound() {
	unsigned long length = initSoundTable(soundlist, 0);
	_96C = new HashSortTable(length);
	for(unsigned long i = 0; i < length; i++) {
		_96C -> add(soundlist[i].name, i, false);
	}
	_96C -> sort();
	_970 = 0;
}

unsigned long Mario::initSoundTable(SoundList *list, unsigned long globalTablePosition) {
	const char** currswap = soundswaplist + globalTablePosition;
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
				if(soundswaplist[j][0] == '\0') break;
				if(strcmp(pos -> name, soundswaplist[j]) == 0) {
					if(currswap[j] != 0) pos -> _14 = currswap[j];
					break;
				}
				j += 4;
			}
		}
		i++;
	}
	return i;
}*/
