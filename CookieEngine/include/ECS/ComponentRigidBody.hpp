#ifndef __COMPONENT_RIGIDBODY_HPP__
#define __COMPONENT_RIGIDBODY_HPP__

#include "Vec3.hpp"

namespace Cookie
{
    namespace ECS
    {

        class ComponentRigidBody
        {
        public:
            Core::Math::Vec3 linearVelocity {0, 0, 0};
            float            mass {1};
            float            drag {0.1};
            //float          bounciness {0.5};
            Core::Math::Vec3 targetPosition{ 0, 0, 0 };
            float            speed{0};
            bool             goTowardTarget{false};


            ComponentRigidBody() {}
            ComponentRigidBody(const Core::Math::Vec3& _linearVelocity, const float _mass = 1, const float _drag = 0.1) : linearVelocity(_linearVelocity), mass(_mass), drag(_drag) {}
            ComponentRigidBody(const ComponentRigidBody& other) : linearVelocity(other.linearVelocity), mass(other.mass), drag(other.drag) {}
            ~ComponentRigidBody() {}

            void AddForce(const Core::Math::Vec3& directVec) { if (mass != 0) linearVelocity += directVec / mass; }
            void ToDefault()
            {
                linearVelocity = {0, 0, 0};
                mass = 1;
                drag = 0.1;

                targetPosition = { 0, 0, 0 };
                speed = 0;
                goTowardTarget = false;
            }
        };

    }
}

#endif