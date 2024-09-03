import pathlib
import hashlib
import struct
import os
import sys
from collections import OrderedDict

class FunctionLibrary:
    def __init__(self, parent):
        self.parent = parent
        self.libraries = dict()
        self.functions = dict()

    def load(self):
        self.libraries.clear()
        self.functions.clear()

        basePath = f"libs/{self.parent}/csv"

        if self.parent == "Game":
            basePath = "csv"

        # Load CSV files
        for file in os.listdir(basePath):            
            library = file[0:file.rfind(".")]
            symbols = OrderedDict()

            with open(pathlib.Path(f"{basePath}/{file}"), "r") as input:
                is_first_line = True
                for line in input:
                    if is_first_line:
                        is_first_line = False
                        continue

                    line_split = line.rstrip().split(",")

                    symbol = line_split[0]
                    obj_file = line_split[1]
                    library_name = line_split[2]
                    status = line_split[3]

                    if (symbol, obj_file) in symbols:
                        print(f"Duplicate symbol {symbol} in .o file {obj_file}.")
                        sys.exit(1)

                    symbols[(symbol, obj_file)] = (library_name, status)

            self.libraries[library] = symbols

        # Load addresses from symbol map
        with open("data/map_for_dol.map", "r") as input:
            for line in input:
                line_split = line.rstrip().split("=")

                symbol = line_split[1].split(":")[0]

                number_split = line_split[0].split(" ")
                address = int(number_split[4], 16)
                size = int(number_split[3], 16)

                self.functions[symbol] = (address, size)

    def save(self):
        basePath = f"libs/{self.parent}/csv"

        if self.parent == "Game":
            basePath = "csv"

        for library, symbols in self.libraries.items():
            with open(f"{basePath}/{library}.csv", "w") as output:
                output.write("Symbol Name, Object File, Library Archive, Matching\n")

                for (symbol, obj_file), values in symbols.items():
                    output.write(f"{symbol},{obj_file},{values[0]},{str(values[1]).lower()}\n")

    def get_obj_names_from_symbol(self, symbol_lookup):
        names = []

        for symbols in self.libraries.values():
            for (symbol, obj_name) in symbols.keys():
                if symbol == symbol_lookup:
                    names.append(obj_name)

        return names

    def get_function_status(self, library_name, symbol, obj_file):
        if library_name in self.libraries:
            library = self.libraries[library_name]
            
            if (symbol, obj_file) in library:
                return library[(symbol, obj_file)][1]

        return False

    def mark_symbol_status(self, library_name, symbol, obj_file, status):
        if library_name in self.libraries:
            library = self.libraries[library_name]
            
            if (symbol, obj_file) in library:
                library[(symbol, obj_file)] = (library[(symbol, obj_file)][0], status)

    def get_library_from_symbol(self, symbol, obj_file):
        for library, symbols in self.libraries.items():
            if (symbol, obj_file) in symbols:
                return library

        return None

    def get_address_from_symbol(self, symbol):
        if symbol in self.functions:
            return self.functions[symbol][0]

        return None
        
    def get_size_from_symbol(self, symbol):
        if symbol in self.functions:
            return self.functions[symbol][1]

        return None

    def get_symbols_marked_as_decompiled(self):
        for symbols in self.libraries.values():
            for (symbol, obj_file), values in symbols.items():
                if values[1] == True:
                    yield (symbol, obj_file)

def is_dol_correct():
    with open("baserom.dol", "rb") as input:
        data = input.read()

        hash = hashlib.sha256(data).hexdigest().upper()
        return hash == "8B7F28D193170F998F92E02EA638107822FB72073691D0893EB18857BE0C6FCF" or hash == "69F93FCC0FA34837347B5AC05168BC783ADCACB3C02697CFDA087A3B63ABC9E0"
    
def get_code_from_dol(address, size):
    with open("baserom.dol", "rb") as input:
        data = input.read()
        
        # Grab .text offset and start offset
        txt_offset, = struct.unpack_from(">I", data, 4)
        start_address, = struct.unpack_from(">I", data, 0x4C)

        offset = address - start_address

        return data[txt_offset + offset:txt_offset + offset + size]