#include "JSystem/JKernel/JKRFileLoader.h"

namespace {
	JKRFileLoader *gFileLoader;
}

JKRFileLoader::JKRFileLoader() : JKRDisposer(), _18(this) {
	_28 = 0;
	_2C = 0;
	_34 = 0;
}

JKRFileLoader::~JKRFileLoader() {
	if (gFileLoader == this) {
		gFileLoader = NULL;
	}
}

void JKRFileLoader::unmount() {
	if (_34 == 0) {
		return;
	}

	if (--_34 != 0) {
		return;
	}

	delete this;
}
