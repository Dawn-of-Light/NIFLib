#pragma once

class CVector;
class CMatrix;

/**
* Memory stream
**/
class CNifStream
{
	public:

		/**
		* Creates the stream and reads the file head
		*
		* NOTE: Check Version it creation was succefull.
		*
		*	@param data
		*			Pointer to read-only memory block (will not be released)
		*			Make sure memory isn't changeing while parsing the block.
		*	@param size
		*			The size of the memory block.
		**/
		CNifStream(char*data, size_t size);
	
		/**
		*	Use "Version()" to check version of Nif-File.
		*	If it return 0 there was an error while reading the header.
		*
		*	@return version of NIF-File
		*			0 on error
		**/
		DWORD			Version();




		// ------------------ reading -------------------
		char *			readstring(int maxlen);
		char *			readstring();
		unsigned short	readWord();
		int				readInt();
		unsigned int	readUInt();
		float			readFloat();
		unsigned char	readUChar();
		unsigned char*	readBlock(int size);
		void			readMatrix(CMatrix &p);
		void			readPoints(CVector *p,int n);
		bool			readBool();		// <- depending on dw_version

		// ------------------ reading special -----------

		/**
		*	skips a block of data
		*
		* @param i how much data to skip.
		**/
		void			readSkip(int i);

		/**
		*	Get a value at given position (depending on current file-pointer)
		*
		* @param i position
		**/
		unsigned char	peek(int i);


		// ------------------ direct access ------------

		/**
		* Get current file pointer position
		* NOTE: additional check the SIZE 
		**/
		char*			GetPosition()	{ return g_mempointer;}

		/**
		* Gives the current size of the stream.
		* (each readXX operation decreases this)
		*
		* @return bytes still in stream
		**/
		long			Size()			{ return g_memsize;}


	private:

		/**
		* 	
		**/
		void	readHead();


		/**
		*	file pointer
		**/
		char*	g_mempointer;

		/**
		*	rest size
		**/
		long	g_memsize;

		/**
		*	NIF-Version
		**/
		DWORD	dw_version;
};
