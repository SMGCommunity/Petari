import regex
from regex import Match

symbol_match = r"(?P<symbol>.+?)\s=\s(?P<section>.+?)\b:(?P<address>0x[0-9a-fA-F]+?)\b; // type:\b(?P<type>[a-zA-Z]+?)\b\ssize:\b(?P<size>0x[0-9a-fA-F]+?)\b(?P<other>.*)"
symbol_regex = regex.compile(symbol_match)

def get_symbol(line: str) -> Match[str] | None:
    return symbol_regex.match(line.rstrip())

def get_symbols(path: str) -> list[Match[str] | None]:
    with open(path, "r") as f:
        return [get_symbol(line) for line in f]
