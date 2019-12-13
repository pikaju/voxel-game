#pragma once

#include <vt/graphics/model/IndexedMesh.h>

#include "ChunkManager.h"
#include "voxel/VoxelType.h"
#include "voxel/container/VoxelRunLengthContainer.h"

static const int CHUNK_SIZE = 32;

enum ChunkNeighbor
{
	CHUNKNEIGHBOR_TOP = 0,
	CHUNKNEIGHBOR_BOTTOM,
	CHUNKNEIGHBOR_LEFT,
	CHUNKNEIGHBOR_RIGHT,
	CHUNKNEIGHBOR_FRONT,
	CHUNKNEIGHBOR_BACK,
	CHUNKNEIGHBOR_COUNT
};

enum ChunkProgress
{
	CHUNKPROGRESS_PRE_NEIGHBOR = 0,
	CHUNKPROGRESS_POST_NEIGHBOR,
	CHUNKPROGRESS_MESH,
	CHUNKPROGRESS_MODEL,
	CHUNKPROGRESS_DONE
};

class Chunk
{
public:
	Chunk(ChunkManager* chunkManager, const vt::Vec3i& position);
	~Chunk();

	void initialize();
	void cleanup();

	void update();
	void render();
	void renderWater();
	void renderGrass();

	void process();

	void preNeighborProcess();
	void postNeighborProcess();
	void calculateSkyLight();
	void createMesh();
	void createModel();

	inline unsigned int getID() const { return m_id; }
	inline void setID(unsigned int id) { m_id = id; }

	inline const vt::Vec3i& getPosition() const { return m_position; }

	void calculateDistanceToCamera(const vt::Vec3f& cameraPosition);
	inline float getDistanceToCamera() const { return m_distanceToCamera; }

	unsigned int calculateLOD() const;

	inline bool hasModel() const { return m_wasDone; }

	inline const VoxelData& getVoxelData(unsigned int x, unsigned int y, unsigned int z) const { return m_voxels.getData(x, y, z); }
	const VoxelData& getVoxelDataRelative(int x, int y, int z) const;
	inline VoxelRunLengthContainer<VoxelData, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE>& getVoxelContainer() { return m_voxels; }
	inline void setVoxelData(unsigned int x, unsigned int y, unsigned int z, const VoxelData& data) { m_voxels.setData(x, y, z, data); }

	inline Chunk* getNeighbor(const ChunkNeighbor& neighbor) { return m_neighbors[neighbor]; }
	void setNeighbor(const ChunkNeighbor& neighbor, Chunk* chunk);

	inline const ChunkProgress& getProgress() const { return m_progress; }
	bool shouldProcess();
	inline bool isRunning() const { return m_running; }
	inline void setRunning(bool running) { m_running = running; }
	inline void recreateMesh() { if (m_progress >= CHUNKPROGRESS_MESH) m_recreateMesh = true; }
private:
	ChunkManager* m_chunkManager;
	unsigned int m_id;
	vt::Vec3i m_position;
	float m_distanceToCamera;

	vt::GLIndexedModel m_model;
	vt::IndexedMesh<VoxelVertex, unsigned int> m_mesh;
	vt::GLIndexedModel m_waterModel;
	vt::IndexedMesh<VoxelVertex, unsigned int> m_waterMesh;
	vt::GLIndexedModel m_grassModel;
	vt::IndexedMesh<VoxelVertex, unsigned int> m_grassMesh;

	VoxelRunLengthContainer<VoxelData, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE> m_voxels;

	Chunk* m_neighbors[CHUNKNEIGHBOR_COUNT];
	unsigned int m_numNeighbors;

	ChunkProgress m_progress;
	bool m_running;
	bool m_recreateMesh;
	bool m_wasDone;
};