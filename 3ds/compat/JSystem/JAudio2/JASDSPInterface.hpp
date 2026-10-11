// Case-fix shim: the decomp includes JASDSPInterface.hpp but the file is named JASDspInterface.hpp.
// Case-insensitive filesystems (Windows/macOS) do not need this; Linux does.
#include "JSystem/JAudio2/JASDspInterface.hpp"
