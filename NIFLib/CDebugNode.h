#pragma once
#include "cnode.h"

class CDebugNode :	public CNode
{
	public:
		CDebugNode(void);
		~CDebugNode(void);

		bool	Load(CNifStream&) { ASSERT(false);return false;};

		void	Render();

		void	AddPoint(CVector);
		void	AddLine(CVector,CVector);
		void	AddLine2(CVector,CVector);

	private:
		TArray <CVector> points;
		TArray <CVector> lines;
		TArray <CVector> lines2;
};
