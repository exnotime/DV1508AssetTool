#include "FrameBuffer.h"
#include "MaterialBank.h"
using namespace gfx;

FrameBuffer::FrameBuffer(){
}

FrameBuffer::~FrameBuffer(){

}

void FrameBuffer::Init(){
	glGenFramebuffers(1, &m_Handle);
}

void FrameBuffer::Apply(){
	glBindFramebuffer(GL_FRAMEBUFFER, m_Handle);
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);
	glViewport(0, 0, m_Width, m_Height);
}

void FrameBuffer::SetTexture(gfx::TextureHandle handle){
	Texture* tex = g_MaterialBank.GetTexture(handle);
	m_Width = tex->GetWidth();
	m_Height = tex->GetHeight();
	glBindFramebuffer(GL_FRAMEBUFFER, m_Handle);
	glBindTexture(GL_TEXTURE_2D, tex->GetHandle());
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->GetHandle(), 0);
	GLuint depth;
	glGenTextures(1, &depth);
	glBindTexture(GL_TEXTURE_2D, depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Width, m_Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth, 0);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch (status){
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT :
		printf("FBO error, incomplete attachment\n");
		break;
	case GL_FRAMEBUFFER_COMPLETE:
		int i = 0;
		break;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}