import os
import pathlib

error_count = 0

for path, subdirs, files in os.walk(pathlib.Path("include")):
    for file_name in files:
        full_name = os.path.join(path, file_name)

        with open(full_name, "r") as input:
            first_line = input.readline().rstrip()

            if first_line != "#pragma once":
                print(f"ERROR: Missing pragma once in {full_name}")
                error_count += 1

print(f"Check finished with {error_count} error(s).")
