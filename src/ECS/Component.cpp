#include "ECS/Component.hpp"

namespace ECS {
    static uint32_t ComponentID = 0;

    const uint64_t BaseComponent::NextID() {
        return 1 << ComponentID++;
    }
}