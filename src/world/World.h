#pragma once

#include <vt/util/Camera.h>
#include <vt/graphics/texture/GLFramebuffer.h>
#include <vt/graphics/model/GLIndexedModel.h>

#include "chunk/ChunkManager.h"
#include "../graphics/lighting/LightManager.h"
#include "../graphics/sky/Atmosphere.h"
#include "../graphics/sky/Stars.h"
#include "../graphics/antialiasing/MSAA.h"

class Game;

namespace vt {
	class Window;
}

class World
{
public:
	World(Game* game);
	~World();

	void initialize();
	void cleanup();

	void update(float delta, const vt::Input& input);
	void render(float delta, const vt::Window* window);

	inline const vt::Vec3f& getCameraPosition() const { return m_camera.position; }
	inline unsigned int getRenderDistance() const { return 14; }
public:
	Game* m_game;

	ChunkManager m_chunkManager;
	float m_time;
	vt::Camera m_camera;
	vt::GLFramebuffer m_sceneBuffer;
	vt::GLShaderProgram m_voxelShader;
	vt::GLShaderProgram m_postProcessingShader;
	Atmosphere m_atmosphere;
	Stars m_stars;
	LightManager m_lightManager;
	MSAA m_msaa;
	vt::Mat4f m_previousViewProjectionMatrix;
	float m_fogDistance;

	vt::GLIndexedModel m_testModel;
};