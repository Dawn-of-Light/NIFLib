using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Text;

namespace NIFLib_ExampleFrameWork
{
	/// <summary>
	/// Object, terrain and figure browser
	/// </summary>
	public class FileSelect : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Panel panel1;
		private System.Windows.Forms.RadioButton radioButton1;
		private System.Windows.Forms.RadioButton radioButton2;
		private System.Windows.Forms.RadioButton radioButton3;
		private System.Windows.Forms.ListView result_view;
		private System.Windows.Forms.ColumnHeader field1;
		private System.Windows.Forms.ColumnHeader field2;
		/// <summary>
		/// Designervars
		/// </summary>
		private System.ComponentModel.Container components = null;

		public	int		m_type;
		private System.Windows.Forms.ColumnHeader field3; // 0-> monster, 1-> item, 2->zone
		public	int		m_ID;

		public FileSelect()
		{
			//
			// Form-Designersupport
			//
			InitializeComponent();
		}

		/// <summary>
		/// Clear resources
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Vom Windows Form-Designer generierter Code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung. 
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		private void InitializeComponent()
		{
			this.panel1 = new System.Windows.Forms.Panel();
			this.radioButton3 = new System.Windows.Forms.RadioButton();
			this.radioButton2 = new System.Windows.Forms.RadioButton();
			this.radioButton1 = new System.Windows.Forms.RadioButton();
			this.result_view = new System.Windows.Forms.ListView();
			this.field1 = new System.Windows.Forms.ColumnHeader();
			this.field2 = new System.Windows.Forms.ColumnHeader();
			this.field3 = new System.Windows.Forms.ColumnHeader();
			this.panel1.SuspendLayout();
			this.SuspendLayout();
			// 
			// panel1
			// 
			this.panel1.Controls.Add(this.radioButton3);
			this.panel1.Controls.Add(this.radioButton2);
			this.panel1.Controls.Add(this.radioButton1);
			this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
			this.panel1.Location = new System.Drawing.Point(0, 0);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(528, 48);
			this.panel1.TabIndex = 0;
			// 
			// radioButton3
			// 
			this.radioButton3.Location = new System.Drawing.Point(320, 8);
			this.radioButton3.Name = "radioButton3";
			this.radioButton3.TabIndex = 2;
			this.radioButton3.Text = "Zones";
			this.radioButton3.CheckedChanged += new System.EventHandler(this.radioButton1_CheckedChanged);
			// 
			// radioButton2
			// 
			this.radioButton2.Location = new System.Drawing.Point(176, 8);
			this.radioButton2.Name = "radioButton2";
			this.radioButton2.TabIndex = 1;
			this.radioButton2.Text = "Object";
			this.radioButton2.CheckedChanged += new System.EventHandler(this.radioButton1_CheckedChanged);
			// 
			// radioButton1
			// 
			this.radioButton1.Location = new System.Drawing.Point(24, 8);
			this.radioButton1.Name = "radioButton1";
			this.radioButton1.Size = new System.Drawing.Size(112, 24);
			this.radioButton1.TabIndex = 0;
			this.radioButton1.Text = "Monster/NPC/PC";
			this.radioButton1.CheckedChanged += new System.EventHandler(this.radioButton1_CheckedChanged);
			// 
			// result_view
			// 
			this.result_view.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
																						  this.field1,
																						  this.field2,
																						  this.field3});
			this.result_view.Dock = System.Windows.Forms.DockStyle.Fill;
			this.result_view.FullRowSelect = true;
			this.result_view.Location = new System.Drawing.Point(0, 48);
			this.result_view.MultiSelect = false;
			this.result_view.Name = "result_view";
			this.result_view.Size = new System.Drawing.Size(528, 366);
			this.result_view.TabIndex = 1;
			this.result_view.View = System.Windows.Forms.View.Details;
			this.result_view.DoubleClick += new System.EventHandler(this.result_view_DoubleClick);
			// 
			// field1
			// 
			this.field1.Text = "ID";
			// 
			// field2
			// 
			this.field2.Text = "Name";
			// 
			// field3
			// 
			this.field3.Text = "Info";
			// 
			// FileSelect
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(528, 414);
			this.Controls.Add(this.result_view);
			this.Controls.Add(this.panel1);
			this.Name = "FileSelect";
			this.Text = "FileSelect";
			this.panel1.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion



		private void radioButton1_CheckedChanged(object sender, System.EventArgs e)
		{
			
			Cursor = Cursors.WaitCursor;

			int[] ids = new int[0];
			int nof = 0;

			// Get ID's depending on selection
			if (radioButton1.Checked)
			{
				// get all IDs of Monsters
				nof = NIF.DAOCAPI.GetMonsterIDs(ids,0);
				ids = new int[nof];
				nof = NIF.DAOCAPI.GetMonsterIDs(ids,nof);
			}
			if (radioButton2.Checked)
			{
				// get all IDs of Items
				nof = NIF.DAOCAPI.GetItemIDs(ids,0);
				ids = new int[nof];
				nof = NIF.DAOCAPI.GetItemIDs(ids,nof);
			}
			if (radioButton3.Checked)
			{
				// get all IDs of Items
				nof = NIF.DAOCAPI.GetZoneIDs(ids,0);
				ids = new int[nof];
				nof = NIF.DAOCAPI.GetZoneIDs(ids,nof);
			}


			// begin listview fill
			result_view.BeginUpdate();
			result_view.Items.Clear();

			for (int t=0;t<nof;t++)
			{
				StringBuilder s_name = new StringBuilder(256);
				StringBuilder s_info = new StringBuilder(256);

				if (radioButton1.Checked)
				{
					NIF.DAOCAPI.GetMonsterName(ids[t],s_name,256);
					NIF.DAOCAPI.GetMonsterInfo(ids[t],s_info,256);
				}
				if (radioButton2.Checked)
				{
					NIF.DAOCAPI.GetItemName(ids[t],s_name,256);
					NIF.DAOCAPI.GetItemInfo(ids[t],s_info,256);

				}
				if (radioButton3.Checked)
				{
					NIF.DAOCAPI.GetZoneName(ids[t],s_name,256);
					NIF.DAOCAPI.GetZoneInfo(ids[t],s_info,256);

				}

				ListViewItem item1 = new ListViewItem(String.Format("{0}", ids[t]));
				item1.SubItems.Add(String.Format("{0}", s_name));
				item1.SubItems.Add(String.Format("{0}", s_info));
				item1.Tag = ids[t];

				result_view.Items.Add(item1);
				Application.DoEvents();
			}

			result_view.Columns[0].Width = -1;
			result_view.Columns[1].Width = -1;
			result_view.Columns[2].Width = -1;
			result_view.EndUpdate();
			// end

			Cursor = Cursors.Default;
		}


		private void result_view_DoubleClick(object sender, System.EventArgs e)
		{
			m_type = -1;
			if (radioButton1.Checked)
				m_type = 0;
			if (radioButton2.Checked)
				m_type = 1;
			if (radioButton3.Checked)
				m_type = 2;
			
			m_ID = (int)result_view.SelectedItems[0].Tag;

			DialogResult = DialogResult.OK;
			Close();
		}
	}
}
