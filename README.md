# Petari
A decompilation of Super Mario Galaxy 1. The end goal is to be as close to 1:1 to the original code as possible.

# Compiling
Ensure you have CodeWarrior in your $PATH enviornment. You also need to have your own copy of the Nintendo Wii SDK (this will NOT be provided!) and ensure the two paths in sources.txt is accurate. After you have those configured, just run the python script:

```
python build.py [flags]
```

Use the -nolink flag to compile all of the files, but not link any of them. However, the script will still create archives for the objects.

If you do not have CodeWarrior in $PATH, stick the executables into a folder on the root named "tools".

If it successfully builds, all of the binaries are located in the "build" folder.

# Contributing
All contributions are welcome, just as long as they follow the current coding style. Look at any source file to see the general style. Also, before contributing, be sure that the code matches 1:1 COMPLETELY to the original game.