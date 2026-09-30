#!/usr/bin/env python3
"""
FABI Slot Manager and AT Command Parser Utility.

This tool models and validates FABI slot management and AT command handling,
supporting slot renaming via the AT SN command (Issue #29).

AT SN Command specification:
- AT SN <name> : Renames current active configuration slot to <name>.
- AT SN        : Queries and outputs the name of the current active slot.
- Slot names must be 1 to 14 characters in length (MAX_NAME_LEN - 1).
- If the target slot name already exists in another slot, error 'E: name exists' is returned.
"""

import os
import re

MAX_NAME_LEN = 15
MAX_SLOTS = 10

class FabiSlotManager:
    def __init__(self):
        self.slots = {}  # slot_index -> dict(name=str, config=[commands])
        self.active_slot = 0
        self.output_buffer = []

        # Initialize with default slot
        self.slots[0] = {
            "name": "default",
            "config": [
                "AT AX 0", "AT AY 0", "AT DX 0", "AT DY 0",
                "AT MS 50", "AT AC 20", "AT MM 0", "AT RO 0",
                "AT SB 0", "AT TS 0", "AT TP 1023", "AT WS 1",
                "AT SP 1023", "AT SS 0", "AT BT 1", "AT KL en_US",
                "AT SC 0xffffff"
            ]
        }

    def print_output(self, msg):
        self.output_buffer.append(str(msg))

    def get_last_output(self):
        return self.output_buffer[-1] if self.output_buffer else ""

    def clear_output(self):
        self.output_buffer.clear()

    def slot_name_to_number(self, slot_name):
        for idx, slot in self.slots.items():
            if slot["name"] == slot_name:
                return idx
        return -1

    def list_slots(self):
        for idx in sorted(self.slots.keys()):
            self.print_output(f"Slot{idx}:{self.slots[idx]['name']}")
        self.print_output("OK")

    def save_slot(self, slot_name):
        slot_name = slot_name.strip()
        if not slot_name or len(slot_name) >= MAX_NAME_LEN:
            self.print_output("?")
            return False

        existing_idx = self.slot_name_to_number(slot_name)
        if existing_idx != -1:
            # Overwrite existing
            self.slots[existing_idx]["name"] = slot_name
            self.print_output("OK")
            return True

        if len(self.slots) >= MAX_SLOTS:
            self.print_output("E: eeprom full")
            return False

        new_idx = max(self.slots.keys()) + 1 if self.slots else 0
        self.slots[new_idx] = {
            "name": slot_name,
            "config": list(self.slots[self.active_slot]["config"])
        }
        self.print_output("OK")
        return True

    def load_slot(self, slot_name):
        slot_name = slot_name.strip()
        idx = self.slot_name_to_number(slot_name)
        if idx != -1:
            self.active_slot = idx
            self.print_output("OK")
            return True
        else:
            self.print_output("E: not found")
            return False

    def rename_slot(self, new_name):
        """
        Rename current active slot to new_name.
        Returns:
            1 if success
            -1 if name already exists on another slot
            0 if invalid name or active slot invalid
        """
        if not new_name:
            return 0

        new_name = new_name.strip()
        if len(new_name) == 0 or len(new_name) >= MAX_NAME_LEN:
            return 0

        if self.active_slot not in self.slots:
            return 0

        existing_idx = self.slot_name_to_number(new_name)
        if existing_idx != -1 and existing_idx != self.active_slot:
            return -1

        self.slots[self.active_slot]["name"] = new_name
        return 1

    def execute_command(self, cmd_line):
        """
        Execute an AT command line, e.g. 'AT SN mouse', 'AT SN', 'AT LI', etc.
        """
        cmd_line = cmd_line.strip()
        if not cmd_line.startswith("AT"):
            self.print_output("?")
            return

        rest = cmd_line[2:].strip()
        if not rest:
            self.print_output("OK")
            return

        parts = rest.split(maxsplit=1)
        cmd = parts[0].upper()
        arg = parts[1] if len(parts) > 1 else ""

        if cmd == "SN":
            arg = arg.strip()
            if arg:
                if len(arg) < MAX_NAME_LEN:
                    res = self.rename_slot(arg)
                    if res == 1:
                        self.print_output("OK")
                    elif res == -1:
                        self.print_output("E: name exists")
                    else:
                        self.print_output("?")
                else:
                    self.print_output("?")
            else:
                # Query current active slot name
                self.print_output(self.slots[self.active_slot]["name"])

        elif cmd == "SA":
            self.save_slot(arg)

        elif cmd == "LO":
            self.load_slot(arg)

        elif cmd == "LI":
            self.list_slots()

        elif cmd == "NE":
            # Next slot
            keys = sorted(self.slots.keys())
            curr_pos = keys.index(self.active_slot)
            next_pos = (curr_pos + 1) % len(keys)
            self.active_slot = keys[next_pos]
            self.print_output("OK")

        else:
            self.print_output("?")
