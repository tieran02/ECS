#ifndef ECS_LIB_ENTITY_HPP
#define ECS_LIB_ENTITY_HPP

#include <cstdint>
#include <memory>
#include <vector>
#include <bitset>
#include "Component.hpp"

namespace ECS {

    struct Entity {
        friend class ECSManager;

        Entity(uint32_t id, std::string name = "Entity") : ID(id), m_name(name) {}

        const uint32_t ID;

        /// <summary> Get a key that fits a set of components of the entity </summary>
        /// <returns> A bitset that is used a to represent a key </returns>
        std::bitset<64> GetKey(){ return m_key;}

        /// <summary> Set a name for this entity </summary>
        /// <param name="name"> New name of the entity </param>
        void SetName(const std::string& name) { m_name = name;}

        /// <summary> Get the name for this entity </summary>
        /// <returns> Name of the entity </returns>
        const std::string& GetName(){ return m_name;}
    private:
        std::bitset<64> m_key{0};
        std::vector<std::shared_ptr<BaseComponent>> components;

        std::string m_name;
    };
}

#endif //ECS_LIB_ENTITY_HPP
