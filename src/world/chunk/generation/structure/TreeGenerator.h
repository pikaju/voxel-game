#pragma once

#include <vt/math/Vec3.h>

class ChunkManager;
class VoxelData;

class TreeGenerator
{
public:
	static void generateTree(ChunkManager& chunkManager, int x, int y, int z);
private:
	static void generateBox(ChunkManager& chunkManager, int x, int y, int z, int xs, int ys, int zs, const VoxelData& data);
	static void generateLeaves(ChunkManager& chunkManager, int x, int y, int z, int xs, int ys, int zs, const VoxelData& data);
	static void generateBranch(ChunkManager& chunkManager, const vt::Vec3f& position, const vt::Vec3f& direction, float size, const VoxelData& data, int level);
};