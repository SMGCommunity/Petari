from pathlib import Path
import re
import os

INCLUDE_PATTERN = re.compile(r'^#include (?:<([^<>]+)>|"([^"]+)")', re.MULTILINE)

LIBRARIES: list[tuple[Path, Path]] = [
    (Path("include/"),                 Path("src/Game/")),       # Game
    (Path("libs/JSystem/include/"),    Path("src/JSystem/")),    # JSystem
    (Path("libs/MetroTRK/include/"),   Path("src/MetroTRK/")),   # MetroTRK
    (Path("libs/MSL_C/include/"),      Path("src/MSL_C/")),      # MSL_C
    (Path("libs/MSL_C++/include/"),    Path("src/MSL_C++/")),    # MSL_C++
    (Path("libs/nw4r/include/"),       Path("src/nw4r/")),       # nw4r
    (Path("libs/Runtime/include/"),    Path("src/Runtime/")),    # Runtime
    (Path("libs/RVL_SDK/include/"),    Path("src/RVL_SDK/")),    # RVL_SDK
    (Path("libs/RVLFaceLib/include/"), Path("src/RVLFaceLib/")), # RVLFaceLib
]

EXTENSIONS: list[str] = [
    ".c",
    ".cpp",
    ".h",
    ".hpp",
]

def is_local_include(header: str, local_include_directory: Path) -> bool:
    return (local_include_directory / header).exists()

def format_include(match: re.Match, local_include_directory: Path) -> str:
    header: str = match.group(1) or match.group(2)
    header = header.replace('\\', '/')

    if is_local_include(header, local_include_directory):
        return f'#include "{header}"'

    return f'#include <{header}>'

def format_file(file_path: Path, local_include_directory: Path) -> None:
    with open(file_path, 'r', encoding='utf-8') as f:
        text: str = f.read()

    text_replaced: str = INCLUDE_PATTERN.sub(lambda match: format_include(match, local_include_directory), text)

    if text != text_replaced:
        with open(file_path, 'w', encoding='utf-8') as f:
            f.write(text_replaced)

def format_files(directory: Path, local_include_directory: Path):
    for root, dirs, files in os.walk(directory):
        for file in files:
            for extension in EXTENSIONS:
                if file.endswith(extension):
                    break
            else:
                continue

            file_path: Path = Path(os.path.join(root, file))

            print(f'[{local_include_directory}] Formatting {file_path}')
            format_file(file_path, local_include_directory)

def format_all():
    for (include_directory, source_directory) in LIBRARIES:
        if include_directory.is_dir():
            format_files(include_directory, include_directory)

            if source_directory.is_dir():
                format_files(source_directory, include_directory)

if __name__ == "__main__":
    format_all()
