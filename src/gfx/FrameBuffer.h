#pragma once
#include <gl/glew.h>
namespace gfx{
	typedef int TextureHandle;
	class FrameBuffer{
	public:
		FrameBuffer();
		~FrameBuffer();
		void Init();
		void Apply();
		void SetTexture(TextureHandle handle);
		TextureHandle GetTexture();
	private:
		GLuint m_Handle;
		GLuint m_Width;
		GLuint m_Height;
		TextureHandle m_TargetTexture;
	};
}
