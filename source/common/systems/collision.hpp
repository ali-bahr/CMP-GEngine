#pragma once

#include "../ecs/world.hpp"
#include "../maze/maze.hpp"

#include "../components/camera.hpp"
#include "../components/coin.hpp"
#include "../components/bolt.hpp"
#include "../components/rocket.hpp"
#include "../components/key.hpp"
#include "../components/portal.hpp"
#include "../components/master-key.hpp"
#include "../components/free-camera-controller.hpp"
#include "../components/door.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include "../application.hpp"
#include"../systems/game-actions.hpp"
#include <iostream>
namespace our{

    class CollisionSystem {
        Application *app;

        public:
        static bool checkCollision(const glm::vec3 &playerPosition, const glm::vec3 &targetPosition, PhysicalComponent *physicalComponent)
        {
            float minX = targetPosition.x - physicalComponent->collisionCellX / 2;
            float maxX = targetPosition.x + physicalComponent->collisionCellX / 2;
            float minZ = targetPosition.z - physicalComponent->collisionCellZ / 2;
            float maxZ = targetPosition.z + physicalComponent->collisionCellZ / 2;

            if (playerPosition.x >= minX && playerPosition.x <= maxX && playerPosition.z >= minZ && playerPosition.z <= maxZ)
            {
                // Play collision sound if given one
               // std::cout << "Collision" << std::endl;
                return true;
            }

            // No collision
            return false;
        }

        void enter(Application *app)
        {
            this->app = app;
        }            // std::cout<< "CollisionSystem::update" << std::endl;

        void update(World *world, float deltaTime)
        {
            // Get Player position from Camera
            CameraComponent *camera = nullptr;
            FreeCameraControllerComponent *controller = nullptr;

            for (auto entity : world->getEntities())
            {
                camera = entity->getComponent<CameraComponent>();
                controller = entity->getComponent<FreeCameraControllerComponent>();
                if (camera && controller)
                    break;
            }

            if (!(camera && controller))
                return; //Must be a camera entity
            Entity *entity = camera->getOwner();

            glm::vec3 &position = entity->localTransform.position;

            // Check entity collision with coins, bolts, rockets, keys, portals, and doors
            auto entities = world->getEntities();

            for (auto entity : entities)
            {
                if (entity->getComponent<CoinComponent>())
                {
                    // Calculate world-space positions of the coin
                    glm::vec3 coinPosition = glm::vec3(entity->getLocalToWorldMatrix()[3]);
                    if (checkCollision(position, coinPosition, entity->getComponent<CoinComponent>()))
                    {
                        std::cout << "COIN" << std::endl;
                        world->markForRemoval(entity);
                        our::GameActionsSystem::collectCoin();
                    }
                }
                else if(entity->getComponent<BoltComponent>())
                {
                    // Calculate world-space positions of the bolt
                    glm::vec3 boltPosition = glm::vec3(entity->getLocalToWorldMatrix()[3]);
                    if (checkCollision(position, boltPosition, entity->getComponent<BoltComponent>()))
                    {
                        std::cout << "BOLT!" << std::endl;
                        our::GameActionsSystem::setSpeedUp();
                        our::GameActionsSystem::collectPowerup();
                        our::GameActionsSystem::resetPowerupTimer(our::powerups::speedUp);
                        world->markForRemoval(entity);
                    }
                }
                else if(entity->getComponent<RocketComponent>())
                {
                    // Calculate world-space positions of the rocket
                    glm::vec3 rocketPosition = glm::vec3(entity->getLocalToWorldMatrix()[3]);
                    if (checkCollision(position, rocketPosition, entity->getComponent<RocketComponent>()))
                    {
                        std::cout << "ROCKET!" << std::endl;
                        our::GameActionsSystem::setGravityUp();
                        our::GameActionsSystem::collectPowerup();
                        our::GameActionsSystem::resetPowerupTimer(our::powerups::gravityUp);
                        world->markForRemoval(entity);
                        our::GameActionsSystem::setGravity(rocketPosition.x,rocketPosition.z);
                    }
                }
                else if(entity->getComponent<KeyComponent>())
                {
                    // Calculate world-space positions of the key
                    glm::vec3 keyPosition = glm::vec3(entity->getLocalToWorldMatrix()[3]);
                    if (checkCollision(position, keyPosition, entity->getComponent<KeyComponent>()))
                    {
                        std::cout << "KEY!" << std::endl;
                        our::GameActionsSystem::collectKey();
                        world->markForRemoval(entity);
                    }
                }
                else if(entity->getComponent<PortalComponent>()&&our::GameActionsSystem::getPortalState()==portalState::off)
                {
                    // Calculate world-space positions of the portal
                    glm::vec3 portalPosition = glm::vec3(entity->getLocalToWorldMatrix()[3]);
                    if (checkCollision(position, portalPosition, entity->getComponent<PortalComponent>()))
                    {
                        std::cout << "PORTAL!" << std::endl;
                        our::GameActionsSystem::collectPowerup();
                        our::GameActionsSystem::portalStateInc();
                        world->markForRemoval(entity);
                    }
                }
                else if(entity->getComponent<MasterKeyComponent>())
                {
                    // Calculate world-space positions of the master key
                    glm::vec3 masterKeyPosition = glm::vec3(entity->getLocalToWorldMatrix()[3]);
                    if (checkCollision(position, masterKeyPosition, entity->getComponent<MasterKeyComponent>()))
                    {
                        std::cout << "MASTER KEY!" << std::endl;
                        if(our::GameActionsSystem::getKeysCollected() != our::GameActionsSystem::getTotalKeys() && !our::GameActionsSystem::getCantCollectMasterKey())                        {
                            our::GameActionsSystem::setCantCollectMasterKey();
                        }
                        else if (our::GameActionsSystem::getKeysCollected() == our::GameActionsSystem::getTotalKeys() )
                        {
                          our::GameActionsSystem::collectExitKey();
                          world->markForRemoval(entity);
                            
                        }
                       
                    }
                }
                else if(entity->getComponent<DoorComponent>())
                {
                    // Calculate world-space positions of the door
                    glm::vec3 doorPosition = glm::vec3(entity->getLocalToWorldMatrix()[3]);
                    if( our::GameActionsSystem::getOpenDoor())
                    {
                        entity->localTransform.rotation.y=(our::GameActionsSystem::doorStartAngle-0.3*(our::GameActionsSystem::getPowerupTimer(powerups::door)));
                        return;
                    }
                    if (abs(position.x - doorPosition.x) < 2.3 && abs(position.z - doorPosition.z) < 10.8)
                    {
                        std::cout << "DOOR!" << std::endl;
                        if(!our::GameActionsSystem::getExitKey()&& !our::GameActionsSystem::getTouchDoor())
                        {
                            our::GameActionsSystem::setTouchDoor();
                        }
                        else if (our::GameActionsSystem::getExitKey())
                        {
                            std::cout <<  "OPEN DOOR!" << std::endl;
                            our::GameActionsSystem::setOpenDoor();
                        }
                    }
                }
            }
        }
    };
}