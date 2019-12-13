#include "MSAA.h"

MSAA::MSAA() : m_framebuffer()
{
}

MSAA::~MSAA()
{
}

void MSAA::initialize()
{
}

void MSAA::cleanup()
{
	m_framebuffer.cleanup();
}

void MSAA::setScreenSize(unsigned int width, unsigned int height, const std::vector<vt::Attachment>& attachments, const std::vector<vt::Format>& formats)
{
	m_framebuffer.cleanup();
	m_framebuffer.initialize(width, height, attachments.size(), attachments.data(), formats.data(), 4);
}

void MSAA::bind()
{
	m_framebuffer.bind();
}
