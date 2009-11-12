#include "StdAfx.h"
#include "cmatrix.h"
#include "math.h"

CMatrix::CMatrix(void)
{
	Identity();
}

CMatrix::~CMatrix(void)
{
}

void	CMatrix::Identity()
{
	memset(m,0,sizeof(float)*16);
	m[0] = 1.0f;
	m[5] = 1.0f;
	m[10] = 1.0f;
	m[15] = 1.0f;
}

void	CMatrix::LoadGLMatrix()
{
	glGetFloatv(GL_MODELVIEW_MATRIX,m);
};


CMatrix& CMatrix::operator =(const CMatrix& b)
{
	memcpy(m,b.m,sizeof(float)*16);
	return *this;
}


CVector		CMatrix::Transform(const CVector& in) const
{
	float x,y,z;

	x = m[0]*in.GetX()+m[4]*in.GetY()+m[8]*in.GetZ()+m[12];
	y = m[1]*in.GetX()+m[5]*in.GetY()+m[9]*in.GetZ()+m[13];
	z = m[2]*in.GetX()+m[6]*in.GetY()+m[10]*in.GetZ()+m[14];
	
	return CVector(x,y,z);
}



CMatrix& CMatrix::operator*=(CMatrix &b)
{
	float a[16];
	memcpy(a,m,sizeof(float)*16);

	m[0] = a[0]*b.m[0] + a[4]*b.m[1] + a[8]*b.m[2] + a[12]*b.m[3];
	m[1] = a[1]*b.m[0] + a[5]*b.m[1] + a[9]*b.m[2] + a[13]*b.m[3];
	m[2] = a[2]*b.m[0] + a[6]*b.m[1] + a[10]*b.m[2] + a[14]*b.m[3];
	m[3] = a[3]*b.m[0] + a[7]*b.m[1] + a[11]*b.m[2] + a[15]*b.m[3];

	m[4] = a[0]*b.m[4] + a[4]*b.m[5] + a[8]*b.m[6] + a[12]*b.m[7];
	m[5] = a[1]*b.m[4] + a[5]*b.m[5] + a[9]*b.m[6] + a[13]*b.m[7];
	m[6] = a[2]*b.m[4] + a[6]*b.m[5] + a[10]*b.m[6] + a[14]*b.m[7];
	m[7] = a[3]*b.m[4] + a[7]*b.m[5] + a[11]*b.m[6] + a[15]*b.m[7];

	m[8] = a[0]*b.m[8] + a[4]*b.m[9] + a[8]*b.m[10] + a[12]*b.m[11];
	m[9] = a[1]*b.m[8] + a[5]*b.m[9] + a[9]*b.m[10] + a[13]*b.m[11];
	m[10] = a[2]*b.m[8] + a[6]*b.m[9] + a[10]*b.m[10] + a[14]*b.m[11];
	m[11] = a[3]*b.m[8] + a[7]*b.m[9] + a[11]*b.m[10] + a[15]*b.m[11];

	m[12] = a[0]*b.m[12] + a[4]*b.m[13] + a[8]*b.m[14] + a[12]*b.m[15];
	m[13] = a[1]*b.m[12] + a[5]*b.m[13] + a[9]*b.m[14] + a[13]*b.m[15];
	m[14] = a[2]*b.m[12] + a[6]*b.m[13] + a[10]*b.m[14] + a[14]*b.m[15];
	m[15] = a[3]*b.m[12] + a[7]*b.m[13] + a[11]*b.m[14] + a[15]*b.m[15];

	return *this;

}


CMatrix CMatrix::operator*(CMatrix &b)
{
	CMatrix	c_result;

	c_result.m[0] = m[0]*b.m[0] + m[4]*b.m[1] + m[8]*b.m[2] + m[12]*b.m[3];
	c_result.m[1] = m[1]*b.m[0] + m[5]*b.m[1] + m[9]*b.m[2] + m[13]*b.m[3];
	c_result.m[2] = m[2]*b.m[0] + m[6]*b.m[1] + m[10]*b.m[2] + m[14]*b.m[3];
	c_result.m[3] = m[3]*b.m[0] + m[7]*b.m[1] + m[11]*b.m[2] + m[15]*b.m[3];

	c_result.m[4] = m[0]*b.m[4] + m[4]*b.m[5] + m[8]*b.m[6] + m[12]*b.m[7];
	c_result.m[5] = m[1]*b.m[4] + m[5]*b.m[5] + m[9]*b.m[6] + m[13]*b.m[7];
	c_result.m[6] = m[2]*b.m[4] + m[6]*b.m[5] + m[10]*b.m[6] + m[14]*b.m[7];
	c_result.m[7] = m[3]*b.m[4] + m[7]*b.m[5] + m[11]*b.m[6] + m[15]*b.m[7];

	c_result.m[8] = m[0]*b.m[8] + m[4]*b.m[9] + m[8]*b.m[10] + m[12]*b.m[11];
	c_result.m[9] = m[1]*b.m[8] + m[5]*b.m[9] + m[9]*b.m[10] + m[13]*b.m[11];
	c_result.m[10] = m[2]*b.m[8] + m[6]*b.m[9] + m[10]*b.m[10] + m[14]*b.m[11];
	c_result.m[11] = m[3]*b.m[8] + m[7]*b.m[9] + m[11]*b.m[10] + m[15]*b.m[11];

	c_result.m[12] = m[0]*b.m[12] + m[4]*b.m[13] + m[8]*b.m[14] + m[12]*b.m[15];
	c_result.m[13] = m[1]*b.m[12] + m[5]*b.m[13] + m[9]*b.m[14] + m[13]*b.m[15];
	c_result.m[14] = m[2]*b.m[12] + m[6]*b.m[13] + m[10]*b.m[14] + m[14]*b.m[15];
	c_result.m[15] = m[3]*b.m[12] + m[7]*b.m[13] + m[11]*b.m[14] + m[15]*b.m[15];

	return c_result;
}



void	CMatrix::Create( float r_x, float r_y, float r_z,
						float t_x, float t_y, float t_z,
						float s_x, float s_y, float s_z	)
{

	float sa = sin(r_x);
	float ca = cos(r_x);
	float sb = sin(r_y);
	float cb = cos(r_y);
	float sc = sin(r_z);
	float cc = cos(r_z);

	float sasb = sa*sb;
	float casb = ca*sb;

	m[0] = (cb*cc) * s_x;
	m[1] = (cb*sc) * s_y;
	m[2] = (-sb)   * s_z;
	m[3] = 0.0f;

	m[4] = (sasb*cc-ca*sc) * s_x;
	m[5] = (sasb*sc+ca*cc) * s_y;
	m[6] = (sa*cb)		   * s_z;
	m[7] = 0.0f;

	m[8] = (casb*cc+sa*sc) * s_x;
	m[9] = (casb*sc-sa*cc) * s_y;
	m[10] = (ca*cb)        * s_z;
	m[11] = 0.0f;

	m[12] = t_x * s_x;
	m[13] = t_y * s_y;
	m[14] = t_z * s_z;
	m[15] = 1.0f;

}



void	CMatrix::AddTranslation(float r_x, float r_y, float r_z)
{

	m[12] += m[0]*r_x + m[4]*r_y + m[8]* r_z;
	m[13] += m[1]*r_x + m[5]*r_y + m[9]* r_z;
	m[14] += m[2]*r_x + m[6]*r_y + m[10]*r_z;
	m[15] += m[3]*r_x + m[7]*r_y + m[11]*r_z;
}


void	CMatrix::AddScale(float r_x, float r_y, float r_z)
{
	m[0] *= r_x;
	m[1] *= r_x;
	m[2] *= r_x;
	m[3] *= r_x;

	m[4] *= r_y;
	m[5] *= r_y;
	m[6] *= r_y;
	m[7] *= r_y;

	m[8] *= r_z;
	m[9] *= r_z;
	m[10] *= r_z;
	m[11] *= r_z;
}

void	CMatrix::AddTranslationBefore(float r_x, float r_y, float r_z)
{
	m[0] += r_x*m[3];
	m[1] += r_y*m[3];
	m[2] += r_z*m[3];

	m[4] += r_x*m[7];
	m[5] += r_y*m[7];
	m[6] += r_z*m[7];
	
	m[8] += r_x*m[11];
	m[9] += r_y*m[11];
	m[10] += r_z*m[11];

	m[12] += r_x*m[15];
	m[13] += r_y*m[15];
	m[14] += r_z*m[15];
}

void	CMatrix::AddScaleBefore(float r_x, float r_y, float r_z)
{

	m[0] *= r_x;
	m[1] *= r_y;
	m[2] *= r_z;

	m[4] *= r_x;
	m[5] *= r_y;
	m[6] *= r_z;

	m[8] *= r_x;
	m[9] *= r_y;
	m[10]*= r_z;

	m[12]*= r_x;
	m[13]*= r_y;
	m[14]*= r_z;

}

void	CMatrix::AddRotation(float r_x, float r_y, float r_z)
{

	float sa = sin(r_x);
	float ca = cos(r_x);
	float sb = sin(r_y);
	float cb = cos(r_y);
	float sc = sin(r_z);
	float cc = cos(r_z);

	float b0 = cb*cc;
	float b1 = cb*sc;
	float b4 = sa*sb*cc-ca*sc;
	float b5 = sa*sb*sc+ca*cc;
	float b6 = sa*cb;
	float b8 = ca*sb*cc+sa*sc;
	float b9 = ca*sb*sc-sa*cc;
	float b10 = ca*cb;


	float a[16];
	memcpy(a,m,sizeof(float)*16);

	m[0] = a[0]*b0 + a[4]*b1 - a[8]*sb;
	m[1] = a[1]*b0 + a[5]*b1 - a[9]*sb;
	m[2] = a[2]*b0 + a[6]*b1 - a[10]*sb;
	m[3] = a[3]*b0 + a[7]*b1 - a[11]*sb;

	m[4] = a[0]*b4 + a[4]*b5 + a[8]*b6;
	m[5] = a[1]*b4 + a[5]*b5 + a[9]*b6;
	m[6] = a[2]*b4 + a[6]*b5 + a[10]*b6;
	m[7] = a[3]*b4 + a[7]*b5 + a[11]*b6;

	m[8] = a[0]*b8 + a[4]*b9 + a[8]*b10;
	m[9] = a[1]*b8 + a[5]*b9 + a[9]*b10;
	m[10] = a[2]*b8 + a[6]*b9 + a[10]*b10;
	m[11] = a[3]*b8 + a[7]*b9 + a[11]*b10;

}




void	CMatrix::AddRotationBefore(float r_x, float r_y, float r_z)
{
	

	float a2[16];

	float sa = sin(r_x);
	float ca = cos(r_x);
	float sb = sin(r_y);
	float cb = cos(r_y);
	float sc = sin(r_z);
	float cc = cos(r_z);

	a2[0] = cb*cc;
	a2[1] = cb*sc;
	a2[2] = -sb;

	a2[4] = sa*sb*cc-ca*sc;
	a2[5] = sa*sb*sc+ca*cc;
	a2[6] = sa*cb;

	a2[8] = ca*sb*cc+sa*sc;
	a2[9] = ca*sb*sc-sa*cc;
	a2[10] = ca*cb;



	float a1[16];
	memcpy(a1,m,sizeof(float)*16);



	m[0] = a2[0]*a1[0] + a2[4]*a1[1] + a2[8]*a1[2];
	m[1] = a2[1]*a1[0] + a2[5]*a1[1] + a2[9]*a1[2];
	m[2] = a2[2]*a1[0] + a2[6]*a1[1] + a2[10]*a1[2];

	m[4] = a2[0]*a1[4] + a2[4]*a1[5] + a2[8]*a1[6];
	m[5] = a2[1]*a1[4] + a2[5]*a1[5] + a2[9]*a1[6];
	m[6] = a2[2]*a1[4] + a2[6]*a1[5] + a2[10]*a1[6];

	m[8] = a2[0]*a1[8] + a2[4]*a1[9] + a2[8]*a1[10];
	m[9] = a2[1]*a1[8] + a2[5]*a1[9] + a2[9]*a1[10];
	m[10] = a2[2]*a1[8] + a2[6]*a1[9] + a2[10]*a1[10];

	m[12] = a2[0]*a1[12] + a2[4]*a1[13] + a2[8]*a1[14];
	m[13] = a2[1]*a1[12] + a2[5]*a1[13] + a2[9]*a1[14];
	m[14] = a2[2]*a1[12] + a2[6]*a1[13] + a2[10]*a1[14];
}



void	CMatrix::SetTranslation(float r_x, float r_y, float r_z)
{
	Identity();
	m[12] = r_x;
	m[13] = r_y;
	m[14] = r_z;
}

void	CMatrix::SetScale(float r_x, float r_y, float r_z)
{
	memset(m,0,sizeof(float)*16);
	m[0] = r_x;
	m[5] = r_y;
	m[10] = r_z;
	m[15] = 1.0f;
}


void	CMatrix::SetRotation(float r_x, float r_y, float r_z)
{
	float sa = sin(r_x);
	float ca = cos(r_x);
	float sb = sin(r_y);
	float cb = cos(r_y);
	float sc = sin(r_z);
	float cc = cos(r_z);

	m[0] = cb*cc;
	m[1] = cb*sc;
	m[2] = -sb;
	m[3] = 0.0f;

	m[4] = sa*sb*cc-ca*sc;
	m[5] = sa*sb*sc+ca*cc;
	m[6] = sa*cb;
	m[7] = 0.0f;

	m[8] = ca*sb*cc+sa*sc;
	m[9] = ca*sb*sc-sa*cc;
	m[10] = ca*cb;
	m[11] = 0.0f;

	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = 0.0f;
	m[15] = 1.0f;
}







static void lubksb(float* m, int *indx, float *b)
{
	int		i, j, ii=-1, ip;
	float	sum;

	for (i=0; i<4; i++)
	{
		ip = indx[i];
		sum = b[ip];
		b[ip] = b[i];
		if (ii>=0)
		{
			for (j=ii; j<=i-1; j++)
			{
				sum -= m[i*4+j] * b[j];
			}
		} else if (sum != 0.0)
		{
			ii = i;
		}
		b[i] = sum;
	}
	for (i=3; i>=0; i--)
	{
		sum = b[i];
		for (j=i+1; j<4; j++)
		{
			sum -= m[i*4+j] * b[j];
		}
		b[i] = sum/ m[i*4+i];
	}
} // end lubksb


static void ludcmp(float* m, int *indx)
{
	float	vv[4];               /* implicit scale for each row */
	float	big, dum, sum, tmp;
	int		i, imax = 0, j, k;

	for (i=0; i<4; i++)
	{
		big = 0.0f;
		for (j=0; j<4; j++)
		{
			if ((tmp = (float) fabs(m[i*4+j])) > big)
			{
				big = tmp;
			}
		}
		vv[i] = 1.0f/big;
	}

	for (j=0; j<4; j++)
	{
		for (i=0; i<j; i++)
		{
			sum = m[i*4+j];
			for (k=0; k<i; k++)
			{
				sum -= m[i*4+k] * m[k*4+j];
			}
			m[i*4+j] = sum;
		}
		big = 0.0f;
		for (i=j; i<4; i++)
		{
			sum = m[i*4+j];
			for (k=0; k<j; k++)
			{
				sum -= m[i*4+k]*m[k*4+j];
			}
			m[i*4+j] = sum;
			if ((dum = vv[i] * (float)fabs(sum)) >= big)
			{
				big = dum;
				imax = i;
			}
		}
		if (j != imax)
		{
			for (k=0; k<4; k++)
			{
				dum = m[imax*4+k];
				m[imax*4+k] = m[j*4+k];
				m[j*4+k] = dum;
			}
			vv[imax] = vv[j];
		}
		indx[j] = imax;
		if (j != 3)
		{
			if (m[j*4+j] == 0.0f)
				dum = 0.0f;
			else
				dum = 1.0f/m[j*4+j];

			for (i=j+1; i<4; i++)
				m[i*4+j] *= dum;
		}
	}
} 



// http://www.mathpreprints.com/math/Preprint/M-K/20010720/1/Mak8.pdf
void	CMatrix::Invers()
{
	CMatrix old = *this;

	int			indx[4];
	float		col[4];

	CMatrix mat = *this;

	ludcmp(&mat[0], indx);

	for (int j=0; j<4; j++)
	{
		for (int i=0; i<4; i++)
		{
			col[i] = 0.0f;
		}

		col[j] = 1.0f;
		lubksb(&mat[0], indx, col);
		for (i=0; i<4; i++)
			m[j*4+i] = col[i];
	}


	CMatrix check = old*(*this);
	CMatrix unique;
	for (int t=0;t<16;t++)
		ASSERT(check[t] == unique[t]);
}







// Analyse Output
#ifdef ANALYSE_NIF
void	CMatrix::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	char temp[1024];

	void* parent = addfkt(par,NULL,"Matrix",TIC_TRANSPOSE | TICS_FINISHED);

	sprintf(temp,"r1 %8g %8g %8g %8g",m[0],m[4],m[8],m[12]); 
	addfkt(parent,NULL,temp,TIC_DATA | TICS_FINISHED);
	sprintf(temp,"r2 %8g %8g %8g %8g",m[1],m[5],m[9],m[13]); 
	addfkt(parent,NULL,temp,TIC_DATA | TICS_FINISHED);
	sprintf(temp,"r3 %8g %8g %8g %8g",m[2],m[6],m[10],m[14]); 
	addfkt(parent,NULL,temp,TIC_DATA | TICS_FINISHED);
	sprintf(temp,"r4 %8g %8g %8g %8g",m[3],m[7],m[11],m[15]);
	addfkt(parent,NULL,temp,TIC_DATA | TICS_FINISHED);
}
#endif


