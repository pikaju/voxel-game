#include "VoxelType.h"

std::vector<Voxel*> VoxelType::m_types;

Voxel VoxelType::AIR = Voxel("Air");
Voxel VoxelType::WATER = Voxel("Water");
Voxel VoxelType::GRASS_CULMS = Voxel("Grass culms");
SolidVoxel VoxelType::SOIL = SolidVoxel("Soil", vt::Vec3b(64, 64, 32));
SolidVoxel VoxelType::GRASS = SolidVoxel("Grass", vt::Vec3b(80, 128, 80));
SolidVoxel VoxelType::ROCK = SolidVoxel("Rock", vt::Vec3b(64, 64, 64));
SolidVoxel VoxelType::SNOW = SolidVoxel("Snow", vt::Vec3b(255, 255, 255));
SolidVoxel VoxelType::SAND = SolidVoxel("Sand", vt::Vec3b(200, 180, 100));
SolidVoxel VoxelType::WOOD = SolidVoxel("Wood", vt::Vec3b(50, 40, 20));
SolidVoxel VoxelType::LEAVES = SolidVoxel("Leaves", vt::Vec3b(80, 140, 80));

void VoxelType::registerVoxel(Voxel* voxel)
{
	voxel->setID((VoxelID)m_types.size());
	m_types.push_back(voxel);
}

Voxel* VoxelType::getFromID(const VoxelID& id)
{
	return m_types[id];
}