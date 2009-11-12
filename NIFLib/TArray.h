/**
* TArray
* by M.Jurkait (long time ago)
*
* Just my own Array-Class
*
*
**/


#pragma once
#include <assert.h>

template <class T>
class TArray
{
	public:

		/**
		*	TArray(int initlen, int steps)
		*	constructor
		*	@param initlen  default allocated size (not array size)
		*	@param steps    how much the memoryblock should grow if space is needed
		**/
		TArray(int initlen, int steps)
		{
			assert(initlen >0&& steps >0);
			n_realoc_steps = steps; 
			n_memsize=initlen; 
			ma_data = new T[initlen]; 
			n_size = 0;
		};

		/**
		*	TArray()
		*	Standard constructor
		**/
		TArray()
		{
			n_realoc_steps = 8; 
			n_memsize=8; 
			ma_data = new T[8]; 
			n_size = 0;

		};

		/**
		*	~TArray()
		*	Destructor
		**/
		~TArray()						{ delete [] ma_data;}

		/**
		*	RealocSteps(int n)
		*	Changes the 'Grow' value
		**/
		void	RealocSteps(int n)		{ assert(n_realoc_steps>0); n_realoc_steps = n;}

		/**
		*	RemoveAll()
		*	(same as SetSize(0)) - just more readfriendly
		**/
		void	RemoveAll()				{ SetSize(0); }

  

		void	Insert(int index, T obj)			
		{
			// move and make space
			if (n_memsize<=n_size)
			{
				T* n = new T[n_memsize+n_realoc_steps];
				memcpy(n,ma_data,index*sizeof(T));
				memcpy(n+index+1,ma_data+index,(n_size-index)*sizeof(T));
				n_memsize += n_realoc_steps;
				delete [] ma_data;
				ma_data = n;
			}
			else
			{
				memmove(ma_data+index+1,ma_data+index,(n_size-index)*sizeof(T));
			}

			n_size++;


			assert(n_memsize>=n_size);

			memcpy(&ma_data[index],&obj,sizeof(T));
		}

		/**
		*	Removes all unused memoryspace
		**/
		void	CleanUp()
		{
			int newsize = min(n_size,n_realoc_steps);
			T* n = new T[newsize];
			memcpy(n,ma_data,n_size*sizeof(T));
			n_memsize = newsize;
			delete [] ma_data;
			ma_data = n;
		}

		/**
		*	int		Count()	const
		*	@return Number of elemets
		**/
		int		Count()	const			{ return n_size;}

		/**
		*	void	AddLast(T obj)
		*	Adds new elements
		**/
		void	AddLast(T obj)			
			{
				if (n_memsize<=n_size)
				{
					T* n = new T[n_memsize+n_realoc_steps];
					memcpy(n,ma_data,n_memsize*sizeof(T));
					n_memsize += n_realoc_steps;
					delete [] ma_data;
					ma_data = n;
				}
				assert(n_memsize>=n_size);

				memcpy(&ma_data[n_size],&obj,sizeof(T));
				n_size++;
			}

		T& GetLast()				{ assert(n_size>0); return ma_data[n_size-1];}


		/**
		*	Access operator
		**/
		T& operator[] (int i)		{ assert(i<n_size); return ma_data[i];}


		/**
		*	Deletes an element
		**/
		void	Delete(int n)
		{
				assert(n<n_size && n>=0);
				memmove(&ma_data[n],&ma_data[n+1],sizeof(T)*(n_size-n-1));
				n_size--;
		}


		/**
		*	Deletes an element (with lesser memory movement)
		*	NOTE: THIS WILL CHANGE THE ORDER OF THE ELEMENTS
		**/
		void	DeleteQuick(int n)
		{
				assert(n<n_size && n>=0);
				assert(n_size>0);
				memmove(&ma_data[n],&ma_data[n_size-1],sizeof(T));
				n_size--;
		}


		/**
		*	Set an element.
		*	If the Index is out of the array bounds	the array will grow.
		*	NOTE: if the array grows all new elements are uninitialized.
		**/
		void	SetGrow(int n, T obj)
		{
			if (n>=n_size)	SetSize(n+1);
			memcpy(&ma_data[n],&obj,sizeof(T));
		}


		/**
		*	Simply changes the size of the array
		*	No memory is freed - for that you should call CLEANUP
		**/
		void	SetSize(int t)
		{
			if (n_memsize<=t)
			{
				T* n = new T[t];
				memcpy(n,ma_data,n_size*sizeof(T));
				#ifdef _DEBUG
					memset(n+n_memsize,0xcd,sizeof(T)*(t-n_memsize));
				#endif
				n_memsize = t;
				delete [] ma_data;
				ma_data = n;
			}

			assert(n_memsize>=n_size);
			n_size = t;
		}


		/**
		*	@return the pure data pointer
		*	for faster iterations
		**/
		T* GetData() const	{ return ma_data;}


	private:

		/**
		*	data pointer
		**/
		T*	ma_data;

		/**
		*	allocated memory
		**/
		int n_memsize;

		/**
		*	number of elements
		**/
		int	n_size;

		/**
		*	memory allocation steps (at this time only used by 'AddLast')
		**/
		int n_realoc_steps;
};

