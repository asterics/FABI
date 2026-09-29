#!/usr/bin/env python3
"""
Unit tests for FABI Keyboard Layout Generator and Command Parser.

Verifies:
- Backslash ('\\') and special character mapping across keyboard layouts (Issue #36)
- Key identifier validity
- Command parsing for KW, KP, KH, KR, KT
- Macro string escaping and unescaping
- Validation of existing FABI .set configuration files
"""

import os
import sys
import glob
import unittest

# Add project root to sys.path
REPO_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
sys.path.insert(0, REPO_ROOT)

from tools.keyboard_layout_generator import (
    STANDARD_KEY_IDENTIFIERS,
    KEYPAD_IDENTIFIERS,
    ALL_KEY_IDENTIFIERS,
    resolve_character_to_keys,
    parse_at_command,
    format_at_command,
    escape_macro_string,
    unescape_macro_string,
    format_macro,
    generate_command_for_text,
    validate_key_token,
    validate_settings_file,
    export_layout_json,
    generate_markdown_key_table,
)


class TestKeyboardLayoutMapping(unittest.TestCase):
    """Tests character-to-key mappings across keyboard layouts."""

    def test_backslash_mapping_en_us(self):
        """Verify backslash maps directly to KEY_BACKSLASH in en_US."""
        keys = resolve_character_to_keys("\\", layout="en_US")
        self.assertEqual(keys, ["KEY_BACKSLASH"])

    def test_backslash_mapping_de_de(self):
        """
        Verify backslash maps to AltGr + Minus (KEY_RIGHT_ALT KEY_MINUS) in de_DE.
        On physical German keyboards, the 'ß' key is located at the US Minus position,
        and pressing AltGr + 'ß' produces the backslash '\\'.
        """
        keys = resolve_character_to_keys("\\", layout="de_DE")
        self.assertEqual(keys, ["KEY_RIGHT_ALT", "KEY_MINUS"])

    def test_backslash_mapping_other_layouts(self):
        """Verify backslash mapping on other European layouts."""
        keys_fr = resolve_character_to_keys("\\", layout="fr_FR")
        self.assertEqual(keys_fr, ["KEY_RIGHT_ALT", "KEY_8"])

        keys_es = resolve_character_to_keys("\\", layout="es_ES")
        self.assertEqual(keys_es, ["KEY_RIGHT_ALT", "KEY_QUOTE"])

        keys_it = resolve_character_to_keys("\\", layout="it_IT")
        self.assertEqual(keys_it, ["KEY_BACKSLASH"])

    def test_special_characters_mapping(self):
        """Verify various special characters resolve properly."""
        # Slash
        self.assertEqual(resolve_character_to_keys("/", layout="en_US"), ["KEY_SLASH"])
        self.assertEqual(resolve_character_to_keys("/", layout="de_DE"), ["KEY_SHIFT", "KEY_7"])

        # Semicolon
        self.assertEqual(resolve_character_to_keys(";", layout="en_US"), ["KEY_SEMICOLON"])
        self.assertEqual(resolve_character_to_keys(";", layout="de_DE"), ["KEY_SHIFT", "KEY_COMMA"])

        # Pipe
        self.assertEqual(resolve_character_to_keys("|", layout="en_US"), ["KEY_SHIFT", "KEY_BACKSLASH"])

        # Braces and brackets
        self.assertEqual(resolve_character_to_keys("[", layout="en_US"), ["KEY_LEFT_BRACE"])
        self.assertEqual(resolve_character_to_keys("]", layout="en_US"), ["KEY_RIGHT_BRACE"])
        self.assertEqual(resolve_character_to_keys("{", layout="en_US"), ["KEY_SHIFT", "KEY_LEFT_BRACE"])
        self.assertEqual(resolve_character_to_keys("}", layout="en_US"), ["KEY_SHIFT", "KEY_RIGHT_BRACE"])

    def test_alphanumeric_mapping(self):
        """Verify alphanumeric characters resolve properly."""
        self.assertEqual(resolve_character_to_keys("a", layout="en_US"), ["KEY_A"])
        self.assertEqual(resolve_character_to_keys("Z", layout="en_US"), ["KEY_SHIFT", "KEY_Z"])
        self.assertEqual(resolve_character_to_keys("5", layout="en_US"), ["KEY_5"])
        self.assertEqual(resolve_character_to_keys(" ", layout="en_US"), ["KEY_SPACE"])


class TestKeyIdentifierRegistry(unittest.TestCase):
    """Tests the registry of supported key tokens."""

    def test_backslash_identifier_present(self):
        """Verify KEY_BACKSLASH is registered and has valid HID usage."""
        self.assertIn("KEY_BACKSLASH", ALL_KEY_IDENTIFIERS)
        self.assertEqual(ALL_KEY_IDENTIFIERS["KEY_BACKSLASH"]["char"], "\\")
        self.assertEqual(ALL_KEY_IDENTIFIERS["KEY_BACKSLASH"]["hid"], 0x31)

    def test_punctuation_identifiers_present(self):
        """Verify punctuation keys are registered."""
        expected = [
            "KEY_BACKSLASH", "KEY_SLASH", "KEY_SEMICOLON", "KEY_COMMA",
            "KEY_PERIOD", "KEY_MINUS", "KEY_EQUAL", "KEY_LEFT_BRACE",
            "KEY_RIGHT_BRACE", "KEY_QUOTE", "KEY_TILDE"
        ]
        for key in expected:
            self.assertIn(key, ALL_KEY_IDENTIFIERS)
            self.assertTrue(validate_key_token(key))

    def test_token_validation(self):
        """Verify token validation function."""
        self.assertTrue(validate_key_token("KEY_BACKSLASH"))
        self.assertTrue(validate_key_token("KEY_SPACE"))
        self.assertTrue(validate_key_token("KEY_RIGHT_ALT"))
        self.assertTrue(validate_key_token("KEYPAD_ENTER"))
        self.assertFalse(validate_key_token("KEY_INVALID_NAME"))
        self.assertFalse(validate_key_token(""))


class TestCommandParsingAndEscaping(unittest.TestCase):
    """Tests command parsing and macro escaping."""

    def test_parse_at_command_kw_backslash(self):
        """Verify parsing 'AT KW \\'."""
        cmd, param = parse_at_command("AT KW \\")
        self.assertEqual(cmd, "KW")
        self.assertEqual(param, "\\")

    def test_parse_at_command_kp_backslash(self):
        """Verify parsing 'AT KP KEY_BACKSLASH'."""
        cmd, param = parse_at_command("AT KP KEY_BACKSLASH")
        self.assertEqual(cmd, "KP")
        self.assertEqual(param, "KEY_BACKSLASH")

    def test_parse_at_command_kp_de_de_backslash(self):
        """Verify parsing 'AT KP KEY_RIGHT_ALT KEY_MINUS'."""
        cmd, param = parse_at_command("AT KP KEY_RIGHT_ALT KEY_MINUS")
        self.assertEqual(cmd, "KP")
        self.assertEqual(param, "KEY_RIGHT_ALT KEY_MINUS")

    def test_macro_escaping_and_unescaping(self):
        """
        Verify that backslashes and semicolons in macros are escaped and unescaped
        without corruption or loss.
        """
        # A command that writes a file path with backslashes
        original_cmd = "KW C:\\Users\\User\\docs"
        escaped = escape_macro_string(original_cmd)
        self.assertEqual(escaped, "KW C:\\\\Users\\\\User\\\\docs")

        macro_string = format_macro([original_cmd, "KP KEY_ENTER"])
        self.assertEqual(macro_string, "AT MA KW C:\\\\Users\\\\User\\\\docs;KP KEY_ENTER;")

        # Parse macro back
        sub_cmds = unescape_macro_string("KW C:\\\\Users\\\\User\\\\docs;KP KEY_ENTER;")
        self.assertEqual(sub_cmds, ["KW C:\\Users\\User\\docs", "KP KEY_ENTER"])

    def test_macro_semicolon_escaping(self):
        """Verify escaping semicolons within macros (Issue #28 regression protection)."""
        original_cmd = "KW text;more"
        escaped = escape_macro_string(original_cmd)
        self.assertEqual(escaped, "KW text\\;more")

        macro_line = format_macro([original_cmd, "CL"])
        self.assertEqual(macro_line, "AT MA KW text\\;more;CL;")

        sub_cmds = unescape_macro_string("KW text\\;more;CL;")
        self.assertEqual(sub_cmds, ["KW text;more", "CL"])

    def test_generate_text_commands(self):
        """Verify command generation for strings containing backslashes."""
        cmds_us = generate_command_for_text("\\", layout="en_US", method="kp")
        self.assertEqual(cmds_us, ["AT KP KEY_BACKSLASH"])

        cmds_de = generate_command_for_text("\\", layout="de_DE", method="kp")
        self.assertEqual(cmds_de, ["AT KP KEY_RIGHT_ALT KEY_MINUS"])

        cmds_kw = generate_command_for_text("path\\file", layout="en_US", method="kw")
        self.assertEqual(cmds_kw, ["AT KW path\\file"])


class TestSettingsValidation(unittest.TestCase):
    """Validates existing settings files in the repository."""

    def test_existing_set_files(self):
        """Verify all repo .set files pass validation."""
        settings_dir = os.path.join(REPO_ROOT, "Settings")
        set_files = glob.glob(os.path.join(settings_dir, "*.set"))
        self.assertGreater(len(set_files), 0, "No .set files found to validate")

        for set_file in set_files:
            valid, errors = validate_settings_file(set_file)
            self.assertTrue(
                valid,
                f"Validation failed for {os.path.basename(set_file)}: {errors}"
            )

    def test_backslash_settings_file(self):
        """Verify the dedicated backslash-SpecialKeys.set configuration passes validation."""
        path = os.path.join(REPO_ROOT, "Settings", "backslash-SpecialKeys (USB).set")
        self.assertTrue(os.path.exists(path))
        valid, errors = validate_settings_file(path)
        self.assertTrue(valid, f"Validation errors in backslash-SpecialKeys.set: {errors}")


class TestLayoutExport(unittest.TestCase):
    """Tests JSON export and markdown table generation."""

    def test_export_json(self):
        """Verify JSON export contains backslash mapping."""
        json_str = export_layout_json("en_US")
        self.assertIn('"\\\\"', json_str)
        self.assertIn("KEY_BACKSLASH", json_str)

    def test_export_markdown(self):
        """Verify Markdown table generation includes KEY_BACKSLASH."""
        md_table = generate_markdown_key_table()
        self.assertIn("KEY_BACKSLASH", md_table)
        self.assertIn("KEY_RIGHT_ALT", md_table)



class TestCLIIntegration(unittest.TestCase):
    """Tests CLI command execution of keyboard_layout_generator.py."""

    def test_cli_char_backslash(self):
        import subprocess
        result = subprocess.run(
            [sys.executable, os.path.join(REPO_ROOT, "tools", "keyboard_layout_generator.py"),
             "--char", "\\", "--layout", "en_US"],
            capture_output=True, text=True, check=True
        )
        self.assertIn("KEY_BACKSLASH", result.stdout)

    def test_cli_char_backslash_de(self):
        import subprocess
        result = subprocess.run(
            [sys.executable, os.path.join(REPO_ROOT, "tools", "keyboard_layout_generator.py"),
             "--char", "\\", "--layout", "de_DE"],
            capture_output=True, text=True, check=True
        )
        self.assertIn("KEY_RIGHT_ALT KEY_MINUS", result.stdout)

    def test_cli_validate_settings(self):
        import subprocess
        set_path = os.path.join(REPO_ROOT, "Settings", "backslash-SpecialKeys (USB).set")
        result = subprocess.run(
            [sys.executable, os.path.join(REPO_ROOT, "tools", "keyboard_layout_generator.py"),
             "--validate-settings", set_path],
            capture_output=True, text=True, check=True
        )
        self.assertIn("is valid", result.stdout)


if __name__ == "__main__":
    unittest.main()

