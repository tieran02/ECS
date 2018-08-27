#ifndef ECS_LIB_SYSTEM_HPP
#define ECS_LIB_SYSTEM_HPP

#include <cstdint>
#include <vector>
#include "Entity.hpp"
#include "Component.hpp"
#include <unordered_map>

namespace ECS {
    class ECSManager;

    class System {
    public:
        System(const uint64_t &signature) : m_signature(signature){}

        /// <summary> Virtual update function to update any system </summary>
        /// <param name="entityID"> ID of the entity </param>
        /// <param name="components"> A vector of the components of the entity </param>
        virtual void Update(const ECSManager& ecsManager, const uint32_t& entityID, std::vector<std::shared_ptr<BaseComponent>> components) = 0;

        /// <summary> Virtual update function to update any system </summary>
        /// <returns> A uint64 that is used a bitset to represent a signature of the system </returns>
        const std::bitset<64> &GetSignature() { return m_signature; }

    private:
        const std::bitset<64> m_signature;
    };
}


#endif //ECS_LIB_SYSTEM_HPP
