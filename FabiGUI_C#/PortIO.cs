using System;
using System.Windows.Forms;
using System.Drawing;
using System.Threading.Tasks;
using System.Threading;
using System.IO.Ports;
using System.IO;


namespace FabiGUI
{
    public partial class FabiGUI
    {
        int fabiOnline = 0;


        // serial port / communication handling
        private bool Connect(string portName)
        {
            if (!serialPort1.IsOpen)
            {
                serialPort1.PortName = portName;
                serialPort1.BaudRate = 115200;
                serialPort1.DataBits = 8;
                serialPort1.Parity = Parity.None;
                serialPort1.Handshake = Handshake.None;
                serialPort1.DtrEnable = true;
                serialPort1.ReadTimeout = 10000;
                serialPort1.WriteTimeout = 10000;
                serialPort1.NewLine = "\n";
                serialPort1.Encoding = System.Text.Encoding.Default;

                try
                {
                    serialPort1.Open();
                    return (true);
                }
                catch (Exception ex)
                {
                    addToLog("Could not open COM port");
                }
            }
            return (false);
        }

        private void updateComPorts()
        {
            var ports = SerialPort.GetPortNames();
            portComboBox.DataSource = ports;
        }

        private void sendCmd(string command)
        {
            if (serialPort1.IsOpen)
            {
                Console.WriteLine("Send:" + command);
                try
                {
                    serialPort1.Write(command + "\r");
                }
                catch (Exception ex)
                {
                    Console.WriteLine("Could not write to COM port:" + ex.ToString());
                    addToLog("Could not write to COM port:" + ex.ToString());
                }
            }
        }

        public void WorkThreadFunction()
        {
            String receivedString = "";

            Console.WriteLine("sending ID request ..");
            sendIDCommand();   // start (after connect): request ID String from Lipmouse; receive ID before timeout ! (else close port)

            try
            {
                while (serialPort1.IsOpen && !readDone)
                {
                    try
                    {
                        receivedString = serialPort1.ReadLine();
                        //  byte[] my_bytes = System.Text.Encoding.Default.GetBytes(command);

                        if (!receivedString.Contains("VALUES"))
                            Console.Write("received:" + receivedString);

                        BeginInvoke(this.stringReceivedDelegate, new Object[] { receivedString });
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine("Could not read from COM port: " + ex.ToString());
                    }
                }
            }
            catch (Exception ex)
            {
            }
        }


        void IdTimer_Tick(object sender, EventArgs e)
        {
            IdTimer.Stop();

            if (fabiOnline == 0)
            {
                addToLog("ID-Timeout ! No Fabi module found !");
                disconnnectComButton_Click(this, null);
            }
        }


        public void stringReceived(String newLine)
        {
            if (newLine.ToUpper().Contains(PREFIX_FABI_VERSION))  // read FABI Version ID 
            {
                gotID(newLine);
            }
            else if (newLine.ToUpper().StartsWith(PREFIX_REPORT_VALUES))  // read raw report (ADC values)
            {
                drawRawValues(newLine.Substring(PREFIX_REPORT_VALUES.Length));
            }
            else if (newLine.ToUpper().StartsWith(PREFIX_SLOT_NAME))  // read next slot name 
            {
                gotSlotNames(newLine.Substring(PREFIX_SLOT_NAME.Length));
            }
            else if (newLine.ToUpper().StartsWith(PREFIX_AT_COMMAND))  // read setting for a slot 
            {
                gotAT(newLine);
            }
            else if (newLine.ToUpper().StartsWith(PREFIX_END_OF_SLOTS))  // end of slot 
            {
                gotEnd();
            }
            else if (newLine.ToUpper().StartsWith(PREFIX_FREE_EEPROM))
            {
                int freePercent = Int32.Parse(newLine.Substring(newLine.IndexOf(":") + 1));
                freeMemLabel.Text = "mem usage:" + (100 - freePercent).ToString() + "%";
                drawFreeMem(100 - freePercent);
            }
            else if (newLine.ToUpper().Contains(PREFIX_START_UPGRADE))  // upgrate BT module firmware
            {
                gotUpgradeReady();
            }
        }

        public void gotEnd()
        {
            if (slots.Count < 1)
                slots.Add(new Slot());

            actSlot = 0;
            displaySlot(actSlot);
            sendFreeMemCommand();

            addToLog("The settings were loaded from Fabi device!");
            sendStartReportingCommand();
        }

        public void gotID(String newLine)
        {
            addToLog("Fabi detected:" + newLine);
            fabiOnline = 1;
            slotNames.Items.Clear();
            sendStartReportingCommand();   // start reporting raw values !

            COMAliveTimer.Enabled = true;
            COMAliveTimer.Start();

            DialogResult dialogResult = MessageBox.Show("Detected " + newLine + "\nDo you want to load the slots and settings which are stored in the Fabi device ?", "Load Settings ?", MessageBoxButtons.YesNo);
            if (dialogResult == DialogResult.Yes)
            {
                loadSettingsFromFabi();
            }
        }

        public void gotUpgradeReady()
        {
            firmwareUpdate_running = true;
            addToLog("Upgrade Process running ...");
            Console.WriteLine("Upgrade Process running ...");

            // Set Minimum to 1 to represent the first file being copied.
            pBar1.Minimum = 1;
            // Set Maximum to the total number of files to copy.
            try
            {
                if (updateFirmwareStream != null)
                {
                    byte[] buf;
                    int count = 0;
                    System.IO.BinaryReader file = new System.IO.BinaryReader(updateFirmwareStream);

                    pBar1.Maximum = (int)updateFirmwareStream.Length;
                    // Set the initial value of the ProgressBar.
                    pBar1.Value = 1;
                    // Set the Step property to a value of 1 to represent each file being copied.
                    pBar1.Step = 1;

                    buf = file.ReadBytes(128);
                    while (buf.Length != 0)
                    {
                        serialPort1.Write(buf, 0, buf.Length);
                        count += buf.Length;
                        pBar1.Value = count;
                        pBar1.Invalidate();
                        pBar1.Update();
                        pBar1.Refresh();
                        Application.DoEvents();
                        // Console.WriteLine(count);
                        Thread.Sleep(10);
                        buf = file.ReadBytes(128);
                    }
                    file.Close();
                    updateFirmwareStream.Close();
                    addToLog("Done.");
                    Console.WriteLine("Done.");
                    File.Delete(downloadPath);
                }
            }
            catch (Exception ex)
            {
                addToLog("Error: Could not read file from disk. Original error: " + ex.Message);
            }
            pBar1.Visible = false;
            updateAddOnButton.Enabled = true;

        }




        public void gotSlotNames(String newSlotName)
        {
            slots.Add(new Slot());
            newSlotName = newSlotName.Replace("\r",""); newSlotName = newSlotName.Replace("\n", "");
            Console.WriteLine("Slotname:" + newSlotName);
            slotNames.Items.Add(newSlotName);
            actSlot = slots.Count - 1;
            slots[actSlot].settingStrings.Clear();
            slots[actSlot].slotName=newSlotName;
        }

        public void gotAT(String newATCommand)
        {
            newATCommand = newATCommand.Replace("\r", ""); 
            newATCommand = newATCommand.Replace("\n", "");
            Console.WriteLine("adding:" + newATCommand);
            if (actSlot < 0) return;
            slots[actSlot].settingStrings.Add(newATCommand);
            if (newATCommand.StartsWith("AT SC"))
            {
                slots[actSlot].slotColor = newATCommand.Substring(6);
                actSlotColor = slots[actSlot].slotColor;
                Console.WriteLine("Color found: " + actSlotColor);
//                drawSlotColor(actSlotColor);
            }
        }


        private void disconnect()
        {
            COMAliveTimer.Stop();
            readDone = true;
            if (serialPort1.IsOpen)
            {
                sendEndReportingCommand();  // end reporting raw values !

                portStatus.Text = "Disconnected";
                addToLog("Port " + portComboBox.Text + " is now disconnected");

                portStatus.ForeColor = Color.SlateGray;

                try
                {
                    slotNames.Items.Clear();
                    serialPort1.Close();
                }
                catch (Exception ex)
                {
                    addToLog("Error disconnecting COM Port");
                }
            }
            fabiOnline = 0;
        }

        public void storeSettingsToFabi()
        {
            if (serialPort1.IsOpen)
            {
                Cursor.Current = Cursors.WaitCursor;
                sendEndReportingCommand();
                sendClearCommand();  // delete all slots

                slotCounter = 0;
                for (slotCounter = 0; slotCounter < slots.Count; slotCounter++)
                {
                    displaySlot(slotCounter);
                    sendApplyCommands();
                    sendSaveSlotCommands(slots[slotCounter].slotName);
                    addToLog("Slot " + slots[slotCounter].slotName + " is stored into Fabi device.");
                }
                addToLog("The settings were stored!");
                sendFreeMemCommand();          
                sendNextCommand();
                sendStartReportingCommand();
                // Thread.Sleep(3000);  // time to activate config
                Cursor.Current = Cursors.Default;
            }

        }

        public void loadSettingsFromFabi()
        {
            if (serialPort1.IsOpen)
            {
                sendEndReportingCommand();
                slotNames.Items.Clear();
                slots.Clear();
                actSlot = -1;
                sendLoadAllCommand();
            }
        }
    }
}


