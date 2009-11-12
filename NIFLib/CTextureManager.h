#pragma once


class CDirList;

#define MAX_TEXTURES 2


/**
*	CTextureManager
*
*   Manages all textures.
*   This means:
*    - it checks if a texture is already loaded
*	 - when a texture is no longer needed
*	 - checks where the texture file is
*	 - load's the diffent formats
*
* \todo check if GL-Extention is supported
* \todo generate mipmaps of file-internal pics
**/
class CTextureManager
{
	private:
		struct s_texture
		{
			GLuint	id;
			int		references;
			char	name[120];
		};


		struct image_data
		{
			unsigned	nof_mipmaps;
			int			format;		//...format of a dds-texture
			int			channels;	//...The channels in the image(3=RGB, 4=RGBA)...
			int			sizeX;		//...The width of the image in pixels...
			int			sizeY;		//...The height of the image in pixels...
			unsigned char *data;	//...The image pixel data...
		};


	public:
		CTextureManager(void);
		~CTextureManager(void);

		// --- OpenGl activate ---
		void	ActivateTexture(GLuint id);
		void	ActivateTexture(GLuint id, int channel);
		void	DeActivateTexture(int channel);

		// ----------- loading / creating / freeing -----------------

		/**
		*	Get a texture
		**/
		GLuint	GetTexture(const char* filename);

		/**
		*	Get a texture (referenz increase)
		**/
		GLuint	GetTexture(GLuint id);

		/**
		*	
		**/
		void	ReleaseTexture(GLuint id);

		/**
		*	Create a texture from given RGB data.
		*	It can't be 'found' anymore so make sure you release the Texture-Id if not used anymore
		**/
		GLuint	CreateTexture(int x, int y, unsigned char* data, int channels);

		// --- search pathes ---
		void	AddSearchPath(const char*);
		void	RemoveSearchPath(const char*);

		/**
		*	
		**/
		void	TextureChanged();


	private:

		/**
		*	file read method
		**/
		int			LoadTexture(const char* name);

		/**
		*	search if texture is in cache
		**/
		int			FindTexture(const char* name);

		int			CreateAndInsertTexture(image_data*p,const char* name);

		image_data* _LoadBMP(char*mem, size_t size);
		image_data* _LoadTGA(char*mem, size_t size);
		image_data*	_LoadDDS(char*mem, size_t size);


		GLuint				mn_current_active;
		GLuint				mn_current_texture[MAX_TEXTURES];

		TArray <s_texture>	ms_textures;
		TArray <CDirList*>	ma_searchpath;
};




extern CTextureManager* gpc_texturemanager;