#pragma once

#include <vector>
#include <unordered_map>

#include <vt/math/Vec3.h>
#include <vt/graphics/shader/GLShaderProgram.h>
#include <vt/graphics/texture/GLTexture.h>
#include <vt/util/ThreadPool.h>

#include "generation/ChunkGenerator.h"
#include "voxel/Voxel.h"

class World;
class Chunk;
enum ChunkProgress;

class ChunkManager
{
public:
	ChunkManager(World* world);
	~ChunkManager();

	void initialize();
	void cleanup();

	void update();
	void render(const vt::GLShaderProgram shader, const vt::Mat4f& viewMatrix, const vt::Mat4f& projectionMatrix);
	void renderWater(const vt::Mat4f& viewMatrix, const vt::Mat4f& projectionMatrix, float time);
	void renderGrass(const vt::Mat4f& viewMatrix, const vt::Mat4f& projectionMatrix, float time);

	inline World* getWorld() { return m_world; }

	void addChunk(Chunk* chunk);
	Chunk* getChunk(const vt::Vec3i& position);
	void removeChunk(Chunk* chunk);
	void generateChunk(const vt::Vec3i& position);
	bool isChunkSurrounded(const vt::Vec3i& position, const ChunkProgress& minimumProgress);
	void setVoxelData(int x, int y, int z, const VoxelData& data);
	bool pickVoxel(vt::Vec3f position, const vt::Vec3f& direction, float maxDistance, vt::Vec3i& result);
	
	inline const ChunkGenerator& getChunkGenerator() const { return m_chunkGenerator; }

	inline float getMaxRenderDistance() const { return m_maxRenderDistance; }
private:
	static bool sortChunks(Chunk* c0, Chunk* c1);
	static void processChunk(void* c);

	World* m_world;

	std::vector<Chunk*> m_chunkVector;
	std::vector<Chunk*> m_chunkProcessingVector;
	std::unordered_map<vt::Vec3i, Chunk*> m_chunkMap;
	vt::ThreadPool m_threadPool;
	ChunkGenerator m_chunkGenerator;

	vt::GLShaderProgram m_waterShader;
	vt::GLTexture* m_waterTexture;
	vt::GLShaderProgram m_grassShader;

	float m_maxRenderDistance;
};