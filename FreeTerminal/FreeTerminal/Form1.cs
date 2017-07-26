using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace FreeTerminal
{
    public partial class Form1 : Form
    {
        string A = "";
        public Form1()
        {
            InitializeComponent();
        }

        private void openPort_Click(object sender, EventArgs e)
        {
            serialPort1.Open();
            openPort.Enabled = false;
            getAddress.Enabled = true;
            resetZero.Enabled = true;
            closePort.Enabled = true;

        }

        private void getAddress_Click(object sender, EventArgs e)
        {
            serialPort1.Write("&00000000A000$");
            System.Threading.Thread.Sleep(500);
            A = serialPort1.ReadExisting();
            if (A != "")
           {
                addressBox.Text = A;
           }
           A = "";
        }

        private void resetZero_Click(object sender, EventArgs e)
        {
            serialPort1.Write("&17060002R000$");
            System.Threading.Thread.Sleep(500);
            A = serialPort1.ReadExisting();
            if (A == "&17060002R000$")
            {
                MessageBox.Show("Нуль сброшен");
            }
            A = "";
        }

        private void closePort_Click(object sender, EventArgs e)
        {
            serialPort1.Close();
            addressBox.Text = "";
            openPort.Enabled = true;
            getAddress.Enabled = false;
            resetZero.Enabled = false;
            closePort.Enabled = false;
        }
    }
}
