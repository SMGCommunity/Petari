import io
import json
import os
import re
import subprocess
from tools import ninja_syntax

BUILD_DIRECTORY = "build/"

MWCCEPPC_DIRECTORY = BUILD_DIRECTORY + "Compilers/"
MWCCEPPC_VERSION = "Wii/1.5/" # Vanilla game uses GC/3.0a3/
MWCCEPPC_PATH = MWCCEPPC_DIRECTORY + MWCCEPPC_VERSION + "mwcceppc.exe"

KAMEK_DIRECTORY = BUILD_DIRECTORY + "Kamek/"
KAMEK_PATH = KAMEK_DIRECTORY + "Kamek.exe"

KARIPON_SOURCE_DIRECTORY = "Karipon/source/"
KARIPON_INCLUDE_DIRECTORY = "Karipon/include/"
KARIPON_LOADER_DIRECTORY = "Karipon/loader/"

KARIPON_LOADER_SOURCE = KARIPON_LOADER_DIRECTORY + "Loader.cpp"
KARIPON_LOADER_OUTPUT = BUILD_DIRECTORY + "Loader.o"

KARIPON_OUTPUT_BIN = BUILD_DIRECTORY + "CustomCode.bin"
KARIPON_OUTPUT_MAP = BUILD_DIRECTORY + "CustomCode.map"
KARIPON_OUTPUT_LOADER_XML = BUILD_DIRECTORY + "Loader.xml"

KARIPON_SYMBOL_MAP = "config/RMGK01/symbols.txt"
KARIPON_SYMBOL_MAP_KAMEK = BUILD_DIRECTORY + "symbols.kamek.txt"
KARIPON_SYMBOL_REGEX = re.compile(r"^(\S+)\s*=\s*\.[^:]+:(0x[0-9a-fA-F]+);")
KARIPON_SYMBOL_APPEND: dict[str, int] = {
    "getAreaObj__16AreaObjContainerCFPCcRCQ29JGeometry8TVec3f": 0x8001EDA4,
}

# Compiler flags
CFLAGS = [
    "-lang=c++",
    "-nodefaults",
    "-proc gekko",
    "-align powerpc",
    "-enum int",
    "-fp hardware",
    "-Cpp_exceptions off",
    "-O2",
    "-inline auto",
    "-maxerrors 1",
    "-nosyspath",
    "-RTTI off",
    "-str reuse",
    "-enc SJIS",
    "-sdata 0",
    "-sdata2 0",
    "-ipa file",
    "-I-",

    "-DPATCH_360_FP_CAM",
    "-DPATCH_KOR_DIFFS",
    "-DPATCH_MISSING_MSG",
    "-DPATCH_NO_COMET_RETRY",
    "-DPATCH_QUICK_PAUSE",
]

# Linker flags
LFLAGS = [
    f"-externals={KARIPON_SYMBOL_MAP_KAMEK}",
    f"-output-kamek={KARIPON_OUTPUT_BIN}",
    f"-output-map={KARIPON_OUTPUT_MAP}",
]

LFLAGS_LOADER = [
    f"-externals={KARIPON_SYMBOL_MAP_KAMEK}",
    "-static=0x80001800",
    f"-output-riiv={KARIPON_OUTPUT_LOADER_XML}",
]

# compile_commands.json flags
COMPILE_COMMANDS = []
COMPILE_COMMANDS_CFLAGS = [
    "--target=powerpc-unknown-eabi",
    "-m32",
    "-std=c++98",
    "-nostdinc",
    "-nostdinc++",
    "-Wno-multichar",
    "-Wno-int-to-pointer-cast",
    "-Wno-pointer-to-int-cast",
    "-fno-builtin",
    "-fno-short-enums",
    "-fno-cxx-exceptions",
    "-fno-rtti",

    "-DPATCH_360_FP_CAM",
    "-DPATCH_KOR_DIFFS",
    "-DPATCH_MISSING_MSG",
    "-DPATCH_NO_COMET_RETRY",
    "-DPATCH_QUICK_PAUSE",
]

# Include directories
INCLUDE_DIRECTORIES: list[str] = [
    KARIPON_INCLUDE_DIRECTORY,

    "include",
    "libs/JSystem/include",
    "libs/MetroTRK/include",
    "libs/MSL_C/include",
    "libs/MSL_C++/include",
    "libs/nw4r/include",
    "libs/Runtime/include",
    "libs/RVL_SDK/include",
    "libs/RVLFaceLib/include",
]

# Make build directory
os.makedirs(BUILD_DIRECTORY, exist_ok=True)

# Add include directories to flags
for directory in INCLUDE_DIRECTORIES:
    CFLAGS.append(f"-I{directory}")
    COMPILE_COMMANDS_CFLAGS.append(f"-I{os.path.abspath(directory)}")

# Collect files
KARIPON_FILES: dict[str, str] = {}

for root, dirs, files in os.walk(KARIPON_SOURCE_DIRECTORY):
    for file in files:
        if not file.endswith(".cpp"):
            continue

        source_path = os.path.join(root, file).replace('\\', '/')
        output_path = source_path.replace("source", "build").replace(".cpp", ".o")

        os.makedirs(os.path.dirname(output_path), exist_ok=True)

        KARIPON_FILES[source_path] = output_path
        COMPILE_COMMANDS.append({
            "directory": os.getcwd(),
            "file": os.path.abspath(source_path),
            "output": os.path.abspath(output_path),
            "arguments": [
                "clang++",
                *COMPILE_COMMANDS_CFLAGS,
                "-c",
                os.path.abspath(source_path),
                "-o",
                os.path.abspath(output_path),
            ]
        })

# Generate symbol map
with open(KARIPON_SYMBOL_MAP, "r", encoding="utf-8") as infile, open(KARIPON_SYMBOL_MAP_KAMEK, "w", encoding="utf-8") as outfile:
    for line in infile:
        match = KARIPON_SYMBOL_REGEX.match(line.strip())

        if match:
            name, addr = match.groups()
            outfile.write(f"{name}={addr}\n")

    for name, addr in KARIPON_SYMBOL_APPEND.items():
        outfile.write(f"{name}={hex(addr)}\n")

# Generate ninja.build
out = io.StringIO()
n = ninja_syntax.Writer(out)

n.variable("cflags", CFLAGS)
n.variable("lflags", LFLAGS)
n.variable("lflags_loader", LFLAGS_LOADER)
n.variable("mwccepp_path", MWCCEPPC_PATH)
n.variable("kamek_path", KAMEK_PATH)
n.newline()

n.rule(
    name="download_tool",
    command=f"python tools\\download_tool.py $tool $out --tag $tag",
    description="TOOL $out",
    depfile="$out.d",
    deps="gcc"
)
n.rule(
    name="mwcc",
    command=f"$mwccepp_path $cflags -MDfile \"$out.d\" -c \"$in\" -o \"$out\"",
    description="MWCC $out",
    depfile="$out.d",
    deps="gcc"
)
n.rule(
    name="link",
    command=f"$kamek_path $in $lflags",
    description="Kamek $out"
)
n.rule(
    name="link_loader",
    command=f"$kamek_path $in $lflags_loader",
    description="Kamek $out"
)
n.newline()

n.build(
    outputs=MWCCEPPC_DIRECTORY,
    rule="download_tool",
    variables={
        "tool": "compilers",
        "tag": "20251118",
    },
)

n.build(
    outputs=KAMEK_DIRECTORY,
    rule="download_tool",
    variables={
        "tool": "kamek",
        "tag": "2026-02-24",
    },
)

for source_path, output_path in KARIPON_FILES.items():
    n.build(
        outputs=output_path,
        rule="mwcc",
        inputs=source_path,
    )

n.build(
    outputs=KARIPON_LOADER_OUTPUT,
    rule="mwcc",
    inputs=KARIPON_LOADER_SOURCE,
)

n.build(
    outputs=[KARIPON_OUTPUT_BIN, KARIPON_OUTPUT_MAP],
    rule="link",
    inputs=KARIPON_FILES.values(),
)

n.build(
    outputs=[KARIPON_OUTPUT_LOADER_XML],
    rule="link_loader",
    inputs=KARIPON_LOADER_OUTPUT,
)

with open("build.ninja", "w", encoding="utf-8") as f:
    f.write(out.getvalue())

out.close()

# Write compile_commands.json
with open("compile_commands.json", "w", encoding="utf-8") as f:
    json.dump(COMPILE_COMMANDS, f, indent=2)

subprocess.run("ninja", check=True)
