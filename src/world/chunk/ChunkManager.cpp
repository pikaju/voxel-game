#include "ChunkManager.h"

#include <algorithm>
#include <vt/io/ImageLoader.h>

#include "Chunk.h"
#include "../World.h"
#include "../../util/FrustumCuller.h"

ChunkManager::ChunkManager(World* world) : m_world(world), m_chunkVector(), m_chunkProcessingVector(), m_chunkMap(), m_threadPool(), m_chunkGenerator(0), m_waterShader(), m_grassShader(), m_maxRenderDistance(0.0f)
{
}

ChunkManager::~ChunkManager()
{
}

void ChunkManager::initialize()
{
	m_threadPool.start(7);

	m_waterShader.initialize();
	m_waterShader.loadShader("res/shaders/Water.vert", vt::SHADERTYPE_VERTEX);
	m_waterShader.loadShader("res/shaders/Water.frag", vt::SHADERTYPE_FRAGMENT);
	m_waterShader.link();

	m_grassShader.initialize();
	m_grassShader.loadShader("res/shaders/Grass.vert", vt::SHADERTYPE_VERTEX);
	m_grassShader.loadShader("res/shaders/Grass.frag", vt::SHADERTYPE_FRAGMENT);
	m_grassShader.link();

	m_waterTexture = vt::io::loadGLTexture("res/textures/Water.png", vt::FILTER_LINEAR);
}

void ChunkManager::cleanup()
{
	m_grassShader.cleanup();
	m_waterTexture->cleanup();
	delete m_waterTexture;
	m_waterShader.cleanup();
	
	m_threadPool.stop();

	for (unsigned int i = 0; i < m_chunkVector.size(); i++) {
		m_chunkVector[i]->cleanup();
		delete m_chunkVector[i];
	}
	m_chunkVector.clear();
	m_chunkMap.clear();
}

void ChunkManager::update()
{
	vt::Vec3i cameraChunkPosition = (vt::Vec3i)(m_world->getCameraPosition() / (float)CHUNK_SIZE);
	Chunk* cameraChunk = getChunk(cameraChunkPosition);
	if (cameraChunk == nullptr) {
		generateChunk(cameraChunkPosition);
	}

	m_maxRenderDistance = (float)(m_world->getRenderDistance() * CHUNK_SIZE);
	const float distanceOffset = (float)sqrt(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE / 8);
	for (unsigned int i = 0; i < m_chunkVector.size(); i++) {
		Chunk* chunk = m_chunkVector[i];
		chunk->calculateDistanceToCamera(m_world->getCameraPosition());
		chunk->update();
		if (chunk->getDistanceToCamera() - distanceOffset < m_maxRenderDistance && !chunk->hasModel()) {
			m_maxRenderDistance = chunk->getDistanceToCamera() - distanceOffset;
		}
		if (chunk->shouldProcess()) {
			m_chunkProcessingVector.push_back(chunk);
		}
	}
	if (m_maxRenderDistance < 0.0f) {
		m_maxRenderDistance = 0.0f;
	}

	std::sort(m_chunkProcessingVector.begin(), m_chunkProcessingVector.end(), sortChunks);

	for (unsigned int i = 0; i < m_chunkProcessingVector.size(); i++) {
		if (m_threadPool.isSaturated()) {
			break;
		}

		Chunk* chunk = m_chunkProcessingVector[i];
		chunk->setRunning(true);
		m_threadPool.addTask(vt::ThreadTask(processChunk, (void*)chunk));
	}

	m_chunkProcessingVector.clear();

	for (unsigned int i = 0; i < m_chunkVector.size(); i++) {
		Chunk* chunk = m_chunkVector[i];
		if (m_chunkVector[i]->getDistanceToCamera() > (m_world->getRenderDistance() + 1) * CHUNK_SIZE) {
			for (int x = chunk->getPosition().x - 1; x <= chunk->getPosition().x + 1; x++) {
				for (int y = chunk->getPosition().y - 1; y <= chunk->getPosition().y + 1; y++) {
					for (int z = chunk->getPosition().z - 1; z <= chunk->getPosition().z + 1; z++) {
						Chunk* c = getChunk(vt::Vec3i(x, y, z));
						if (c != nullptr && c->isRunning()) {
							goto skip;
						}
					}
				}
			}
			removeChunk(chunk);
			i--;
		skip:
			continue;
		}
	}
}

void ChunkManager::render(const vt::GLShaderProgram shader, const vt::Mat4f& viewMatrix, const vt::Mat4f& projectionMatrix)
{
	vt::Mat4f viewProjectionMatrix = projectionMatrix * viewMatrix;
	FrustumCuller culler;
	culler.extract(viewProjectionMatrix);

	shader.enable();
	shader.setUniform("viewProjectionMatrix", viewProjectionMatrix);
	for (unsigned int i = 0; i < m_chunkVector.size(); i++) {
		vt::Vec3f translation = (vt::Vec3f)(m_chunkVector[i]->getPosition() * CHUNK_SIZE);
		if (culler.sphereInFrustum(translation + vt::Vec3f((float)CHUNK_SIZE / 2.0f), (float)sqrt(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE / 8))) {
			m_chunkVector[i]->render();
		}
	}
	shader.disable();
}

void ChunkManager::renderWater(const vt::Mat4f& viewMatrix, const vt::Mat4f& projectionMatrix, float time)
{
	vt::Mat4f viewProjectionMatrix = projectionMatrix * viewMatrix;
	FrustumCuller culler;
	culler.extract(viewProjectionMatrix);

	m_waterTexture->enable(0);

	m_waterShader.enable();
	m_waterShader.setUniform("waterTexture", 0);
	m_waterShader.setUniform("viewProjectionMatrix", viewProjectionMatrix);
	m_waterShader.setUniform("time", time);
	for (unsigned int i = 0; i < m_chunkVector.size(); i++) {
		vt::Vec3f translation = (vt::Vec3f)(m_chunkVector[i]->getPosition() * CHUNK_SIZE);
		if (culler.sphereInFrustum(translation + vt::Vec3f((float)CHUNK_SIZE / 2.0f), (float)sqrt(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE / 8))) {
			m_chunkVector[i]->renderWater();
		}
	}
	m_waterShader.disable();
}

void ChunkManager::renderGrass(const vt::Mat4f& viewMatrix, const vt::Mat4f& projectionMatrix, float time)
{
	vt::Mat4f viewProjectionMatrix = projectionMatrix * viewMatrix;
	FrustumCuller culler;
	culler.extract(viewProjectionMatrix);

	m_grassShader.enable();
	m_grassShader.setUniform("viewProjectionMatrix", viewProjectionMatrix);
	m_grassShader.setUniform("time", time);
	for (unsigned int i = 0; i < m_chunkVector.size(); i++) {
		vt::Vec3f translation = (vt::Vec3f)(m_chunkVector[i]->getPosition() * CHUNK_SIZE);
		if (culler.sphereInFrustum(translation + vt::Vec3f((float)CHUNK_SIZE / 2.0f), (float)sqrt(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE / 8))) {
			m_chunkVector[i]->renderGrass();
		}
	}
	m_grassShader.disable();
}

void ChunkManager::addChunk(Chunk* chunk)
{
	chunk->setID(m_chunkVector.size());
	m_chunkVector.push_back(chunk);
	m_chunkMap[chunk->getPosition()] = chunk;
	chunk->initialize();
}

Chunk* ChunkManager::getChunk(const vt::Vec3i& position)
{
	auto& it = m_chunkMap.find(position);
	if (it == m_chunkMap.end()) {
		return nullptr;
	}
	return it->second;
}

void ChunkManager::removeChunk(Chunk* chunk)
{
	m_chunkVector[chunk->getID()] = m_chunkVector.back();
	m_chunkVector[chunk->getID()]->setID(chunk->getID());
	m_chunkVector.pop_back();
	m_chunkMap.erase(chunk->getPosition());
	chunk->cleanup();
	delete chunk;
}

void ChunkManager::generateChunk(const vt::Vec3i& position)
{
	vt::Vec3f worldPosition = ((vt::Vec3f)position + 0.5) * (float)CHUNK_SIZE;
	float distanceSquared = (m_world->getCameraPosition() - worldPosition).getLengthSquared();
	float generationDistance = (float)((m_world->getRenderDistance() + 1) * CHUNK_SIZE);
	if (distanceSquared > generationDistance * generationDistance) {
		return;
	}
	addChunk(new Chunk(this, position));
}

bool ChunkManager::isChunkSurrounded(const vt::Vec3i& position, const ChunkProgress& minimumProgress)
{
	for (int x = position.x - 1; x <= position.x + 1; x++) {
		for (int y = position.y - 1; y <= position.y + 1; y++) {
			for (int z = position.z - 1; z <= position.z + 1; z++) {
				if (x == position.x && y == position.y && z == position.z) {
					continue;
				}
				Chunk* chunk = getChunk(vt::Vec3i(x, y, z));
				if (chunk == nullptr || chunk->getProgress() <= minimumProgress) {
					return false;
				}
			}
		}
	}
	return true;
}

void ChunkManager::setVoxelData(int x, int y, int z, const VoxelData& data)
{
	Chunk* chunk = getChunk(vt::Vec3i((int)floor((float)x / (float)CHUNK_SIZE), (int)floor((float)y / (float)CHUNK_SIZE), (int)floor((float)z / (float)CHUNK_SIZE)));
	chunk->setVoxelData(x - chunk->getPosition().x * CHUNK_SIZE, y - chunk->getPosition().y * CHUNK_SIZE, z - chunk->getPosition().z * CHUNK_SIZE, data);
	chunk->recreateMesh();
}

bool ChunkManager::pickVoxel(vt::Vec3f position, const vt::Vec3f& direction, float maxDistance, vt::Vec3i& result)
{
	float distance = 0.0f;
	float step = 0.125f;
	Chunk* chunk = getChunk(vt::Vec3i((int)floor(position.x / (float)CHUNK_SIZE), (int)floor(position.y / (float)CHUNK_SIZE), (int)floor(position.z / (float)CHUNK_SIZE)));
	if (chunk == nullptr) {
		return false;
	}
	position -= (vt::Vec3f)(chunk->getPosition() * CHUNK_SIZE);
	while (distance <= maxDistance) {
		position += direction * step;
		distance += step;
		if (position.x < 0.0f) { chunk = chunk->getNeighbor(CHUNKNEIGHBOR_LEFT); position.x += (float)CHUNK_SIZE; }
		if (position.y < 0.0f) { chunk = chunk->getNeighbor(CHUNKNEIGHBOR_BOTTOM); position.y += (float)CHUNK_SIZE; }
		if (position.z < 0.0f) { chunk = chunk->getNeighbor(CHUNKNEIGHBOR_BACK); position.z += (float)CHUNK_SIZE; }
		if (position.x >= (float)CHUNK_SIZE) { chunk = chunk->getNeighbor(CHUNKNEIGHBOR_RIGHT); position.x -= (float)CHUNK_SIZE; }
		if (position.y >= (float)CHUNK_SIZE) { chunk = chunk->getNeighbor(CHUNKNEIGHBOR_TOP); position.y -= (float)CHUNK_SIZE; }
		if (position.z >= (float)CHUNK_SIZE) { chunk = chunk->getNeighbor(CHUNKNEIGHBOR_FRONT); position.z -= (float)CHUNK_SIZE; }
		if (chunk == nullptr) {
			return false;
		}
		if (chunk->getVoxelData((unsigned int)position.x, (unsigned int)position.y, (unsigned int)position.z).getID() != 0) {
			result = (vt::Vec3i)position + chunk->getPosition() * CHUNK_SIZE;
			return true;
		}
	}
	return false;
}

bool ChunkManager::sortChunks(Chunk* c0, Chunk* c1)
{
	return c0->getDistanceToCamera() < c1->getDistanceToCamera();
}

void ChunkManager::processChunk(void* c)
{
	Chunk* chunk = (Chunk*)c;
	chunk->process();
}
