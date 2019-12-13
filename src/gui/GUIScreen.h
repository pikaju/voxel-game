#pragma once

#include "GUIComponent.h"

class GUIScreen : public GUIComponent
{
public:
	GUIScreen();
	~GUIScreen();

	virtual void initialize() override;

	virtual void setActive(bool active) override;

	inline void add(GUIComponent* component) { m_components.push_back(component); }
private:
	std::vector<GUIComponent*> m_components;
};