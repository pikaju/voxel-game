#pragma once

#include "GUISystem.h"

class GUIComponent
{
public:
	GUIComponent();
	~GUIComponent();

	virtual void initialize() {  }
	virtual void cleanup() {  }

	virtual void update(float delta, const vt::Input& input) {  }
	virtual void render(const vt::Window* window) {  }

	inline void setSystem(GUISystem* system) { m_system = system; }

	virtual inline bool isActive() const { return m_active; }
	virtual inline void setActive(bool active) { m_active = active; }
protected:
	GUISystem* m_system;
	bool m_active;
};