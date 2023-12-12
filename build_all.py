import subprocess, sys

libs = [ "JSystem", "MetroTRK", "MSL_C", "nw4r", "Runtime", "RVL_SDK", "RVLFaceLib" ]

if len(sys.argv) >= 2:
    lib_name = sys.argv[1]
    
    if lib_name not in libs:
        print("Error: library not found")
        sys.exit(1)
    else:
        subprocess.call("python build.py -archive", cwd=f"libs/{lib_name}")
else:
    for lib in libs:
        subprocess.call("python build.py -archive", cwd=f"libs/{lib}")

subprocess.call("python build.py -link")

if subprocess.call("python make_dol.py main.elf main.dol") == 1:
    print("NOT OK...")
    sys.exit(1)

print("OK")