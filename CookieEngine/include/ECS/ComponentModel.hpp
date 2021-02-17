#ifndef __COMPONENT_MODEL_HPP__
#define __COMPONENT_MODEL_HPP__

#include "Resources/Mesh.hpp"
#include "Resources/Shader.hpp"

namespace Cookie
{
    namespace ECS
    {

        class ComponentModel
        {
        private:
            Resources::Mesh* mesh {nullptr};
            Resources::Shader* shader {nullptr};
            //Texture* texture {nullptr};


            //Material material;

        public:
            ComponentModel() {}
            ~ComponentModel() {}

            void ToDefault()
            {
                //mesh = nullptr;
                //shader = nullptr;
                //texture = nullptr;
            }
        };

    }
}

#endif