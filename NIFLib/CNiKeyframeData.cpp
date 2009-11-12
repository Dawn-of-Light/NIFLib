#include "StdAfx.h"
#include ".\cnikeyframedata.h"

CNiKeyframeData::CNiKeyframeData(void)
{
}

CNiKeyframeData::~CNiKeyframeData(void)
{
	for (int t=0;t<m_tracks.Count();t++)
	{
		for (int k=0;k<m_tracks[t]->keys.Count();k++)
			delete (m_tracks[t]->keys[k]);

		delete m_tracks[t];
	}
}


CNiKeyframeData::s_track*	CNiKeyframeData::LoadTrack(CNifStream& stream,bool b_trans)
{
	s_key_data*	ps_key;
	s_track*	ps_track;


	unsigned keys = stream.readUInt();

	if (keys == NULL) return NULL;
	ps_track = new s_track;

	ps_track->type = stream.readUInt(); 
	assert(ps_track->type == 1 || ps_track->type == 2);
	if (b_trans)	ps_track->type = 1;
	//	1 = linear	2 = bezier


	ps_track->max_time = 0.0f;
	for (unsigned k=0;k<keys;k++)
	{
			ps_key = new s_key_data;
			ps_key->time	= stream.readFloat();
			ps_key->x	= stream.readFloat();

			if (ps_track->type ==2 || b_trans)
			{
				ps_key->y	= stream.readFloat();
				ps_key->z	= stream.readFloat();
			}
			ps_track->keys.AddLast(ps_key);
			ps_track->max_time = max(ps_track->max_time,ps_key->time);
	}

	return ps_track;
}


bool	CNiKeyframeData::Load(CNifStream& stream)
{
		mn_rot_tracks = -1;
		mn_trans_track = -1;
		mn_scale_track = -1;



		unsigned frames;

		// Rotation
		frames = stream.readUInt();
		if (frames > 0)
		{

			s_key_data* ps_key;

			// Create ROT-Tracks
			mn_rot_tracks = m_tracks.Count();
			s_track*	ps_track_x =  new s_track;
			s_track*	ps_track_y =  new s_track;
			s_track*	ps_track_z =  new s_track;
			m_tracks.AddLast(ps_track_x);
			m_tracks.AddLast(ps_track_y);
			m_tracks.AddLast(ps_track_z);



			unsigned controllertype = stream.readUInt(); 
			assert(controllertype < 5 && controllertype != 0);


			if (controllertype < 4) 
			{
				ps_track_x->type = controllertype;
				ps_track_y->type = controllertype;
				ps_track_z->type = controllertype;

				for (unsigned fr = 0; fr<frames;fr++)
				{
					float time = stream.readFloat(); // time

					float a1 = stream.readFloat(); // rot val in RAD
					float a2 = stream.readFloat(); // rot val in RAD
					float a3 = stream.readFloat(); // rot val in RAD
					float a4 = stream.readFloat(); // rot val in RAD
					if (controllertype == 3) 
					{
						stream.readFloat(); 
						stream.readFloat(); 
						stream.readFloat(); 
					}
					
					ps_key = new s_key_data;
					ps_key->time= time;
					ps_key->x	= 0.0f;//frx;
					ps_key->y	= 0.0f;
					ps_key->z	= 0.0f;
					ps_track_x->keys.AddLast(ps_key);

					ps_key = new s_key_data;
					ps_key->time= time;
					ps_key->x	= 0.0f;//fry;
					ps_key->y	= 0.0f;
					ps_key->z	= 0.0f;
					ps_track_y->keys.AddLast(ps_key);

					ps_key = new s_key_data;
					ps_key->time= time;
					ps_key->x	= a1+a2+a3+a4;
					ps_key->y	= 0.0f;
					ps_key->z	= 0.0f;
					ps_track_z->keys.AddLast(ps_key);
				}

			}


			if (controllertype == 4) 
			{
				unsigned a1 = stream.readUInt(); 

				for (unsigned t=0;t<3;t++)
				{
					unsigned keys = stream.readUInt(); 
					unsigned trackcontrollertype = stream.readUInt(); 
					assert(trackcontrollertype == 1 || trackcontrollertype == 2);
					//	1 = linear	2 = bezier

					switch (t)
					{
						case 0:	ps_track_x->type = trackcontrollertype; break;
						case 1:	ps_track_y->type = trackcontrollertype; break;
						case 2:	ps_track_z->type = trackcontrollertype; break;
					}

					for (unsigned k=0;k<keys;k++)
					{
						ps_key = new s_key_data;
						ps_key->time= stream.readFloat(); // time
						ps_key->x	= stream.readFloat(); // rot val in RAD
						ps_key->y	= 0.0f;
						ps_key->z	= 0.0f;
						

						if (trackcontrollertype == 2)
						{
							ps_key->y = stream.readFloat(); // in
							ps_key->z = stream.readFloat(); // out
						}

						switch (t)
						{
							case 0:	ps_track_x->keys.AddLast(ps_key); break;
							case 1:	ps_track_y->keys.AddLast(ps_key); break;
							case 2:	ps_track_z->keys.AddLast(ps_key); break;
						}
					}
				}
			}
		}

		// Translation
		frames = stream.readUInt();
		if (frames>0)
			{
				s_key_data* ps_key;

				// Create POS-Tracks
				mn_trans_track = m_tracks.Count();
				s_track*	ps_track_pos =  new s_track;
				m_tracks.AddLast(ps_track_pos);



				unsigned controllertype =stream.readUInt();
				assert( controllertype == 1 || controllertype == 2 || controllertype == 3);
				for (unsigned t = 0;t<frames;t++)
				{
					float f1 = stream.readFloat(); // time
					float f2 = stream.readFloat(); // x
					float f3 = stream.readFloat(); // y
					float f4 = stream.readFloat(); // z
					if (controllertype == 3)
					{
						float f5 = stream.readFloat(); // ?
						float f6 = stream.readFloat(); // ?
						float f7 = stream.readFloat(); // ?
					}
					if (controllertype == 2)
					{
						float f5 = stream.readFloat(); // inout
						float f6 = stream.readFloat(); // inout
						float f7 = stream.readFloat(); // inout
						float f8 = stream.readFloat(); // inout
						float f9 = stream.readFloat(); // inout
						float f10 = stream.readFloat(); // inout
					}

					ps_key = new s_key_data;
					ps_key->time = f1;
					ps_key->x = f2;
					ps_key->y = f3;
					ps_key->z = f4;
					ps_track_pos->keys.AddLast(ps_key);

				}
			}


		// Scale
		frames = stream.readUInt();
		if (frames>0)
		{
//			TRACE("Scale Keys\n");

			unsigned controllertype = stream.readUInt(); 
			assert( controllertype == 1 || controllertype == 2 || controllertype == 3);

			for (unsigned k=0;k<frames;k++)
			{
				float f1 = stream.readFloat(); // time
				float f2 = stream.readFloat(); // x
				float f3 = 0;
				float f4 = 0;

				if (controllertype == 2)
				{
					f3 = stream.readFloat(); // ?
					f4 = stream.readFloat(); // ?
				}
				if (controllertype == 3)
				{
					f3 = stream.readFloat(); // ?
					f4 = stream.readFloat(); // ?
					float f5 = stream.readFloat(); // ?
				}
			}
		}

		/*
		// Rotation
		data_availible = stream.readUInt(); // ?????
		if (data_availible)
		{
			mn_rot_tracks = 0;


			unsigned nof_tracks = stream.readUInt()-1; 
			unsigned a1 = stream.readUInt(); 


			for (unsigned t=0;t<nof_tracks;t++)
			{
				ps_track = LoadTrack(stream,false);
				m_tracks.AddLast(ps_track);
			}
		}



		// Translation
		ps_track = LoadTrack(stream,true);
		if (ps_track)
		{
			mn_trans_track = m_tracks.Count();
			m_tracks.AddLast(ps_track);
		}


		// Scale
		ps_track = LoadTrack(stream,false);
		if (ps_track)
		{
			mn_scale_track = m_tracks.Count();
			m_tracks.AddLast(ps_track);
		}

*/

		return true;
}



#ifdef ANALYSE_NIF
void	CNiKeyframeData::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "KeyframeData",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

}
#endif