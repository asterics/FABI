using System;
using System.Windows.Forms;
using System.Drawing;
using System.IO;


namespace FabiGUI
{
    public partial class FabiGUI
    {
        // draw live values on panels 
        public void drawRawValues(String newValues)
        {
            
            if (newValues.Length == 0)
                return;

            String[] values = newValues.Split(',');

            if (values.Length >= 3)
            {
                Int32 value = Convert.ToInt32(values[0]); //1023-Convert.ToInt32(values[0]);
                pressureLabel.Text = value.ToString();
                Graphics g = pressureDrawPanel.CreateGraphics();
                Brush brush = new SolidBrush(Color.Green);
                Brush brush2 = new SolidBrush(Color.White);
                value = value * pressureDrawPanel.Height / 1024;
                g.FillRectangle(brush, 0, pressureDrawPanel.Height - value, pressureDrawPanel.Width-1, value);
                g.FillRectangle(brush2, 0, 0, pressureDrawPanel.Width-1, pressureDrawPanel.Height - value);

                brush = new SolidBrush(Color.Red);
                g = puffLevelIndicatorPanel.CreateGraphics();
                if (Convert.ToInt32(values[0]) > puffThresholdBar.Value)
                    g.FillRectangle(brush, 0, 0, puffLevelIndicatorPanel.Width - 1, puffLevelIndicatorPanel.Height - 1);
                else
                    g.FillRectangle(brush2, 0, 0, puffLevelIndicatorPanel.Width - 1, puffLevelIndicatorPanel.Height - 1);
                
                g = sipLevelIndicatorPanel.CreateGraphics();
                if (Convert.ToInt32(values[0]) < sipThresholdBar.Value)
                    g.FillRectangle(brush, 0, 0, sipLevelIndicatorPanel.Width - 1, sipLevelIndicatorPanel.Height - 1);
                else
                    g.FillRectangle(brush2, 0, 0, sipLevelIndicatorPanel.Width - 1, sipLevelIndicatorPanel.Height - 1);


                g = button1IndicatorPanel.CreateGraphics();
                if (values[1].Substring(0, 1) == "1")
                    g.FillRectangle(brush, 0, 0, button1IndicatorPanel.Width - 1, button1IndicatorPanel.Height - 1);
                else
                    g.FillRectangle(brush2, 0, 0, button1IndicatorPanel.Width - 1, button1IndicatorPanel.Height - 1);

                g = button2IndicatorPanel.CreateGraphics();
                if (values[1].Substring(1, 1) == "1")
                    g.FillRectangle(brush, 0, 0, button2IndicatorPanel.Width - 1, button2IndicatorPanel.Height - 1);
                else
                    g.FillRectangle(brush2, 0, 0, button2IndicatorPanel.Width - 1, button2IndicatorPanel.Height - 1);

                g = button3IndicatorPanel.CreateGraphics();
                if (values[1].Substring(2, 1) == "1")
                    g.FillRectangle(brush, 0, 0, button3IndicatorPanel.Width - 1, button3IndicatorPanel.Height - 1);
                else
                    g.FillRectangle(brush2, 0, 0, button3IndicatorPanel.Width - 1, button3IndicatorPanel.Height - 1);

                g = button4IndicatorPanel.CreateGraphics();
                if (values[1].Substring(3, 1) == "1")
                    g.FillRectangle(brush, 0, 0, button4IndicatorPanel.Width - 1, button4IndicatorPanel.Height - 1);
                else
                    g.FillRectangle(brush2, 0, 0, button4IndicatorPanel.Width - 1, button4IndicatorPanel.Height - 1);

                g = button5IndicatorPanel.CreateGraphics();
                if (values[1].Substring(4, 1) == "1")
                    g.FillRectangle(brush, 0, 0, button5IndicatorPanel.Width - 1, button5IndicatorPanel.Height - 1);
                else
                    g.FillRectangle(brush2, 0, 0, button5IndicatorPanel.Width - 1, button5IndicatorPanel.Height - 1);

                g = button6IndicatorPanel.CreateGraphics();
                if (values[1].Substring(5, 1) == "1")
                    g.FillRectangle(brush, 0, 0, button6IndicatorPanel.Width - 1, button6IndicatorPanel.Height - 1);
                else
                    g.FillRectangle(brush2, 0, 0, button6IndicatorPanel.Width - 1, button6IndicatorPanel.Height - 1);

                g = button7IndicatorPanel.CreateGraphics();
                if (values[1].Substring(6, 1) == "1")
                    g.FillRectangle(brush, 0, 0, button7IndicatorPanel.Width - 1, button7IndicatorPanel.Height - 1);
                else
                    g.FillRectangle(brush2, 0, 0, button7IndicatorPanel.Width - 1, button7IndicatorPanel.Height - 1);

                g = button8IndicatorPanel.CreateGraphics();
                if (values[1].Substring(7, 1) == "1")
                    g.FillRectangle(brush, 0, 0, button8IndicatorPanel.Width - 1, button8IndicatorPanel.Height - 1);
                else
                    g.FillRectangle(brush2, 0, 0, button8IndicatorPanel.Width - 1, button8IndicatorPanel.Height - 1);

                g = button9IndicatorPanel.CreateGraphics();
                if (values[1].Substring(8, 1) == "1")
                    g.FillRectangle(brush, 0, 0, button9IndicatorPanel.Width - 1, button9IndicatorPanel.Height - 1);
                else
                    g.FillRectangle(brush2, 0, 0, button9IndicatorPanel.Width - 1, button9IndicatorPanel.Height - 1);

                int goToSlot = Convert.ToInt32(values[2])-1;
                if ((slots.Count > goToSlot) && (goToSlot>=0))
                {
                   actSlot = goToSlot;
                   displaySlot(actSlot);
                }

            }
            else Console.WriteLine("got unknown VALUES:" + newValues);
        }

        public void drawFreeMem(int percent)
        {    
                Graphics g = freeMemPanel.CreateGraphics();
                Brush brush = new SolidBrush(Color.LightBlue);
                Brush brush2 = new SolidBrush(Color.White);
                percent = percent * freeMemPanel.Width / 100;
                g.FillRectangle(brush2, 0, 0, freeMemPanel.Width - 1, freeMemPanel.Height - 1);
                g.FillRectangle(brush, 0, 0, percent, freeMemPanel.Height - 1);
        }
    }
}
