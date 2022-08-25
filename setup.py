# setup.py -- by shibboleet
# Allows for the easy setup of RVL_SDK and its dependencies 

import zipfile
import urllib.request
import os
import sys
import subprocess

def install(what):
    subprocess.check_call([sys.executable, '-m', 'pip', 'install', what])

if not os.path.exists("baserom.dol"):
    print("baserom.dol does not exist. Place a SMG1 dol named baserom.dol in the root.")
    sys.exit(1)
else:
    print("Found baserom.dol...")

try:
    import capstone
    print("Found capstone, continuing...")
except ModuleNotFoundError:
    print("Module 'capstone' not installed. Installing...")
    install("capstone")

try:
    import elftools
    print("Found elftools, continuing...")
except ModuleNotFoundError:
    print("Module `elftools` not found. Installing...")
    install("pyelftools")

try:
    import ninja
    print("Found ninja, continuing...")
except ModuleNotFoundError:
    print("Module 'ninja' not found. Installing...")
    install("ninja")

try:
    import colorama
    print("Found colorama, continuing...")
except ModuleNotFoundError:
    print("Module 'colorama' not found. Installing...")
    install("colorama")

if not os.path.exists("Compilers"):
    print("Compilers folder not found, downloading...")

    with urllib.request.urlopen("http://shibbo.net/smg/Compilers.zip") as response, open("Compilers.zip", 'wb') as out:
        data = response.read()
        out.write(data)

    if os.path.exists("Compilers.zip"):
        os.mkdir("Compilers")
        print("Extracting file...")
        with zipfile.ZipFile("Compilers.zip", "r") as zip:
            zip.extractall("Compilers")

        os.remove("Compilers.zip")
    else:
        print("Compilers.zip failed to download.")
        sys.exit(1)
else:
    print("Found compilers folder, continuing...")

print("Done.")
sys.exit(1)