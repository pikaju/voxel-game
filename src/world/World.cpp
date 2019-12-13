#include "World.h"

#include <vt/graphics/window/Window.h>
#include <vt/graphics/renderer/GLAttributelessRenderer.h>

#include "chunk/Chunk.h"

#include "../graphics/model/ModelLoader.h"
#include "chunk/generation/structure/TreeGenerator.h"

World::World(Game* game) : m_game(game), m_chunkManager(this), m_time(0.0f), m_camera(), m_sceneBuffer(), m_voxelShader(), m_postProcessingShader(), m_lightManager(), m_atmosphere(), m_stars(), m_msaa(), m_previousViewProjectionMatrix(vt::Mat4f::identity()), m_fogDistance(0.0f)
{
	m_camera.position = vt::Vec3f(50.0f, 0.0f, 0.0f);
}

World::~World()
{
}

void World::initialize()
{
	m_voxelShader.initialize();
	m_voxelShader.loadShader("res/shaders/Voxel.vert", vt::SHADERTYPE_VERTEX);
	m_voxelShader.loadShader("res/shaders/Voxel.frag", vt::SHADERTYPE_FRAGMENT);
	m_voxelShader.link();

	m_postProcessingShader.initialize();
	m_postProcessingShader.loadShader("res/shaders/PostProcessing.vert", vt::SHADERTYPE_VERTEX);
	m_postProcessingShader.loadShader("res/shaders/PostProcessing.frag", vt::SHADERTYPE_FRAGMENT);
	m_postProcessingShader.link();

	m_lightManager.initialize();
	m_atmosphere.initialize();
	m_stars.initialize();

	m_msaa.initialize();

	m_chunkManager.initialize();

	m_testModel = ModelLoader::loadVOXModel("res/models/Radiant Tower Temple.vox");
}

void World::cleanup()
{
	m_sceneBuffer.cleanup();
	m_chunkManager.cleanup();
	m_stars.cleanup();
	m_atmosphere.cleanup();
	m_msaa.cleanup();
	m_lightManager.cleanup();
	m_postProcessingShader.cleanup();
	m_voxelShader.cleanup();
}

void World::update(float delta, const vt::Input& input)
{
	m_chunkManager.update();
	m_fogDistance = m_chunkManager.getMaxRenderDistance() - pow(0.25f, delta) * (m_chunkManager.getMaxRenderDistance() - m_fogDistance);
	m_camera.debugUpdate(4.0f, delta, input);
	if (input.getButtonState(input.MOUSE_BUTTON_LEFT)) {
		vt::Vec3i result;
		if (m_chunkManager.pickVoxel(m_camera.position, m_camera.getViewDirection(), 256.0f, result)) {
			for (int x = result.x - 4; x <= result.x + 4; x++) {
				for (int y = result.y - 4; y <= result.y + 4; y++) {
					for (int z = result.z - 4; z <= result.z + 4; z++) {
						m_chunkManager.setVoxelData(x, y, z, VoxelData(0, 0));
					}
				}
			}
		}
	}
	if (input.getKeyEvent(input.KEY_T, true)) {
		vt::Vec3i result;
		if (m_chunkManager.pickVoxel(m_camera.position, m_camera.getViewDirection(), 256.0f, result)) {
			TreeGenerator::generateTree(m_chunkManager, result.x, result.y, result.z);
		}
	}
	if (input.getKeyState(input.KEY_Q)) {
		m_lightManager.setDirectionalLightDirection(m_camera.getViewDirection());
	}
	m_time += delta;
}

void World::render(float delta, const vt::Window* window)
{
	if ((window->getWidth() != m_sceneBuffer.getWidth() || window->getHeight() != m_sceneBuffer.getHeight()) && (window->getWidth() != 0 && window->getHeight() != 0)) {
		std::vector<vt::Attachment> attachments;
		attachments.push_back(vt::ATTACHMENT_COLOR);
		attachments.push_back(vt::ATTACHMENT_COLOR);
		attachments.push_back(vt::ATTACHMENT_COLOR);
		attachments.push_back(vt::ATTACHMENT_COLOR);
		attachments.push_back(vt::ATTACHMENT_DEPTH);
		std::vector<vt::Format> formats;
		formats.push_back(vt::FORMAT_RGBA8);
		formats.push_back(vt::FORMAT_RGB32F);
		formats.push_back(vt::FORMAT_RGB8);
		formats.push_back(vt::FORMAT_RGB8);
		formats.push_back(vt::FORMAT_DEPTH24);
		m_sceneBuffer.cleanup();
		m_sceneBuffer.initialize(window->getWidth(), window->getHeight(), attachments.size(), attachments.data(), formats.data());
		m_msaa.setScreenSize(window->getWidth(), window->getHeight(), attachments, formats);
		m_lightManager.setScreenSize(window->getWidth(), window->getHeight());
	}
	
	vt::GLAttributelessRenderer renderer;

	vt::Mat4f viewMatrix = m_camera.getViewMatrix();
	vt::Mat4f projectionMatrix = vt::Mat4f::perspective(window->getAspectRatio(), 90.0f, 0.125f, (float)(getRenderDistance() * CHUNK_SIZE));
	vt::Mat4f inverseViewProjectionMatrix = vt::Mat4f::inverse(viewMatrix) * vt::Mat4f::inverse(projectionMatrix);

	m_msaa.bind();
	renderer.clear(true, true, false);
	renderer.setWireframe(window->getInput().getKeyState(window->getInput().KEY_1));
	m_chunkManager.render(m_voxelShader, viewMatrix, projectionMatrix);
	renderer.setCulling(vt::CULLING_DISABLED);
	m_chunkManager.renderGrass(viewMatrix, projectionMatrix, m_time);
	m_voxelShader.enable();
	m_testModel.render();
	renderer.setBlending(vt::BLENDING_DEFAULT);
	m_chunkManager.renderWater(viewMatrix, projectionMatrix, m_time);
	renderer.setCulling(vt::CULLING_BACK);
	renderer.setBlending(vt::BLENDING_DISABLED);
	renderer.setWireframe(false);

	m_msaa.blit(m_sceneBuffer);

	m_lightManager.render(m_chunkManager, m_voxelShader, m_camera.position, (float)(getRenderDistance() * CHUNK_SIZE), m_sceneBuffer, 1, 2, 3);
	
	window->bind();
	renderer.setDepthRead(false);
	m_atmosphere.render(inverseViewProjectionMatrix, m_camera.position, m_lightManager.getDirectionalLightDirection() * -1.0f);
	m_stars.render(projectionMatrix * viewMatrix, m_camera.position);

	renderer.setBlending(vt::BLENDING_DEFAULT);
	m_sceneBuffer.use(0, 0);
	m_sceneBuffer.use(1, 1);
	m_sceneBuffer.use(2, 2);
	m_sceneBuffer.use(4, 3);
	m_lightManager.getFramebuffer().use(0, 4);

	m_postProcessingShader.enable();
	m_postProcessingShader.setUniform("inverseViewProjectionMatrix", inverseViewProjectionMatrix);
	m_postProcessingShader.setUniform("previousViewProjectionMatrix", m_previousViewProjectionMatrix);
	m_postProcessingShader.setUniform("colorTexture", 0);
	m_postProcessingShader.setUniform("positionTexture", 1);
	m_postProcessingShader.setUniform("normalTexture", 2);
	m_postProcessingShader.setUniform("depthTexture", 3);
	m_postProcessingShader.setUniform("lightTexture", 4);
	m_postProcessingShader.setUniform("cameraPosition", m_camera.position);
	m_postProcessingShader.setUniform("fogDistance", m_fogDistance);
	m_postProcessingShader.setUniform("delta", delta);
	renderer.render(vt::PRIMITIVE_TRIANGLE_STRIP, 4);
	m_postProcessingShader.disable();
	renderer.setBlending(vt::BLENDING_DISABLED);

	renderer.setDepthRead(true);

	m_previousViewProjectionMatrix = projectionMatrix * viewMatrix;
}
