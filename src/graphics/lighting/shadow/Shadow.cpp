#include "Shadow.h"

Shadow::Shadow() : m_framebuffer(), m_viewMatrix(vt::Mat4f::identity()), m_projectionMatrix(vt::Mat4f::identity()), m_changed(false)
{
}

Shadow::~Shadow()
{
}

void Shadow::initialize(unsigned int width, unsigned int height)
{
	std::vector<vt::Attachment> attachments;
	attachments.push_back(vt::ATTACHMENT_DEPTH);
	std::vector<vt::Format> formats;
	formats.push_back(vt::FORMAT_SHADOW16);
	m_framebuffer.initialize(width, height, attachments.size(), attachments.data(), formats.data());
}

void Shadow::cleanup()
{
	m_framebuffer.cleanup();
}
