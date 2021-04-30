

/* 
     FabiGUI - Graphical user Interface for the FABI box
     AsTeRICS Foundation - http://www.asterics-foundation.org
   
     for a list of supported AT commands, see commands.cs
   
*/



using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Windows.Forms;
using System.IO;
using System.Text.RegularExpressions;



namespace FabiGUI
{
    public partial class FabiGUI : Form
    {
        const string VERSION_STRING = "2.5";
        const int SPECIALMODE_CHANGE_STEP = 5;
        const int PRESSURE_CHANGE_STEP = 1;

        Boolean readDone = false;
        static int slotCounter = 0;
        static int actSlot = 0;
        static String actSlotColor = "0x00ff00";  // default slot color


        public delegate void StringReceivedDelegate(string newLine);
        public StringReceivedDelegate stringReceivedDelegate;

        public delegate void ClickDelegate(object sender, EventArgs e);
        ClickDelegate functionPointer;

        System.Windows.Forms.Timer clickTimer = new System.Windows.Forms.Timer();
        System.Windows.Forms.Timer IdTimer = new System.Windows.Forms.Timer();


        const int MAX_SLOTS = 10;
        public List<Slot> slots = new List<Slot>();

        public void initSlots()
        {
            slots.Clear();
            slots.Add(new Slot());
            slotNames.Items.Clear();
            slotNames.Items.Add("cursorkeys");
        }

        public void storeSlot(int slotNumber)
        {
            slots[slotNumber].settingStrings.Clear();
            slots[slotNumber].slotName = slotNames.Text;
            slotNames.Items.Clear();
            foreach (Slot s in slots) slotNames.Items.Add(s.slotName); 


            foreach (CommandGuiLink guiLink in commandGuiLinks)
            {
                String actSettingString =guiLink.cmd;
                if (actSettingString.StartsWith("AT BM "))
                {
                 //   guiLink.cb.SelectedIndex
                    slots[slotNumber].settingStrings.Add(actSettingString);
                    actSettingString=allCommands.getCommand(guiLink.cb.Text);
                }
                switch (allCommands.getGuiTypeFromCommand(actSettingString))
                {
                    case GUITYPE_INTFIELD: actSettingString+=(" "+guiLink.nud.Value); break;
                    case GUITYPE_KEYSELECT:
                    case GUITYPE_TEXTFIELD: actSettingString+=(" "+guiLink.tb.Text); break;
                    case GUITYPE_COMBO_ONLY: actSettingString += (" " + guiLink.cb.Text); break;
                    case GUITYPE_COMBO_INDEX: actSettingString += (" " + guiLink.cb.SelectedIndex); break;
                    case GUITYPE_SLIDER: actSettingString += (" " + guiLink.tr.Value); break;
                    case GUITYPE_BOOLEAN: if (guiLink.rb1.Checked) actSettingString += (" 1"); 
                                          else actSettingString += (" 0");
                                          break;
                    case GUITYPE_STANDARD: break;
                }
                slots[slotNumber].settingStrings.Add(actSettingString);
            }
        }

        public void displaySlot(int slotNumber)
        {
            CommandGuiLink actButtonLink = null;
            String strValue="";

            slotNames.Text = slots[slotNumber].slotName;
            actSlotColor = slots[slotNumber].slotColor;
            drawSlotColor(actSlotColor);

            foreach (String settingString in slots[slotNumber].settingStrings)
            {
                if (settingString == null) { 
                    Console.WriteLine("null value detected **** SlotNr."+slotNumber); break; 
                }
                if (actButtonLink != null)
                {
                    String cmd = settingString.Substring(0, 5);
                    switch (allCommands.getGuiTypeFromCommand(cmd))
                    {
                        case GUITYPE_INTFIELD: actButtonLink.cb.SelectedIndex = allCommands.getSelectionIndex(cmd);
                            strValue = settingString.Substring(6);
                            actButtonLink.nud.Value = Int32.Parse(strValue); break;
                        case GUITYPE_KEYSELECT:
                        case GUITYPE_TEXTFIELD: actButtonLink.cb.SelectedIndex = allCommands.getSelectionIndex(cmd);
                            strValue = settingString.Substring(6);
                            actButtonLink.tb.Text = strValue; break;
                        case GUITYPE_COMBO_ONLY:
                            actButtonLink.cb.Text = settingString.Substring(6);
                            break;
                        case GUITYPE_COMBO_INDEX:
                            strValue = settingString.Substring(actButtonLink.cmd.Length + 1);
                            actButtonLink.cb.SelectedIndex = Int32.Parse(strValue);
                            break;
                        case GUITYPE_SLIDER: actButtonLink.tr.Value = Int32.Parse(strValue);
                            actButtonLink.tl.Text = strValue; break;
                        case GUITYPE_STANDARD: actButtonLink.cb.SelectedIndex = allCommands.getSelectionIndex(cmd);
                            break;
                    }
                    actButtonLink = null;
                }

                else foreach (CommandGuiLink guiLink in commandGuiLinks)
                {
                    if (settingString.StartsWith(guiLink.cmd))  // improve this !
                    {
                        if (settingString.StartsWith("AT BM")) 
                        {
                            actButtonLink = guiLink;
                            break;
                        }
                        else
                        {
                            switch (allCommands.getGuiTypeFromCommand(guiLink.cmd))
                            {
                                case GUITYPE_INTFIELD: guiLink.cb.SelectedIndex = allCommands.getSelectionIndex(guiLink.cmd);
                                    strValue = settingString.Substring(guiLink.cmd.Length+1);
                                    guiLink.nud.Value = Int32.Parse(strValue); break;
                                case GUITYPE_TEXTFIELD: guiLink.cb.SelectedIndex = allCommands.getSelectionIndex(guiLink.cmd);
                                    strValue = settingString.Substring(guiLink.cmd.Length+1);
                                    guiLink.tb.Text = strValue; break;
                                case GUITYPE_SLIDER:
                                    strValue = settingString.Substring(guiLink.cmd.Length + 1);
                                    guiLink.tr.Value = Int32.Parse(strValue);
                                    guiLink.tl.Text = strValue; break;
                                case GUITYPE_COMBO_ONLY:
                                    guiLink.cb.Text = settingString.Substring(6);
                                    break;
                                case GUITYPE_COMBO_INDEX:
                                    strValue = settingString.Substring(guiLink.cmd.Length + 1);
                                    guiLink.cb.SelectedIndex = Int32.Parse(strValue);
                                    break;
                                case GUITYPE_BOOLEAN: 
                                    strValue = settingString.Substring(guiLink.cmd.Length + 1);
                                    int value= Int32.Parse(strValue);
                                    if (value == 1)  {
                                        guiLink.rb1.Checked = true; guiLink.rb2.Checked = false;
                                    }
                                    else {
                                        guiLink.rb1.Checked = false; guiLink.rb2.Checked = true;
                                    }
                                    break;

                            }
                        }
                    }
                }
            }
        }


        public FabiGUI()
        {

            InitializeComponent();

            initCommands();
            initCommandGuiLinks();
            initSlots();

            clickTimer.Interval = 500; // specify interval time as you want
            clickTimer.Tick += new EventHandler(timer_Tick);

            IdTimer.Tick += new EventHandler(IdTimer_Tick);

            Text += " "+ VERSION_STRING;


            for (int i = 0; i < allCommands.length(); i++)
            {
                if (allCommands.getComboEntry(i) == COMBOENTRY_YES)
                {
                    string str = allCommands.getCommandDescription(i);
                    Button1FunctionBox.Items.Add(str);
                    Button2FunctionBox.Items.Add(str);
                    Button3FunctionBox.Items.Add(str);
                    Button4FunctionBox.Items.Add(str);
                    Button6FunctionBox.Items.Add(str);
                    Button5FunctionBox.Items.Add(str);
                    Button7FunctionBox.Items.Add(str);
                    Button8FunctionBox.Items.Add(str);
                    Button9FunctionBox.Items.Add(str);
                    SipFunctionMenu.Items.Add(str);
                    PuffFunctionMenu.Items.Add(str);
                }
            }
         
            foreach (string str in keyOptions)
            {
                Button1ComboBox.Items.Add(str);
                Button2ComboBox.Items.Add(str);
                Button3ComboBox.Items.Add(str);
                Button4ComboBox.Items.Add(str);
                Button5ComboBox.Items.Add(str);
                Button6ComboBox.Items.Add(str);
                Button7ComboBox.Items.Add(str);
                Button8ComboBox.Items.Add(str);
                Button9ComboBox.Items.Add(str);
                SipComboBox.Items.Add(str);
                PuffComboBox.Items.Add(str);
            }

            HIDComboBox.Items.Add("disable");
            HIDComboBox.Items.Add("USB HID only");
            HIDComboBox.Items.Add("Bluetooth HID only");
            HIDComboBox.Items.Add("both (USB + BT)");
            HIDComboBox.SelectedIndex = 2;

            displaySlot(0);

            updateComPorts();

            addToLog("Fabi GUI ready!");
            this.Load += FabiGUI_Load;
            this.FormClosed += MyWindow_Closing;
        }


        private void connectComButton_click(object sender, EventArgs e) //select button
        {
            addToLog("Trying to open COM port...");
            if (portComboBox.SelectedIndex > -1)
            {
                if (serialPort1.IsOpen)
                {
                    addToLog(String.Format("Port '{0}' is already connected.", portComboBox.SelectedItem));
                    disconnectComButton.Enabled = true;
                    loadSlotSettingsMenuItem.Enabled = true;
                    storeSlotSettingsMenuItem.Enabled = true;
                    freeMemLabel.Enabled = true;
                    freeMemPanel.Enabled = true;
                }
                else
                {

                    if (Connect(portComboBox.SelectedItem.ToString()))
                    {
                        addToLog(String.Format("Port '{0}' is now connected", portComboBox.SelectedItem));
                        portStatus.Text = "Connected";
                        portStatus.ForeColor = Color.Green;
                        disconnectComButton.Enabled = true;
                        loadSlotSettingsMenuItem.Enabled = true;
                        storeSlotSettingsMenuItem.Enabled = true;
                        freeMemLabel.Enabled = true;
                        freeMemPanel.Enabled = true;
                        ApplyButton.Enabled = true;
                        StoreButton.Enabled = true;
                        connectComButton.Enabled = false;

                        IdTimer.Interval = 1500;
                        IdTimer.Start();
                        Console.WriteLine("IdTimer started!");

                        readDone = false;
                        Thread thread = new Thread(new ThreadStart(WorkThreadFunction));
                        thread.Start();
                    }
                }
            }
            else addToLog("No port has been selected");
        }

        private void disconnnectComButton_Click(object sender, EventArgs e) //disconnect button
        {
            addToLog("Disconnecting from COM Port...");
            disconnect();
            disconnectComButton.Enabled = false;
            loadSlotSettingsMenuItem.Enabled = false;
            storeSlotSettingsMenuItem.Enabled = false;
            ApplyButton.Enabled = false;
            StoreButton.Enabled = false;
            connectComButton.Enabled = true;
            freeMemLabel.Enabled = false;
            freeMemPanel.Enabled = false;
        }


        private void FabiGUI_Load(object sender, EventArgs e)
        {
            this.stringReceivedDelegate = new StringReceivedDelegate(stringReceived);
            BeginInvoke(this.stringReceivedDelegate, new Object[] { "VALUES:512,00000000000,1" });
        }

        // update paint areas if tabs are changed
        private void tabControl_SelectedIndexChanged(object sender, EventArgs e)
        {
            BeginInvoke(this.stringReceivedDelegate, new Object[] { "VALUES:512,00000000000,1" });
            drawSlotColor(actSlotColor);
        }


        private void loadFromFileMenuItem_Click(object sender, EventArgs e)
        {
            loadSettingsFromFile();
        }

        private void saveToFileMenuItem_Click(object sender, EventArgs e)
        {
            storeSlot(actSlot);
            saveSettingsToFile();
        }

        private void exitMenuItem_Click(object sender, EventArgs e)
        {
            disconnect();
            System.Windows.Forms.Application.Exit();
        }

        void MyWindow_Closing(object sender, FormClosedEventArgs e)
        {
            disconnect();
        }


        private void portComboBox_Click(object sender, EventArgs e)
        {
            updateComPorts();
        }

        // update activity log
        private void addToLog(String text)
        {
            activityLogTextbox.SelectedText = DateTime.Now.ToString() + ": ";
            activityLogTextbox.AppendText(text); 
            activityLogTextbox.AppendText("\n");
        }


        private void prevSlotButton_Click(object sender, EventArgs e)
        {
            storeSlot(actSlot);
            if (actSlot > 0) actSlot--;
            else actSlot = slots.Count - 1;
            displaySlot(actSlot);
        }

        private void nextSlotButton_Click(object sender, EventArgs e)
        {
            storeSlot(actSlot);
            if (slots.Count - 1 > actSlot) actSlot++;
            else actSlot = 0;
            displaySlot(actSlot);
        }

        private void newSlotButton_Click(object sender, EventArgs e)
        {
            if (slots.Count < MAX_SLOTS) 
            {
                int cnt=1;
                Boolean exists;
                String newSlotName;

                storeSlot(actSlot);
                do
                {
                    newSlotName = "Slot" + cnt++;
                    exists = false;
                    foreach (Slot s in slots)
                        if (s.slotName.Equals(newSlotName)) exists = true;
                } while (exists == true);
                
                slotNames.Items.Add(newSlotName);
                slots.Add(new Slot(newSlotName));

                slots[slots.Count - 1].settingStrings.Clear();
                foreach (string s in slots[actSlot].settingStrings)
                    slots[slots.Count - 1].settingStrings.Add(s);
                actSlot = slots.Count - 1;
                displaySlot(actSlot);
            }
            else MessageBox.Show("Maximum number of slots reached !");
        }

        private void deleteSlotButton_Click(object sender, EventArgs e)
        {
            if (slots.Count > 1)
            {
                slots.RemoveAt(actSlot);
                slotNames.Items.Clear();
                foreach (Slot s in slots) slotNames.Items.Add(s.slotName);
                if (actSlot > 0) actSlot--;
                displaySlot(actSlot);
            }
            else MessageBox.Show("One slot must stay active !");
        }



        // send all current settings for active slot
        private void ApplyButton_Click(object sender, EventArgs e)
        {
            storeSlot(actSlot);            
            if (serialPort1.IsOpen)
            {
                sendEndReportingCommand();
                sendApplyCommands();
                sendStartReportingCommand();
                addToLog("The selected settings have been applied to the FABI box");

            }
            else addToLog("Please connect a device before applying configuration changes.");            
        }

        private void storeSlotSettingsMenuItem_Click(object sender, EventArgs e)
        {
            storeSlot(actSlot);   
            storeSettingsToFabi();
            actSlot = 0;
            displaySlot(actSlot);
        }

        private void loadSlotSettingsMenuItem_Click(object sender, EventArgs e)
        {
            loadSettingsFromFabi();

        }


        // update visibility of parameter fields:

        private void updateVisibility(string selectedFunction, TextBox tb, NumericUpDown nud, ComboBox cb, Label la, Button bu)
        {
            switch (allCommands.getGuiTypeFromDescription(selectedFunction))
            {
                case GUITYPE_INTFIELD: la.Visible = true; la.Text = "   Value:"; nud.Visible = true; tb.Visible = false; cb.Visible = false; bu.Visible = false; bu.Enabled = false; break;
                case GUITYPE_TEXTFIELD: la.Visible = true; la.Text = "    Text:"; nud.Visible = false; tb.Enabled = true; tb.ReadOnly = false; tb.Visible = true; tb.Text = ""; cb.Visible = false; bu.Visible = true; bu.Enabled = true; break;
                case GUITYPE_KEYSELECT: la.Visible = true; la.Text = "KeyCodes:"; nud.Visible = false; tb.Visible = true; tb.Text = ""; tb.ReadOnly = true; cb.Visible = true; bu.Visible = true; bu.Enabled = true; break;
                default: la.Visible = false; nud.Visible = false; tb.Visible = false; cb.Visible = false; bu.Visible = false; bu.Enabled = false; break;
            }
        }

        private void Button1FunctionBox_SelectedIndexChanged_1(object sender, EventArgs e)
        {
            updateVisibility(Button1FunctionBox.Text, Button1ParameterText, Button1NumericParameter, Button1ComboBox, Button1Label, clearButton1);
        }

        private void Button2FunctionBox_SelectedIndexChanged_1(object sender, EventArgs e)
        {
            updateVisibility(Button2FunctionBox.Text, Button2ParameterText, Button2NumericParameter, Button2ComboBox, Button2Label, clearButton2);
        }

        private void Button3FunctionBox_SelectedIndexChanged_1(object sender, EventArgs e)
        {
            updateVisibility(Button3FunctionBox.Text, Button3ParameterText, Button3NumericParameter, Button3ComboBox, Button3Label, clearButton3);
        }

        private void Button4FunctionBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(Button4FunctionBox.Text, Button4ParameterText, Button4NumericParameter, Button4ComboBox, Button4Label, clearButton4);
        }

        private void Button5FunctionBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(Button5FunctionBox.Text, Button5ParameterText, Button5NumericParameter, Button5ComboBox, Button5Label, clearButton5);
        }

        private void Button6FunctionMenu_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(Button6FunctionBox.Text, Button6ParameterText, Button6NumericParameter, Button6ComboBox, Button6Label, clearButton6);
        }

        private void Button7FunctionMenu_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(Button7FunctionBox.Text, Button7ParameterText, Button7NumericParameter, Button7ComboBox, Button7Label, clearButton7);
        }

        private void Button8FunctionMenu_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(Button8FunctionBox.Text, Button8ParameterText, Button8NumericParameter, Button8ComboBox, Button8Label, clearButton8);
        }

        private void Button9FunctionMenu_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(Button9FunctionBox.Text, Button9ParameterText, Button9NumericParameter, Button9ComboBox, Button9Label, clearButton9);
        }

        private void SipFunctionMenu_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(SipFunctionMenu.Text, SipParameterText, SipNumericParameter, SipComboBox, SipParameterLabel, clearButtonSip);
        }

        private void PuffFunctionMenu_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(PuffFunctionMenu.Text, PuffParameterText, PuffNumericParameter, PuffComboBox, PuffParameterLabel, clearButtonPuff);
        }

        // update the keycode parameters:

        private void updateKeyCodeParameter(ComboBox cb, TextBox tb)
        {
            if (cb.SelectedIndex == 0)
                tb.Text = "";
            else
            {
                String add = cb.Text.ToString() + " ";
                if (!tb.Text.Contains(add))
                    tb.Text += add;
            }
        }

        private void Button1ComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(Button1ComboBox,Button1ParameterText);
        }
        private void clearButton1_Click(object sender, EventArgs e)
        {
            Button1ParameterText.Text = "";
        }

        private void Button2ComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(Button2ComboBox, Button2ParameterText);
        }
        private void clearButton2_Click(object sender, EventArgs e)
        {
            Button2ParameterText.Text = "";
        }

        private void Button3ComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(Button3ComboBox, Button3ParameterText);
        }
        private void clearButton3_Click(object sender, EventArgs e)
        {
            Button3ParameterText.Text = "";
        }

        private void Button4ComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(Button4ComboBox, Button4ParameterText);
        }
        private void clearButton4_Click(object sender, EventArgs e)
        {
            Button4ParameterText.Text = "";
        }

        private void Button5ComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(Button5ComboBox, Button5ParameterText);
        }
        private void clearButton5_Click(object sender, EventArgs e)
        {
            Button5ParameterText.Text = "";
        }
        private void Button6ComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(Button6ComboBox, Button6ParameterText);
        }
        private void clearButton6_Click(object sender, EventArgs e)
        {
            Button6ParameterText.Text = "";
        }

        private void Button7ComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(Button7ComboBox, Button7ParameterText);
        }
        private void clearButton7_Click(object sender, EventArgs e)
        {
            Button7ParameterText.Text = "";
        }

        private void Button8ComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(Button8ComboBox, Button8ParameterText);
        }
        private void clearButton8_Click(object sender, EventArgs e)
        {
            Button8ParameterText.Text = "";
        }

        private void Button9ComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(Button9ComboBox, Button9ParameterText);
        }
        private void clearButton9_Click(object sender, EventArgs e)
        {
            Button9ParameterText.Text = "";
        }


        private void SipComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(SipComboBox, SipParameterText);
        }
        private void clearButtonSip_Click(object sender, EventArgs e)
        {
            SipParameterText.Text = "";
        }

        private void PuffComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(PuffComboBox, PuffParameterText);
        }
        private void clearButtonPuff_Click(object sender, EventArgs e)
        {
            PuffParameterText.Text = "";
        }

        private void puffThresholdBar_Scroll(object sender, EventArgs e)
        {
            puffThresholdLabel.Text = puffThresholdBar.Value.ToString();
        }
        private void decPuffThreshold_Click(object sender, EventArgs e)
        {
            if (puffThresholdBar.Value >= puffThresholdBar.Minimum + PRESSURE_CHANGE_STEP)
                puffThresholdBar.Value -= PRESSURE_CHANGE_STEP;
            puffThresholdLabel.Text = puffThresholdBar.Value.ToString();

        }
        private void decPuffThreshold_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(decPuffThreshold_Click);
            clickTimer.Start();
        }
        private void incPuffThreshold_Click(object sender, EventArgs e)
        {
            if (puffThresholdBar.Value <= puffThresholdBar.Maximum - PRESSURE_CHANGE_STEP)
                puffThresholdBar.Value += PRESSURE_CHANGE_STEP;
            puffThresholdLabel.Text = puffThresholdBar.Value.ToString();
        }
        private void incPuffThreshold_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(incPuffThreshold_Click);
            clickTimer.Start();
        }

        private void sipThresholdBar_Scroll(object sender, EventArgs e)
        {
            sipThresholdLabel.Text = sipThresholdBar.Value.ToString();
        }
        private void decSipThreshold_Click(object sender, EventArgs e)
        {
            if (sipThresholdBar.Value >= sipThresholdBar.Minimum + PRESSURE_CHANGE_STEP)
                sipThresholdBar.Value -= PRESSURE_CHANGE_STEP;
            sipThresholdLabel.Text = sipThresholdBar.Value.ToString();

        }
        private void decSipThreshold_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(decSipThreshold_Click);
            clickTimer.Start();
        }
        private void incSipThreshold_Click(object sender, EventArgs e)
        {
            if (sipThresholdBar.Value <= sipThresholdBar.Maximum - PRESSURE_CHANGE_STEP)
                sipThresholdBar.Value += PRESSURE_CHANGE_STEP;
            sipThresholdLabel.Text = sipThresholdBar.Value.ToString();

        }
        private void incSipThreshold_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(incSipThreshold_Click);
            clickTimer.Start();
        }



        void timer_Tick(object sender, EventArgs e)
        {
             Console.Write("*");
             functionPointer(this, null);
        }

        private void stop_ClickTimer(object sender, EventArgs e)
        {
            clickTimer.Stop();
        }

        private void slotNames_SelectedIndexChanged(object sender, EventArgs e)
        {
            actSlot = slotNames.SelectedIndex;
            displaySlot(actSlot);

        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Fabi V"+VERSION_STRING+" - AsTeRICS Foundation\nFor more information see: https://www.asterics-foundation.org");
        }

        private void timeThresholdBar_Scroll(object sender, EventArgs e)
        {
            timeThresholdLabel.Text = timeThresholdBar.Value.ToString();

        }

        private void antiTremorPressBar_Scroll(object sender, EventArgs e)
        {
            antiTremorPressLabel.Text = antiTremorPressBar.Value.ToString();
        }

        private void antiTremorReleaseBar_Scroll(object sender, EventArgs e)
        {
            antiTremorReleaseLabel.Text = antiTremorReleaseBar.Value.ToString();
        }

        private void antiTremorIdleBar_Scroll(object sender, EventArgs e)
        {
            antiTremorIdleLabel.Text = antiTremorIdleBar.Value.ToString();

        }

        private void timeThresholdBar_Scroll_1(object sender, EventArgs e)
        {
            timeThresholdLabel.Text = timeThresholdBar.Value.ToString();
        }

        private void doublePressTimeBar_Scroll(object sender, EventArgs e)
        {
            doublePressTimeLabel.Text = doublePressTimeBar.Value.ToString();
        }

        private void autoDwellTimeBar_Scroll(object sender, EventArgs e)
        {
            autoDwellTimeLabel.Text = autoDwellTimeBar.Value.ToString();
        }

        private void chooseColorPanel_MouseClick(object sender, MouseEventArgs e)
        {
            ColorDialog colorDlg = new ColorDialog();
            colorDlg.AllowFullOpen = true;
            colorDlg.Color = ColorTranslator.FromHtml(actSlotColor);
            colorDlg.FullOpen = true;
            if (colorDlg.ShowDialog() == DialogResult.OK)
            {
                actSlotColor = "0x" + colorDlg.Color.R.ToString("X2") + colorDlg.Color.G.ToString("X2") + colorDlg.Color.B.ToString("X2");
                Console.WriteLine("Chosen color = " + actSlotColor);
                drawSlotColor(actSlotColor);
                slots[actSlot].slotColor = actSlotColor;
            }
        }
    }
}
