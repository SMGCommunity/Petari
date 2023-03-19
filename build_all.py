import subprocess

libs = [ "JSystem", "MetroTRK", "MSL_C", "nw4r", "Runtime", "RVL_SDK", "RVLFaceLib" ]

for lib in libs:
    print(f"libs/{lib}")
    subprocess.call("python build.py -archive", cwd=f"libs/{lib}")

subprocess.call("python build.py -link")