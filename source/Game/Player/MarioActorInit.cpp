/*#include "Game/Player/MarioActor.h"

void MarioActor::initMember() {
	_8c = 0;
	_90 = 0;
	for(int i = 0; i < 0x40; i++) {
		_94[i] = 0;
	}
	_194 = _198 = _19c = 0;
	_1a0 = _1a1 = 1;
	_1a4 = 10f;
	_1a8 = _1aa = 0;
	_1ac = 10f;
	_1b0.r = 0;
	_1b0.g = 0;
	_1b0.b = 0;
	_1b0.a = 0;
	_1b4 = 0;
	_1b5 = 0;
	_1b8 = 0;
	_1bc = nullptr;
	_1c0 = 0;
	_1c1 = 0;
	_1c2 = 0;
	_1c3 = 0;
	_1c4 = 0;
	_1c6 = 0;
	_1c8 = 10f;
	_1cc = 10f;
	_1d0 = _1d1 = 0;
	_1d4 = 0f;
	_1d8 = _1dc = nullptr;
	_1e0 = _1e1 = _1e2 = 0;
	_1e4 = 10f;
	_1e8 = 0;
	_1ec = 10f;
	_1f0 = TVec3f::zero();
	_1fc = TVec3f::zero();
	_20c = 0;
	_208 = 10f;
	_210 = _211 = 0;
	_214 = nullptr;
	_218 = _21c = _220 = _224 = _228 = _22c = 0;
	_230 = _234 = _238 = _23c = nullptr;
	// sinit?
	_240 = 10f;
	_244 = 0.20000000298;
	_248 = 10f;
	_24c = 10f;
	_250 = 0.20000000298f;
	_254 = 10f;
	_25c = 10f;
	_260 = 0.20000000298f;
	_264 = TVec3f::zero();
	_270 = TVec3f::zero();
	_27c = TVec3f::zero();
	_288 = TVec3f::zero();
	_294 = TVec3f::zero();
	_2a0 = TVec3f::zero();
	_2ac = TVec3f::zero();
	_2b8 = TVec3f::zero();
	_2c4 = TVec3f::zero();
	_2d0 = TVec3f::zero();
	_2dc = TVec3f::zero();
	_2e8 = TVec3f::zero();
	_2f4 = TVec3f::zero();
	_300 = TVec3f::zero();
	_30c = TVec3f::zero();
	_318 = TVec3f::zero();
	_324 = TVec3f::zero();
	_330 = _332 = _334 = _336 = _338 = 0;
	_33c = TVec3f::zero();
	_348 = TVec3f::zero();
	_354 = TVec3f::zero();
	_360 = TVec3f::zero();
	_36c = nullptr;
	_370 = 0;
	_374 = 10f;
	_378 = 0;
	_37c = _380 = _384 = _388 = _38c = _390 = _394 = _398 = 0;
	_39c = _39d = 0;
	_3a0 = _3a4 = 0;
	_3a8 = _3aa = _3ac = 0;
	_3a0 = 10f;
	_3b4 = TVec3f::zero();
	_3c0 = _3c1 = 0;
	_3c4 = TVec3f::zero();
	_3d0 = _3d2 = _3d4 = _3d6 = _3d8 = _3da = _3dc = 0;
	_3de = _3df = 0;
	_3e0 = 0;
	_3e4 = _3e5 = _3e6 = _3e7 = _3e8 = 0;
	_3ec = TMtx34f::identity();
	_420 = _424 = 0;
	for(int i = 0; i < 4; i++) {
		_428[i] = 0; // maybe an actual iter?
	}

	_468 = TVec3f::zero();
	__468 = __46c = __470 = 0;
	_474 = 0;
	_478 = 10f;
	_47c = 0;
	_480 = _481 = _482 = _483 = 0;
	_484 = TVec3f::zero();
	_494 = 0;
	_490 = 10f;
	_498 = _49c = nullptr;
	_4a0 = _4a4 = _4a8 = 0;
	_4ac = _4b0 = _4b4 = 10f;
	_4b8 = TVec3f::zero();
	_4c4 = TVec3f::zero();
	for(int i = 0; i < 0x80; i++) {
		_4c8[i] = 0;
	}
	_6d0 = 0;
	_6d4 = _6d8 = 10f;
	for(int i = 0; i < 0x40; i++) {
		_6dc[i] = 0;
	}
	_7dc = _7de = _7e0 = 0;
	_7e2 = 0;
	for(int i = 0; i < 0x40; i++) {
		_7e4[i] = 0;
	}
	for(int i = 0; i < 0x40; i++) {
		_8e4[i] = 0;
	}
	_924 = _928 = _92c = _930 = 0;
	_934 = _935 = 0;
	_938 = TVec3f::zero();
	_944 = _945 = 0;
	_946 = _948 = _94a = _94c = _94e = 0;
	_950 = _951 = 0;
	for(int i = 0; i < 8; i++) {
		_954[i] = 0;
	}
	_974 = 0;
	_978 = TVec3f::zero();
	_988 = 0;
	_984 = 10f;
	_989 = 0;
	_994 = 0;
	_998 = _99c = _9a0 = _9a4 = 0;
	_9a8 = _9ac _9b0 = 10f;
	_9b4 = 0;
	_9b8 = _9bc = _9c0 = _9c4 = _9c8 = 0;
	_9cc = _9d0 = 10f;
	_9d4 = 0;
	_9d8 = TVec3f::zero();
	_9e4 = _9e8 = _9ec = 0;
	_9f0 = _9f1 = 0;
	_9f2 = 0;
	_9f4 = TVec3f::zero();
	_a00 = _a04 = 0;
	_a08 = _a09 = _a0a = _a0b = _a0c = 0;
	_a10 = _a14 = 0;
	_a18 = TVec3f::zero();
	_a24 = 0;
	_a25 = 0;
	for(int i = 0; i < 6; i++) {
		_a28[i] = 0;
	}
	_a40 = _a44 = _a48 = _a4c = _a50 = _a54 = 0;
	_a58 = _a59 = _a5a = _a5b = 0;
	_a5c = 0;
	_a60 = _a61 = 0;
	_a64 = 0;
	_a68 = 10f;
	_a6c = 0;
	_a6e = 0;
	for(int i = 0; i < 8; i++) {
		_a70[i] = 0;
		_a90[i] = 0;
	}
	_ab0 = TMtx34f::identity();
	_ae0 = TMtx34f::identity();
	_b10 = _b12 = _b14 = 0;
	_b18 = TVec3f::zero();
	_b24 = _b28 = _b2c = _b30 = _b34 = _b38 = _b3c = _b40 = 10f;
	_b44 = 0;
	_b48 = nullptr;
	_b4c = 0;
	_b50 = 0;
	for(int i = 0; i < 3; i++) {
		_b54[i] = 0;
	}
	_b60 = 0;
	_b64 = 0;
	_b68 = 0;
	_b6a = 0;
	_b6c = 0;
	_b70 = 0;
	_b72 = 0;
	_b74 = 0;
	_b78 = _b7c = _b80 = _b84 = 0;
	_b88 = 0;
	_b8c = 0;
	_b90 = _b91 = _b92 = 0;
	_b94 = 0;
	_b96 = 0;
	_b9c = _b9e = 0;
	_ba0 = _ba4 = _ba8 = 0;
	_bac = TVec3f::zero();
	_bb8 = TVec3f::zero();
	_bc4 = 0;
	_bc8 = TMtx34f::identity();
	_bf8 = TMtx34f::identity();
	
}*/
