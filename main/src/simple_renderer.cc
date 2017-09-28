#include "SDL_gpu.h"
#include <cmath>

#include "light_component.h"
#include "simple_renderer.h"
#include "render_component.h"
#include "transform_component.h"
#include "vector.h"

SimpleRenderer::SimpleRenderer(EntityManager& entity_manager)
    : Renderer(entity_manager) {
  // No implementation
}

Requirements SimpleRenderer::chooseRequirements() const {
  Requirements requirements = {
    TransformComponent::getTypeId(),
    RenderComponent::getTypeId(),
    LightComponent::getTypeId()
  };
  return requirements;
}

void SimpleRenderer::update(long ms, const std::set<Entity>& entities) {
  // No implementation
}

void SimpleRenderer::render(long ms, const std::set<Entity>& entities,
                            GPU_Target* gpu_target) {
  for (const auto& entity : entities) {
    auto transform = entity_manager.getComponent<TransformComponent>(entity);
    auto render = entity_manager.getComponent<RenderComponent>(entity);
    auto light = entity_manager.getComponent<LightComponent>(entity);
    render->getImage()->draw(
        gpu_target,
        transform->transform.position.x,
        transform->transform.position.y,
        transform->transform.rotation.angle() * 180 / M_PI);
    transform->transform.rotation.rotate(0.01f * M_PI / 180.0f);

    light->light->draw(gpu_target, transform->transform.position);
  }
}
