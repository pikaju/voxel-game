#pragma once

#include <vector>

#include "types/SolidVoxel.h"

class VoxelType
{
public:
	static Voxel AIR;
	static Voxel WATER;
	static Voxel GRASS_CULMS;
	static SolidVoxel SOIL;
	static SolidVoxel GRASS;
	static SolidVoxel ROCK;
	static SolidVoxel SNOW;
	static SolidVoxel SAND;
	static SolidVoxel WOOD;
	static SolidVoxel LEAVES;

	static void registerVoxel(Voxel* voxel);
	static Voxel* getFromID(const VoxelID& id);
private:
	static std::vector<Voxel*> m_types;
};