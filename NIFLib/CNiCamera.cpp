#include "StdAfx.h"
#include ".\cnicamera.h"

CNiCamera::CNiCamera(void)
{
	mp_cameraname = NULL;
}

CNiCamera::~CNiCamera(void)
{
	SAFE_DELETE(mp_cameraname);
}


bool	CNiCamera::Load(CNifStream& stream)
{
	mp_cameraname = stream.readstring();

	if (stream.Version() == 0x02030000)	stream.readSkip(191);	
	else								stream.readSkip(135);	


	/*
		NifReadString(FileHandle, m_CameraName);
		FileHandle.ReadLong(m_Unknown1);
		FileHandle.ReadLong(m_Unknown2);
		FileHandle.ReadShort(m_Unknown3);
		FileHandle.Read((char *)&m_Floats[0], sizeof(float)*NIF_CAMERA_NUMFLOATS);
		FileHandle.ReadLong(m_Unknown4);
		FileHandle.ReadLong(m_Unknown5);
	*/

	return true;
}



#ifdef ANALYSE_NIF
void	CNiCamera::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;

	char temp[256]; sprintf(temp,"Camera: %s",mp_cameraname);
	newpar = addfkt(par,this, temp,TIC_ABSTRACT);

	newsub = addfkt(newpar,NULL,"Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

}
#endif