#include "Chunk.h"

#include "ChunkMesher.h"
#include "../World.h"
#include "../../util/Noise.h"

Chunk::Chunk(ChunkManager* chunkManager, const vt::Vec3i& position) : m_chunkManager(chunkManager), m_position(position), m_distanceToCamera(0.0f), m_model(), m_mesh(), m_waterModel(), m_waterMesh(), m_voxels(), m_neighbors{ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr }, m_numNeighbors(0), m_progress(CHUNKPROGRESS_PRE_NEIGHBOR), m_running(false), m_recreateMesh(false), m_wasDone(false)
{
}

Chunk::~Chunk()
{
}

void Chunk::initialize()
{
	m_mesh.addAttribute(vt::Attribute(vt::TYPE_FLOAT, 3));
	m_mesh.addAttribute(vt::Attribute(vt::TYPE_UNSIGNED_BYTE, 4, true));
	m_mesh.addAttribute(vt::Attribute(vt::TYPE_FLOAT, 3));

	m_waterMesh.addAttribute(vt::Attribute(vt::TYPE_FLOAT, 3));
	m_waterMesh.addAttribute(vt::Attribute(vt::TYPE_UNSIGNED_BYTE, 4, true));
	m_waterMesh.addAttribute(vt::Attribute(vt::TYPE_FLOAT, 3));
	
	m_grassMesh.addAttribute(vt::Attribute(vt::TYPE_FLOAT, 3));
	m_grassMesh.addAttribute(vt::Attribute(vt::TYPE_UNSIGNED_BYTE, 4, true));
	m_grassMesh.addAttribute(vt::Attribute(vt::TYPE_FLOAT, 3));

	Chunk* neighbor;
	if ((neighbor = m_chunkManager->getChunk(m_position + vt::Vec3i(0, 1, 0))) != nullptr) { setNeighbor(CHUNKNEIGHBOR_TOP, neighbor); neighbor->setNeighbor(CHUNKNEIGHBOR_BOTTOM, this); }
	if ((neighbor = m_chunkManager->getChunk(m_position + vt::Vec3i(0, -1, 0))) != nullptr) { setNeighbor(CHUNKNEIGHBOR_BOTTOM, neighbor); neighbor->setNeighbor(CHUNKNEIGHBOR_TOP, this); }
	if ((neighbor = m_chunkManager->getChunk(m_position + vt::Vec3i(-1, 0, 0))) != nullptr) { setNeighbor(CHUNKNEIGHBOR_LEFT, neighbor); neighbor->setNeighbor(CHUNKNEIGHBOR_RIGHT, this); }
	if ((neighbor = m_chunkManager->getChunk(m_position + vt::Vec3i(1, 0, 0))) != nullptr) { setNeighbor(CHUNKNEIGHBOR_RIGHT, neighbor); neighbor->setNeighbor(CHUNKNEIGHBOR_LEFT, this); }
	if ((neighbor = m_chunkManager->getChunk(m_position + vt::Vec3i(0, 0, 1))) != nullptr) { setNeighbor(CHUNKNEIGHBOR_FRONT, neighbor); neighbor->setNeighbor(CHUNKNEIGHBOR_BACK, this); }
	if ((neighbor = m_chunkManager->getChunk(m_position + vt::Vec3i(0, 0, -1))) != nullptr) { setNeighbor(CHUNKNEIGHBOR_BACK, neighbor); neighbor->setNeighbor(CHUNKNEIGHBOR_FRONT, this); }
}

void Chunk::cleanup()
{
	Chunk* neighbor;
	if ((neighbor = m_neighbors[CHUNKNEIGHBOR_TOP]) != nullptr) { neighbor->setNeighbor(CHUNKNEIGHBOR_BOTTOM, nullptr); }
	if ((neighbor = m_neighbors[CHUNKNEIGHBOR_BOTTOM]) != nullptr) { neighbor->setNeighbor(CHUNKNEIGHBOR_TOP, nullptr); }
	if ((neighbor = m_neighbors[CHUNKNEIGHBOR_LEFT]) != nullptr) { neighbor->setNeighbor(CHUNKNEIGHBOR_RIGHT, nullptr); }
	if ((neighbor = m_neighbors[CHUNKNEIGHBOR_RIGHT]) != nullptr) { neighbor->setNeighbor(CHUNKNEIGHBOR_LEFT, nullptr); }
	if ((neighbor = m_neighbors[CHUNKNEIGHBOR_FRONT]) != nullptr) { neighbor->setNeighbor(CHUNKNEIGHBOR_BACK, nullptr); }
	if ((neighbor = m_neighbors[CHUNKNEIGHBOR_BACK]) != nullptr) { neighbor->setNeighbor(CHUNKNEIGHBOR_FRONT, nullptr); }

	if (m_model.isInitialized()) {
		m_model.cleanup();
	}
	if (m_waterModel.isInitialized()) {
		m_waterModel.cleanup();
	}
	if (m_grassModel.isInitialized()) {
		m_grassModel.cleanup();
	}
}

void Chunk::update()
{
	if (m_progress == CHUNKPROGRESS_MODEL) {
		process();
	}
	if (m_progress != CHUNKPROGRESS_PRE_NEIGHBOR && m_numNeighbors != CHUNKNEIGHBOR_COUNT) {
		if (m_neighbors[CHUNKNEIGHBOR_TOP] == nullptr) { m_chunkManager->generateChunk(m_position + vt::Vec3i(0, 1, 0)); }
		if (m_neighbors[CHUNKNEIGHBOR_BOTTOM] == nullptr) { m_chunkManager->generateChunk(m_position + vt::Vec3i(0, -1, 0)); }
		if (m_neighbors[CHUNKNEIGHBOR_LEFT] == nullptr) { m_chunkManager->generateChunk(m_position + vt::Vec3i(-1, 0, 0)); }
		if (m_neighbors[CHUNKNEIGHBOR_RIGHT] == nullptr) { m_chunkManager->generateChunk(m_position + vt::Vec3i(1, 0, 0)); }
		if (m_neighbors[CHUNKNEIGHBOR_FRONT] == nullptr) { m_chunkManager->generateChunk(m_position + vt::Vec3i(0, 0, 1)); }
		if (m_neighbors[CHUNKNEIGHBOR_BACK] == nullptr) { m_chunkManager->generateChunk(m_position + vt::Vec3i(0, 0, -1)); }
	}
	if (m_recreateMesh && m_progress == CHUNKPROGRESS_DONE) {
		m_progress = CHUNKPROGRESS_MESH;
		m_recreateMesh = false;
	}
}

void Chunk::render()
{
	if (m_model.isInitialized()) {
		m_model.render();
	}
}

void Chunk::renderWater()
{
	if (m_waterModel.isInitialized()) {
		m_waterModel.render();
	}
}

void Chunk::renderGrass()
{
	if (m_grassModel.isInitialized()) {
		m_grassModel.render();
	}
}

void Chunk::process()
{
	switch (m_progress) {
	case CHUNKPROGRESS_PRE_NEIGHBOR: preNeighborProcess(); break;
	case CHUNKPROGRESS_POST_NEIGHBOR: postNeighborProcess(); break;
	case CHUNKPROGRESS_MESH: createMesh(); break;
	case CHUNKPROGRESS_MODEL: createModel(); break;
	case CHUNKPROGRESS_DONE: break;
	}

	m_running = false;
}

void Chunk::preNeighborProcess()
{
	m_chunkManager->getChunkGenerator().generateBaseTerrain(*this);
	m_progress = CHUNKPROGRESS_POST_NEIGHBOR;
}

void Chunk::postNeighborProcess()
{
	m_progress = CHUNKPROGRESS_MESH;
}

void Chunk::createMesh()
{
	ChunkMesher::createMesh(*this, m_mesh, m_waterMesh, m_grassMesh);
	m_progress = CHUNKPROGRESS_MODEL;
}

void Chunk::createModel()
{
	if (!m_mesh.isEmpty()) {
		if (!m_model.isInitialized()) {
			m_model.initialize();
		}
		m_mesh.createGLModel(m_model);
		m_mesh.clear();
	}
	else {
		if (m_model.isInitialized()) {
			m_model.cleanup();
		}
	}
	if (!m_waterMesh.isEmpty()) {
		if (!m_waterModel.isInitialized()) {
			m_waterModel.initialize();
		}
		m_waterMesh.createGLModel(m_waterModel);
		m_waterMesh.clear();
	}
	else {
		if (m_waterModel.isInitialized()) {
			m_waterModel.cleanup();
		}
	}
	if (!m_grassMesh.isEmpty()) {
		if (!m_grassModel.isInitialized()) {
			m_grassModel.initialize();
		}
		m_grassMesh.createGLModel(m_grassModel);
		m_grassMesh.clear();
	}
	else {
		if (m_grassModel.isInitialized()) {
			m_grassModel.cleanup();
		}
	}
	m_progress = CHUNKPROGRESS_DONE;
	m_wasDone = true;
}

void Chunk::calculateDistanceToCamera(const vt::Vec3f& cameraPosition)
{
	m_distanceToCamera = (cameraPosition - (((vt::Vec3f)m_position + 0.5f) * (float)CHUNK_SIZE)).getLength();
}

const VoxelData& Chunk::getVoxelDataRelative(int x, int y, int z) const
{
	if (x < 0) return m_neighbors[CHUNKNEIGHBOR_LEFT]->getVoxelDataRelative(x + CHUNK_SIZE, y, z);
	if (y < 0) return m_neighbors[CHUNKNEIGHBOR_BOTTOM]->getVoxelDataRelative(x, y + CHUNK_SIZE, z);
	if (z < 0) return m_neighbors[CHUNKNEIGHBOR_BACK]->getVoxelDataRelative(x, y, z + CHUNK_SIZE);
	if (x >= CHUNK_SIZE) return m_neighbors[CHUNKNEIGHBOR_RIGHT]->getVoxelDataRelative(x - CHUNK_SIZE, y, z);
	if (y >= CHUNK_SIZE) return m_neighbors[CHUNKNEIGHBOR_TOP]->getVoxelDataRelative(x, y - CHUNK_SIZE, z);
	if (z >= CHUNK_SIZE) return m_neighbors[CHUNKNEIGHBOR_FRONT]->getVoxelDataRelative(x, y, z - CHUNK_SIZE);
	return getVoxelData((unsigned int)x, (unsigned int)y, (unsigned int)z);
}

void Chunk::setNeighbor(const ChunkNeighbor& neighbor, Chunk* chunk)
{
	if (m_neighbors[neighbor] != chunk) {
		m_neighbors[neighbor] = chunk;
		m_numNeighbors += chunk == nullptr ? -1 : 1;
	}
}

bool Chunk::shouldProcess()
{
	if (m_running) {
		return false;
	}
	switch (m_progress) {
	case CHUNKPROGRESS_PRE_NEIGHBOR: return true; break;
	case CHUNKPROGRESS_POST_NEIGHBOR: return m_chunkManager->isChunkSurrounded(m_position, CHUNKPROGRESS_PRE_NEIGHBOR); break;
	case CHUNKPROGRESS_MESH: return m_chunkManager->isChunkSurrounded(m_position, CHUNKPROGRESS_PRE_NEIGHBOR); break;
	case CHUNKPROGRESS_MODEL: return false; break;
	case CHUNKPROGRESS_DONE: return false; break;
	}
	return false;
}
