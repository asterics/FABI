using System;
using System.Windows.Forms;
using System.Drawing;
using System.IO;


namespace MouseApp2
{
    public partial class FabiGUI
    {
        // draw live values on panels 
        public void drawRawValues(String newValues)
        {
            
            if (newValues.Length == 0)
                return;

            String[] values = newValues.Split(',');

            if (values.Length == 2)
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
                
            }
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
