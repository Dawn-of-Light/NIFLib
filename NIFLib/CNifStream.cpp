#include "StdAfx.h"
#include "CNifStream.h"
#include "assert.h"
#include "CMatrix.h"


CNifStream::CNifStream(char*data, size_t size)
{
	g_mempointer = data;
	g_memsize = (long)size;

	readHead();
}





void	CNifStream::readHead()
{
	// Maik Jurkait  24.05.2004 16:59:40
	// ------------------------------------------
	//	All (known) Version strings
	// ------------------------------------------
	struct s_header
	{
		DWORD dw_version;
		const char* str;
	} allversions[]
	=
	{
		{0x02030000, "NetImmerse File Format, Version 2.3\nNumerical Design Limited, Chapel Hill, NC 27514\nCopyright (c) 1996-1999\nAll Rights Reserved\n"},
		{0x03000000, "NetImmerse File Format, Version 3.0\nNumerical Design Limited, Chapel Hill, NC 27514\nCopyright (c) 1996-2000\nAll Rights Reserved\n"},
		{0x03010000, "NetImmerse File Format, Version 3.1\nNumerical Design Limited, Chapel Hill, NC 27514\nCopyright (c) 1996-2000\nAll Rights Reserved\n"},
		{0x03030000, "NetImmerse File Format, Version 3.03\nNumerical Design Limited, Chapel Hill, NC 27514\nCopyright (c) 1996-2000\nAll Rights Reserved\n"},
		{0x04000002, "NetImmerse File Format, Version 4.0.0.2\n"},
		{0x0401000c, "NetImmerse File Format, Version 4.1.0.12\n"},
		{0x04020100, "NetImmerse File Format, Version 4.2.1.0\n"},
		{0x04020200, "NetImmerse File Format, Version 4.2.2.0\n"}
	};

	// Maik Jurkait  24.05.2004 16:59:58
	// ------------------------------------------
	//	Compare file-head
	// ------------------------------------------
	const max = sizeof(allversions)/sizeof(allversions[0]);
	int pos;
	for (pos=0;pos<max;pos++)
		if (strncmp(allversions[pos].str,g_mempointer, strlen(allversions[pos].str))==0) break;


	// Maik Jurkait  24.05.2004 17:00:16
	// ------------------------------------------
	//	unknown head?
	// ------------------------------------------
	assert(pos<max);
	if (pos>=max)
	{
		dw_version = 0;
		return;
	}


	// Maik Jurkait  24.05.2004 17:04:12
	// ------------------------------------------
	//	Store + Skip
	// ------------------------------------------
	dw_version = allversions[pos].dw_version;
	readSkip((int)strlen(allversions[pos].str));


	// Maik Jurkait  24.05.2004 17:04:20
	// ------------------------------------------
	//	V4+ have addition version info
	// ------------------------------------------
	if (dw_version >= 0x04000000)
	{
		DWORD version2 = readUInt();
		assert(dw_version==version2);// "String and internal version differs");
		readUInt();//unknow (date?)
	}
}






DWORD	CNifStream::Version()
{
	return dw_version;
}


unsigned char	CNifStream::peek(int i)
{
	assert(g_memsize>=i);
	return g_mempointer[i];

}

unsigned char*	CNifStream::readBlock(int size)
{
	assert(g_memsize>=size);

	unsigned char *res;
	res = new unsigned char[size];
	memcpy(res,g_mempointer,size);
	g_mempointer += size;
	g_memsize -= size;

	return res;
}


char *CNifStream::readstring()
{
	assert(g_memsize>=4);
	unsigned int len= *(unsigned int*)g_mempointer; g_mempointer+=4;
	assert(len<0xfff);
	assert(g_memsize>=(int)(4+len));
	char *res = new char[len+1];
	memcpy(res,g_mempointer,len),
	res[len]=0;
	g_mempointer+=len;
	g_memsize -= 4 + len;
	return res;
}

char *CNifStream::readstring(int maxlen)
{
	if (g_memsize<4) return NULL;
	unsigned int len= *(unsigned int*)g_mempointer; g_mempointer+=4;
	if (len>(unsigned)maxlen) return NULL;
	if (g_memsize<(int)(4+len)) return NULL;
	char *res = new char[len+1];
	memcpy(res,g_mempointer,len),
	res[len]=0;
	g_mempointer+=len;
	g_memsize -= 4 + len;
	return res;
}


void CNifStream::readSkip(int i)
{
	assert(g_memsize>=i);
	g_mempointer += i;
	g_memsize -= i;
}

unsigned char	CNifStream::readUChar()
{
	assert(g_memsize>=1);
	unsigned char len= *(unsigned char*)g_mempointer;
	g_mempointer+=1;
	g_memsize -= 1;
	return len;
}



unsigned short CNifStream::readWord()
{
	assert(g_memsize>=2);
	unsigned short len= *(unsigned short*)g_mempointer;
	g_mempointer+=2;
	g_memsize -= 2;
	return len;
}

int CNifStream::readInt()
{ 
	assert(g_memsize>=4);
	int len= *(int*)g_mempointer;
	g_mempointer+=4;
	g_memsize -= 4;
	return len;
}

unsigned int CNifStream::readUInt()
{ 
	assert(g_memsize>=4);
	unsigned int len= *(unsigned int*)g_mempointer;
	g_mempointer+=4;
	g_memsize -= 4;
	return len;
}

float CNifStream::readFloat()
{ 
	assert(g_memsize>=4);
	float len= *(float*)g_mempointer;
	g_mempointer+=4;
	g_memsize -= 4;
	return len;
}

void CNifStream::readMatrix(CMatrix &p)
{
	float m[16];
	for (int n=0;n<16;n++)
	{
		m[n] = readFloat();
	}

	// transform
	p[0] = m[3];	p[1] = m[6];	p[2] = m[9];	p[3] = m[15];
	p[4] = m[4];	p[5] = m[7];	p[6] = m[10];	p[7] = m[14];
	p[8] = m[5];	p[9] = m[8];	p[10] = m[11];	p[11] = m[13];
	p[12] = m[0];	p[13] = m[1];	p[14] = m[2];	p[15] = m[12];
}


void CNifStream::readPoints(CVector *p,int n)
{
	while (n>0)
	{
		float x = readFloat();
		float y = readFloat();
		float z = readFloat();
		p->Set(x,y,z);
		p++;
		n--;
	}
}

bool CNifStream::readBool()
{
	if (dw_version < 0x04000003) return 0!=readUInt();
	return 0!=readUChar();
}

