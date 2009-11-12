#pragma once
#include "cnode.h"


/**
*	\todo not working on all models
**/
class CNiSkinPartition : public CNode
{
	public:
		CNiSkinPartition(void);
		~CNiSkinPartition(void);

		bool	Load(CNifStream&);

		DECLARE_CHUNK(NiSkinPartition,"NiSkinPartition");


	private:
		struct	s_unknown
		{
			unsigned nof1;
			unsigned nof2;
			unsigned nof3;
			unsigned nof4;
			unsigned nof5;
		};


		TArray	<s_unknown>	mas_unknown;

};
