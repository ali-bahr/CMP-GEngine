#pragma once

#include "../ecs/entity.hpp"
#include "camera.hpp"
#include "mesh-renderer.hpp"
#include "free-camera-controller.hpp"
#include "movement.hpp"
#include "light.hpp"
#include "bolt.hpp"
#include "coin.hpp"
#include "door.hpp"
#include "key.hpp"
#include "master-key.hpp"
#include "portal.hpp"
#include "rocket.hpp"
#include "wall.hpp"
#include "../systems/game-actions.hpp"
namespace our
{

    // Given a json object, this function picks and creates a component in the given entity
    // based on the "type" specified in the json object which is later deserialized from the rest of the json object
    inline void deserializeComponent(const nlohmann::json &data, Entity *entity)
    {
        std::string type = data.value("type", "");
        Component *component = nullptr;
        // DONE TODO: (Req 8) Add an option to deserialize a "MeshRendererComponent" to the following if-else statement
        if (type == CameraComponent::getID())
        {
            component = entity->addComponent<CameraComponent>();
        }
        else if (type == FreeCameraControllerComponent::getID())
        {
            component = entity->addComponent<FreeCameraControllerComponent>();
        }
        else if (type == MovementComponent::getID())
        {
            component = entity->addComponent<MovementComponent>();
        }
        else if (type == MeshRendererComponent::getID())
        {
            component = entity->addComponent<MeshRendererComponent>();
        }
        else if (type == LightComponent::getID())
        {
            component = entity->addComponent<LightComponent>();
        }
        else if (type == BoltComponent::getID())
        {
            component = entity->addComponent<BoltComponent>();
        }
        else if (type == CoinComponent::getID())
        {
            component = entity->addComponent<CoinComponent>();
        }
        else if (type == DoorComponent::getID())
        {
            component = entity->addComponent<DoorComponent>();
            our::GameActionsSystem::doorStartAngle=entity->localTransform.rotation.y;

        }
        else if (type == KeyComponent::getID())
        {
            component = entity->addComponent<KeyComponent>();
        }
        else if (type == MasterKeyComponent::getID())
        {
            component = entity->addComponent<MasterKeyComponent>();
        }
        else if (type == PortalComponent::getID())
        {
            component = entity->addComponent<PortalComponent>();
        }
        else if (type == RocketComponent::getID())
        {
            component = entity->addComponent<RocketComponent>();
        }
        else if (type == WallComponent::getID())
        {
            component = entity->addComponent<WallComponent>();
        }
        if (component)
            component->deserialize(data);
    }

}