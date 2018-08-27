#ifndef ECS_LIB_COMPONENTMANAGER_HPP
#define ECS_LIB_COMPONENTMANAGER_HPP

#include <cstdint>
#include <unordered_map>
#include <vector>
#include <tuple>
#include <memory>
#include <iostream>
#include "Component.hpp"
#include "Entity.hpp"
#include "System.hpp"

namespace ECS {

    class ECSManager {
        typedef std::tuple<Entity, std::shared_ptr<BaseComponent>> entityComponent;
    public:
        ECSManager() = default;
        ~ECSManager();

        /// <summary> Makes a new entity within the ECS manager </summary>
        /// <param name="entity"> A reference to the entity to create using the ECS system </param>
        /// <returns> A reference to the newly created entity </returns>
        Entity &MakeEntity();

        /// <summary> Remove an entity from the ECS manager, </summary>
        /// <param name="entity"> A reference to the entity to remove in the ECS system </param>
        void RemoveEntity(Entity &entity);

        /// <summary> Get the count of components an entity contains </summary>
        /// <param name="entity"> A reference to the entity to get component from </param>
        /// <param name="id"> The component ID of the component to receive </param>
        /// <returns> A shared pointer of the given component (returns nullptr if not found)</returns>
        std::shared_ptr<BaseComponent> GetComponentFromID(const Entity &entity, uint64_t id);

        /// <summary> Get the count of components an entity contains </summary>
        /// <param name="entity"> A reference to the entity to get component count </param>
        /// <returns> A size_t of components </returns>
        size_t EntityComponentCount(const Entity &entity);

        /// <summary> Add a component to a given entity </summary>
        /// <T> Component type to add </T>
        /// <param name="entity"> A reference to the entiy to add the component too </param>
        template<class T>
        void AddComponent(Entity &entity) {
            if (EntityHasComponenet<T>(entity)) {
                std::cout << "ECS: Entity already has component\n";
                return;
            }

            //create new component in memory
            auto component = std::make_shared<T>();
            m_components[T::ID].push_back(std::make_tuple(entity, component));
            entity.components.push_back(component);

            //add component to entity key
            entity.m_key = entity.m_key.to_ulong() + T::ID;
        }

        /// <summary> Remove a component from a given entity  </summary>
        /// <T> Component type to remove </T>
        /// <param name="entity"> A reference to the entity to remove a component from </param>
        template<class T>
        void RemoveComponent(Entity &entity) {
            auto &components = m_components[T::ID];
            for (int i = 0; i < components.size(); ++i) {
                auto entityTuple = components[i];

                if (std::get<0>(entityTuple).ID == entity.ID) {
                    components.erase(components.begin() + i);
                    //remove component from entity shared pointer vector aswell
                    for (int j = 0; j < entity.components.size(); ++j) {
                        entity.components.erase(entity.components.begin() + j);
                    }

                    //remove from entity key
                    entity.m_key &= 1UL << T::ID;
                    return;
                }

            }
        }

        /// <summary> Check if an entity has a given component </summary>
        /// <T> Component type to check against </T>
        /// <param name="entity"> A reference to the entity to check from </param>
        /// <returns> A boolean value whether the component exists within the entity </returns>
        template<class T>
        bool EntityHasComponenet(Entity &entity) {
            for (const auto &component : entity.components) {
                uint64_t componentID = T::ID;
                uint64_t entityComponentID = component.get()->id;
                if (componentID == entityComponentID) {
                    return true;
                }
            }
            return false;
        }

        /// <summary> Get the total count of components that exist in the ECS manager </summary>
        /// <returns> A size_t of all the components that exist in the ECS manager </returns>
        const size_t ComponentCount() { return m_components.size(); }

        /// <summary> Add a system to the ECS manager </summary>
        /// <T> System type to add </T>
        template<class T>
        void AddSystem() {
            for (const auto &system : m_systems) {
                if (dynamic_cast<T *>(system.get()) != nullptr) {
                    std::cout << "ECS: Cant add system as it already exist\n";
                    return;
                }
            }

            m_systems.push_back(std::make_unique<T>());
        }

        /// <summary> Remove a system from the ECS manager </summary>
        /// <T> System type to remove </T>
        template<class T>
        void RemoveSystem() {
            for (int i = 0; i < m_systems.size(); i++) {
                if (dynamic_cast<T *>(m_systems[i].get()) != nullptr) {
                    m_systems.erase(m_systems.begin() + i);
                    return;
                }
            }
        }

        /// <summary> Get a system from the ECS manager </summary>
        /// <T> System type to get </T>
        /// <returns> A pointer to the found system (nullptr if not found) </returns>
        template<class T>
        System* GetSystem(){
            for(const auto& system : m_systems){
                if(dynamic_cast<T*>(system.get()) != nullptr)
                    return system.get();
            }
            return nullptr;
        }

        /// <summary> Get the exact components required for a system from a given entity</summary>
        /// <param name="system"> A pointer to a given system </param>
        /// <param name="entity"> A reference to the entity </param>
        /// <returns> A vector of shared pointers that are required for a given system </returns>
        std::vector<std::shared_ptr<BaseComponent>> GetRequiredComponentsFromEntity(System *system,
                                                                                    const Entity &entity);

        /// <summary> Update all systems within the ECS manager </summary>
        void UpdateSystems();

    private:
        //component id, tuple with entity ID and component pointer
        std::unordered_map<uint32_t, std::vector<entityComponent>> m_components;
        std::vector<std::unique_ptr<Entity>> m_entities;
        std::vector<std::unique_ptr<System>> m_systems;
        uint32_t entityID = 0;

        /// <summary> Clear all the memory of components, entities and systems </summary>
        void freeSystem();

    };
}
#endif //ECS_LIB_COMPONENTMANAGER_HPP
