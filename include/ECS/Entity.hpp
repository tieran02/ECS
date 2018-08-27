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

        Entity(uint32_t id) : ID(id) {}

        uint32_t ID;

        /// <summary> Get a key that fits a set of components of the entity </summary>
        /// <returns> A bitset that is used a to represent a key </returns>
        std::bitset<64> GetKey(){ return m_key;}
    private:
        std::bitset<64> m_key{0};
        std::vector<std::shared_ptr<BaseComponent>> components;
    };
}

#endif //ECS_LIB_ENTITY_HPP
