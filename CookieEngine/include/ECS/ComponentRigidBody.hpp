#ifndef __COMPONENT_RIGIDBODY_HPP__
#define __COMPONENT_RIGIDBODY_HPP__



#include <reactphysics3d/reactphysics3d.h>

namespace Cookie
{
    namespace ECS
    {

        class ComponentRigidBody
        {
        public:
            reactphysics3d::RigidBody* physBody = nullptr;

            ComponentRigidBody() {}
            ComponentRigidBody(const ComponentRigidBody& other) : physBody{other.physBody} {}
            ~ComponentRigidBody() {}

            //void AddForce(const Core::Math::Vec3& directVec) {}
            void ToDefault()
            {
                physBody = nullptr;
            }
        };

    }
}

#endif