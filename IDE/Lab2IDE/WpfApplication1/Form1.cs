using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Media;

namespace WpfApplication1
{
    public partial class UseIt : Form
    {
        double seconds = 0;

        public UseIt()
        {
            InitializeComponent();
        }

        private void UseIt_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            SoundPlayer simpleSound = new SoundPlayer(@"D:\University\С++\IDE\Lab2IDE\WpfApplication1\Click.wav");
            simpleSound.Play();
            Form2 form = new Form2();
            form.Show();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            SoundPlayer simpleSound = new SoundPlayer(@"D:\University\С++\IDE\Lab2IDE\WpfApplication1\Laser.wav");
            simpleSound.Play();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            SoundPlayer simpleSound = new SoundPlayer(@"D:\University\С++\IDE\Lab2IDE\WpfApplication1\Light.wav");
            simpleSound.Play();
        }

        private DateTime startTime;

        private void button4_Click(object sender, EventArgs e)
        {
            SoundPlayer simpleSounde = new SoundPlayer(@"D:\University\С++\IDE\Lab2IDE\WpfApplication1\Self.wav");
            simpleSounde.Play();
            if ((result.Text == "00")||(result.Text == "0"))
            {
                result.Clear();
                result.Text = result.Text + "60";
            }
            
            seconds = Double.Parse(result.Text);

            startTime = DateTime.Now;
            
            
            while (seconds >= 0)
            {
                int elapsedSeconds = (int)(DateTime.Now - startTime).TotalSeconds;
                if (elapsedSeconds == 1)
                {
                    result.Text = (seconds - 10).ToString();
                    seconds = Double.Parse(result.Text);
                    elapsedSeconds = 0;
                    startTime = DateTime.Now;
                    this.Refresh();
                    if (seconds == 4)
                    {
                        SoundPlayer simpleSound = new SoundPlayer(@"D:\University\С++\IDE\WpfApplication1\WpfApplication1\clock.wav");
                        simpleSound.Play();
                    }
                    if (seconds == 0)
                    {
                        this.Close();
                    }
                }
                
            }
        }





        private void button6_Click(object sender, EventArgs e)
        {
            result.Clear();
            result.Text = result.Text + "00";
        }
        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            
        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            label1.Text =  trackBar1.Value.ToString();
            if (trackBar1.Value == 0)
            { this.Opacity = 1; }
            else { this.Opacity = trackBar1.Value - trackBar1.Value * 0.7; }
        }

        private void button5_Click(object sender, EventArgs e)
        {
            label1.Text = "0";
            trackBar1.Value = 0;
            this.Opacity = 1;
        }

    }
}

namespace SoundApiExample
{
    public class SoundTestForm : System.Windows.Forms.Form
    {  
        private SoundPlayer player;
        private void InitializeSound()
        {
                   player = new SoundPlayer();
        }
    }
}