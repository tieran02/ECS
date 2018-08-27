#include <iostream>
#include "ECS/ECSManager.hpp"

namespace ECS {
    ECSManager::~ECSManager() {
        freeSystem();
    }


    Entity &ECSManager::MakeEntity() {
        //Create new unque pointer and add it to the entites list
        m_entities.push_back(std::make_unique<Entity>(entityID++));
        //return the newly created entity
        return *m_entities.back();
    }

    void ECSManager::RemoveEntity(Entity &entity) {
        for (int i = 0; i < m_entities.size(); ++i) {
            if (m_entities[i]->ID == entity.ID) {
                m_entities.erase(m_entities.begin() + i);
                return;
            }
        }
    }

    std::shared_ptr<BaseComponent> ECSManager::GetComponentFromID(const Entity &entity, uint64_t id) {
        //check whether entity has any components
        if(entity.components.size() >= 1){
            for(const auto& component : entity.components){
                if(component.get()->id == id)
                    return component;
            }
        }
        return nullptr;
    }

    size_t ECSManager::EntityComponentCount(const Entity &entity) {

        return entity.components.size();
    }

    void ECSManager::freeSystem() {
        m_components.clear();
        m_entities.clear();
        m_systems.clear();
    }

    void ECSManager::UpdateSystems() {
        for (const auto &system : m_systems) {
            std::bitset<64> signature = system.get()->GetSignature();
            for (const auto &entity : m_entities) {
                std::bitset<64> key = entity.get()->GetKey();
                if ((signature & ~key).none()) {
                    std::vector<std::shared_ptr<BaseComponent>> requiredComponents;

                    system.get()->Update((*this), entity.get()->ID, GetRequiredComponentsFromEntity(system.get(), *entity));
                }
            }
        }
    }

    std::vector<std::shared_ptr<BaseComponent>>
    ECSManager::GetRequiredComponentsFromEntity(System *system, const Entity &entity) {
        std::vector<std::shared_ptr<BaseComponent>> requiredComponents;

        for(size_t i = 0; i < system->GetSignature().size(); i++){
            if(system->GetSignature()[i] != 0) {
                uint64_t id = static_cast<uint64_t>(1) << i;
                std::shared_ptr<BaseComponent> component = GetComponentFromID(entity, id);
                if(component != nullptr)
                    requiredComponents.push_back(component);
                else
                    std::cout << "ECS:: Can't find component with an ID of " << id << " in entity " << entity.ID << std::endl;
            }
        }

        return requiredComponents;
    }
}
