# setup.py -- by shibboleet
# Allows for the easy setup of Petari and its dependencies 

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
    install("elftools")

try:
    import ninja
    print("Found ninja, continuing...")
except ModuleNotFoundError:
    print("Module 'ninja' not found. Installing...")
    install("ninja")

if not os.path.exists("deps"):
    print("Dependency folder not found, downloading...")

    with urllib.request.urlopen("http://shibboleet.us.to/deps.zip") as response, open("deps.zip", 'wb') as out:
        data = response.read()
        out.write(data)

    if os.path.exists("deps.zip"):
        os.mkdir("deps")
        print("Extracting file...")
        with zipfile.ZipFile("deps.zip", "r") as zip:
            zip.extractall("deps")

        os.remove("deps.zip")
    else:
        print("deps.zip failed to download.")
        sys.exit(1)

print("Done.")
sys.exit(1)