#include "StdAfx.h"
#include ".\cniparticlesystemcontroller.h"

CNiParticleSystemController::CNiParticleSystemController(void)
{
}

CNiParticleSystemController::~CNiParticleSystemController(void)
{
}


bool	CNiParticleSystemController::Load(CNifStream& stream)
{
		if (stream.Version() < 0x04000000)
		{
			stream.readSkip(154);
			mc_controller.read(stream); 
			mc_collider.read(stream,1); 
			mc_color.read(stream,1); 
			stream.readSkip(8);
/*
 Result = FileHandle.ReadLong(m_Unknown1);
  if (Result) Result = FileHandle.ReadShort(m_Unknown2);
  if (Result) Result = FileHandle.Read((char *)&m_Position1, sizeof(nifpos4_t));
  if (Result) Result = FileHandle.ReadLong(m_Unknown3);
  if (Result) Result = FileHandle.Read((char *)&m_Position2[0], sizeof(nifpos4_t)*NIF_PSC_NUMPOS2);
  if (Result) Result = FileHandle.ReadLong(m_Unknown4);
  if (Result) Result = FileHandle.ReadLong(m_Unknown5);
  if (Result) Result = FileHandle.ReadLong(m_Unknown6);
  if (Result) Result = FileHandle.ReadChar(m_Unknown7);
  if (Result) Result = FileHandle.ReadLong(m_Unknown8);
  if (Result) Result = FileHandle.ReadLong(m_Unknown9);
  if (Result) Result = FileHandle.ReadLong(m_Unknown10);
  if (Result) Result = FileHandle.ReadLong(m_Unknown11);
  if (Result) Result = FileHandle.ReadLong(m_Unknown12);
  if (Result) Result = FileHandle.ReadFloat(m_Unknown13);
  if (Result) Result = FileHandle.ReadLong(m_Unknown14);
  if (Result) Result = FileHandle.ReadLong(m_Unknown15);
  if (Result) Result = FileHandle.ReadShort(m_Unknown16);
  if (Result) Result = FileHandle.ReadShort(m_NumData);
  if (Result) Result = FileHandle.ReadShort(m_Unknown17);
  if (!Result) return (false);

  if (m_NumData < 0) {
    ErrorHandler.AddError(ERR_CUSTOM, "Invalud data size %d!", m_NumData);
    return (false);
   }

  //SystemLog.Printf ("\t\tOffset = 0x%08lX", FileHandle.Tell());

  if (m_NumData > 0) {
    CreateArrayPointer(m_pData, nifpscdata_t, m_NumData);
    Result = FileHandle.Read((char *)&m_pData[0], sizeof(nifpscdata_t)*m_NumData);
    if (!Result) return (false);
   }
  
  //SystemLog.Printf ("\t\tOffset = 0x%08lX", FileHandle.Tell());
  Result = FileHandle.ReadLong(m_Unknown18); 
  if (Result) Result = FileHandle.ReadShort(m_Unknown19); 
  if (Result) Result = FileHandle.ReadShort(m_Unknown20); 
  if (Result) Result = FileHandle.ReadLong(m_Unknown21); 
  if (Result) Result = FileHandle.ReadChar(m_Unknown22); 
  if (!Result) return (false);
*/

			return true;
		}

		stream.readUInt();
		stream.readWord();
		stream.readFloat();
		stream.readFloat();
		stream.readFloat();
		stream.readFloat();

		unsigned val = stream.readUInt(); // parent?
		stream.readSkip(16*sizeof(float)); // matrix?

		stream.readUChar(); // 0
		stream.readFloat();
		stream.readFloat();
		stream.readFloat();

		stream.readWord();
		stream.readFloat();
		stream.readFloat();
		stream.readFloat();

		stream.readWord();
		stream.readFloat();
		stream.readFloat();

		stream.readWord();
		stream.readFloat();
		stream.readFloat();


		unsigned frames = stream.readWord();
		unsigned val2 = stream.readWord();
		stream.readSkip(frames*(9*sizeof(float)+2*sizeof(short)));

//12,13
		stream.readUInt(); // controller?
		mc_controller.read(stream,1); 
		mc_collider.read(stream,1); 
		stream.readUChar(); // 1


	return true;
}


void	CNiParticleSystemController::CreateHierachy(CNode**ppc_nodes, int nof)
{
	mc_controller.Resolve(ppc_nodes, nof);		
	mc_collider.Resolve(ppc_nodes, nof);
	mc_color.Resolve(ppc_nodes, nof);
}



#ifdef ANALYSE_NIF
void	CNiParticleSystemController::CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image))
{
	void* newpar;
	void* newsub;
	newpar = addfkt(par,this,"ParticleSystemController",TIC_PARTICLE);

	newsub = addfkt(newpar,NULL, "Data",TIC_DATA);
	CNode::CreateDbgTree(newsub,addfkt);

	CreateDbgTreeChilds(mc_controller,"Controller",TIC_CONTROLLER,newpar,addfkt);
	CreateDbgTreeChilds(mc_collider,"Collider",TIC_CHILDS,newpar,addfkt);
	CreateDbgTreeChilds(mc_color,"Color",TIC_TEXTURE,newpar,addfkt);
}
#endif
