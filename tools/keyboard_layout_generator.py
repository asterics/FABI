#!/usr/bin/env python3
"""
FABI Keyboard Layout Generator and Command Parser

Provides physical and virtual keyboard layout definitions, character-to-key mappings,
AT command parsing, macro escaping, and layout generation for the FABI system.

Addresses Issue #36: ensures backslash ('\\') and other special characters are properly
supported across keyboard layouts and command parsing/macros without escaping errors.
"""

import sys
import os
import re
import json
import argparse
from typing import Dict, List, Tuple, Optional, Any

# Standard FABI key identifiers with prefix "KEY_"
STANDARD_KEY_IDENTIFIERS: Dict[str, Dict[str, Any]] = {
    # Letters
    **{f"KEY_{chr(c)}": {"type": "letter", "char": chr(c).lower(), "hid": 0x04 + (c - ord('A'))}
       for c in range(ord('A'), ord('Z') + 1)},

    # Digits
    **{f"KEY_{chr(c)}": {"type": "digit", "char": chr(c), "hid": 0x1E + (c - ord('1')) if c != ord('0') else 0x27}
       for c in range(ord('0'), ord('9') + 1)},

    # Function keys F1 - F24
    **{f"KEY_F{i}": {"type": "function", "char": None, "hid": 0x3A + (i - 1) if i <= 12 else 0x68 + (i - 13)}
       for i in range(1, 25)},

    # Navigation keys
    "KEY_UP": {"type": "navigation", "char": None, "hid": 0x52},
    "KEY_DOWN": {"type": "navigation", "char": None, "hid": 0x51},
    "KEY_LEFT": {"type": "navigation", "char": None, "hid": 0x50},
    "KEY_RIGHT": {"type": "navigation", "char": None, "hid": 0x4F},
    "KEY_TAB": {"type": "navigation", "char": "\t", "hid": 0x2B},
    "KEY_PAGE_UP": {"type": "navigation", "char": None, "hid": 0x4B},
    "KEY_PAGE_DOWN": {"type": "navigation", "char": None, "hid": 0x4E},
    "KEY_HOME": {"type": "navigation", "char": None, "hid": 0x4A},
    "KEY_END": {"type": "navigation", "char": None, "hid": 0x4D},

    # Modifiers
    "KEY_SHIFT": {"type": "modifier", "char": None, "hid": 0xE1},
    "KEY_CTRL": {"type": "modifier", "char": None, "hid": 0xE0},
    "KEY_ALT": {"type": "modifier", "char": None, "hid": 0xE2},
    "KEY_RIGHT_ALT": {"type": "modifier", "char": None, "hid": 0xE6},  # AltGr
    "KEY_GUI": {"type": "modifier", "char": None, "hid": 0xE3},
    "KEY_RIGHT_GUI": {"type": "modifier", "char": None, "hid": 0xE7},

    # Special / Control keys
    "KEY_ENTER": {"type": "special", "char": "\n", "hid": 0x28},
    "KEY_SPACE": {"type": "special", "char": " ", "hid": 0x2C},
    "KEY_BACKSPACE": {"type": "special", "char": "\b", "hid": 0x2A},
    "KEY_DELETE": {"type": "special", "char": None, "hid": 0x4C},
    "KEY_INSERT": {"type": "special", "char": None, "hid": 0x49},
    "KEY_ESC": {"type": "special", "char": None, "hid": 0x29},
    "KEY_NUM_LOCK": {"type": "special", "char": None, "hid": 0x53},
    "KEY_SCROLL_LOCK": {"type": "special", "char": None, "hid": 0x47},
    "KEY_CAPS_LOCK": {"type": "special", "char": None, "hid": 0x39},
    "KEY_PAUSE": {"type": "special", "char": None, "hid": 0x48},
    "KEY_PRINTSCREEN": {"type": "special", "char": None, "hid": 0x46},
    "KEY_MENU": {"type": "special", "char": None, "hid": 0x65},

    # Punctuation and Symbol keys (physical / US layout base)
    "KEY_BACKSLASH": {"type": "punctuation", "char": "\\", "hid": 0x31},
    "KEY_SLASH": {"type": "punctuation", "char": "/", "hid": 0x38},
    "KEY_SEMICOLON": {"type": "punctuation", "char": ";", "hid": 0x33},
    "KEY_COMMA": {"type": "punctuation", "char": ",", "hid": 0x36},
    "KEY_PERIOD": {"type": "punctuation", "char": ".", "hid": 0x37},
    "KEY_MINUS": {"type": "punctuation", "char": "-", "hid": 0x2D},
    "KEY_EQUAL": {"type": "punctuation", "char": "=", "hid": 0x2E},
    "KEY_LEFT_BRACE": {"type": "punctuation", "char": "[", "hid": 0x2F},
    "KEY_RIGHT_BRACE": {"type": "punctuation", "char": "]", "hid": 0x30},
    "KEY_QUOTE": {"type": "punctuation", "char": "'", "hid": 0x34},
    "KEY_TILDE": {"type": "punctuation", "char": "`", "hid": 0x35},
}

# Keypad keys with prefix "KEYPAD_"
KEYPAD_IDENTIFIERS: Dict[str, Dict[str, Any]] = {
    **{f"KEYPAD_{i}": {"type": "keypad", "char": str(i), "hid": 0x59 + (i - 1) if i > 0 else 0x62}
       for i in range(10)},
    "KEYPAD_SLASH": {"type": "keypad", "char": "/", "hid": 0x54},
    "KEYPAD_ASTERIX": {"type": "keypad", "char": "*", "hid": 0x55},
    "KEYPAD_MINUS": {"type": "keypad", "char": "-", "hid": 0x56},
    "KEYPAD_PLUS": {"type": "keypad", "char": "+", "hid": 0x57},
    "KEYPAD_ENTER": {"type": "keypad", "char": "\n", "hid": 0x58},
    "KEYPAD_PERIOD": {"type": "keypad", "char": ".", "hid": 0x63},
}

ALL_KEY_IDENTIFIERS = {**STANDARD_KEY_IDENTIFIERS, **KEYPAD_IDENTIFIERS}

# Layout-specific character to physical key sequences
# Maps each character to a list of FABI key tokens to press simultaneously
LAYOUT_CHARACTER_MAPPINGS: Dict[str, Dict[str, List[str]]] = {
    "en_US": {
        "\\": ["KEY_BACKSLASH"],
        "|": ["KEY_SHIFT", "KEY_BACKSLASH"],
        "/": ["KEY_SLASH"],
        "?": ["KEY_SHIFT", "KEY_SLASH"],
        ";": ["KEY_SEMICOLON"],
        ":": ["KEY_SHIFT", "KEY_SEMICOLON"],
        ",": ["KEY_COMMA"],
        "<": ["KEY_SHIFT", "KEY_COMMA"],
        ".": ["KEY_PERIOD"],
        ">": ["KEY_SHIFT", "KEY_PERIOD"],
        "-": ["KEY_MINUS"],
        "_": ["KEY_SHIFT", "KEY_MINUS"],
        "=": ["KEY_EQUAL"],
        "+": ["KEY_SHIFT", "KEY_EQUAL"],
        "[": ["KEY_LEFT_BRACE"],
        "{": ["KEY_SHIFT", "KEY_LEFT_BRACE"],
        "]": ["KEY_RIGHT_BRACE"],
        "}": ["KEY_SHIFT", "KEY_RIGHT_BRACE"],
        "'": ["KEY_QUOTE"],
        '"': ["KEY_SHIFT", "KEY_QUOTE"],
        "`": ["KEY_TILDE"],
        "~": ["KEY_SHIFT", "KEY_TILDE"],
        "!": ["KEY_SHIFT", "KEY_1"],
        "@": ["KEY_SHIFT", "KEY_2"],
        "#": ["KEY_SHIFT", "KEY_3"],
        "$": ["KEY_SHIFT", "KEY_4"],
        "%": ["KEY_SHIFT", "KEY_5"],
        "^": ["KEY_SHIFT", "KEY_6"],
        "&": ["KEY_SHIFT", "KEY_7"],
        "*": ["KEY_SHIFT", "KEY_8"],
        "(": ["KEY_SHIFT", "KEY_9"],
        ")": ["KEY_SHIFT", "KEY_0"],
    },
    "de_DE": {
        # On physical German keyboard, backslash is AltGr + ß (the key located at US MINUS 0x2D)
        "\\": ["KEY_RIGHT_ALT", "KEY_MINUS"],
        "|": ["KEY_RIGHT_ALT", "KEY_1"],       # or AltGr + < (less-than key)
        "/": ["KEY_SHIFT", "KEY_7"],
        "?": ["KEY_SHIFT", "KEY_MINUS"],       # Shift + ß produces ?
        ";": ["KEY_SHIFT", "KEY_COMMA"],
        ":": ["KEY_SHIFT", "KEY_PERIOD"],
        ",": ["KEY_COMMA"],
        ".": ["KEY_PERIOD"],
        "-": ["KEY_SLASH"],                   # German - is on physical US / key
        "_": ["KEY_SHIFT", "KEY_SLASH"],
        "=": ["KEY_SHIFT", "KEY_0"],
        "+": ["KEY_RIGHT_BRACE"],             # German + is on physical US ] key
        "*": ["KEY_SHIFT", "KEY_RIGHT_BRACE"],
        "[": ["KEY_RIGHT_ALT", "KEY_8"],
        "]": ["KEY_RIGHT_ALT", "KEY_9"],
        "{": ["KEY_RIGHT_ALT", "KEY_7"],
        "}": ["KEY_RIGHT_ALT", "KEY_0"],
        "~": ["KEY_RIGHT_ALT", "KEY_RIGHT_BRACE"],
        "@": ["KEY_RIGHT_ALT", "KEY_Q"],
        "!": ["KEY_SHIFT", "KEY_1"],
        '"': ["KEY_SHIFT", "KEY_2"],
        "§": ["KEY_SHIFT", "KEY_3"],
        "$": ["KEY_SHIFT", "KEY_4"],
        "%": ["KEY_SHIFT", "KEY_5"],
        "&": ["KEY_SHIFT", "KEY_6"],
        "(": ["KEY_SHIFT", "KEY_8"],
        ")": ["KEY_SHIFT", "KEY_9"],
    },
    "es_ES": {
        "\\": ["KEY_RIGHT_ALT", "KEY_QUOTE"],  # AltGr + 'º/ª'
        "|": ["KEY_RIGHT_ALT", "KEY_1"],
        "@": ["KEY_RIGHT_ALT", "KEY_2"],
        "#": ["KEY_RIGHT_ALT", "KEY_3"],
        "~": ["KEY_RIGHT_ALT", "KEY_4"],
        "[": ["KEY_RIGHT_ALT", "KEY_LEFT_BRACE"],
        "]": ["KEY_RIGHT_ALT", "KEY_RIGHT_BRACE"],
        "{": ["KEY_RIGHT_ALT", "KEY_QUOTE"],
        "}": ["KEY_RIGHT_ALT", "KEY_TILDE"],
    },
    "fr_FR": {
        "\\": ["KEY_RIGHT_ALT", "KEY_8"],      # AltGr + _ (8 key)
        "|": ["KEY_RIGHT_ALT", "KEY_6"],
        "@": ["KEY_RIGHT_ALT", "KEY_0"],
        "#": ["KEY_RIGHT_ALT", "KEY_3"],
        "[": ["KEY_RIGHT_ALT", "KEY_5"],
        "]": ["KEY_RIGHT_ALT", "KEY_RIGHT_BRACE"],
        "{": ["KEY_RIGHT_ALT", "KEY_4"],
        "}": ["KEY_RIGHT_ALT", "KEY_EQUAL"],
        "~": ["KEY_RIGHT_ALT", "KEY_2"],
    },
    "it_IT": {
        "\\": ["KEY_BACKSLASH"],              # physical backslash key top left
        "|": ["KEY_SHIFT", "KEY_BACKSLASH"],
        "@": ["KEY_RIGHT_ALT", "KEY_SEMICOLON"],
        "#": ["KEY_RIGHT_ALT", "KEY_QUOTE"],
        "[": ["KEY_RIGHT_ALT", "KEY_LEFT_BRACE"],
        "]": ["KEY_RIGHT_ALT", "KEY_RIGHT_BRACE"],
        "{": ["KEY_RIGHT_ALT", "KEY_SHIFT", "KEY_LEFT_BRACE"],
        "}": ["KEY_RIGHT_ALT", "KEY_SHIFT", "KEY_RIGHT_BRACE"],
        "~": ["KEY_RIGHT_ALT", "KEY_TILDE"],
    }
}

# Supported AT Commands
VALID_AT_COMMANDS = {
    # ID and Button Mode
    "ID", "BM",
    # Mouse
    "CL", "CR", "CM", "CD", "PL", "PR", "PM", "HL", "HR", "HM",
    "RL", "RR", "RM", "TL", "TR", "TM", "WU", "WD", "WS", "MX", "MY",
    # Joystick
    "J0", "J1", "J2", "J3", "J4", "J5", "JP", "JR", "JH",
    # Keyboard
    "KW", "KP", "KH", "KT", "KR", "RA", "KL",
    # Slots
    "SA", "LO", "LA", "LI", "NE", "DE", "RS", "RE",
    # Slot settings and reporting
    "NC", "BT", "SC", "SR", "ER", "CA", "TT", "AP", "AR", "AI", "DP", "AD",
    # Macros
    "MA", "WA",
    # Sip & Puff
    "TS", "TP", "SP", "SS",
    # Infrared
    "IR", "IP", "IH", "IS", "IC", "IW", "IL", "IT",
    # 2D mouse cursor control
    "MM", "SW", "AX", "AY", "DX", "DY", "MS", "AC", "RO", "SB",
    # Audio feedback
    "AT",
}


def escape_macro_string(command_string: str) -> str:
    """
    Escapes a command string for inclusion within an AT MA macro.
    In FABI firmware, semicolons ';' are command delimiters.
    To pass a literal ';' or '\\', a backslash escape is used:
      '\\' -> '\\\\'
      ';'  -> '\\;'
    """
    out = []
    for ch in command_string:
        if ch == '\\':
            out.append('\\\\')
        elif ch == ';':
            out.append('\\;')
        else:
            out.append(ch)
    return "".join(out)


def unescape_macro_string(macro_content: str) -> List[str]:
    """
    Parses an AT MA macro content string into individual sub-commands,
    correctly respecting backslash escapes ('\\\\', '\\;').
    """
    commands = []
    current = []
    i = 0
    n = len(macro_content)

    while i < n:
        ch = macro_content[i]
        if ch == '\\':
            if i + 1 < n:
                next_ch = macro_content[i + 1]
                if next_ch in ('\\', ';'):
                    current.append(next_ch)
                    i += 2
                    continue
                else:
                    # Trailing or unrecognized escape: preserve escape character
                    current.append('\\')
                    current.append(next_ch)
                    i += 2
                    continue
            else:
                current.append('\\')
                i += 1
                continue
        elif ch == ';':
            cmd_str = "".join(current).strip()
            if cmd_str:
                commands.append(cmd_str)
            current = []
            i += 1
        else:
            current.append(ch)
            i += 1

    remaining = "".join(current).strip()
    if remaining:
        commands.append(remaining)

    return commands


def parse_at_command(line: str) -> Tuple[Optional[str], Optional[str]]:
    """
    Parses a single AT command line (e.g. 'AT KW \\', 'AT KP KEY_BACKSLASH').
    Returns (command_code, parameter_string) or (None, None) if invalid.
    """
    line = line.strip()
    if not line:
        return None, None

    if line.startswith("AT "):
        content = line[3:].strip()
    elif line.startswith("AT"):
        content = line[2:].strip()
    else:
        content = line

    if " " in content:
        parts = content.split(" ", 1)
        cmd = parts[0].upper()
        param = parts[1]
    else:
        # Check for compact command without space (e.g. MX120, WA50, MY-10)
        upper_content = content.upper()
        if len(content) >= 3 and upper_content[:2] in VALID_AT_COMMANDS:
            cmd = upper_content[:2]
            param = content[2:]
        else:
            cmd = upper_content
            param = ""

    if cmd not in VALID_AT_COMMANDS:
        return None, None

    return cmd, param


def format_at_command(cmd: str, param: str = "") -> str:
    """Formats an AT command line cleanly."""
    cmd = cmd.upper()
    if param:
        return f"AT {cmd} {param}"
    return f"AT {cmd}"


def format_macro(commands: List[str]) -> str:
    """
    Combines a list of sub-commands into an AT MA macro command string
    with appropriate escaping.
    """
    escaped_cmds = [escape_macro_string(c.strip()) for c in commands if c.strip()]
    return f"AT MA {';'.join(escaped_cmds)};"


def resolve_character_to_keys(char: str, layout: str = "en_US") -> List[str]:
    """
    Resolves a single character to a list of FABI key tokens
    for the specified keyboard layout.
    """
    # 1. Check layout-specific mappings (including backslash, symbols)
    layout_map = LAYOUT_CHARACTER_MAPPINGS.get(layout, LAYOUT_CHARACTER_MAPPINGS["en_US"])
    if char in layout_map:
        return list(layout_map[char])

    # Fallback to en_US if character exists there
    if layout != "en_US" and char in LAYOUT_CHARACTER_MAPPINGS["en_US"]:
        return list(LAYOUT_CHARACTER_MAPPINGS["en_US"][char])

    # 2. Check standard ASCII letters
    if 'a' <= char <= 'z':
        return [f"KEY_{char.upper()}"]
    if 'A' <= char <= 'Z':
        return ["KEY_SHIFT", f"KEY_{char}"]

    # 3. Check digits
    if '0' <= char <= '9':
        return [f"KEY_{char}"]

    # 4. Check whitespace / control
    if char == ' ':
        return ["KEY_SPACE"]
    if char == '\t':
        return ["KEY_TAB"]
    if char == '\n':
        return ["KEY_ENTER"]

    # 5. Fallback for unrecognized character: return empty list
    return []


def generate_command_for_text(text: str, layout: str = "en_US", method: str = "auto") -> List[str]:
    """
    Generates FABI commands to type the given text on a target keyboard layout.
    Method can be:
      - 'kw': uses 'AT KW <text>' (firmware keyboard write)
      - 'kp': uses sequence of 'AT KP <keys>' (physical keypresses)
      - 'auto': uses 'KW' if supported directly, or 'KP' for layout-dependent characters
    """
    commands = []

    if method == "kw":
        commands.append(format_at_command("KW", text))
    elif method == "kp":
        for ch in text:
            keys = resolve_character_to_keys(ch, layout=layout)
            if keys:
                commands.append(format_at_command("KP", " ".join(keys)))
    else:  # auto
        # If text contains backslash and layout is de_DE, KW might fail on firmware
        # lacking AltGr lookup, so provide physical KP key sequence
        if "\\" in text and layout == "de_DE":
            for ch in text:
                keys = resolve_character_to_keys(ch, layout=layout)
                if keys:
                    commands.append(format_at_command("KP", " ".join(keys)))
                else:
                    commands.append(format_at_command("KW", ch))
        else:
            commands.append(format_at_command("KW", text))

    return commands


def validate_key_token(token: str) -> bool:
    """Validates if a key token is a recognized FABI key identifier."""
    if token in ALL_KEY_IDENTIFIERS:
        return True
    # Firmware also accepts raw single characters '0'-'9' and 'A'-'Z'
    if len(token) == 1 and token.isalnum():
        return True
    return False


def validate_settings_file(filepath: str) -> Tuple[bool, List[str]]:
    """
    Validates a FABI .set configuration file for syntax errors,
    unknown key identifiers, or macro escaping problems.
    """
    errors = []
    if not os.path.exists(filepath):
        return False, [f"File not found: {filepath}"]

    with open(filepath, "r", encoding="utf-8", errors="replace") as f:
        lines = f.readlines()

    for idx, raw_line in enumerate(lines, start=1):
        line = raw_line.strip()
        if not line or line.startswith("#") or line.startswith("Slot:"):
            continue

        cmd, param = parse_at_command(line)
        if cmd is None:
            errors.append(f"Line {idx}: Unrecognized or invalid AT command '{line}'")
            continue

        # Check KP, KH, KT, KR key arguments
        if cmd in ("KP", "KH", "KT", "KR"):
            tokens = param.split()
            for token in tokens:
                if not validate_key_token(token):
                    errors.append(f"Line {idx}: Unrecognized key identifier '{token}' in '{line}'")

        # Check MA macro syntax and escaping
        if cmd == "MA":
            sub_cmds = unescape_macro_string(param)
            if not sub_cmds:
                errors.append(f"Line {idx}: Empty or malformed macro '{line}'")
            for sub in sub_cmds:
                sub_cmd, _ = parse_at_command(sub)
                if sub_cmd is None:
                    errors.append(f"Line {idx}: Invalid sub-command '{sub}' inside macro")

    return len(errors) == 0, errors


def export_layout_json(layout: str = "en_US") -> str:
    """Exports character-to-key definitions as formatted JSON."""
    mapping = LAYOUT_CHARACTER_MAPPINGS.get(layout, LAYOUT_CHARACTER_MAPPINGS["en_US"])
    data = {
        "layout": layout,
        "description": f"FABI keyboard mapping definitions for {layout}",
        "mappings": mapping,
        "supported_key_identifiers": list(ALL_KEY_IDENTIFIERS.keys()),
    }
    return json.dumps(data, indent=2)


def generate_markdown_key_table() -> str:
    """Generates markdown documentation for supported key identifiers."""
    groups: Dict[str, List[str]] = {
        "Letters": [f"KEY_{chr(c)}" for c in range(ord('A'), ord('Z') + 1)],
        "Digits": [f"KEY_{chr(c)}" for c in range(ord('0'), ord('9') + 1)],
        "Function keys": [f"KEY_F{i}" for i in range(1, 25)],
        "Navigation keys": [
            "KEY_UP", "KEY_DOWN", "KEY_LEFT", "KEY_RIGHT", "KEY_TAB",
            "KEY_PAGE_UP", "KEY_PAGE_DOWN", "KEY_HOME", "KEY_END"
        ],
        "Special and Punctuation keys": [
            "KEY_ENTER", "KEY_SPACE", "KEY_BACKSPACE", "KEY_DELETE", "KEY_INSERT",
            "KEY_ESC", "KEY_NUM_LOCK", "KEY_SCROLL_LOCK", "KEY_CAPS_LOCK", "KEY_PAUSE",
            "KEY_PRINTSCREEN", "KEY_MENU", "KEY_BACKSLASH", "KEY_SLASH", "KEY_SEMICOLON",
            "KEY_COMMA", "KEY_PERIOD", "KEY_MINUS", "KEY_EQUAL", "KEY_LEFT_BRACE",
            "KEY_RIGHT_BRACE", "KEY_QUOTE", "KEY_TILDE"
        ],
        "Keypad keys": list(KEYPAD_IDENTIFIERS.keys()),
        "Modifier keys": [
            "KEY_SHIFT", "KEY_CTRL", "KEY_ALT", "KEY_RIGHT_ALT", "KEY_GUI", "KEY_RIGHT_GUI"
        ]
    }

    lines = [
        "### Supported Key Identifiers\n",
        "| Category | Supported Identifiers |",
        "| :--- | :--- |"
    ]

    for cat, keys in groups.items():
        keys_str = " ".join(keys)
        lines.append(f"| **{cat}** | {keys_str} |")

    return "\n".join(lines)


def main():
    parser = argparse.ArgumentParser(description="FABI Keyboard Layout Generator and Command Parser")
    parser.add_argument("--layout", default="en_US", choices=list(LAYOUT_CHARACTER_MAPPINGS.keys()),
                        help="Target keyboard layout (default: en_US)")
    parser.add_argument("--char", type=str, help="Resolve a single character to physical key identifiers")
    parser.add_argument("--text", type=str, help="Generate FABI commands to type text")
    parser.add_argument("--method", default="auto", choices=["auto", "kw", "kp"],
                        help="Typing method for --text (auto, kw, kp)")
    parser.add_argument("--macro", action="store_true", help="Format generated commands into an AT MA macro")
    parser.add_argument("--export-json", action="store_true", help="Export layout mapping to JSON")
    parser.add_argument("--export-markdown", action="store_true", help="Export supported keys markdown table")
    parser.add_argument("--validate-settings", type=str, help="Validate a .set settings file")

    args = parser.parse_args()

    if args.char:
        keys = resolve_character_to_keys(args.char, layout=args.layout)
        print(f"Character: '{args.char}' on layout {args.layout}")
        print(f"Keys: {' '.join(keys) if keys else '(no mapping found)'}")
        return

    if args.text:
        cmds = generate_command_for_text(args.text, layout=args.layout, method=args.method)
        if args.macro:
            print(format_macro(cmds))
        else:
            for c in cmds:
                print(c)
        return

    if args.export_json:
        print(export_layout_json(args.layout))
        return

    if args.export_markdown:
        print(generate_markdown_key_table())
        return

    if args.validate_settings:
        valid, errors = validate_settings_file(args.validate_settings)
        if valid:
            print(f"OK: '{args.validate_settings}' is valid.")
        else:
            print(f"ERRORS in '{args.validate_settings}':")
            for err in errors:
                print(f"  - {err}")
            sys.exit(1)
        return

    parser.print_help()


if __name__ == "__main__":
    main()
