using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Diagnostics;

namespace Lab6
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void CheckEnterKeyPress(object sender, System.Windows.Forms.KeyPressEventArgs e)
        {
            if (textBox1.Text != String.Empty)
            {
                if (e.KeyChar == (char)Keys.Return)
                {
                    listBox1.Items.Add(textBox1.Text);
                    textBox1.Text = "";
                    textBox1.Select();
                }
            }
       }

        private void button1_Click(object sender, EventArgs e)
        {
            if (textBox1.Text == String.Empty)
            {
                listBox1.Items.Add("Do Nothing");
                textBox1.Select();
            }
            else
            {
                listBox1.Items.Add(textBox1.Text);
                textBox1.Text = "";
                textBox1.Select();
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            listBox1.Items.RemoveAt(listBox1.SelectedIndex);
        }

        private void button4_Click(object sender, EventArgs e)
        {
            listBox1.Items.Clear();
            listBox2.Items.Clear();
        }

        private void button5_Click(object sender, EventArgs e)
        {
            FontDialog fd = new FontDialog();
            if (fd.ShowDialog() == DialogResult.OK)
            {
                this.Font = fd.Font;
            }
        }

        private void button6_Click(object sender, EventArgs e)
        {
            if (colorDialog1.ShowDialog() == DialogResult.OK)
            {
                this.listBox1.BackColor = colorDialog1.Color;
                this.listBox2.BackColor = colorDialog1.Color;
            }
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            this.textBox1.KeyPress += new System.Windows.Forms.KeyPressEventHandler(CheckEnterKeyPress);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            listBox2.Items.Add(listBox1.Text);
            listBox1.Items.RemoveAt(listBox1.SelectedIndex);
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void button7_Click(object sender, EventArgs e)
        {
            listBox1.Items.Add(listBox2.Text);
            listBox2.Items.RemoveAt(listBox2.SelectedIndex);
        }

        private void label4_Click(object sender, EventArgs e)
        {
            
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Svetlana Sumilin \nLaboratory Work 6\n11. 06. 2014", "About", MessageBoxButtons.OK);
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (listBox1.Items.Count > 0)
            {
                using (TextWriter TW = new StreamWriter("list.txt"))
                {
                    foreach (string itemText in listBox1.Items)
                    {
                        TW.WriteLine(itemText);
                    }
                }
                Process.Start("list.txt");
            }
        }

        private void copyToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (listBox1.Items.Count > 0)
            {
                StringBuilder SB = new StringBuilder();
                foreach (string itemValue in listBox1.Items)
                {
                    SB.AppendLine(itemValue);
                }
                string result = SB.ToString().TrimEnd('\n');

                Clipboard.SetText(result);
            }
        }

    }
}
