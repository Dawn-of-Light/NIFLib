using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.IO;


namespace NIFLib_ExampleFrameWork
{
	/// <summary>
	/// Main view
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.OpenFileDialog openFileDialog1;
		private System.Windows.Forms.MainMenu mainMenu1;
		private System.Windows.Forms.MenuItem menuItem1;
		private System.Windows.Forms.MenuItem menuItem2;
		private System.Windows.Forms.MenuItem menuItem3;
		private NIF.View view1;
		private System.Windows.Forms.MenuItem menuItem5;
		private System.Windows.Forms.MenuItem menuItem6;
		private System.Windows.Forms.MenuItem menuItem7;
		private System.Windows.Forms.MenuItem menuItem4;
		private System.Windows.Forms.OpenFileDialog openFileDialog_daoc;
		private System.Windows.Forms.MenuItem menuItem8;
		private System.Windows.Forms.MenuItem menuItem10;
		private System.Windows.Forms.MenuItem menuItem11;
		private System.Windows.Forms.MenuItem menuItem9;


		public Form1()
		{
			
			InitializeComponent();

		}

		/// <summary>
		/// Clear resources
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			base.Dispose( disposing );
		}

		#region Vom Windows Form-Designer generierter Code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung. 
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		private void InitializeComponent()
		{
			this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
			this.mainMenu1 = new System.Windows.Forms.MainMenu();
			this.menuItem1 = new System.Windows.Forms.MenuItem();
			this.menuItem2 = new System.Windows.Forms.MenuItem();
			this.menuItem5 = new System.Windows.Forms.MenuItem();
			this.menuItem3 = new System.Windows.Forms.MenuItem();
			this.menuItem9 = new System.Windows.Forms.MenuItem();
			this.menuItem8 = new System.Windows.Forms.MenuItem();
			this.menuItem4 = new System.Windows.Forms.MenuItem();
			this.menuItem6 = new System.Windows.Forms.MenuItem();
			this.menuItem7 = new System.Windows.Forms.MenuItem();
			this.view1 = new NIF.View();
			this.openFileDialog_daoc = new System.Windows.Forms.OpenFileDialog();
			this.menuItem10 = new System.Windows.Forms.MenuItem();
			this.menuItem11 = new System.Windows.Forms.MenuItem();
			this.SuspendLayout();
			// 
			// openFileDialog1
			// 
			this.openFileDialog1.Filter = "Nif and Npk|*.nif;*.npk|All Files|*.*";
			this.openFileDialog1.RestoreDirectory = true;
			// 
			// mainMenu1
			// 
			this.mainMenu1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					  this.menuItem1,
																					  this.menuItem6});
			// 
			// menuItem1
			// 
			this.menuItem1.Index = 0;
			this.menuItem1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					  this.menuItem2,
																					  this.menuItem5,
																					  this.menuItem3,
																					  this.menuItem9,
																					  this.menuItem8,
																					  this.menuItem4});
			this.menuItem1.Text = "File";
			// 
			// menuItem2
			// 
			this.menuItem2.Index = 0;
			this.menuItem2.Text = "Open NIF ...";
			this.menuItem2.Click += new System.EventHandler(this.menuItem2_Click);
			// 
			// menuItem5
			// 
			this.menuItem5.Index = 1;
			this.menuItem5.Text = "Open ID ...";
			this.menuItem5.Click += new System.EventHandler(this.menuItem5_Click);
			// 
			// menuItem3
			// 
			this.menuItem3.Index = 2;
			this.menuItem3.Text = "-";
			// 
			// menuItem9
			// 
			this.menuItem9.Index = 3;
			this.menuItem9.Text = "DAoC Path...";
			this.menuItem9.Click += new System.EventHandler(this.menuItem9_Click);
			// 
			// menuItem8
			// 
			this.menuItem8.Index = 4;
			this.menuItem8.Text = "-";
			// 
			// menuItem4
			// 
			this.menuItem4.Index = 5;
			this.menuItem4.Text = "Exit";
			this.menuItem4.Click += new System.EventHandler(this.menuItem4_Click);
			// 
			// menuItem6
			// 
			this.menuItem6.Index = 1;
			this.menuItem6.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					  this.menuItem7,
																					  this.menuItem10,
																					  this.menuItem11});
			this.menuItem6.Text = "View";
			// 
			// menuItem7
			// 
			this.menuItem7.Index = 0;
			this.menuItem7.Text = "Rotate";
			this.menuItem7.Click += new System.EventHandler(this.menuItem7_Click);
			// 
			// view1
			// 
			this.view1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.view1.Location = new System.Drawing.Point(0, 0);
			this.view1.Name = "view1";
			this.view1.Size = new System.Drawing.Size(648, 486);
			this.view1.TabIndex = 0;
			// 
			// openFileDialog_daoc
			// 
			this.openFileDialog_daoc.FileName = "camelot.exe";
			this.openFileDialog_daoc.Filter = "Camelot.exe|camelot.exe|All (*.*)|*.*";
			this.openFileDialog_daoc.RestoreDirectory = true;
			// 
			// menuItem10
			// 
			this.menuItem10.Index = 1;
			this.menuItem10.Text = "-";
			// 
			// menuItem11
			// 
			this.menuItem11.Index = 2;
			this.menuItem11.Text = "Delta camera movement";
			this.menuItem11.Click += new System.EventHandler(this.menuItem11_Click);
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(648, 486);
			this.Controls.Add(this.view1);
			this.Menu = this.mainMenu1;
			this.Name = "Form1";
			this.Text = "Form1";
			this.Load += new System.EventHandler(this.Form1_Load);
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// Main.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new Form1());
		}

		private void menuItem2_Click(object sender, System.EventArgs e)
		{
			if (openFileDialog1.ShowDialog() == DialogResult.OK)
			{
				String path;
				String file;

				int pos = openFileDialog1.FileName.LastIndexOfAny("\\/".ToCharArray());
				if (pos<0) return;

				openFileDialog1.FileName.ToLower();
				if (openFileDialog1.FileName.EndsWith(".npk"))
				{
					path = openFileDialog1.FileName;
					file = openFileDialog1.FileName.Substring(pos+1);
					file.Replace(".npk",".nif");
				}
				else
				{
					path = openFileDialog1.FileName.Substring(0,pos+1);
					file = openFileDialog1.FileName.Substring(pos+1);
				}

				view1.load(path,file);
			}
		}

		private void menuItem4_Click(object sender, System.EventArgs e)
		{
			Close();
		}

		private void menuItem5_Click(object sender, System.EventArgs e)
		{
			FileSelect dia = new FileSelect();
			if ( dia.ShowDialog() == DialogResult.OK)
			{
				view1.loadDAOC(dia.m_type,dia.m_ID);
			}

		}


		private void menuItem7_Click(object sender, System.EventArgs e)
		{
			view1.mb_rotation = !view1.mb_rotation;
			menuItem7.Checked = view1.mb_rotation;
		}

		private void menuItem11_Click(object sender, System.EventArgs e)
		{
			view1.mb_delta_camera = !view1.mb_delta_camera;
			menuItem11.Checked = view1.mb_delta_camera;
		}


		private void menuItem9_Click(object sender, System.EventArgs e)
		{
			if (openFileDialog_daoc.ShowDialog() == DialogResult.OK)
			{
				String path;

				int pos = openFileDialog_daoc.FileName.LastIndexOfAny("\\/".ToCharArray());
				if (pos<0) return;

				path = openFileDialog_daoc.FileName.Substring(0,pos+1);

			
				NIF.DAOCAPI.SetRootDirectory(path);

				StreamWriter sr = File.CreateText("config.dat");
				sr.WriteLine (path);
				sr.Close();
			}
		}

		private void Form1_Load(object sender, System.EventArgs e)
		{
			if (!File.Exists("config.dat")) 
			{
				MessageBox.Show("You have to setup the path to DAoC first.", "No path set",
					MessageBoxButtons.OK, MessageBoxIcon.Warning);
				menuItem9_Click(null,null);
			}

			if (File.Exists("config.dat")) 
			{
				StreamReader sr = File.OpenText("config.dat");
				String input;
				input=sr.ReadLine();
				sr.Close();

				NIF.DAOCAPI.SetRootDirectory(input);
			}

			menuItem7.Checked = view1.mb_rotation;
			menuItem11.Checked = view1.mb_delta_camera;
		}


	

	}
}
