#pragma once
#include "cnode.h"

class CNiKeyframeData : public CNode
{
	private:
		struct s_key_data
		{
			float time;
			float x;//value;
			float y;//in;
			float z;//out;
		};

		struct s_track
		{
			TArray <s_key_data*>	keys;
			float					max_time;
			unsigned				type; // 1= linear, 2=bezier
		};

		TArray <s_track*>			m_tracks;



		int	mn_rot_tracks;
		int	mn_trans_track;
		int	mn_scale_track;

		float		GetValue(int track, float absolute_time);

		s_track*	LoadTrack(CNifStream&,bool b_trans); // trans is always linear but got 4 values


	public:
		CNiKeyframeData(void);
		~CNiKeyframeData(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(NiKeyframeData,"NiKeyframeData");
};
