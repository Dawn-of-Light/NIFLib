#include "StdAfx.h"
#include ".\cniuvdata.h"

CNiUVData::CNiUVData(void)
{
}

CNiUVData::~CNiUVData(void)
{
}


bool	CNiUVData::Load(CNifStream& stream)
{

	unsigned frames;

	for (int k = 0; k<4;k++)
	{
		frames = stream.readUInt();
		if (frames > 0)
		{
			unsigned controllertype = stream.readUInt(); 
			assert(controllertype < 5 && controllertype != 0);

			for (unsigned t = 0;t<frames;t++)
			{
				float f1 = stream.readFloat(); // time
				float f2 = stream.readFloat(); // x
				float f3 = stream.readFloat(); // y
				float f4 = stream.readFloat(); // z
			}
		}
	}

	/*
  Result = FileHandle.ReadLong(m_NumRecords);
  if (!Result) return (false);
  if (m_NumRecords == 0) return (true);

  CreateArrayPointer(m_pRecords, nifpos4_t, m_NumRecords);
  
  Result = FileHandle.ReadLong(m_Unknown1);
  if (Result) Result = FileHandle.Read((char *)m_pRecords, sizeof(nifpos4_t)*m_NumRecords);
  if (!Result) return (false);
*/

	return true;
}



#ifdef ANALYSE_NIF
void	CNiUVData::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	newpar = addfkt(par,this, "UVData",TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

}
#endif