using System;
using System.Runtime.InteropServices;
using System.Text;


namespace NIF
{
	/// <summary>
	/// NIFAPI
	/// Main functions for reading and rendering NIF/NPK files
	/// </summary>
	/// <author>Maik Jurkait</author>
	public class NIFAPI
	{

	// --------------BASIC FUNCTIONS -------------------

		/// <summary>Initialize OpenGL and this Library</summary>
		/// <param name="wnd">Window handle which will be used for drawing</param>
		/// <returns>false on error</returns>
		/// <author>Maik Jurkait</author>
		[DllImport("NIFLib.dll",EntryPoint="?Create@NIF@@YA_NPAUHWND__@@@Z")]
		public static extern bool	Create(IntPtr wnd);


		/// <summary>Releases OpenGL and all memory.
		/// Make sure to release all object handles before.</summary>
		/// <author>Maik Jurkait</author>
		[DllImport("NIFLib.dll",EntryPoint="?Release@NIF@@YAXXZ")]
		public static extern void	Release();



		/// <summary>Call this if the associated window was resized.</summary>
		/// <author>Maik Jurkait</author>
		[DllImport("NIFLib.dll",EntryPoint="?OnWindowResize@NIF@@YAXXZ")]
		public static extern void	OnWindowResize();


	// ----------------------- NIF FUNCTIONS -------------------------

		/// <summary>Loads a NIF/NPK file.</summary>
		/// <param name="path">Directory or MPK-Filename</param>
		/// <param name="filename">filename</param>
		/// <returns>A memory pointer to the loaded object. Zero on error.
		/// They object is under your control. You have to release it to free memory.
		/// </returns>
		/// <remarks>currently(!) if you want load a NPK you have to provide the NPK filename
		/// as directory and the NIF-File name as filename (it's the same name but with .nif extention.</remarks>
		/// <example>
		/// IntPtr obj = CreateObject("c:/cam/nifs/", "bag.nif")
		/// if (obj == IntPtr.Zero) {Errorout();}
		/// </example>
		/// <author>Maik Jurkait</author>
		[DllImport("NIFLib.dll",EntryPoint="?CreateObject@NIF@@YAPAXPBD0@Z")]
		public static extern IntPtr	CreateObject(string path, string filename);


		/// <summary>Releases the object and frees the memory</summary>
		/// <param name="obj">A object handle which was recieved from "CreateObject"</param>
		/// <remarks>After that call the handle is ilegal and will produce error if done something more with it.</remarks>
		/// <example>
		///	ReleaseObject(obj);
		/// obj = new IntPtr(0);
		/// </example>
		/// <author>Maik Jurkait</author>
		[DllImport("NIFLib.dll",EntryPoint="?ReleaseObject@NIF@@YAXPAX@Z")]
		public static extern  void	ReleaseObject(IntPtr obj);


		/// <summary>Renders a object.</summary>
		/// <param name="obj">A object handle, recieved by "CreateObject"</param>
		/// <remarks>You have to call glClear and wglSwapbuffer. This function really just renders the object
		/// regardless of the state of OpenGL.
		/// </remarks>
		/// <example>
		/// GL.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
		/// //---- this can be skip if using a CCamera
		///	GL.glMatrixMode(GL.GL_MODELVIEW);								// Reset the matrix
		///	GL.glLoadIdentity();
		///	
		///	GL.glTranslatef(0.0f,0.0f,-200.0f );	// Move object
		///	GL.glRotatef(260.0f, 1.0f, 0.0f,0.0f ); // rotate object
		/// //---- if using a CCamera, just call: camera.setView();
		///
		///	NIFAPI.Render(m_current_object);	// RENDER
		///	
		/// GL.wglSwapBuffers(m_hdc);		// display it
		/// </example>
		/// <author>Maik Jurkait</author>
		[DllImport("NIFLib.dll",EntryPoint="?Render@NIF@@YAXPAX@Z")]
		public static extern  void	Render(IntPtr obj);



		/// <summary>Calculates the size and position of an object</summary>
		/// <param name="obj">a object handle</param>
		/// <returns>false if no object is loaded or has no visible structures</returns>
		/// <remarks>You get 2 points (x1,y1,z1 and x2,y2,z2). These points defines a box which surround the object.</remarks>
		/// <author>Maik Jurkait</author>
		[DllImport("NIFLib.dll",EntryPoint="?GetBoundingBox@NIF@@YA_NPAXAAM11111@Z")]
		public static extern  bool	GetBoundingBox(IntPtr obj, ref float x1, ref float y1, ref float z1, ref float x2, ref float y2, ref float z2 );
	}

	public class DAOCAPI
	{
		/// <summary>Set the base dir of DAoC</summary>
		/// <param name="path">Directory where camelot.exe is</param>
		/// <remarks>It's needed to find textures and definition files (zones/items/..).
		/// You should call it once before any object is loaded.</remarks>
		/// <example>SetRootDirectory("c:/mythic/camelot/")</example>
		/// <author>Maik Jurkait</author>
		[DllImport("NIFLib.dll",EntryPoint="?SetRootDirectory@NIF@@YAXPBD@Z")]
		public static extern  void	SetRootDirectory(String path);


		// ------------ GETTING ID'S ----------------

		/// <summary></summary>
		/// <param name="buffer">buffer which will recieve all ID's.
		///		if NULL the function just return how much IDs are possible</param>
		/// <returns>number of filled IDs.
		///		if buffer hasn't enough space more could be availabe
		///		if pc_buffer was NULL this function returns how many IDs are possible</returns>
		/// <example>C/C++
		///	int nof = DAoC_GetMonsterIDs(NULL,0);
		///	int* buffer = new int[nof];
		///	nof = DAoC_GetMonsterIDs(buffer,nof)</example>
		/// <author>Maik Jurkait</author>
		[DllImport("NIFLib.dll",EntryPoint="?DAoC_GetItemIDs@NIF@@YAHPAHH@Z")]
		public static extern  Int32	GetItemIDs(Int32 [] buffer, Int32 size);

		/// <summary>see GetItemIDs</summary>
		/// <author>Maik Jurkait</author>
		[DllImport("NIFLib.dll",EntryPoint="?DAoC_GetMonsterIDs@NIF@@YAHPAHH@Z")]
		public static extern  Int32	GetMonsterIDs(Int32 [] buffer, Int32 size);

		/// <summary>see GetItemIDs  todo </summary>
		/// <author>Maik Jurkait</author>
		[DllImport("NIFLib.dll",EntryPoint="?DAoC_GetZoneIDs@NIF@@YAHPAHH@Z")]
		public static extern  Int32	GetZoneIDs(Int32 [] buffer, Int32 size);


		// ------------ GETTING NAMES + INFOS ----------------

		/// <summary>Get the name of a monster by ID</summary>
		/// <author>Maik Jurkait</author>
		[DllImport("NIFLib.dll",EntryPoint="?DAoC_GetMonsterName@NIF@@YA_NHPADH@Z")]
		public static extern  bool		GetMonsterName(Int32 ID, StringBuilder buffer, Int32 size);

		/// <summary>see GetMonsterName</summary>
		/// <author>Maik Jurkait</author>
		[DllImport("NIFLib.dll",EntryPoint="?DAoC_GetItemName@NIF@@YA_NHPADH@Z")]
		public static extern  bool		GetItemName(Int32 ID, StringBuilder buffer, Int32 size);

		/// <summary>see GetMonsterName</summary>
		/// <remarks>a '(Clone)' inside the returned string indicates that this zone
		/// uses the data of another zone.
		/// (atm these are ToA Zones which are all the same in all 3 realms)
		///  </remarks>
		/// <author>Maik Jurkait</author>
		[DllImport("NIFLib.dll",EntryPoint="?DAoC_GetZoneName@NIF@@YA_NHPADH@Z")]
		public static extern  bool		GetZoneName(Int32 ID, StringBuilder buffer, Int32 size);



		/// <summary>atm no extra info</summary>
		/// <author>Maik Jurkait</author>
		[DllImport("NIFLib.dll",EntryPoint="?DAoC_GetMonsterExtraInfo@NIF@@YA_NHPADH@Z")]
		public static extern  bool		GetMonsterInfo(Int32 ID, StringBuilder buffer, Int32 size);

		/// <summary>Detail info of Item</summary>
		/// <remarks>empty or 'Housing' Item.</remarks>
		/// <author>Maik Jurkait</author>
		[DllImport("NIFLib.dll",EntryPoint="?DAoC_GetItemExtraInfo@NIF@@YA_NHPADH@Z")]
		public static extern  bool		GetItemInfo(Int32 ID, StringBuilder buffer, Int32 size);

		/// <summary>Detail info of zones</summary>
		/// <remarks>zone type.</remarks>
		/// <author>Maik Jurkait</author>
		[DllImport("NIFLib.dll",EntryPoint="?DAoC_GetZoneExtraInfo@NIF@@YA_NHPADH@Z")]
		public static extern  bool		GetZoneInfo(Int32 ID, StringBuilder buffer, Int32 size);








		// --------------- LOADING ------------------------

		/// <summary>Loads an item by ID</summary>
		/// <remarks>
		/// Loads the model of a item-ID.
		/// Nearly the same as loading a Nif manual.
		/// </remarks>
		/// <author>Maik Jurkait</author>
		[DllImport("NIFLib.dll",EntryPoint="?DAoC_LoadItem@NIF@@YAPAXH@Z")]
		public static extern  IntPtr	LoadItem(Int32 ID);


		/// <summary>Loads a Monster by ID</summary>
		/// <remarks>
		/// Loading a Monsters is little bit more complicated.
		/// Because they can have different skins, heads, hairsets, styles...
		/// (atm only skins are supported)
		/// </remarks>
		/// <author>Maik Jurkait</author>
		[DllImport("NIFLib.dll",EntryPoint="?DAoC_LoadMonster@NIF@@YAPAXH@Z")]
		public static extern  IntPtr	LoadMonster(Int32 ID);


		/// <summary>Loads a Zone by ID</summary>
		/// <remarks>
		/// It can take a while to load a complet zone and rendering can be slow.
		/// There are different types of zones: city, terrain, dungeon, housing, skycity
		/// Each type has a unique loading routine and is build different.
		/// This will all be handled by this function but it's good to know what happens behind
		/// the scenes :)
		/// </remarks>
		/// <author>Maik Jurkait</author>
		[DllImport("NIFLib.dll",EntryPoint="?DAoC_LoadZone@NIF@@YAPAXH@Z")]
		public static extern  IntPtr	LoadZone(Int32 ID);

	}
}








/// some basic OpenGl functions
/// for full opengl support take (or take a look at)
///     http://www.colinfahey.com/opengl/csharp.htm
///     by cpfahey@earthlink.net      (Colin P. Fahey)
namespace OpenGL
{
	/// <summary>
	/// just a simple OpenGL function collection
	/// for further information see any OpenGL paper or SDK
	/// </summary>
	public class GL
	{
		public const string GL_DLL = "opengl32";

		public const uint  GL_COMPILE                           =       0x1300;
		public const uint  GL_COMPILE_AND_EXECUTE               =       0x1301;

		public const uint  GL_DEPTH_BUFFER_BIT                  =   0x00000100;
		public const uint  GL_COLOR_BUFFER_BIT                  =   0x00004000;

		public const uint  GL_MODELVIEW                         =       0x1700;
		public const uint  GL_PROJECTION                        =       0x1701;
		public const uint  GL_TEXTURE                           =       0x1702;

		public const uint  GL_MODELVIEW_MATRIX                  =       0x0ba6;


		[DllImport(GL_DLL)]	public static extern uint wglSwapBuffers		( IntPtr hdc );

		[DllImport(GL_DLL)] public static extern void glCallList            ( uint list ); 
		[DllImport(GL_DLL)] public static extern void glClear				( uint mask ); 
		[DllImport(GL_DLL)] public static extern void glDeleteLists         ( uint list, int range ); 
		[DllImport(GL_DLL)] public static extern void glEndList             ( ); 
		[DllImport(GL_DLL)] public static extern void glGetFloatv			( uint pname, float[] paramsx ); 
		[DllImport(GL_DLL)] public static extern uint glGenLists            ( int range ); 
		[DllImport(GL_DLL)] public static extern void glLoadIdentity        ( ); 
		[DllImport(GL_DLL)] public static extern void glLoadMatrixf         ( float[] m ); 
		[DllImport(GL_DLL)] public static extern void glMatrixMode          ( uint mode ); 
		[DllImport(GL_DLL)] public static extern void glNewList             ( uint list, uint mode ); 
		[DllImport(GL_DLL)] public static extern void glPopMatrix           ( );
		[DllImport(GL_DLL)] public static extern void glPushMatrix          ( ); 
		[DllImport(GL_DLL)] public static extern void glTranslatef          ( float x, float y, float z ); 
		[DllImport(GL_DLL)] public static extern void glRotatef             ( float angle, float x, float y, float z ); 

		[DllImport("user32")]	public static extern IntPtr GetDC( IntPtr hwnd );
		[DllImport("user32")]	public static extern int	ReleaseDC( IntPtr hwnd, IntPtr hdc );

	}


	/// <summary>
	/// Camera class.
	/// This will make moving the camera easier.
	/// </summary>
	public class CCamera
	{
		private float[] maf_matrix;


		public CCamera()
		{
			// init matrix
			maf_matrix = new float[16];
			Reset();
		}

		/// <summary>
		/// Reset camera to zero position and rotation
		/// </summary>
		public void Reset()
		{
			for (int t=0;t<16;t++) maf_matrix[t]=0.0f;
			maf_matrix[0] = 1.0f;
			maf_matrix[5] = 1.0f;
			maf_matrix[10] = -1.0f;
			maf_matrix[15] = 1.0f;
		}

		/// <summary>
		/// Call this before render something.
		/// </summary>
		public void setView()
		{
			GL.glMatrixMode(GL.GL_MODELVIEW);

			float[] viewmatrix =
						{//Remove the three '-' for non-inverted z-axis
						  maf_matrix[0], maf_matrix[4], -maf_matrix[8], 0,
						  maf_matrix[1], maf_matrix[5], -maf_matrix[9], 0,
						  maf_matrix[2], maf_matrix[6], -maf_matrix[10], 0,

						  -(maf_matrix[0]*maf_matrix[12] +
						  maf_matrix[1]*maf_matrix[13] +
						  maf_matrix[2]*maf_matrix[14]),

						  -(maf_matrix[4]*maf_matrix[12] +
						  maf_matrix[5]*maf_matrix[13] +
						  maf_matrix[6]*maf_matrix[14]),

						  //add a - like above for non-inverted z-axis
						  (maf_matrix[8]*maf_matrix[12] +
						  maf_matrix[9]*maf_matrix[13] +
						  maf_matrix[10]*maf_matrix[14]), 1};
			GL.glLoadMatrixf(viewmatrix);
		}


		/// <summary>
		/// Move camera in local axis space.
		/// </summary>
		public void moveLocal(float x, float y, float z) 
		{
			float dx=x*maf_matrix[0] + y*maf_matrix[4] + z*maf_matrix[8];
			float dy=x*maf_matrix[1] + y*maf_matrix[5] + z*maf_matrix[9];
			float dz=x*maf_matrix[2] + y*maf_matrix[6] + z*maf_matrix[10];
			maf_matrix[12] += dx;
			maf_matrix[13] += dy;
			maf_matrix[14] += dz;
		}

		/// <summary>
		/// Move camera in world space.
		/// </summary>
		public void moveGlobal(float x, float y, float z) 
		{
			   maf_matrix[12] += x;
			   maf_matrix[13] += y;
			   maf_matrix[14] += z;
		}

		/// <summary>
		/// Rotate camera in local(camera) space
		/// </summary>
		public void rotateLocal(float deg, float x, float y, float z) 
		{
			GL.glMatrixMode(GL.GL_MODELVIEW);
			GL.glPushMatrix();
			GL.glLoadMatrixf(maf_matrix);
			GL.glRotatef(deg, x,y,z);
			GL.glGetFloatv(GL.GL_MODELVIEW_MATRIX, maf_matrix);
			GL.glPopMatrix();
		}

		/// <summary>
		/// Rotate camera in world space
		/// </summary>
		public void rotateGlobal(float deg, float x, float y, float z) 
		{
			float dx=x*maf_matrix[0] + y*maf_matrix[1] + z*maf_matrix[2];
			float dy=x*maf_matrix[4] + y*maf_matrix[5] + z*maf_matrix[6];
			float dz=x*maf_matrix[8] + y*maf_matrix[9] + z*maf_matrix[10];
			GL.glMatrixMode(GL.GL_MODELVIEW);
			GL.glPushMatrix();
			GL.glLoadMatrixf(maf_matrix);
			GL.glRotatef(deg, dx,dy,dz);
			GL.glGetFloatv(GL.GL_MODELVIEW_MATRIX, maf_matrix);
			GL.glPopMatrix();
		}



		public enum e_viewPosition 
		{ Left, Right, Top, Below, Front, Back
		};



		/// <summary>Calculates a camera position which will show the complet object.</summary>
		/// <param name="nif_obj">an object handle from NIFAPI</param>
		/// <param name="pos">which side will be viewed</param>
		/// <remarks>This is surely just the mathematical view.
		/// It's still possible that the object is shown from top if you choose "Front".
		/// ! BUG ! distance isn't always correct
		/// </remarks>
		public void	targetObject(IntPtr nif_obj, e_viewPosition pos)
		{
			if (nif_obj == IntPtr.Zero) return;

			float x1, y1, z1, x2, y2, z2;
			x1 = y1 = z1 = x2 = y2 = z2 = 0; // just to keep compiler happy
			bool res = NIF.NIFAPI.GetBoundingBox(nif_obj, ref x1, ref y1, ref z1, ref x2, ref y2, ref z2);
			if (!res) return;

			Reset();
	
			moveGlobal((x1+ x2)/2.0f, (y1+ y2)/2.0f, (z1+ z2)/2.0f);

			float distancemin = 50; // atleast near-clipping plane must be reached
			float distanceX = ( x2 - x1 );
			float distanceY = ( y2 - y1 );
			float distanceZ = ( z2 - z1 );

			
			distanceX = (distanceX/2)/ (float) System.Math.Tan(45/2 * 3.14 / 180);
			distanceY = (distanceY/2)/ (float) System.Math.Tan(45/2 * 3.14 / 180);
			distanceZ = (distanceZ/2)/ (float) System.Math.Tan(45/2 * 3.14 / 180);
	
			distanceX = System.Math.Max(distancemin,distanceX);
			distanceY = System.Math.Max(distancemin,distanceY);
			distanceZ = System.Math.Max(distancemin,distanceZ);

	
			switch (pos)
			{
				case e_viewPosition.Top:
					moveLocal(0,0,-System.Math.Max(distanceX,distanceY));
					break;

				case e_viewPosition.Front:
					rotateLocal(-90.0f,0.0f,0.0f,1.0f);
					rotateLocal(-90.0f,1.0f,0.0f,0.0f);
					moveLocal(0,0,-System.Math.Max(distanceY,distanceZ));
					break;

				case e_viewPosition.Below:
					rotateLocal(180.0f,1.0f,0.0f,0.0f);
					moveLocal(0,0,-System.Math.Max(distanceX,distanceY));
					break;

				case e_viewPosition.Back:
					rotateLocal(90.0f,0.0f,0.0f,1.0f);
					rotateLocal(-90.0f,1.0f,0.0f,0.0f);
					moveLocal(0,0,-System.Math.Max(distanceY,distanceZ));
					break;

				case e_viewPosition.Right:
					rotateLocal(-90.0f,1.0f,0.0f,0.0f);
					moveLocal(0,0,-System.Math.Max(distanceX,distanceY));
					break;

				case e_viewPosition.Left:
					rotateLocal(90.0f,1.0f,0.0f,0.0f);
					rotateLocal(180.0f,0.0f,0.0f,1.0f);
					moveLocal(0,0,-System.Math.Max(distanceX,distanceY));
					break;


			}

		}
	}

}




