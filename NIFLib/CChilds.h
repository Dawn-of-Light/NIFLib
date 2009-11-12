#pragma once

class CNode;

#define FOREACH(t,x,y)	for(int _r=0;_r<x.GetCount();_r++) {CNode*t=x[_r];if(t) {y}}


class CChilds
{
	public:
		CChilds(void);
		~CChilds(void);

		/**
		*	reads a list of ID's from stream
		*	if len == -1 the list size is variable
		*	this means the first thing in stream is a size value.
		**/
		void	read(CNifStream&, int len = -1);

		/**
		*	Size of list
		**/
		int		GetCount();

		/**
		*	Converts the ID's to real pointers
		*	NOTE: after that "ma_child_ids" is cleared
		**/
		void	Resolve(CNode**ppc_nodes, int nof);

		/**
		* Use this for PARENT connections.
		*
		* Same as "Resolve(TArray <CNode*> ap_nodes, TArray <int> an_referenzes)"
		* But it doesn't increase the referenzes.
		**/
		void	ResolveWithoutReferenz(CNode**ppc_nodes, int nof);

		CNode*	operator[](int i)		{ return map_childs[i];}


		/**
		*	Deletes the childs
		**/
		void	Clear();


		/**
		*	Add a node directly
		**/
		void	AddNode(CNode*);

	private:

		/**
		*	if TRUE all childs will be deleted on destruction
		**/
		bool					mb_referenzed;

		// tree constrution
		TArray<unsigned int>	ma_child_ids;

		// tree (after generation)
		TArray<CNode*>			map_childs;
};
