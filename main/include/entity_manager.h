#ifndef ENTITY_MANAGER_H_
#define ENTITY_MANAGER_H_
#include <algorithm>
#include <cstdint>
#include <memory>
#include <set>
#include <unordered_map>
#include <utility>
#include <vector>

#include "base_component.h"
#include "entity_manager_util.h"


typedef uint64_t Entity;
using ComponentPtr = std::shared_ptr<BaseComponent>;
using ComponentsMap = std::unordered_map<ComponentId, ComponentPtr>;
using EntityComponentsMap = std::unordered_map<Entity, ComponentsMap>;

using Capabilities = std::set<ComponentId>;
using Requirements = std::set<ComponentId>;

class EntityManager {
 public:
  EntityManager();
  Entity createEntity();
  void deleteEntity(Entity entity);
  std::vector<Entity> getEntities();

  // TODO(jaween): Separate the implementation into a separate file!
  template<class T>
  void addComponent(Entity entity) {
    // Adds component to collection
    ComponentPtr component = std::shared_ptr<T>(new T);
    ComponentId id = T::getTypeId();
    ComponentsMap& map = entity_components_map[entity];
    map.insert(std::pair<ComponentId, ComponentPtr>(id, component));

    // Adds to this entity's capability collection
    Capabilities capabilities = entity_capabilities[entity];
    capabilities.insert(T::getTypeId());

    // Tracks requirements met by an entity and enities which meet requirements
    Capabilities intersection;
    for (const auto& pair : entities_which_match_requirements) {
      Requirements requirements = pair.first;
      std::set_intersection(capabilities.begin(),
                            capabilities.end(),
                            requirements.begin(),
                            requirements.end(),
                            std::inserter(intersection, intersection.begin()));
      if (intersection == requirements) {
        entities_which_match_requirements[requirements].insert(entity);
        requirements_matched_by_entities[entity].insert(requirements);
      }
    }
  }

  template<class T>
  void removeComponent(Entity entity) {
    // Removes the component from this entity
    ComponentsMap& map = entity_components_map[entity];
    map.erase(T::getTypeId());

    // The entity now has fewer capabilities
    entity_capabilities[entity].erase(T::getTypeId());

    // Removes requirements which no longer match this entity's capabilities
    std::multiset<Requirements> requirements_set =
        requirements_matched_by_entities[entity];
    for (const auto& r : requirements_set) {
      if (r.find(T::getTypeId()) != r.end()) {
        requirements_matched_by_entities[entity].erase(r);

        // Inversely, the entity no loger matches this requirements
        entities_which_match_requirements[r].erase(entity);
       }
    }
  }

  // TODO(jaween): Separate the implementation into a separate file!
  template<class T>
  std::vector<ComponentPtr> getComponents(Entity entity) {
    ComponentsMap map = entity_components_map[entity];

    std::vector<ComponentPtr> output;
    std::transform(map.begin(), map.end(), std::back_inserter(output),
        [](std::pair<ComponentId, ComponentPtr> pair) {
          return pair.second;
        });
    return output;
  }

 private:
  uint64_t next_entity_id;
  std::set<Entity> entities;
  EntityComponentsMap entity_components_map;

  std::unordered_map<Requirements, std::multiset<Entity>, SetHasher>
      entities_which_match_requirements;
  std::unordered_map<Entity, Capabilities> entity_capabilities;
  std::unordered_map<Entity, std::multiset<Requirements>>
      requirements_matched_by_entities;
};

#endif // ENTITY_MANAGER_H_
