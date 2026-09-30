#!/usr/bin/env python3
"""
Unit tests for FABI AT SN command (Issue #29: Add AT command for renaming slots).

Verifies:
1. Firmware C++ implementation consistency:
   - enum atCommands in commands.h matches atCommands[] in commands.cpp exactly
   - CMD_SN and {"SN", PARTYPE_STRING} presence and ordering
   - renameSlot declaration in eeprom.h and implementation in eeprom.cpp
   - Error messages: ERRORMESSAGE_NAME_EXISTS defined and used
2. User documentation in English and German:
   - Fabi User Manual.md includes AT SN command
   - Fabi Anwendungsanleitung.md includes AT SN command
3. Functional slot renaming logic via FabiSlotManager:
   - AT SN queries current slot name
   - AT SN <name> renames current slot
   - Leading/trailing whitespace trimming
   - Length limits (1..14 characters)
   - Rejection of conflicting duplicate names (E: name exists)
   - Re-naming to same name succeeds
   - AT LI lists updated slot name
   - AT LO <new_name> loads renamed slot successfully
"""

import os
import re
import sys
import unittest

REPO_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
sys.path.insert(0, REPO_ROOT)

from tools.slot_manager import FabiSlotManager, MAX_NAME_LEN

class TestAtSnCommandFirmware(unittest.TestCase):
    def setUp(self):
        self.commands_h_path = os.path.join(REPO_ROOT, "FabiWare", "src", "commands.h")
        self.commands_cpp_path = os.path.join(REPO_ROOT, "FabiWare", "src", "commands.cpp")
        self.eeprom_h_path = os.path.join(REPO_ROOT, "FabiWare", "src", "eeprom.h")
        self.eeprom_cpp_path = os.path.join(REPO_ROOT, "FabiWare", "src", "eeprom.cpp")

    def test_commands_h_and_cpp_enum_array_consistency(self):
        """Verify that enum atCommands and atCommands[] array have identical entries and order."""
        with open(self.commands_h_path, "r", encoding="utf-8") as f:
            h_content = f.read()
        with open(self.commands_cpp_path, "r", encoding="utf-8") as f:
            cpp_content = f.read()

        enum_match = re.search(r"enum atCommands\s*\{(.*?)\};", h_content, re.DOTALL)
        self.assertIsNotNone(enum_match, "enum atCommands not found in commands.h")
        enum_raw = enum_match.group(1)
        enum_items = [x.strip() for x in re.sub(r"#.*", "", enum_raw).split(",") if x.strip() and x.strip() != "NUM_COMMANDS"]

        array_match = re.search(r"atCommands\[\]\s*PROGMEM\s*=\s*\{(.*?)\};", cpp_content, re.DOTALL)
        self.assertIsNotNone(array_match, "atCommands[] array not found in commands.cpp")
        array_raw = array_match.group(1)
        array_items = re.findall(r'\{\s*"([^"]+)"\s*,\s*([^}]+)\s*\}', array_raw)

        self.assertEqual(len(enum_items), len(array_items),
                         f"Enum count ({len(enum_items)}) must match Array count ({len(array_items)})")

        self.assertIn("CMD_SN", enum_items, "CMD_SN missing from enum atCommands")
        sn_idx = enum_items.index("CMD_SN")
        cmd_str, partype = array_items[sn_idx]
        self.assertEqual(cmd_str, "SN", f"Array entry at index {sn_idx} must be 'SN', got {cmd_str}")
        self.assertEqual(partype.strip(), "PARTYPE_STRING", f"SN partype must be PARTYPE_STRING, got {partype}")

    def test_commands_cpp_implements_cmd_sn(self):
        """Verify that performCommand handles CMD_SN with proper response and error handling."""
        with open(self.commands_cpp_path, "r", encoding="utf-8") as f:
            cpp_content = f.read()

        self.assertIn("case CMD_SN:", cpp_content, "performCommand must handle case CMD_SN:")
        self.assertIn("renameSlot", cpp_content, "performCommand must call renameSlot")
        self.assertIn("ERRORMESSAGE_NAME_EXISTS", cpp_content, "ERRORMESSAGE_NAME_EXISTS must be defined and used")

    def test_eeprom_declares_and_implements_rename_slot(self):
        """Verify that eeprom.h declares renameSlot and eeprom.cpp implements it."""
        with open(self.eeprom_h_path, "r", encoding="utf-8") as f:
            h_content = f.read()
        with open(self.eeprom_cpp_path, "r", encoding="utf-8") as f:
            cpp_content = f.read()

        self.assertIn("int8_t renameSlot(char const * newName);", h_content,
                      "renameSlot declaration missing from eeprom.h")
        self.assertIn("int8_t renameSlot(char const * newName)", cpp_content,
                      "renameSlot implementation missing from eeprom.cpp")
        self.assertIn("slotnameToNumber", cpp_content,
                      "renameSlot must check for duplicate slot names using slotnameToNumber")
        self.assertIn("saveToEEPROMSlotNumber", cpp_content,
                      "renameSlot must persist changes using saveToEEPROMSlotNumber")


class TestAtSnCommandDocumentation(unittest.TestCase):
    def test_english_user_manual_documents_sn(self):
        """Verify that the English user manual contains documentation for the AT SN command."""
        manual_path = os.path.join(REPO_ROOT, "Documentation", "UserManual", "Fabi User Manual.md")
        with open(manual_path, "r", encoding="utf-8") as f:
            content = f.read()

        self.assertTrue(re.search(r"\|\s*SN\s*`<string>`\s*\|.*slot\s*name.*\|", content, re.IGNORECASE),
                        "AT SN command not found in English user manual table")

    def test_german_user_manual_documents_sn(self):
        """Verify that the German user manual contains documentation for the AT SN command."""
        manual_path = os.path.join(REPO_ROOT, "Documentation", "UserManual", "Fabi Anwendungsanleitung.md")
        with open(manual_path, "r", encoding="utf-8") as f:
            content = f.read()

        self.assertTrue(re.search(r"\|\s*SN\s*<string>\s*\|.*Slot.*umbenennen.*\|", content, re.IGNORECASE),
                        "AT SN command not found in German user manual table")


class TestAtSnCommandLogic(unittest.TestCase):
    def setUp(self):
        self.mgr = FabiSlotManager()

    def test_query_active_slot_name(self):
        """AT SN without argument outputs the current active slot name."""
        self.mgr.clear_output()
        self.mgr.execute_command("AT SN")
        self.assertEqual(self.mgr.get_last_output(), "default")

    def test_rename_active_slot_success(self):
        """AT SN <name> renames active slot and returns OK."""
        self.mgr.clear_output()
        self.mgr.execute_command("AT SN mouse1")
        self.assertEqual(self.mgr.get_last_output(), "OK")

        # Query to verify new name
        self.mgr.clear_output()
        self.mgr.execute_command("AT SN")
        self.assertEqual(self.mgr.get_last_output(), "mouse1")

    def test_rename_slot_whitespace_handling(self):
        """AT SN trims leading and trailing whitespace."""
        self.mgr.clear_output()
        self.mgr.execute_command("AT SN    gaming   ")
        self.assertEqual(self.mgr.get_last_output(), "OK")

        self.mgr.clear_output()
        self.mgr.execute_command("AT SN")
        self.assertEqual(self.mgr.get_last_output(), "gaming")

    def test_rename_conflict_duplicate_name(self):
        """AT SN returns 'E: name exists' when renaming to a name of another existing slot."""
        # Create second slot
        self.mgr.execute_command("AT SA keys")
        self.mgr.execute_command("AT LO keys")
        self.assertEqual(self.mgr.slots[self.mgr.active_slot]["name"], "keys")

        # Attempt to rename 'keys' slot to 'default' (which is in slot 0)
        self.mgr.clear_output()
        self.mgr.execute_command("AT SN default")
        self.assertEqual(self.mgr.get_last_output(), "E: name exists")
        # Slot name should remain unchanged
        self.assertEqual(self.mgr.slots[self.mgr.active_slot]["name"], "keys")

    def test_rename_to_same_name_succeeds(self):
        """AT SN to the current slot's own name succeeds."""
        self.mgr.clear_output()
        self.mgr.execute_command("AT SN default")
        self.assertEqual(self.mgr.get_last_output(), "OK")
        self.assertEqual(self.mgr.slots[self.mgr.active_slot]["name"], "default")

    def test_rename_name_length_limit(self):
        """Names of MAX_NAME_LEN or longer are rejected with '?'."""
        long_name = "x" * MAX_NAME_LEN
        self.mgr.clear_output()
        self.mgr.execute_command(f"AT SN {long_name}")
        self.assertEqual(self.mgr.get_last_output(), "?")

        # Exactly 14 characters should succeed
        valid_14_char = "a" * (MAX_NAME_LEN - 1)
        self.mgr.clear_output()
        self.mgr.execute_command(f"AT SN {valid_14_char}")
        self.assertEqual(self.mgr.get_last_output(), "OK")
        self.assertEqual(self.mgr.slots[self.mgr.active_slot]["name"], valid_14_char)

    def test_list_and_load_after_rename(self):
        """After renaming, AT LI lists the new name and AT LO loads it properly."""
        self.mgr.execute_command("AT SN slot_abc")
        self.mgr.execute_command("AT SA slot_xyz")

        # List slots
        self.mgr.clear_output()
        self.mgr.execute_command("AT LI")
        output = "\n".join(self.mgr.output_buffer)
        self.assertIn("Slot0:slot_abc", output)
        self.assertIn("slot_xyz", output)

        # Switch to slot_xyz
        self.mgr.execute_command("AT LO slot_xyz")
        self.assertEqual(self.mgr.slots[self.mgr.active_slot]["name"], "slot_xyz")

        # Switch back to slot_abc
        self.mgr.execute_command("AT LO slot_abc")
        self.assertEqual(self.mgr.slots[self.mgr.active_slot]["name"], "slot_abc")


if __name__ == "__main__":
    unittest.main()
