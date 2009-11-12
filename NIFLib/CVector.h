#pragma once

#define EQUAL_TOLERANZ 0.00001f

class CVector
{
	public:
		CVector() {};
		CVector(float _x,float _y,float _z)
		{
			x = _x;
			y = _y;
			z = _z;
		}

		float	GetX()	const {return x;};
		float	GetY()	const {return y;};
		float	GetZ()	const {return z;};
		void	Set(float _x,float _y,float _z)	{ x = _x; y = _y; z = _z;	}
		void	Clear()							{ x = y = z = 0.0;};
		void	SetX(float _x)	{ x = _x;};
		void	SetY(float _y)	{ y = _y;};
		void	SetZ(float _z)	{ z = _z;};


		bool	operator==(const CVector&in) const;
		CVector& operator=(const CVector&in)	{ x=in.x; y=in.y; z=in.z; return *this;};
		CVector operator+(const CVector&in)	const	{ CVector out; out.x=x+in.x; out.y=y+in.y; out.z=z+in.z; return out;};
		CVector operator-(const CVector&in)	const	{ CVector out; out.x=x-in.x; out.y=y-in.y; out.z=z-in.z; return out;};
		CVector operator*(const CVector&in)	const	{ CVector out; out.x=x*in.x; out.y=y*in.y; out.z=z*in.z; return out;};
		CVector operator*(const float&in)	const	{ CVector out; out.x=x*in; out.y=y*in; out.z=z*in; return out;};
		CVector& operator+=(const CVector&in)	{ x+=in.x; y+=in.y; z+=in.z; return *this;};
		CVector& operator-=(const CVector&in)	{ x-=in.x; y-=in.y; z-=in.z; return *this;};
		CVector& operator*=(const CVector&in)	{ x*=in.x; y*=in.y; z*=in.z; return *this;};
		CVector& operator*=(const float&in)		{ x*=in; y*=in; z*=in; return *this;};
		float	Length();
		void	Normalize()						{ float f = Length(); x/=f; y/=f; z/=f;};

		float	DotProduct(const CVector&in)	const { return x*in.x+y*in.y+z*in.z; }
		void	GLOut();
		void	GLOutNRL();

		CVector	CrossProdukt(const CVector&in)	{ CVector out;
													out.x=y*in.z-z*in.y;
													out.y=z*in.x-x*in.z;
													out.z=x*in.y-y*in.x;
												  return out;
												};


	private:
		float x,y,z;
};
