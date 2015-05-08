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
	//dont change if the target has not changed
	if (m_TargetTexture == handle)
		return;
	m_TargetTexture = handle;

	Texture* tex = g_MaterialBank.GetTexture(m_TargetTexture);
	m_Width = (GLuint)tex->GetWidth();
	m_Height = (GLuint)tex->GetHeight();
	glBindFramebuffer(GL_FRAMEBUFFER, m_Handle);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->GetHandle(), 0);
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

gfx::TextureHandle FrameBuffer::GetTexture(){
	return m_TargetTexture;
}