# Petari
A decompilation of Super Mario Galaxy 1. The end goal is to be as close to 1:1 to the original code as possible.

# Compiling
Ensure you have CodeWarrior in your $PATH enviornment. After you have it, just run the python script:

```
python build.py [flags]
```

Use the -force flag to force a compile without checking for hashes. Use this if you change a header but not any source file.

If you do not have CodeWarrior in $PATH, stick the executables into a folder on the root named "tools".

If it successfully builds, all of the binaries are located in the "build" folder.

# Contributing
All contributions are welcome, just as long as they follow the current coding style. Look at any source file to ese the general style. Also, before contributing, be sure that the code matches 1:1 COMPLETELY to the original game.