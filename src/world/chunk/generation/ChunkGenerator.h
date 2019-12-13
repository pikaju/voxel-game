#pragma once

#include "../../../util/Noise.h"

class Chunk;

class ChunkGenerator
{
public:
	ChunkGenerator(unsigned int seed);
	~ChunkGenerator();

	void generateBaseTerrain(Chunk& chunk) const;
private:
	Noise m_noise;
};