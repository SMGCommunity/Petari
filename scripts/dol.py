import hashlib
import struct
import util
import sys

def getCodeFromDOL(sym, functionSize):
    with open("baserom.dol", "rb") as f:
        data = f.read()
        # verify that the dol is legit
        hash = hashlib.sha256(data).hexdigest().upper()
        assert hash == "8B7F28D193170F998F92E02EA638107822FB72073691D0893EB18857BE0C6FCF"

        # grab .text offset and start offset
        txtOffset, = struct.unpack_from(">I", data, 4)
        startOffset, = struct.unpack_from(">I", data, 0x4C)
        addr = util.findAddrFromSym(sym)

        assert addr != 0

        addrOffset = int(addr, 16) - startOffset
        funcSize = util.findAddrSizeFromSym(sym)
        
        # first check we can do for matching is to check the function size
        # we can immediately terminate if this is not the same
        if int(funcSize,16) != functionSize:
            print("ERROR: Function size mismatch!")
            sys.exit(1)

        return data[txtOffset + addrOffset:txtOffset + addrOffset + functionSize]