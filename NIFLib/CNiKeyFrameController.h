#pragma once
#include "cnode.h"

class CNiKeyFrameController : public CNode
{
	public:
		CNiKeyFrameController(void);
		~CNiKeyFrameController(void);

		bool	Load(CNifStream&);
		void	CreateHierachy(CNode**, int nof);


		DECLARE_CHUNK(NiKeyFrameController,"NiKeyFrameController");

	private:

		CChilds		mc_path;
		CChilds		mc_data;
		float		mf_max_time;

};
