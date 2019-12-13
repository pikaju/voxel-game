#pragma once

#include "../Voxel.h"

class SolidVoxel : public Voxel
{
public:
	SolidVoxel(const vt::String& name, const vt::Vec3b& color) : Voxel(name), m_color(color) {  }
	~SolidVoxel() {  }

	inline bool isOpaque() const override { return true; }
	inline const vt::Vec3b& getColor() const { return m_color; }
private:
	vt::Vec3b m_color;
};