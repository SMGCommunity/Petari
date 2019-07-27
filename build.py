import glob
import hashlib
import subprocess
import sys
import os
from pathlib import Path

numFilesCompiled = 0
forceCompile = False

def isCmdAvailable(cmd):
    test_cmd = "which" if platform.system() != "Windows" else "where"

    try:
        rc = subprocess.call([test_cmd, cmd], stdout=open(os.devnull, 'wb'), stderr=open(os.devnull, 'wb'))
        return rc == 0
    except:
        return False

# create our md5 hash storage if it doesn't exist
if not os.path.exists("files_md5.txt"):
    f = open("files_md5.txt", "w+")

def getMD5(path):
    return hashlib.md5(open(path, "rb").read()).hexdigest()

def checkIfFileIsStored(filename):
    with open("files_md5.txt", "r") as f:
        lines = f.readlines()

    for line in lines:
        if line.split("=")[0] == filename:
            return True

    return False

def checkIfHashIsStored(hash):
    with open("files_md5.txt", "r") as f:
        lines = f.readlines()

    for line in lines:
        if line.split("=")[1].strip("\n") == hash:
            return True

    return False

def appendFilename(filename):
    with open("files_md5.txt", "a") as w:
        w.write(f"{filename}={getMD5(filename)}\n")

def checkAndPerformAppend(filename):
    if checkIfFileIsStored(filename):
        if checkIfHashIsStored(getMD5(filename)):
            return True

    # this file already has an entry but our hash is different
    # we have to remove this and add the new one
    # todo -- find better solution
    if checkIfFileIsStored(filename):
        with open("files_md5.txt", "r+") as f:
            lines = f.readlines()

        os.remove("files_md5.txt")

        with open("files_md5.txt", "w") as w:
            for line in lines:
                if line.split("=")[0] != filename:
                    w.write(line)

    appendFilename(filename)

    return False

if "-force" in sys.argv:
    forceCompile = True

flags = "-i . -I- -i include -nostdinc -Cpp_exceptions off -O4,s -proc gekko -fp hard -enum int -sdata 0 -sdata2 0 -g"
as_flags = "-i . -I- -nostdinc -proc gekko -d __MWERKS__"

req_commands = ['mwcceppc', 'mwasmeppc']

# first check our tools folder
if not os.path.isdir("tools"):
    # don't have it? let's check $PATH
    for cmd in req_commands:
        if isCmdAvailable(cmd):
            continue
        else:
             print(f"Command {cmd} is not available!")
             sys.exit(1)
        

if not os.path.isdir("build"):
    os.mkdir("build")

path = os.path.dirname(os.path.realpath(__file__)) + "\\source\\"

c_files = [f for f in glob.glob(path + "**/*.c", recursive=True)]
cpp_files = [f for f in glob.glob(path + "**/*.cpp", recursive=True)]
assembly_files = [f for f in glob.glob(path + "**/*.s", recursive=True)]

for f in cpp_files:
    file_name = Path(f).stem

    if checkAndPerformAppend(f) and forceCompile == False:
        continue
	
    print(f"Compiling {file_name}.cpp...")

    if subprocess.call(f"mwcceppc.exe {flags} -c -o build/{file_name}.o {f}", shell=True) == 1:
        sys.exit(1)

    numFilesCompiled += 1

for f in c_files:
    file_name = Path(f).stem
    
    if checkAndPerformAppend(f) and forceCompile == False:
        continue
	
    print(f"Compiling {file_name}.c...")

    if subprocess.call(f"mwcceppc.exe {flags} -c -o build/{file_name}.o {f}", shell=True) == 1:
        sys.exit(1)

    numFilesCompiled += 1

for f in assembly_files:
    file_name = Path(f).stem

    if checkAndPerformAppend(f) and forceCompile == False:
        continue
	
    print(f"Assembling {file_name}.s...")

    if subprocess.call(f"mwasmeppc.exe {as_flags} -o build/{file_name}.o {f}", shell=True) == 1:
        sys.exit(1)

    numFilesCompiled += 1

if numFilesCompiled == 0:
    print("No changes to compile.")