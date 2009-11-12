#include "stdafx.h"	
#include "cvector.h"
#include "math.h"


float	CVector::Length()
{
	return (float)sqrt(x*x+y*y+z*z);
}

bool	CVector::operator==(const CVector&in) const
{
	return (fabs(x-in.x)<EQUAL_TOLERANZ &&
			fabs(y-in.y)<EQUAL_TOLERANZ &&
			fabs(z-in.z)<EQUAL_TOLERANZ);
};


void	CVector::GLOut()
{
	glVertex3f(x,y,z);
}

void	CVector::GLOutNRL()
{
	glNormal3f(x,y,z);
}




