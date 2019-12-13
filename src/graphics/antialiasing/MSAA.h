#pragma once

#include <vt/graphics/texture/GLFramebuffer.h>

class MSAA
{
public:
	MSAA();
	~MSAA();

	void initialize();
	void cleanup();

	void setScreenSize(unsigned int width, unsigned int height, const std::vector<vt::Attachment>& attachments, const std::vector<vt::Format>& formats);

	void bind();
	inline void blit(const vt::GLFramebuffer& target) { m_framebuffer.blit(target); }
	vt::GLFramebuffer m_framebuffer;
private:
};