#include "GL/glew.h"
#include <iostream>

#include "engine.h"
#include "room.h"

const int Engine::kScreenWidth = 640;
const int Engine::kScreenHeight = 480;

Engine::Engine() {
  std::clog << "Engine started" << std::endl;
}

void Engine::run(Room& room) {
  if (!init()) {
    return;
  }

  room.init();

  bool keepRunning = true;
  while (keepRunning) {
    // Input handling
    SDL_Event event;
    while (SDL_PollEvent(&event) > 0) {
      switch (event.type) {
        case SDL_QUIT:
          keepRunning = false;
          break;
      }
    }

    room.update();
    for (auto& pair : processors) {
      const long temp_update_ms = 16;
      pair.first->update(
          temp_update_ms,
          entity_manager.getEntities(pair.second));
    }

    room.draw(window);
    GPU_Flip(window);
    SDL_Delay(10);
  }

  finish();
}

EntityManager& Engine::getEntityManager() {
  return entity_manager;
}

bool Engine::init() {
  GPU_SetDebugLevel(GPU_DEBUG_LEVEL_MAX);

  window = GPU_Init(kScreenWidth, kScreenHeight, GPU_DEFAULT_INIT_FLAGS);
  if (window == NULL) {
    std::cerr << "Failed to initialise SDL_gpu: " << std::endl;
    return false;
  }

  // Initialises GLEW (loads OpenGL extension functions that we can then call)
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    GPU_LogError("Failed to initialise GLEW\n");
    return false;
  }

  return true;
}

void Engine::finish() {
  GPU_Quit();

  std::clog << "Engine ended" << std::endl;
}
