#include "Voxel.h"

#include "VoxelType.h"

Voxel::Voxel(const vt::String& name) : m_id(0), m_name(name)
{
	VoxelType::registerVoxel(this);
}

Voxel::~Voxel()
{
}
