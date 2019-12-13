/*
##### TODO #####


--- Atmosphere ---

- Day/night cycle
- Moon/stars
- Clouds
- Light scattering
- Weather effects


--- Graphics ---

- Lighting
- Bloom
- Shadow map bias based on size
- Fix T-Junctions in a better way
- Particle system
- Animated leaves
- Water falls
- Decoration
- Make motion blur work with sky
- Make outlines work with sky
- Vignette

--- Sounds ---

- Velocity based sounds
- Dynamic music loading (?)


--- Miscellaneous ---

- Saving
- Entity Component System
- Player/Mob physics
- Additional GUI components
- Voxel editing tools
- Fix block setting and borders
- Client/Server things in Veletrix


--- GUI ---

- Options menu
- Connection menu
- Make quit button work


*/

#include <vt/io/Console.h>
#include <vt/graphics/GraphicsAPI.h>
#include <vt/audio/AudioAPI.h>
#include <vt/audio/ALBuffer.h>
#include <vt/audio/ALListener.h>
#include <vt/graphics/renderer/GLRenderer.h>
#include <vt/util/Timer.h>
#include <vt/script/Compiler.h>

#include "Game.h"

void vt::main()
{
	vt::graphics::initialize(vt::graphics::GRAPHICS_GLFW);
	vt::audio::initialize(vt::audio::AUDIO_OPENAL);

	vt::Window* window = vt::Window::create();
	window->initialize("Voxel Game  |  Development Build", 1280, 720, false, true, vt::VSYNC_DISABLED);

	vt::ALBuffer buffer;
	buffer.initialize();
	buffer.loadWAV("res/sounds/Birds.wav");

	vt::ALListener listener;
	vt::ALSource source;
	source.initialize();
	source.setLooping(true);
	source.play(buffer);

	vt::GLRenderer renderer;
	renderer.setDepthRead(true);
	renderer.setCulling(vt::CULLING_BACK);

	Game game;
	game.initialize();

	vt::Timer timer;
	vt::Timer fpsTimer;
	unsigned int frames = 0;
	unsigned int fps = 0;

	while (!window->isCloseRequested()) {
		float delta = (float)timer.getPassedSeconds();
		timer.reset();

		window->bind();
		renderer.clear(true, true, false);

		game.update(delta, window->getInput());
		
		game.render(delta, window);

		window->update();
		vt::Timer swapTimer;
		window->swap();

		frames++;
		if (fpsTimer.getPassedSeconds() >= 1.0) {
			fpsTimer.reset();
			fps = frames;
			vt::log::info(frames, "fps");
			frames = 0;
		}
	}

	source.cleanup();
	buffer.cleanup();

	game.cleanup();
	window->cleanup();
	vt::Window::destroy(window);
	vt::audio::cleanup();
	vt::graphics::cleanup();
}