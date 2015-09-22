
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
using System.IO.Ports;
using System.IO;



namespace MouseApp2
{
    public partial class FabiGUI : Form
    {
        const string VERSION_STRING = "1.0";

        const int CMD_NOACTION = 0;
        const int CMD_NEXT = 1;
        const int CMD_CLICK_LEFT = 2;
        const int CMD_CLICK_RIGHT = 3;
        const int CMD_CLICK_MIDDLE = 4;
        const int CMD_CLICK_DOUBLE = 5;
        const int CMD_PRESS_LEFT = 6;
        const int CMD_PRESS_RIGHT = 7;
        const int CMD_PRESS_MIDDLE = 8;
        const int CMD_WHEEL_UP = 9;
        const int CMD_WHEEL_DOWN = 10;
        const int CMD_MOVE_X = 11;
        const int CMD_MOVE_Y = 12;
        const int CMD_WRITE_TEXT = 13;
        const int CMD_PRESS_KEYS = 14;

        const String TOKEN_SEPERATOR = "-,-";

        String[] commands = {  "No Action", "Switch to next configuration", 
                               "Click Left Mouse Button", "Click Right Mouse Button", "Click Middle Mouse Button" , "Double Click Left Mouse Button",
                               "Hold Left Mouse Button", "Hold Right Mouse Button", "Hold Middle Mouse Button", 
                               "Wheel Up", "Wheel down", 
                               "Move Mouse X", "Move Mouse Y",
                               "Write Text", "Press Keys"
                             };
        String[] keyOptions = {    "Clear Keycodes!", "KEY_A","KEY_B","KEY_C","KEY_D","KEY_E","KEY_F","KEY_G","KEY_H","KEY_I","KEY_J","KEY_K","KEY_L",
                                   "KEY_M","KEY_N","KEY_O","KEY_P","KEY_Q","KEY_R","KEY_S","KEY_T","KEY_U","KEY_V","KEY_W","KEY_X",
                                   "KEY_Y","KEY_Z","KEY_1","KEY_2","KEY_3","KEY_4","KEY_5","KEY_6","KEY_7","KEY_8","KEY_9","KEY_0",
                                   "KEY_F1","KEY_F2","KEY_F3","KEY_F4","KEY_F5","KEY_F6","KEY_F7","KEY_F8","KEY_F9","KEY_F10","KEY_F11","KEY_F12",	
                                   "KEY_UP","KEY_DOWN","KEY_LEFT","KEY_RIGHT","KEY_SPACE","KEY_ENTER",
                                   "KEY_ALT","KEY_BACKSPACE","KEY_CAPS_LOCK","KEY_CTRL","KEY_DELETE","KEY_END","KEY_ESC","KEY_GUI",
                                   "KEY_HOME","KEY_INSERT","KEY_NUM_LOCK","KEY_PAGE_DOWN","KEY_PAGE_UP","KEY_PAUSE","KEY_RIGHT_ALT",
                                   "KEY_RIGHT_GUI","KEY_SCROLL_LOCK","KEY_SHIFT","KEY_TAB"
                              };


        String receivedString = "";
        Boolean readDone=false;
        Boolean storingToFile = false;
        static int slotCounter = 0;
        String firstSlotname = null;

        System.IO.StreamWriter settingsFile = null;


        public delegate void SlotValuesDelegate(string newValues);
        public SlotValuesDelegate slotValuesDelegate;
        public delegate void RawValuesDelegate(string newValues);
        public RawValuesDelegate rawValuesDelegate;
        public delegate void LoadValuesDelegate(string newValues);
        public LoadValuesDelegate loadValuesDelegate;
        public delegate void StoreValuesDelegate(string newValues);
        public StoreValuesDelegate storeValuesDelegate;


        public FabiGUI()
        {
            InitializeComponent();

            Text += " " + VERSION_STRING;
            foreach (string str in commands)
            {
                Button1FunctionBox.Items.Add(str);
                Button2FunctionBox.Items.Add(str);
                Button3FunctionBox.Items.Add(str);
                Button4FunctionBox.Items.Add(str);
                Button5FunctionBox.Items.Add(str);
                Button6FunctionBox.Items.Add(str);
                Button1LongFunctionBox.Items.Add(str);
                Button2LongFunctionBox.Items.Add(str);
                Button3LongFunctionBox.Items.Add(str);
                Button4LongFunctionBox.Items.Add(str);
                Button5LongFunctionBox.Items.Add(str);
                Button6LongFunctionBox.Items.Add(str);
                SipFunctionMenu.Items.Add(str);
                LongSipFunctionMenu.Items.Add(str);
                PuffFunctionMenu.Items.Add(str);
                LongPuffFunctionMenu.Items.Add(str);
            }

            Button1FunctionBox.SelectedIndex = CMD_PRESS_LEFT;
            Button2FunctionBox.SelectedIndex = CMD_CLICK_RIGHT;
            Button3FunctionBox.SelectedIndex = CMD_CLICK_DOUBLE;
            Button4FunctionBox.SelectedIndex = CMD_WHEEL_UP;
            Button5FunctionBox.SelectedIndex = CMD_WHEEL_DOWN;
            Button6FunctionBox.SelectedIndex = CMD_NEXT;
            Button1LongFunctionBox.SelectedIndex = CMD_PRESS_LEFT;
            Button2LongFunctionBox.SelectedIndex = CMD_CLICK_RIGHT;
            Button3LongFunctionBox.SelectedIndex = CMD_CLICK_DOUBLE;
            Button4LongFunctionBox.SelectedIndex = CMD_WHEEL_UP;
            Button5LongFunctionBox.SelectedIndex = CMD_WHEEL_DOWN;
            Button6LongFunctionBox.SelectedIndex = CMD_NEXT;
            SipFunctionMenu.SelectedIndex = CMD_PRESS_LEFT;
            LongSipFunctionMenu.SelectedIndex = CMD_NOACTION;
            PuffFunctionMenu.SelectedIndex = CMD_CLICK_RIGHT;
            LongPuffFunctionMenu.SelectedIndex = CMD_NOACTION;

            foreach (string str in keyOptions)
            {
                Button1ComboBox.Items.Add(str);
                Button2ComboBox.Items.Add(str);
                Button3ComboBox.Items.Add(str);
                Button4ComboBox.Items.Add(str);
                Button5ComboBox.Items.Add(str);
                Button6ComboBox.Items.Add(str);
                Button1LongComboBox.Items.Add(str);
                Button2LongComboBox.Items.Add(str);
                Button3LongComboBox.Items.Add(str);
                Button4LongComboBox.Items.Add(str);
                Button5LongComboBox.Items.Add(str);
                Button6LongComboBox.Items.Add(str);
                SipComboBox.Items.Add(str);
                LongSipComboBox.Items.Add(str);
                PuffComboBox.Items.Add(str);
                LongPuffComboBox.Items.Add(str);
            }

            updateComPorts();

            addToLog("Fabi GUI ready!");
            this.Load += LipmouseGUI_Load;
            this.FormClosed += MyWindow_Closing;

        }

        private void saveToFileMenuItem_Click(object sender, EventArgs e)
        {
            if (!serialPort1.IsOpen)
            {
                addToLog("Please connect FABI device to save settings !");
                return;
            }

            if (slotNames.Items.Count < 1)
            {
                addToLog("No slots are stored in the FABI device !");
                return;
            }
            Stream myStream;
            SaveFileDialog saveFileDialog1 = new SaveFileDialog();

            saveFileDialog1.Filter = "settings files (*.fab)|*.fab|All files (*.*)|*.*";
            saveFileDialog1.RestoreDirectory = true;

            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                if ((myStream = saveFileDialog1.OpenFile()) != null)
                {
                    settingsFile = new System.IO.StreamWriter(myStream);
                    slotCounter = 0;
                    storingToFile = true;
                    sendCmd("AT LOAD " + slotNames.Items[0].ToString());   // get first slot
                    // incoming data will now be handeled in gotStoreValues()
                    // Code to write the stream goes here.
                    //myStream.Close();
                }
            }

        }

        private void loadFromFileMenuItem_Click(object sender, EventArgs e)
        {
            if (!serialPort1.IsOpen)
            {
                addToLog("Please connect FABI device to transfer settings !");
                return;
            }
            Stream myStream = null;
            OpenFileDialog fd = new OpenFileDialog();
            fd.DefaultExt = "*.fab";
            fd.Filter = "settings files (*.fab)|*.fab|All files (*.*)|*.*";
            if (fd.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    if ((myStream = fd.OpenFile()) != null)
                    {
                        sendCmd("AT CLEAR");
                        slotNames.Items.Clear();

                        string line;
                        System.IO.StreamReader file = new System.IO.StreamReader(myStream);
                        while ((line = file.ReadLine()) != null)
                        {
                            Console.WriteLine("load line:" + line);

                            gotLoadValues(line);

                            slotNames.Text = slotNames.Text.Replace(" ", "");
                            slotNames.Text = slotNames.Text.Replace("\n", "");
                            slotNames.Text = slotNames.Text.Replace("\r", "");
                            addToLog("Saving Slot: " + slotNames.Text);

                            ApplyButton_Click(this, null);
                            sendCmd("AT SAVE " + slotNames.Text);
                        }
                        file.Close();
                        addToLog("The settings were transferred to the FABI device!");
                        slotNames.Items.Clear();
                        sendCmd("AT LIST");

                    }
                }
                catch (Exception ex)
                {
                    addToLog("Error: Could not read file from disk. Original error: " + ex.Message);
                }
            }


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


        private void updateComPorts()
        {
            var ports = SerialPort.GetPortNames();
            portComboBox.DataSource = ports;
        }

        private void portComboBox_Click(object sender, EventArgs e)
        {
            updateComPorts();
        }

        private void LipmouseGUI_Load(object sender, EventArgs e)
        {
            var ports = SerialPort.GetPortNames();
            portComboBox.DataSource = ports;

            this.slotValuesDelegate = new SlotValuesDelegate(gotSlotValues);
            this.rawValuesDelegate = new RawValuesDelegate(gotValues);
            this.loadValuesDelegate = new LoadValuesDelegate(gotLoadValues);
            this.storeValuesDelegate = new StoreValuesDelegate(gotStoreValues);

            BeginInvoke(this.rawValuesDelegate, new Object[] { "512" });
        }

        // update paint areas if tabs are changed
        private void tabControl_SelectedIndexChanged(object sender, EventArgs e)
        {
            BeginInvoke(this.rawValuesDelegate, new Object[] { "512" });
        }

        public void gotStoreValues(String newValues)
        {
            Console.WriteLine("got store values (" + slotCounter + "):" + newValues);
            String actSlotname = newValues.Substring(0, newValues.IndexOf("-,-"));
            if (settingsFile != null)
            {
                if (slotCounter == 0)
                {
                    firstSlotname = actSlotname;
                    Console.WriteLine("first slotname=" + firstSlotname);
                }

                if ((slotCounter > 0) && (firstSlotname.Equals(actSlotname)))
                {
                    Console.WriteLine("Now closing settings file!");
                    storingToFile = false;
                    settingsFile.Close();
                    settingsFile = null;
                }
                else
                {
                    settingsFile.Write(newValues);
                    sendCmd("AT NEXT");
                    slotCounter++;
                }
            }
        }

        public void gotSlotValues(String newValues)
        {
            slotNames.Items.Add(newValues);
            if (slotNames.Items.Count == 1)
            {
                firstSlotname = newValues;
            }

        }

        // draw live values on panels 

        public void gotValues(String newValues)
        {
            if (newValues.Length == 0)
                return;

            //  Console.WriteLine(newValues);
            pressureLabel.Text = newValues;
            Int32 value = 1023 - Convert.ToInt32(newValues);
            Graphics g = panel1.CreateGraphics();
            Brush brush = new SolidBrush(Color.Green);
            Brush brush2 = new SolidBrush(Color.White);
            value = value * panel1.Height / 1024;
            g.FillRectangle(brush, 0, panel1.Height - value, 30, value);
            g.FillRectangle(brush2, 0, 0, 30, panel1.Height - value);
        }


        public void gotLoadValues(String newValues)
        {
            String actToken;
            int i=0;
            bool done = false;
            while (!done)
            {
                actToken = newValues.Substring(0, newValues.IndexOf("-,-"));
                // Console.WriteLine("Found Token " + i + " " + actToken);
                switch (i)
                {
                    case 0: slotNames.Text = actToken;  break;  // slotname
                    case 1: break;  // mouse wheel stepsize, currently not used
                    case 2: break;  // time threshold for longpress, currently not used
                    case 3: Button1FunctionBox.SelectedIndex = Int32.Parse(actToken); break;
                    case 4: Button1NumericParameter.Value = Int32.Parse(actToken); break;
                    case 5: Button1ParameterText.Text = actToken; break;
                    case 6: Button2FunctionBox.SelectedIndex = Int32.Parse(actToken); break;
                    case 7: Button2NumericParameter.Value = Int32.Parse(actToken); break;
                    case 8: Button2ParameterText.Text = actToken; break;
                    case 9: Button3FunctionBox.SelectedIndex = Int32.Parse(actToken); break;
                    case 10: Button3NumericParameter.Value = Int32.Parse(actToken); break;
                    case 11: Button3ParameterText.Text = actToken; break;
                    case 12: Button4FunctionBox.SelectedIndex = Int32.Parse(actToken); break;
                    case 13: Button4NumericParameter.Value = Int32.Parse(actToken); break;
                    case 14: Button4ParameterText.Text = actToken; break;
                    case 15: Button5FunctionBox.SelectedIndex = Int32.Parse(actToken); break;
                    case 16: Button5NumericParameter.Value = Int32.Parse(actToken); break;
                    case 17: Button5ParameterText.Text = actToken; break;
                    case 18: Button6FunctionBox.SelectedIndex = Int32.Parse(actToken); break;
                    case 19: Button6NumericParameter.Value = Int32.Parse(actToken); break;
                    case 20: Button6ParameterText.Text = actToken; break;
                    case 21: Button1LongFunctionBox.SelectedIndex = Int32.Parse(actToken); break;
                    case 22: Button1LongNumericParameter.Value = Int32.Parse(actToken); break;
                    case 23: Button1LongParameterText.Text = actToken; break;
                    case 24: Button1LongSlide.Value = Int32.Parse(actToken); break;
                    case 25: Button2LongFunctionBox.SelectedIndex = Int32.Parse(actToken); break;
                    case 26: Button2LongNumericParameter.Value = Int32.Parse(actToken); break;
                    case 27: Button2LongParameterText.Text = actToken; break;
                    case 28: Button2LongSlide.Value = Int32.Parse(actToken); break;
                    case 29: Button3LongFunctionBox.SelectedIndex = Int32.Parse(actToken); break;
                    case 30: Button3LongNumericParameter.Value = Int32.Parse(actToken); break;
                    case 31: Button3LongParameterText.Text = actToken; break;
                    case 32: Button3LongSlide.Value = Int32.Parse(actToken); break;
                    case 33: Button4LongFunctionBox.SelectedIndex = Int32.Parse(actToken); break;
                    case 34: Button4LongNumericParameter.Value = Int32.Parse(actToken); break;
                    case 35: Button4LongParameterText.Text = actToken; break;
                    case 36: Button4LongSlide.Value = Int32.Parse(actToken); break;
                    case 37: Button5LongFunctionBox.SelectedIndex = Int32.Parse(actToken); break;
                    case 38: Button5LongNumericParameter.Value = Int32.Parse(actToken); break;
                    case 39: Button5LongParameterText.Text = actToken; break;
                    case 40: Button5LongSlide.Value = Int32.Parse(actToken); break;
                    case 41: Button6LongFunctionBox.SelectedIndex = Int32.Parse(actToken); break;
                    case 42: Button6LongNumericParameter.Value = Int32.Parse(actToken); break;
                    case 43: Button6LongParameterText.Text = actToken; break;
                    case 44: Button6LongSlide.Value = Int32.Parse(actToken); break;
                    case 45: SipFunctionMenu.SelectedIndex = Int32.Parse(actToken); break;
                    case 46: SipNumericParameter.Value = Int32.Parse(actToken); break;
                    case 47: SipParameterText.Text = actToken; break;
                    case 48: LongSipFunctionMenu.SelectedIndex = Int32.Parse(actToken); break;
                    case 49: LongSipNumericParameter.Value = Int32.Parse(actToken); break;
                    case 50: LongSipParameterText.Text = actToken; break;
                    case 51: PuffFunctionMenu.SelectedIndex = Int32.Parse(actToken); break;
                    case 52: PuffNumericParameter.Value = Int32.Parse(actToken); break;
                    case 53: PuffParameterText.Text = actToken; break;
                    case 54: LongPuffFunctionMenu.SelectedIndex = Int32.Parse(actToken); break;
                    case 55: LongPuffNumericParameter.Value = Int32.Parse(actToken); break;
                    case 56: LongPuffParameterText.Text = actToken; break;
                    default: done = true; break;
                }
                newValues = newValues.Substring(actToken.Length + 3);
                if (newValues.ToUpper().StartsWith("END"))
                {
                    done = true;
                }
                else i++;
            }      
            
        }

        bool Connect(string portName)
        {
            if (!serialPort1.IsOpen)
            {
                serialPort1.PortName = portName;
                serialPort1.BaudRate = 115200;
                serialPort1.DataBits = 8;
                serialPort1.Parity = Parity.None;
                serialPort1.Handshake = Handshake.None;
                serialPort1.DtrEnable = true;
                serialPort1.ReadTimeout =2500;
                serialPort1.WriteTimeout =2500;
                serialPort1.NewLine = "\n";
                try {
                    serialPort1.Open();
                    return (true);
                }
                catch (Exception ex)  {
                    addToLog("Could not open COM port");
                }
            }
            return (false);
        }

        public void WorkThreadFunction()
        {
            Console.WriteLine("Started ReaderThread");
            try
            {
                while (serialPort1.IsOpen && !readDone)
                {
                    try  {
                        receivedString = serialPort1.ReadLine();
                        Console.WriteLine("received:"+receivedString);
                        if (receivedString.ToUpper().StartsWith("AT RR "))  // raw report found ?
                        {
                            BeginInvoke(this.rawValuesDelegate, new Object[] { receivedString.Substring(6) });
                        }
                        if (receivedString.ToUpper().StartsWith("SLOT"))  // slot name found ?
                        {
                            BeginInvoke(this.slotValuesDelegate, new Object[] { receivedString.Substring(6) });
                        }
                        if (receivedString.ToUpper().StartsWith("LOADING:"))  // slot name found ?
                        {
                            if (storingToFile)
                                BeginInvoke(this.storeValuesDelegate, new Object[] { receivedString.Substring(8) });
                            else
                                BeginInvoke(this.loadValuesDelegate, new Object[] { receivedString.Substring(8) });
                        }
                    }
                    catch (Exception ex)  {
                        //  Console.WriteLine("timed out ...");
                    }
                }
                Console.WriteLine("Ended ReaderThread");
            }
            catch (Exception ex)
            {
                // log errors
            }
        }


        private void select_Click(object sender, EventArgs e) //select button
        {
            addToLog("Connecting to COM port");
            if (portComboBox.SelectedIndex > -1)
            {
                if (serialPort1.IsOpen)
                {
                    addToLog(String.Format("Port '{0}' is already connected.", portComboBox.SelectedItem));
                }
                else
                {
                    if (Connect(portComboBox.SelectedItem.ToString()))
                    {
                        addToLog(String.Format("Port '{0}' is now connected", portComboBox.SelectedItem));
                        portStatus.Text = "Connected";
                        portStatus.ForeColor = Color.Green;
                        saveSettings.Enabled = true;
                        SelectButton.Enabled = false;
                        dcButton.Enabled = true;
                        loadButton.Enabled = true;
                        ClearButton.Enabled = true;
                        ApplyButton.Enabled = true;

                        readDone = false;
                        Thread thread = new Thread(new ThreadStart(WorkThreadFunction));
                        thread.Start();

                        slotNames.Items.Clear();
                        sendCmd("AT LIST");
                        sendCmd("AT LOAD");
                    }
                }
            }
            else addToLog("No port has been selected");
        }

        private void disconnect()
        {
            readDone = true;
            if (serialPort1.IsOpen)
            {
                sendCmd("AT ER");  // end reporting raw values !

                portStatus.Text = "Disconnected";
                addToLog("Port " + portComboBox.Text + " is now disconnected");

                portStatus.ForeColor = Color.SlateGray;
                saveSettings.Enabled = false;
                loadButton.Enabled = false;
                ClearButton.Enabled = false;
                ApplyButton.Enabled = false;

                try
                {
                    receivedString = "";
                    slotNames.Items.Clear();
                    serialPort1.Close();
                }
                catch (Exception ex)
                {
                    addToLog("Error disconnecting COM Port");
                }
            }
        }


        private void dcButton_Click(object sender, EventArgs e) //disconnect button
        {
            addToLog("Disconnecting from COM Port...");
            disconnect();
            dcButton.Enabled = false;
            SelectButton.Enabled = true;
        }

        private void sendCmd(string command)
        {
            if (serialPort1.IsOpen)
            {
                Console.WriteLine("Send:" + command);
                try {
                    serialPort1.Write(command + "\r");
                }
                catch (Exception ex)  {
                    addToLog("Could not write to COM port");
                }
            }
        }


        private void saveSettings_Click(object sender, EventArgs e) //button to save options to EEPROM
        {
            slotNames.Text = slotNames.Text.Replace(" ", "");
            slotNames.Text = slotNames.Text.Replace("\n", "");
            slotNames.Text = slotNames.Text.Replace("\r", "");
            addToLog("Saving Slot: "+slotNames.Text);
            if (serialPort1.IsOpen)
            {
                ApplyButton_Click(this, null);
                sendCmd("AT SAVE " + slotNames.Text);
                addToLog("The settings were saved");
                slotNames.Items.Clear();
                sendCmd("AT LIST");
            }
            else addToLog("Could not send to device - please connect COM port !");
        }

        private void ClearButton_Click(object sender, EventArgs e)
        {
            addToLog("Clearing EEPROM settings...");
            if (serialPort1.IsOpen)
            {
                sendCmd("AT CLEAR\n");
                addToLog("The EEPROM settings have been cleared.");
                slotNames.Items.Clear();
            }
            else addToLog("Could not send to device - please connect COM port !");
        }

        private void load_Click(object sender, EventArgs e)
        {
            addToLog("Loading Slot: "+slotNames.Text);
            if (serialPort1.IsOpen)
            {
                sendCmd("AT LOAD "+slotNames.Text);
            }
            else addToLog("Could not send to device - please connect COM port !");
        }


        private void updateOneButton(int button, int cmdIndex, String parameter, String numParameter)
        {
            sendCmd("AT BM " + button);  // store command to this button function !
            switch (cmdIndex)
            {
                case CMD_NOACTION: sendCmd("AT IDLE"); break;
                case CMD_NEXT: sendCmd("AT NEXT"); break;
                case CMD_CLICK_LEFT: sendCmd("AT CL"); break;
                case CMD_CLICK_RIGHT: sendCmd("AT CR"); break;
                case CMD_CLICK_MIDDLE: sendCmd("AT CM"); break;
                case CMD_CLICK_DOUBLE: sendCmd("AT CD"); break;
                case CMD_PRESS_LEFT: sendCmd("AT PL"); break;
                case CMD_PRESS_RIGHT: sendCmd("AT PR"); break;
                case CMD_PRESS_MIDDLE: sendCmd("AT PM"); break;
                case CMD_WHEEL_UP: sendCmd("AT WU"); break;
                case CMD_WHEEL_DOWN: sendCmd("AT WD"); break;
                case CMD_MOVE_X: sendCmd("AT MX " + numParameter); break;
                case CMD_MOVE_Y: sendCmd("AT MY " + numParameter); break;
                case CMD_WRITE_TEXT: sendCmd("AT KW " + parameter); break;
                case CMD_PRESS_KEYS: sendCmd("AT KP " + parameter); break;
            }
        }

        private void updateOneLongButton(int button, int cmdIndex, String parameter, String numParameter, Int32 longDelay)
        {
            sendCmd("AT BML " + longDelay + " " + button);  // store command to this button function !
            switch (cmdIndex)
            {
                case CMD_NOACTION: sendCmd("AT IDLE"); break;
                case CMD_NEXT: sendCmd("AT NEXT"); break;
                case CMD_CLICK_LEFT: sendCmd("AT CL"); break;
                case CMD_CLICK_RIGHT: sendCmd("AT CR"); break;
                case CMD_CLICK_MIDDLE: sendCmd("AT CM"); break;
                case CMD_CLICK_DOUBLE: sendCmd("AT CD"); break;
                case CMD_PRESS_LEFT: sendCmd("AT PL"); break;
                case CMD_PRESS_RIGHT: sendCmd("AT PR"); break;
                case CMD_PRESS_MIDDLE: sendCmd("AT PM"); break;
                case CMD_WHEEL_UP: sendCmd("AT WU"); break;
                case CMD_WHEEL_DOWN: sendCmd("AT WD"); break;
                case CMD_MOVE_X: sendCmd("AT MX " + numParameter); break;
                case CMD_MOVE_Y: sendCmd("AT MY " + numParameter); break;
                case CMD_WRITE_TEXT: sendCmd("AT KW " + parameter); break;
                case CMD_PRESS_KEYS: sendCmd("AT KP " + parameter); break;
            }
        }

        private void addToLog(String text)
        {
             activityLogTextbox.SelectedText = DateTime.Now.ToString() + ": ";
             activityLogTextbox.AppendText(text); activityLogTextbox.AppendText("\n");
        }

        private void ApplyButton_Click(object sender, EventArgs e)
        {
            addToLog("Applying Settings...");
            if (serialPort1.IsOpen)
            {
                sendCmd("AT TS " + sipThresholdLabel.Text);
                sendCmd("AT TP " + puffThresholdLabel.Text);
                sendCmd("AT TT " + timeThresholdLabel.Text);

                updateOneButton(1, Button1FunctionBox.SelectedIndex, Button1ParameterText.Text, Button1NumericParameter.Value.ToString());
                updateOneButton(2, Button2FunctionBox.SelectedIndex, Button2ParameterText.Text, Button2NumericParameter.Value.ToString());
                updateOneButton(3, Button3FunctionBox.SelectedIndex, Button3ParameterText.Text, Button3NumericParameter.Value.ToString());
                updateOneButton(4, Button4FunctionBox.SelectedIndex, Button4ParameterText.Text, Button4NumericParameter.Value.ToString());
                updateOneButton(5, Button5FunctionBox.SelectedIndex, Button5ParameterText.Text, Button5NumericParameter.Value.ToString());
                updateOneButton(6, Button6FunctionBox.SelectedIndex, Button6ParameterText.Text, Button6NumericParameter.Value.ToString());

                updateOneLongButton(7, Button1LongFunctionBox.SelectedIndex, Button1LongParameterText.Text, Button1LongNumericParameter.Value.ToString(), Button1LongSlide.Value);
                updateOneLongButton(8, Button2LongFunctionBox.SelectedIndex, Button2LongParameterText.Text, Button2LongNumericParameter.Value.ToString(), Button2LongSlide.Value);
                updateOneLongButton(9, Button3LongFunctionBox.SelectedIndex, Button3LongParameterText.Text, Button3LongNumericParameter.Value.ToString(), Button3LongSlide.Value);
                updateOneLongButton(10, Button4LongFunctionBox.SelectedIndex, Button4LongParameterText.Text, Button4LongNumericParameter.Value.ToString(), Button4LongSlide.Value);
                updateOneLongButton(11, Button5LongFunctionBox.SelectedIndex, Button5LongParameterText.Text, Button5LongNumericParameter.Value.ToString(), Button5LongSlide.Value);
                updateOneLongButton(12, Button6LongFunctionBox.SelectedIndex, Button6LongParameterText.Text, Button6LongNumericParameter.Value.ToString(), Button6LongSlide.Value);

                updateOneButton(13, SipFunctionMenu.SelectedIndex, SipParameterText.Text, SipNumericParameter.Value.ToString());
                updateOneButton(14, LongSipFunctionMenu.SelectedIndex, LongSipParameterText.Text, LongSipNumericParameter.Value.ToString());
                updateOneButton(15, PuffFunctionMenu.SelectedIndex, PuffParameterText.Text, PuffNumericParameter.Value.ToString());
                updateOneButton(16, LongPuffFunctionMenu.SelectedIndex, LongPuffParameterText.Text, LongPuffNumericParameter.Value.ToString());
                addToLog("The selected settings have been applied.");
            }
            else addToLog("Please connect a device before applying configuration changes.");
        }

        private void updateVisibility(int selectedFunction, TextBox tb, NumericUpDown nud, ComboBox cb, Label la)
        {
            switch (selectedFunction)
            {
                case CMD_MOVE_X:
                case CMD_MOVE_Y: la.Visible = true; la.Text = "   Speed:"; nud.Visible = true; tb.Visible = false; cb.Visible = false; break;
                case CMD_WRITE_TEXT: la.Visible = true; la.Text = "    Text:"; nud.Visible = false; tb.Enabled = true; tb.ReadOnly = false; tb.Visible = true; tb.Text = ""; cb.Visible = false; break;
                case CMD_PRESS_KEYS: la.Visible = true; la.Text = "KeyCodes:"; nud.Visible = false; tb.Visible = true; tb.Text = ""; tb.ReadOnly = true; cb.Visible = true; break; // tb.Enabled = false; 
                default: la.Visible = false; nud.Visible = false; tb.Visible = false; cb.Visible = false; break;
            }
        }


        //Update scrollbar -> text
        private void sipThresholdBar_Scroll(object sender, EventArgs e)
        {
            sipThresholdLabel.Text = sipThresholdBar.Value.ToString();
        }
        private void puffThresholdBar_Scroll(object sender, EventArgs e)
        {
            puffThresholdLabel.Text = puffThresholdBar.Value.ToString();
        }
        private void timeThresholdBar_Scroll(object sender, EventArgs e)
        {
            timeThresholdLabel.Text = timeThresholdBar.Value.ToString();
        }
        private void Button1LongSlide_Scroll(object sender, EventArgs e)
        {
            Button1LongTimeLabel.Text = Button1LongSlide.Value.ToString();
        }
        private void Button2LongSlide_Scroll(object sender, EventArgs e)
        {
            Button2LongTimeLabel.Text = Button2LongSlide.Value.ToString();
        }
        private void Button3LongSlide_Scroll(object sender, EventArgs e)
        {
            Button3LongTimeLabel.Text = Button3LongSlide.Value.ToString();
        }
        private void Button4LongSlide_Scroll(object sender, EventArgs e)
        {
            Button4LongTimeLabel.Text = Button4LongSlide.Value.ToString();
        }
        private void Button5LongSlide_Scroll(object sender, EventArgs e)
        {
            Button5LongTimeLabel.Text = Button5LongSlide.Value.ToString();
        }
        private void Button6LongSlide_Scroll(object sender, EventArgs e)
        {
            Button6LongTimeLabel.Text = Button6LongSlide.Value.ToString();
        }

        private void SipFunctionMenu_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(SipFunctionMenu.SelectedIndex, SipParameterText, SipNumericParameter, SipComboBox, SipParameterLabel);
        }

        private void LongSipFunctionMenu_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(LongSipFunctionMenu.SelectedIndex, LongSipParameterText, LongSipNumericParameter, LongSipComboBox, LongSipParameterLabel);
        }

        private void PuffFunctionMenu_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(PuffFunctionMenu.SelectedIndex, PuffParameterText, PuffNumericParameter, PuffComboBox, PuffParameterLabel);
        }

        private void LongPuffFunctionMenu_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(LongPuffFunctionMenu.SelectedIndex, LongPuffParameterText, LongPuffNumericParameter, LongPuffComboBox, LongPuffParameterLabel);
        }



        //Update the parameter fields, according to the action
        private void Button1FunctionBox_SelectedIndexChanged_1(object sender, EventArgs e)
        {
            updateVisibility(Button1FunctionBox.SelectedIndex, Button1ParameterText, Button1NumericParameter, Button1ComboBox, Button1Label);
        }

        private void Button2FunctionBox_SelectedIndexChanged_1(object sender, EventArgs e)
        {
            updateVisibility(Button2FunctionBox.SelectedIndex, Button2ParameterText, Button2NumericParameter, Button2ComboBox, Button2Label);
        }

        private void Button3FunctionBox_SelectedIndexChanged_1(object sender, EventArgs e)
        {
            updateVisibility(Button3FunctionBox.SelectedIndex, Button3ParameterText, Button3NumericParameter, Button3ComboBox, Button3Label);
        }

        private void Button4FunctionBox_SelectedIndexChanged_1(object sender, EventArgs e)
        {
            updateVisibility(Button4FunctionBox.SelectedIndex, Button4ParameterText, Button4NumericParameter, Button4ComboBox, Button4Label);
        }

        private void Button5FunctionBox_SelectedIndexChanged_1(object sender, EventArgs e)
        {
            updateVisibility(Button5FunctionBox.SelectedIndex, Button5ParameterText, Button5NumericParameter, Button5ComboBox, Button5Label);
        }

        private void Button6FunctionBox_SelectedIndexChanged_1(object sender, EventArgs e)
        {
            updateVisibility(Button6FunctionBox.SelectedIndex, Button6ParameterText, Button6NumericParameter, Button6ComboBox, Button6Label);
        }

        private void Button1LongFunctionBox_SelectedIndexChanged_1(object sender, EventArgs e)
        {
            updateVisibility(Button1LongFunctionBox.SelectedIndex, Button1LongParameterText, Button1LongNumericParameter, Button1LongComboBox, Button1LongLabel);
        }

        private void Button2LongFunctionBox_SelectedIndexChanged_1(object sender, EventArgs e)
        {
            updateVisibility(Button2LongFunctionBox.SelectedIndex, Button2LongParameterText, Button2LongNumericParameter, Button2LongComboBox, Button2LongLabel);
        }

        private void Button3LongFunctionBox_SelectedIndexChanged_1(object sender, EventArgs e)
        {
            updateVisibility(Button3LongFunctionBox.SelectedIndex, Button3LongParameterText, Button3LongNumericParameter, Button3LongComboBox, Button3LongLabel);
        }

        private void Button4LongFunctionBox_SelectedIndexChanged_1(object sender, EventArgs e)
        {
            updateVisibility(Button4LongFunctionBox.SelectedIndex, Button4LongParameterText, Button4LongNumericParameter, Button4LongComboBox, Button4LongLabel);
        }

        private void Button5LongFunctionBox_SelectedIndexChanged_1(object sender, EventArgs e)
        {
            updateVisibility(Button5LongFunctionBox.SelectedIndex, Button5LongParameterText, Button5LongNumericParameter, Button5LongComboBox, Button5LongLabel);
        }

        private void Button6LongFunctionBox_SelectedIndexChanged_1(object sender, EventArgs e)
        {
            updateVisibility(Button6LongFunctionBox.SelectedIndex, Button6LongParameterText, Button6LongNumericParameter, Button6LongComboBox, Button6LongLabel);
        }


        private void SipComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(SipComboBox, SipParameterText);
        }

        private void LongSipComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(LongSipComboBox, LongSipParameterText);
        }

        private void PuffComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(PuffComboBox, PuffParameterText);
        }

        private void LongPuffComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(LongPuffComboBox, LongPuffParameterText);
        }


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

        // updating the keycode parameters:
        private void Button1ComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(Button1ComboBox,Button1ParameterText);
        }
        private void Button2ComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(Button2ComboBox, Button2ParameterText);
        }
        private void Button3ComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(Button3ComboBox, Button3ParameterText);
        }
        private void Button4ComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(Button4ComboBox, Button4ParameterText);
        }
        private void Button5ComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(Button5ComboBox, Button5ParameterText);
        }
        private void Button6ComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(Button6ComboBox, Button6ParameterText);
        }


        private void Button1LongComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(Button1LongComboBox, Button1LongParameterText);
        }
        private void Button2LongComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(Button2LongComboBox, Button2LongParameterText);
        }
        private void Button3LongComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(Button3LongComboBox, Button3LongParameterText);
        }
        private void Button4LongComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(Button4LongComboBox, Button4LongParameterText);
        }
        private void Button5LongComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(Button5LongComboBox, Button5LongParameterText);
        }
        private void Button6LongComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(Button6LongComboBox, Button6LongParameterText);
        }
    }
}
