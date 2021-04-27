#ifndef __COMPONENT_MODEL_HPP__
#define __COMPONENT_MODEL_HPP__

struct ID3D11Buffer;

namespace Cookie
{
    namespace Resources
    {
        class Mesh;
        class Shader;
        class Texture;
    }

    namespace ECS
    {

        class ComponentModel
        {
        public:
            std::shared_ptr<Resources::Mesh>    mesh    {nullptr};
            std::shared_ptr<Resources::Texture> texture {nullptr};


            //Material material;

        public:
            ComponentModel();
            ~ComponentModel();

            void ToDefault();

            void Draw(const Core::Math::Mat4& viewProj, const Core::Math::Mat4& modelMat, ID3D11Buffer** CBuffer);
        };

    }
}

#endif