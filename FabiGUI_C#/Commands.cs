﻿
/* 
   Supported AT-commands:  
   (sent via serial interface, 115200 baud, using spaces between parameters.  Enter (<cr>, ASCII-code 0x0d) finishes a command)
   
          AT                returns "OK"
          AT ID             returns identification string (e.g. "Fabi V2.3")
          AT BM <uint>      puts button into programming mode (e.g. "AT BM 2" -> next AT-command defines the new function for button 2)
                            for the FABI, there are 11 buttons available (9 physical buttons, 2 virtual functions - sip / puff) 

          AT MA <string>  execute a command macro containing multiple commands (separated by semicolon) 
                          example: "AT MA MX 100;MY 100;CL;"  use backslash to mask semicolon: "AT MA KW \;;CL;" writes a semicolon and then clicks left 
          AT WA <uint>    wait (given in milliseconds, useful for macro commands)

    Commands for changing settings:
    
          AT WS <uint>    set mouse wheel stepsize (e.g. "AT WS 3" sets the wheel stepsize to 3 rows)
          AT TS <uint>    threshold for sip action  (0-512)
          AT TP <uint>    threshold for puff action (512-1023)
          AT TT <uint>    threshold time between short and long press action (5000=disable)
          AT AP <uint>    antitremor press time (1-500)
          AT AR <uint>    antitremor release time (1-500)
          AT AI <uint>    antitremor idle time (1-500)

    USB HID commands:
      
          AT CL             click left mouse button  
          AT CR             click right mouse button  
          AT CM             click middle mouse button  
          AT CD             click double with left mouse button

          AT PL             press/hold the left mouse button  
          AT PR             press/hold the right mouse button
          AT PM             press/hold the middle mouse button 
  
          AT RL             release the left mouse button  
          AT RR             release the right mouse button
          AT RM             release the middle mouse button 
          
          AT WU             move mouse wheel up  
          AT WD             move mouse wheel down  
   
          AT MX <int>       move mouse in x direction (e.g. "AT MX 4" moves cursor 4 pixels to the right)  
          AT MY <int>       move mouse in y direction (e.g. "AT MY -10" moves cursor 10 pixels up)  

          AT KW <string>    keyboard write string (e.g." AT KW Hello!" writes "Hello!")    
          AT KP <string>    key press: press/hold keys identifier 
                            (e.g. "AT KP KEY_UP" presses the "Cursor-Up" key, "AT KP KEY_CTRL KEY_ALT KEY_DELETE" presses all three keys)
                            for a list of supported key idientifier strings see below ! 
                            
          AT KR <string>    key release: releases all keys identified in the string    
          AT RA             release all: releases all currently pressed keys and buttons    
          
    Housekeeping commands:

          AT SA <string>  save settings and current button modes to slot under given name 
                          a new slot will be appended (e.g. AT SA mouse1)
          AT LO <string>  load button modes from eeprom slot (e.g. AT LO mouse1 -> loads profile named "mouse1")
          AT LA           load all slots (displays names and settings of all stored slots) 
          AT LI           list all saved slot names 
          AT NE           next slot will be loaded (wrap around after last slot)
          AT DE           delete EEPROM content (delete all stored slots)
          AT NC           no command (idle operation)
          AT E0           turn echo off (no debug output on serial console, default and GUI compatible)
          AT E1           turn echo on (debug output on serial console)
          AT SR           start periodic reporting analog values (A0) over serial (starting with "VALUES:") 
          AT ER           end reporting analog values
          AT FR           report free EEPROM bytes in % (starting with "FREE:") 

          

   supported key identifiers for key press command (AT KP):
 
    KEY_A   KEY_B   KEY_C   KEY_D    KEY_E   KEY_F   KEY_G   KEY_H   KEY_I   KEY_J    KEY_K    KEY_L
    KEY_M   KEY_N   KEY_O   KEY_P    KEY_Q   KEY_R   KEY_S   KEY_T   KEY_U   KEY_V    KEY_W    KEY_X 
    KEY_Y   KEY_Z   KEY_1   KEY_2    KEY_3   KEY_4   KEY_5   KEY_6   KEY_7   KEY_8    KEY_9    KEY_0
    KEY_F1  KEY_F2  KEY_F3  KEY_F4   KEY_F5  KEY_F6  KEY_F7  KEY_F8  KEY_F9  KEY_F10  KEY_F11  KEY_F12	
    KEY_F13 KEY_F14 KEY_F15 KEY_F16  KEY_F17 KEY_F18 KEY_F19 KEY_F20 KEY_F21 KEY_F22  KEY_F23  KEY_F24  
    
    KEY_RIGHT   KEY_LEFT       KEY_DOWN        KEY_UP      KEY_ENTER    KEY_ESC   KEY_BACKSPACE   KEY_TAB	
    KEY_HOME    KEY_PAGE_UP    KEY_PAGE_DOWN   KEY_DELETE  KEY_INSERT   KEY_END	  KEY_NUM_LOCK    KEY_SCROLL_LOCK
    KEY_SPACE   KEY_CAPS_LOCK  KEY_PAUSE       KEY_SHIFT   KEY_CTRL     KEY_ALT   KEY_RIGHT_ALT   KEY_GUI 
    KEY_RIGHT_GUI
        
*/



using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Linq;


namespace FabiGUI
{
    public partial class FabiGUI
    {
        const int PARTYPE_NONE = 0;
        const int PARTYPE_UINT = 1;
        const int PARTYPE_INT =  2;
        const int PARTYPE_STRING = 3;

        const int COMBOENTRY_YES = 1;
        const int COMBOENTRY_NO = 0;

        const int GUITYPE_STANDARD  = 0;
        const int GUITYPE_INTFIELD  = 1;
        const int GUITYPE_TEXTFIELD = 2;
        const int GUITYPE_KEYSELECT = 3;
        const int GUITYPE_SLIDER    = 4;
        const int GUITYPE_BOOLEAN   = 5;
        const int GUITYPE_GENERIC   = 6;

        const string PREFIX_FABI_VERSION = "FABI ";
        const string PREFIX_REPORT_VALUES = "VALUES:";
        const string PREFIX_SLOT_NAME = "SLOT:";
        const string PREFIX_AT_COMMAND = "AT ";
        const string PREFIX_END_OF_SLOTS = "END";
        const string PREFIX_FREE_EEPROM = "FREE EEPROM";


        public AllCommands allCommands = new AllCommands();
        public void initCommands()
        {
            allCommands.add(new Command("AT ID", PARTYPE_NONE, "Get Id String", COMBOENTRY_NO, GUITYPE_STANDARD));
            allCommands.add(new Command("AT BM", PARTYPE_UINT, "Update Button mode", COMBOENTRY_NO, GUITYPE_STANDARD));
            allCommands.add(new Command("AT CL", PARTYPE_NONE, "Click Left Mouse Button", COMBOENTRY_YES, GUITYPE_STANDARD));
            allCommands.add(new Command("AT CR", PARTYPE_NONE, "Click Right Mouse Button", COMBOENTRY_YES, GUITYPE_STANDARD));
            allCommands.add(new Command("AT CM", PARTYPE_NONE, "Click Middle Mouse Button", COMBOENTRY_YES, GUITYPE_STANDARD));
            allCommands.add(new Command("AT CD", PARTYPE_NONE, "Double Click Left Mouse Button", COMBOENTRY_YES, GUITYPE_STANDARD));
            allCommands.add(new Command("AT PL", PARTYPE_NONE, "Hold Left Mouse Button", COMBOENTRY_YES, GUITYPE_STANDARD));
            allCommands.add(new Command("AT PR", PARTYPE_NONE, "Hold Right Mouse Button", COMBOENTRY_YES, GUITYPE_STANDARD));
            allCommands.add(new Command("AT PM", PARTYPE_NONE, "Hold Middle Mouse Button", COMBOENTRY_YES, GUITYPE_STANDARD));
            allCommands.add(new Command("AT RL", PARTYPE_NONE, "Release Left Mouse Button", COMBOENTRY_NO, GUITYPE_STANDARD));
            allCommands.add(new Command("AT RR", PARTYPE_NONE, "Release Right Mouse Button", COMBOENTRY_NO, GUITYPE_STANDARD));
            allCommands.add(new Command("AT RM", PARTYPE_NONE, "Release Middle Mouse Button", COMBOENTRY_NO, GUITYPE_STANDARD));
            allCommands.add(new Command("AT WU", PARTYPE_NONE, "Wheel Up", COMBOENTRY_YES, GUITYPE_STANDARD));
            allCommands.add(new Command("AT WD", PARTYPE_NONE, "Wheel Down", COMBOENTRY_YES, GUITYPE_STANDARD));
            allCommands.add(new Command("AT WS", PARTYPE_UINT, "Mouse Wheel Step Size", COMBOENTRY_NO, GUITYPE_STANDARD));
            allCommands.add(new Command("AT MX", PARTYPE_INT, "Move Mouse X", COMBOENTRY_YES, GUITYPE_INTFIELD));
            allCommands.add(new Command("AT MY", PARTYPE_INT, "Move Mouse Y", COMBOENTRY_YES, GUITYPE_INTFIELD));
            allCommands.add(new Command("AT KW", PARTYPE_STRING, "Write Text", COMBOENTRY_YES, GUITYPE_TEXTFIELD));
            allCommands.add(new Command("AT KP", PARTYPE_STRING, "Press Keys", COMBOENTRY_YES, GUITYPE_KEYSELECT));
            allCommands.add(new Command("AT KR", PARTYPE_STRING, "Release Keys", COMBOENTRY_NO, GUITYPE_STANDARD));
            allCommands.add(new Command("AT RA", PARTYPE_NONE, "Release All", COMBOENTRY_NO, GUITYPE_STANDARD));
            allCommands.add(new Command("AT SA", PARTYPE_STRING, "Save Slot", COMBOENTRY_NO, GUITYPE_STANDARD));
            allCommands.add(new Command("AT LO", PARTYPE_STRING, "Load Slot by Name", COMBOENTRY_YES, GUITYPE_TEXTFIELD));
            allCommands.add(new Command("AT LA", PARTYPE_NONE, "Load All", COMBOENTRY_NO, GUITYPE_STANDARD));
            allCommands.add(new Command("AT LI", PARTYPE_NONE, "List Slots", COMBOENTRY_NO, GUITYPE_STANDARD));
            allCommands.add(new Command("AT NE", PARTYPE_NONE, "Load Next Slot", COMBOENTRY_YES, GUITYPE_STANDARD));
            allCommands.add(new Command("AT DE", PARTYPE_NONE, "Delete all configurations", COMBOENTRY_NO, GUITYPE_STANDARD));
            allCommands.add(new Command("AT NC", PARTYPE_NONE, "No Command", COMBOENTRY_YES, GUITYPE_STANDARD));
            allCommands.add(new Command("AT SR", PARTYPE_NONE, "Start Rawvalue reports", COMBOENTRY_NO, GUITYPE_STANDARD));
            allCommands.add(new Command("AT ER", PARTYPE_NONE, "End Rawvalue reports", COMBOENTRY_NO, GUITYPE_STANDARD));
            allCommands.add(new Command("AT TS", PARTYPE_UINT, "Threshold Sip", COMBOENTRY_NO, GUITYPE_SLIDER));
            allCommands.add(new Command("AT TP", PARTYPE_UINT, "Threshold Puff", COMBOENTRY_NO, GUITYPE_SLIDER));
            allCommands.add(new Command("AT MA", PARTYPE_STRING, "Execute Command Macro", COMBOENTRY_YES, GUITYPE_TEXTFIELD));
            allCommands.add(new Command("AT WA", PARTYPE_UINT, "Wait (milliseconds)", COMBOENTRY_NO, GUITYPE_STANDARD));
            allCommands.add(new Command("AT TT", PARTYPE_UINT, "Threshold Time Longpress", COMBOENTRY_NO, GUITYPE_SLIDER));
            allCommands.add(new Command("AT AP", PARTYPE_UINT, "Antitremor Press time", COMBOENTRY_NO, GUITYPE_SLIDER));
            allCommands.add(new Command("AT AR", PARTYPE_UINT, "Antitremor Release time", COMBOENTRY_NO, GUITYPE_SLIDER));
            allCommands.add(new Command("AT AI", PARTYPE_UINT, "Antitremor Idle time", COMBOENTRY_NO, GUITYPE_SLIDER));
        }


        static public List<CommandGuiLink> commandGuiLinks = new List<CommandGuiLink>();

        public void initCommandGuiLinks()
        {
            commandGuiLinks.Add(new CommandGuiLink("AT TT", timeThresholdBar, timeThresholdLabel, "5000"));
            commandGuiLinks.Add(new CommandGuiLink("AT AP", antiTremorPressBar, antiTremorPressLabel, "5"));
            commandGuiLinks.Add(new CommandGuiLink("AT AR", antiTremorReleaseBar, antiTremorReleaseLabel, "2"));
            commandGuiLinks.Add(new CommandGuiLink("AT AI", antiTremorIdleBar, antiTremorIdleLabel, "1"));
            commandGuiLinks.Add(new CommandGuiLink("AT TS", sipThresholdBar, sipThresholdLabel, "0"));
            commandGuiLinks.Add(new CommandGuiLink("AT TP", puffThresholdBar, puffThresholdLabel, "1023"));
            commandGuiLinks.Add(new CommandGuiLink("AT BM 01", Button1FunctionBox, Button1ParameterText, Button1NumericParameter, "AT KP KEY_UP "));
            commandGuiLinks.Add(new CommandGuiLink("AT BM 02", Button2FunctionBox, Button2ParameterText, Button2NumericParameter, "AT KP KEY_DOWN "));
            commandGuiLinks.Add(new CommandGuiLink("AT BM 03", Button3FunctionBox, Button3ParameterText, Button3NumericParameter, "AT KP KEY_LEFT "));
            commandGuiLinks.Add(new CommandGuiLink("AT BM 04", Button4FunctionBox, Button4ParameterText, Button4NumericParameter, "AT KP KEY_RIGHT "));
            commandGuiLinks.Add(new CommandGuiLink("AT BM 05", Button5FunctionBox, Button5ParameterText, Button5NumericParameter, "AT KP KEY_SPACE "));
            commandGuiLinks.Add(new CommandGuiLink("AT BM 06", Button6FunctionBox, Button6ParameterText, Button6NumericParameter, "AT KP KEY_ENTER "));
            commandGuiLinks.Add(new CommandGuiLink("AT BM 07", Button7FunctionBox, Button7ParameterText, Button7NumericParameter, "AT NE"));
            commandGuiLinks.Add(new CommandGuiLink("AT BM 08", Button8FunctionBox, Button8ParameterText, Button8NumericParameter, "AT PL"));
            commandGuiLinks.Add(new CommandGuiLink("AT BM 09", Button9FunctionBox, Button9ParameterText, Button9NumericParameter, "AT PR"));
            commandGuiLinks.Add(new CommandGuiLink("AT BM 10", SipFunctionMenu, SipParameterText, SipNumericParameter, "AT WU"));
            commandGuiLinks.Add(new CommandGuiLink("AT BM 11", PuffFunctionMenu, PuffParameterText, PuffNumericParameter, "AT WD"));            
        }

        String[] keyOptions = {    "Clear Keycodes!", "KEY_A","KEY_B","KEY_C","KEY_D","KEY_E","KEY_F","KEY_G","KEY_H","KEY_I","KEY_J","KEY_K","KEY_L",
                                   "KEY_M","KEY_N","KEY_O","KEY_P","KEY_Q","KEY_R","KEY_S","KEY_T","KEY_U","KEY_V","KEY_W","KEY_X",
                                   "KEY_Y","KEY_Z","KEY_1","KEY_2","KEY_3","KEY_4","KEY_5","KEY_6","KEY_7","KEY_8","KEY_9","KEY_0",
                                   "KEY_F1","KEY_F2","KEY_F3","KEY_F4","KEY_F5","KEY_F6","KEY_F7","KEY_F8","KEY_F9","KEY_F10","KEY_F11","KEY_F12",	
	                               "KEY_F13","KEY_F14","KEY_F15","KEY_F16","KEY_F17","KEY_F18","KEY_F19","KEY_F20","KEY_F21","KEY_F22","KEY_F23","KEY_F24",
                                   "KEY_UP","KEY_DOWN","KEY_LEFT","KEY_RIGHT","KEY_SPACE","KEY_ENTER",
                                   "KEY_ALT","KEY_BACKSPACE","KEY_CAPS_LOCK","KEY_CTRL","KEY_DELETE","KEY_END","KEY_ESC","KEY_GUI",
                                   "KEY_HOME","KEY_INSERT","KEY_NUM_LOCK","KEY_PAGE_DOWN","KEY_PAGE_UP","KEY_PAUSE","KEY_RIGHT_ALT",
                                   "KEY_RIGHT_GUI","KEY_SCROLL_LOCK","KEY_SHIFT","KEY_TAB"
                              };

        public void sendIDCommand()
        {
            sendCmd("AT ID");
        }

        public void sendNextCommand()
        {
            sendCmd("AT NE");
        }

        public void sendLoadAllCommand()
        {
            sendCmd("AT LA");
        }

        public void sendEndReportingCommand()
        {
            sendCmd("AT ER");
        }

        public void sendStartReportingCommand()
        {
            sendCmd("AT SR");
        }
        public void sendFreeMemCommand()
        {
            sendCmd("AT FR");
        }

        public void sendSaveSlotCommands(String slotname)
        {
            sendCmd("AT SA " + slotname);
        }

        public void sendClearCommand()
        {
            sendCmd("AT DE");
        }


        public class Slot
        {
            public String slotName;
            public List<String> settingStrings = new List<String>();
            public Slot()
            {
                this.slotName = "cursorkeys";
                init();
            }
            public Slot(String slotName)
            {
                this.slotName = slotName;
                init();
            }
            void init() {
                foreach (CommandGuiLink cgl in commandGuiLinks)
                {
                    switch (cgl.type)
                    {
                        case GUITYPE_GENERIC:
                            settingStrings.Add(cgl.cmd); settingStrings.Add(cgl.def);
                            break;
                        case GUITYPE_BOOLEAN:
                        case GUITYPE_SLIDER:
                            settingStrings.Add(cgl.cmd + " " + cgl.def);
                            break;
                    }
                }
            }
        };

        private String buildCommandString(String cmdDescription, String stringParam, int intParam)
        {
            String str = allCommands.getCommand(cmdDescription);
            switch (allCommands.getParameterType(cmdDescription))
            {
                case PARTYPE_UINT:
                case PARTYPE_INT: str += " " + intParam; break;
                case PARTYPE_STRING: str += " " + stringParam; break;
            }
            return (str);
        }


        public void sendApplyCommands()
        {

            foreach (CommandGuiLink cgl in commandGuiLinks)
            {
                switch (cgl.type)
                {
                    case GUITYPE_GENERIC:
                        sendCmd(cgl.cmd);
                        sendCmd(buildCommandString(cgl.cb.Text, cgl.tb.Text, (int)cgl.nud.Value));
                        break;
                    case GUITYPE_SLIDER:
                        sendCmd(cgl.cmd + " " + cgl.tl.Text);
                        break;
                    case GUITYPE_BOOLEAN:
                        if (cgl.rb1.Checked)
                            sendCmd(cgl.cmd + " 1");
                        else sendCmd(cgl.cmd + " 0");
                        break;
                }
            }
        }


        public class Command
        {
            public String cmd;
            public int parameterType; // 0=no params, 1=uint paramter, 3=int parameter, 4=string parameter
            public String commandDescription;
            public int comboEntry;
            public int guiType;

            public Command(String cmd, int parameterType, String commandDescription, int comboEntry, int guiType)
            {
                this.cmd = cmd;
                this.parameterType = parameterType;
                this.commandDescription = commandDescription;
                this.comboEntry = comboEntry;
                this.guiType = guiType;
            }
        };

        public class AllCommands
        {
            List<Command> commandList = new List<Command>();

            public void add(Command command)
            {
                commandList.Add(command);
            }
            public String getCommandDescription(int index)
            {
                if ((index>=0)&&(index<commandList.Count))
                    return(commandList[index].commandDescription);
                return (null);
            }
            public int getParameterType(String commandDescription)
            {
                foreach (Command c in commandList)
                    if (c.commandDescription.Equals(commandDescription)) return (c.parameterType);
                return (-1);
            }
            public int getGuiTypeFromDescription(String commandDescription)
            {
                foreach (Command c in commandList)
                    if (c.commandDescription.Equals(commandDescription)) return (c.guiType);
                return (-1);
            }
            public int getGuiTypeFromCommand(String command)
            {
                foreach (Command c in commandList)
                    if (c.cmd.Equals(command)) return (c.guiType);
                return (-1);
            }
            public String getCommand(String commandDescription)
            {
                foreach (Command c in commandList)
                    if (c.commandDescription.Equals(commandDescription)) return (c.cmd);
                return (null);
            }

            public int getComboEntry(String commandDescription)
            {
                foreach (Command c in commandList)
                    if (c.commandDescription.Equals(commandDescription)) return (c.comboEntry);
                return (-1);
            }
            public int getComboEntry(int index)
            {
                if ((index >= 0) && (index < commandList.Count))
                    return (commandList[index].comboEntry);
                return (-1);
            }
            public int getSelectionIndex(int index)
            {
                int selectionIndex = 0;
                if ((index >= 0) && (index < commandList.Count))
                {
                    for (int i = 0; i < index; i++)
                        if (getComboEntry(i) == COMBOENTRY_YES)
                            selectionIndex++;
                }
                return (selectionIndex);
            }
            public int getSelectionIndex(string cmd)
            {
                int selectionIndex = -1;

                for (int i = 0;i < commandList.Count;i++)
                {
                    if (getComboEntry(i) == COMBOENTRY_YES)
                        selectionIndex++;
                    if (cmd.Equals(commandList[i].cmd)) return (selectionIndex);
                }
                return (selectionIndex);
            }
            public int length()
            {
                return commandList.Count;
            }

        };


        public class CommandGuiLink
        {
            public int type;
            public String cmd;
            public String def;

            public ComboBox cb;
            public TextBox tb;
            public NumericUpDown nud;
            public TrackBar tr;
            public Label tl;
            public RadioButton rb1;
            public RadioButton rb2;

            public CommandGuiLink(String cmd, ComboBox cb, TextBox tb, NumericUpDown nud, String def)
            {
                this.type = GUITYPE_GENERIC;
                this.cmd = cmd;
                this.cb = cb;
                this.tb = tb;
                this.nud = nud;
                this.def = def;
            }

            public CommandGuiLink(String cmd, TrackBar tr, Label tl, String def)
            {
                this.type = GUITYPE_SLIDER;
                this.cmd = cmd;
                this.tr = tr;
                this.tl = tl;
                this.def = def;
            }
            public CommandGuiLink(String cmd, RadioButton rb1, RadioButton rb2, String def)
            {
                this.type = GUITYPE_BOOLEAN;
                this.cmd = cmd;
                this.rb1 = rb1;
                this.rb2 = rb2;
                this.def = def;
            }
        };

    }
}
