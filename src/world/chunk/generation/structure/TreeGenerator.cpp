#include "TreeGenerator.h"

#include "../../ChunkManager.h"
#include "../../voxel/VoxelType.h"

void TreeGenerator::generateTree(ChunkManager& chunkManager, int x, int y, int z)
{
	generateBranch(chunkManager, vt::Vec3f((float)x, (float)y, (float)z), vt::Vec3f(0.0f, 1.0f, 0.0f), 2.5f + (rand() % 32) / 32.0f, VoxelType::WOOD, 0);
}

void TreeGenerator::generateBox(ChunkManager& chunkManager, int x, int y, int z, int xs, int ys, int zs, const VoxelData& data)
{
	for (int xx = x; xx < x + xs; xx++) {
		for (int yy = y; yy < y + ys; yy++) {
			for (int zz = z; zz < z + zs; zz++) {
				chunkManager.setVoxelData(xx, yy, zz, data);
			}
		}
	}
}

void TreeGenerator::generateLeaves(ChunkManager& chunkManager, int x, int y, int z, int xs, int ys, int zs, const VoxelData& data)
{
	Noise noise((unsigned int)time(0));
	for (int xo = 0; xo < xs; xo++) {
		float xd = ((float)xo - (float)(xs - 1) / 2.0f) / ((float)(xs - 1) / 2.0f);
		for (int yo = 0; yo < ys; yo++) {
			float yd = ((float)yo - (float)(ys - 1) / 2.0f) / ((float)(ys - 1) / 2.0f);
			for (int zo = 0; zo < zs; zo++) {
				float zd = ((float)zo - (float)(zs - 1) / 2.0f) / ((float)(zs - 1) / 2.0f);
				float d = sqrt(xd * xd + yd * yd + zd * zd);
				float n = noise.noise(xo / 4.0f, yo / 4.0f, zo / 4.0f);
				if (d > 1.0f - (n * 0.5f + 0.5f) / 4.0f) {
					continue;
				}
				chunkManager.setVoxelData(x + xo, y + yo, z + zo, data);
			}
		}
	}
}

void TreeGenerator::generateBranch(ChunkManager& chunkManager, const vt::Vec3f& position, const vt::Vec3f& direction, float size, const VoxelData& data, int level)
{
	if (size <= 0.0f) {
		int leaveRadius = (25 + rand() % 25) / (level + 1);
		generateLeaves(chunkManager, (int)position.x - (leaveRadius - 1) / 2, (int)position.y - (leaveRadius - 1) / 2, (int)position.z - (leaveRadius - 1) / 2, leaveRadius, leaveRadius / 3 * 2, leaveRadius, VoxelType::LEAVES);
		return;
	}
	if (size < 2.0f && size > 1.0f && rand() % 4 == 0) {
		generateBranch(chunkManager, position, vt::Vec3f((float)(rand() % 32 - 16), (float)(rand() % 8), (float)(rand() % 32 - 16)).normalized(), size / 1.5f, data, level + 1);
	}
	generateBox(chunkManager, (int)(position.x - size), (int)(position.y - size), (int)(position.z - size), (int)(size * 2.0f + 1.0f), (int)(size * 2.0f + 1.0f), (int)(size * 2.0f + 1.0f), data);
	generateBranch(chunkManager, position + direction, (direction + vt::Vec3f((float)(rand() % 32 - 16), (float)(rand() % 32 - 16), (float)(rand() % 32 - 16)) / 512.0f).normalized(), size - 1.0f / 12.0f, data, level);
}