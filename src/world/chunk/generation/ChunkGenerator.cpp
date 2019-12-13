#include "ChunkGenerator.h"

#include "../Chunk.h"

ChunkGenerator::ChunkGenerator(unsigned int seed) : m_noise(seed)
{
}

ChunkGenerator::~ChunkGenerator()
{
}

void ChunkGenerator::generateBaseTerrain(Chunk& chunk) const
{
	VoxelArrayContainer<VoxelData, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE>* container = new VoxelArrayContainer<VoxelData, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE>();
	for (unsigned int x = 0; x < CHUNK_SIZE; x++) {
		for (unsigned int y = 0; y < CHUNK_SIZE; y++) {
			for (unsigned int z = 0; z < CHUNK_SIZE; z++) {
				float xx = (float)((int)x + chunk.getPosition().x * CHUNK_SIZE);
				float yy = (float)((int)y + chunk.getPosition().y * CHUNK_SIZE);
				float zz = (float)((int)z + chunk.getPosition().z * CHUNK_SIZE);

				float n0 = m_noise.noise(xx / 16.0f, yy / 16.0f, zz / 16.0f);
				float n1 = m_noise.noise(xx / 64.0f, yy / 64.0f, zz / 64.0f) * 4.0f;
				float n2 = m_noise.noise(xx / 128.0f, yy / 128.0f, zz / 128.0f) * 16.0f;
				float n3 = m_noise.noise(xx / 512.0f, zz / 512.0f) * 0.5f + 0.5f;

				container->setData(x, y, z, VoxelType::AIR);
				if ((n0 + n1 + n2) * n3 > yy / 32.0f) {
					container->setData(x, y, z, VoxelType::GRASS);
					if (n1 < (yy - 128.0f) / 8.0f) {
						container->setData(x, y, z, VoxelType::SNOW);
					}
				}
				else if (yy < -92.0f) {
					container->setData(x, y, z, VoxelType::WATER);
				}
			}
		}
	}
	for (unsigned int x = 0; x < CHUNK_SIZE; x++) {
		for (unsigned int y = 1; y < CHUNK_SIZE; y++) {
			for (unsigned int z = 0; z < CHUNK_SIZE; z++) {
				if (container->getData(x, y - 1, z).getID() == VoxelType::GRASS.getID() && container->getData(x, y, z).getID() == VoxelType::AIR.getID()) {
					container->setData(x, y, z, VoxelType::GRASS_CULMS);
				}
			}
		}
	}
	chunk.getVoxelContainer().compress(*container);
	delete container;
}
