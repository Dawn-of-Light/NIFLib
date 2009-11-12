#include "StdAfx.h"
#include ".\cdebugnode.h"

CDebugNode::CDebugNode(void)
{
}

CDebugNode::~CDebugNode(void)
{
}

void	CDebugNode::AddPoint(CVector v)
{
	points.AddLast(v);
}

void	CDebugNode::AddLine(CVector v1,CVector v2)
{
	lines.AddLast(v1);
	lines.AddLast(v2);
}

void	CDebugNode::AddLine2(CVector v1,CVector v2)
{
	lines2.AddLast(v1);
	lines2.AddLast(v2);
}

void	CDebugNode::Render()
{
	glDisable(GL_TEXTURE);
	glPointSize(8.0f);
	glColor3f(1,0,0);
	glBegin(GL_POINTS);
		for (int t=0;t<points.Count();t++)
			points[t].GLOut();
	glEnd();

	glColor3f(1,0,0);
	glBegin(GL_LINES);
		for (int t=0;t<lines.Count();t++)
			lines[t].GLOut();
	glEnd();

	glColor3f(0,1,0);
	glBegin(GL_LINES);
		for (int t=0;t<lines2.Count();t++)
			lines2[t].GLOut();
	glEnd();

}
