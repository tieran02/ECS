

#include <ECS/ECS.hpp>
#include <iostream>

struct Transform : public ECS::Component<Transform>{
    float x,y;
};

struct Motion : public ECS::Component<Motion>{
    float x,y;
};

struct Sprite : public ECS::Component<Sprite>{
    float width,height;
};

class MovementSystem : public ECS::System{
public:
    MovementSystem() : System(Transform::ID | Motion::ID) {}

    void Update(const ECS::ECSManager& ecsManager, const uint32_t& entityID, std::vector<std::shared_ptr<ECS::BaseComponent>> components) override {
        std::cout << "Updating Movement system on entity " << entityID << std::endl;

        Transform& transform =  static_cast<Transform&>(*components[0].get());
        Motion& motion =  static_cast<Motion&>(*components[1].get());

        std::cout << "Transform X = " << transform.x << std::endl;
        transform.x++;
        std::cout << "Transform X = " << transform.x << std::endl;
    }
};

int main()
{
    std::cout << "Transform ID = " << Transform::ID << std::endl;
    std::cout << "Motion ID = " << Motion::ID << std::endl;

    std::cout << "Creating ECS manager\n";
    ECS::ECSManager ecsManager;
    std::cout << "Created ECS manager\n";

    ecsManager.AddSystem<MovementSystem>();

    ECS::Entity& entity = ecsManager.MakeEntity();
    ECS::Entity& entity1 = ecsManager.MakeEntity();

    ecsManager.AddComponent<Transform>(entity);
    ecsManager.AddComponent<Motion>(entity);
    ecsManager.AddComponent<Sprite>(entity);

    ecsManager.AddComponent<Transform>(entity1);
    ecsManager.AddComponent<Motion>(entity1);

    std::cout << "Entity key = " << entity.GetKey() << std::endl;
    std::cout << "System signature = " << ecsManager.GetSystem<MovementSystem>()->GetSignature() << std::endl;

    for (int i = 0; i < 10; ++i) {
        ecsManager.UpdateSystems();
    }

    return 0;
}