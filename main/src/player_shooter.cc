#include <SDL2/SDL.h>
#include <cmath>
#include <iostream>

#include "gun_component.h"
#include "input_component.h"
#include "light_component.h"
#include "player_shooter.h"
#include "render_component.h"
#include "transform_component.h"

PlayerShooter::PlayerShooter(EntityManager& entity_manager)
    : Processor(entity_manager) { }

Requirements PlayerShooter::chooseRequirements() const {
  Requirements requirements = {
    TransformComponent::getTypeId(),
    InputComponent::getTypeId(),
    GunComponent::getTypeId()
  };
  return requirements;
}

void PlayerShooter::update(long ms, const std::set<Entity>& entities) {
  const uint8_t* key_states = SDL_GetKeyboardState(NULL);

  for (const auto& entity : entities) {
    auto transform = entity_manager.getComponent<TransformComponent>(entity);
    auto gun = entity_manager.getComponent<GunComponent>(entity);
    if (key_states[SDL_SCANCODE_LEFT]) {
      transform->transform.rotation.rotate(-0.05f * M_PI / 180.0f);
    }
    if (key_states[SDL_SCANCODE_RIGHT]) {
      transform->transform.rotation.rotate(0.05f * M_PI / 180.0f);
    }

    uint32_t time = SDL_GetTicks();
    uint32_t delay = time - gun->last_shoot_time;
    const uint32_t reload_time = 200;
    if (key_states[SDL_SCANCODE_SPACE] && delay >= reload_time) {
      gun->last_shoot_time = time;
      Transform bullet_transform = gun->nodes.at(gun->last_used_index);
      gun->last_used_index = (gun->last_used_index + 1) % gun->nodes.size();
      createBullet(transform->transform, bullet_transform);
    }
  }
}

void PlayerShooter::createBullet(const Transform& entity_transform,
                                 const Transform& bullet_transform) {
  Entity bullet = entity_manager.createEntity();
  auto light_component =
      entity_manager.addComponent<LightComponent>(bullet);
  SDL_Color colour = { 0xFF, 0xFF, 0x00, 0xFF };
  light_component->setParameters(colour, 75);
  auto transform_component =
      entity_manager.addComponent<TransformComponent>(bullet);
  transform_component->transform.position = entity_transform.position +
      bullet_transform.position;
  transform_component->transform.rotation = entity_transform.rotation.x;
  auto render_component = entity_manager.addComponent<RenderComponent>(bullet);
  render_component->setImage("main/assets/sprites/bullet.png");
}
