using System;
using System.Collections;
using System.ComponentModel;
using System.ComponentModel.Design;
using System.Drawing;
using System.Drawing.Design;
using System.Data;
using System.Windows.Forms;
using System.Windows.Forms.Design;

using NIF;
using OpenGL;



namespace NIF
{

	/// <summary>
	/// Example Nif viewer.
	/// </summary>
	public class View : System.Windows.Forms.UserControl
	{
		private System.ComponentModel.IContainer components;

		/// <summary>
		/// Device handle of this control
		/// </summary>
 		private IntPtr	m_hdc;

		/// <summary>
		/// Pointer of loadedobject
		/// </summary>
		private IntPtr	m_current_object;

		/// <summary>
		/// A simple animation (commented out in this example
		/// </summary>
		private	float	mf_rotation;

		/// <summary>
		/// A simple animation (commented out in this example
		/// </summary>
		public	bool	mb_rotation;

		/// <summary>
		/// Camera for easy movment in 3d
		/// </summary>
		private CCamera	camera;

		/// <summary>
		/// Last known mouse position (I need only the delta-movement)
		/// </summary>
		private Point	last_mouse_position;

		/// <summary>
		/// Refresh timer
		/// </summary>
		private System.Windows.Forms.Timer timer1;


		/// <summary>
		/// A OpenGL list which holds the complete model.
		/// That way we can simple speed up the rendering. 
		/// But it has many drawback and (hopefully) it isn't needed in the future.
		/// </summary>
		private uint	mn_gl_list;


		/// <summary>
		/// if true we use a camera which moves each frame depending on mouse delta.
		/// This camera movement isn't very popular but I like it :)
		/// </summary>
		public bool			mb_delta_camera;
		private System.Windows.Forms.MouseEventArgs	me_last_mouseevent;



		public View()
		{
			InitializeComponent();

			// Initialize NIFAPI
			NIFAPI.Create(this.Handle);


			// Get DC handle (you've to release it)
			m_hdc = GL.GetDC(this.Handle);

			// Create camera
			camera = new CCamera();

			// init vars
			last_mouse_position = new Point(0,0);
			m_current_object = new IntPtr(0);
			mf_rotation = 0;
			mb_rotation = true;
			mn_gl_list = 0;
			mb_delta_camera = true;
		}


		protected override void Dispose( bool disposing )
		{
			// release object
			if (m_current_object != IntPtr.Zero)
				NIFAPI.ReleaseObject(m_current_object);

			// release NIFAPI
			NIFAPI.Release();

			// release DC
			GL.ReleaseDC(this.Handle,m_hdc);

			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Vom Komponenten-Designer generierter Code
		/// <summary> 
		/// Erforderliche Methode für die Designerunterstützung. 
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			this.timer1 = new System.Windows.Forms.Timer(this.components);
			// 
			// timer1
			// 
			this.timer1.Interval = 50;
			this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
			// 
			// View
			// 
			this.Name = "View";
			this.Size = new System.Drawing.Size(328, 352);
			this.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.View_KeyPress);
			this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.View_MouseUp);
			this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.View_MouseMove);
			this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.View_MouseDown);

		}
		#endregion


		/// <summary>
		/// the rendering part
		/// </summary>
		protected override void OnPaint(PaintEventArgs e)
		{
			// Clear The Screen And The Depth Buffer
			GL.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT);

			// a simple optimation. 
			if (mn_gl_list == 0 && m_current_object != IntPtr.Zero) 
			{
				GL.glMatrixMode(GL.GL_MODELVIEW);
				GL.glLoadIdentity();
				mn_gl_list = GL.glGenLists(1);
				GL.glNewList(mn_gl_list,GL.GL_COMPILE);
				NIFAPI.Render(m_current_object);
				GL.glEndList();
			}


			// set up the camera view (loads the matrix)
			camera.setView();


			// add a object rotation
			if (mb_rotation)
			{
				mf_rotation += 0.5f;
				GL.glRotatef( mf_rotation, 0.0f, 0.0f,1.0f ); 
			}

			if (mn_gl_list != 0)
			{
				GL.glCallList(mn_gl_list);
			}
			else
			{
				// render the object
				if (m_current_object != IntPtr.Zero)
					NIFAPI.Render(m_current_object);
			}

			// display it
			GL.wglSwapBuffers(m_hdc);
		}

		protected override void OnPaintBackground(PaintEventArgs pevent)
		{
			// nothing todo 'cause we will draw the whole thing
		}

		protected override void OnSizeChanged(EventArgs e)
		{
			// let the NIFAPI reset the aspect ratio
			NIFAPI.OnWindowResize();
		}


		public void load(string path, string file)
		{
			// release old object
			if (m_current_object != IntPtr.Zero)	
					NIFAPI.ReleaseObject(m_current_object);
			if (mn_gl_list != 0)
			{
				GL.glDeleteLists(mn_gl_list,1);
				mn_gl_list = 0;
			}

			// load new object
			m_current_object = NIFAPI.CreateObject(path,file);
			if (m_current_object == IntPtr.Zero)
			{
				MessageBox.Show(this,"Error loading object");
			}

			// reset the camera
			camera.targetObject(m_current_object, CCamera.e_viewPosition.Front);
	
			// make sure timer is running
			timer1.Start();
		}



		public void loadDAOC(int type, int id)
		{
			// release old object
			if (m_current_object != IntPtr.Zero)	
				NIFAPI.ReleaseObject(m_current_object);
			if (mn_gl_list != 0)
			{
				GL.glDeleteLists(mn_gl_list,1);
				mn_gl_list = 0;
			}

			// load new object
			switch (type)
			{
				case 0:	m_current_object = DAOCAPI.LoadMonster(id); break;
				case 1:	m_current_object = DAOCAPI.LoadItem(id); break;
				case 2:	m_current_object = DAOCAPI.LoadZone(id);
						mb_rotation = false; // don't want rotating zone :)
						break;
			};

			if (m_current_object == IntPtr.Zero)
			{
				MessageBox.Show(this,"Error loading object");
			}


			// reset the camera
			camera.targetObject(m_current_object, CCamera.e_viewPosition.Right);
	
			// make sure timer is running
			timer1.Start();
		}




		public void ViewObject(CCamera.e_viewPosition side)
		{
			// set camera
			camera.targetObject(m_current_object, side);
		}




		private void timer1_Tick(object sender, System.EventArgs e)
		{
			if (mb_delta_camera)
			{
				// Left+Right pressed -> move on Z-Axis
				if ((me_last_mouseevent.Button & MouseButtons.Left)!=0 && (me_last_mouseevent.Button & MouseButtons.Right)!=0)
				{
					camera.moveLocal(0,0,(last_mouse_position.Y - me_last_mouseevent.Y));
				}
				else
				// Left pressed -> move on X/Y-Axis
				if ((me_last_mouseevent.Button & MouseButtons.Left)!=0)
				{
					camera.moveLocal(-(last_mouse_position.X - me_last_mouseevent.X),
						(last_mouse_position.Y -me_last_mouseevent.Y),0);
				}
				else
				// Right pressed -> rotated on X/Y-Axis
				if ((me_last_mouseevent.Button & MouseButtons.Right)!=0)
				{
					camera.rotateLocal((last_mouse_position.X - me_last_mouseevent.X)/15.0f,0,-1,0);
					camera.rotateLocal((last_mouse_position.Y - me_last_mouseevent.Y)/15.0f,-1,0,0);
				}

				// Middle button -> rotate on Z
				if ((me_last_mouseevent.Button & MouseButtons.Middle)!=0)
				{
					camera.rotateLocal((last_mouse_position.X - me_last_mouseevent.X)/15.0f,0,0,-1);
				}

			}
	
			// recall: OnPaint()
			Invalidate(false);
		}


		/// <summary>
		/// Camera movement (not good but okay for testing)
		/// </summary>
		private void View_MouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			if (mb_delta_camera)
			{
				me_last_mouseevent = e;
				return;
			}

			// Left+Right pressed -> move on Z-Axis
			if ((e.Button & MouseButtons.Left)!=0 && (e.Button & MouseButtons.Right)!=0)
			{
				camera.moveLocal(0,0,(last_mouse_position.Y - e.Y));
			}
			else
			// Left pressed -> move on X/Y-Axis
			if ((e.Button & MouseButtons.Left)!=0)
			{
				camera.moveLocal(-(last_mouse_position.X - e.X),
					(last_mouse_position.Y -e.Y),0);
			}
			else
			// Right pressed -> rotated on X/Y-Axis
			if ((e.Button & MouseButtons.Right)!=0)
			{
				camera.rotateLocal((last_mouse_position.X - e.X)/10.0f,0,-1,0);
				camera.rotateLocal((last_mouse_position.Y - e.Y)/10.0f,-1,0,0);
			}

			// Middle button -> rotate on Z
			if ((e.Button & MouseButtons.Middle)!=0)
			{
				camera.rotateLocal((last_mouse_position.X - e.X)/8.0f,0,0,-1);
			}

	
			// store last mouse position
			last_mouse_position.X = e.X;
			last_mouse_position.Y = e.Y;
		}

		private void View_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			if (mb_delta_camera)
			{
				// store last mouse position
				last_mouse_position.X = e.X;
				last_mouse_position.Y = e.Y;
				me_last_mouseevent = e;
			}
		}

		private void View_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			if (mb_delta_camera)
			{
				// just to prevent additinal movement
				last_mouse_position.X = e.X;
				last_mouse_position.Y = e.Y;
			}
		}



		private void View_KeyPress(object sender, System.Windows.Forms.KeyPressEventArgs e)
		{
			switch (e.KeyChar)
			{
				case 'l':	ViewObject(OpenGL.CCamera.e_viewPosition.Left);	 break;
				case 'r':	ViewObject(OpenGL.CCamera.e_viewPosition.Right); break;
				case 'f':	ViewObject(OpenGL.CCamera.e_viewPosition.Front); break;
				case 'b':	ViewObject(OpenGL.CCamera.e_viewPosition.Below); break;
				case 't':	ViewObject(OpenGL.CCamera.e_viewPosition.Top);	 break;
			}

		}
	
	}

}
