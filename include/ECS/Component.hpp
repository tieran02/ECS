#ifndef ECS_LIB_COMPONENT_HPP
#define ECS_LIB_COMPONENT_HPP

#include <cstdint>
namespace ECS {
    struct BaseComponent {
        static const uint64_t NextID();

        uint64_t id;
    };

    template<typename T>
    struct Component : public BaseComponent {
        Component() { id = ID; }

        static const uint64_t ID;
    };

    template<typename T>
    const uint64_t Component<T>::ID(BaseComponent::NextID());

}

#endif //ECS_LIB_COMPONENT_HPP
