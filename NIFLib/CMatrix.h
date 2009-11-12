#pragma once
#include "cvector.h"


class CMatrix
{

	public:
		CMatrix(void);
		~CMatrix(void);

		float& operator[](int i)	{ return m[i];}
		float*	Get()				{ return m;}


		// einheits matrix
		void	Identity();

		// Set Functions -> Clears matrix and set these porpertys
		void	SetRotation(float r_x, float r_y, float r_z);
		void	SetTranslation(float r_x, float r_y, float r_z);
		void	SetScale(float r_x, float r_y, float r_z);

		// Add Functions -> Adds properties to the matrix
		void	AddRotation(float r_x, float r_y, float r_z);
		void	AddTranslation(float r_x, float r_y, float r_z);
		void	AddScale(float r_x, float r_y, float r_z);

		void	AddRotationBefore(float r_x, float r_y, float r_z);
		void	AddTranslationBefore(float r_x, float r_y, float r_z);
		void	AddScaleBefore(float r_x, float r_y, float r_z);


		void	Invers();

		/*
			Same as:
				SetRotation
				AddTranslationBefore
				AddScaleBefore
		*/
		void	Create( float r_x, float r_y, float r_z,
						float t_x, float t_y, float t_z,
						float s_x, float s_y, float s_z	);


		CMatrix		operator*(CMatrix &b);
		CMatrix&	operator*=(CMatrix &b);
		CMatrix&	operator =(const CMatrix& b);



		CVector		Transform(const CVector&) const;

		// get current gl-matrix
		void	LoadGLMatrix();

		// Analyse Output
		#ifdef ANALYSE_NIF
			void	CreateDbgTree(void* par, void*(*addfkt)(void*, CNode*, char* text, int image));
		#endif

	private:

		float	m[16];
};
