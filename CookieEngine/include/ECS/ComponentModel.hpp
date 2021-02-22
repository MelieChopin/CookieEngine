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
        public:
            std::shared_ptr<Resources::Mesh> mesh {nullptr};
            std::shared_ptr<Resources::Shader> shader {nullptr};
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

            void Draw(Render::RendererRemote& remote, const Core::Math::Mat4& viewProj, const Core::Math::Mat4& modelMat)
            {
                if (shader)
                    shader->Set(remote, viewProj, modelMat);
                if (mesh)
                {
                    mesh->Set(remote);
                    mesh->Draw(remote);
                }
            }
        };

    }
}

#endif