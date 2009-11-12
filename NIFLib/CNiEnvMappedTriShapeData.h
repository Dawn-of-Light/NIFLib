#pragma once


class CNiEnvMappedTriShapeData : public CNode
{
	public:
		CNiEnvMappedTriShapeData(void);
		~CNiEnvMappedTriShapeData(void);

		bool	Load(CNifStream& stream);

		DECLARE_CHUNK(NiEnvMappedTriShapeData,"NiEnvMappedTriShapeData");

};
