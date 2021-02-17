#ifndef __COMPONENT_MODEL_HPP__
#define __COMPONENT_MODEL_HPP__

namespace Cookie
{
    namespace ECS
    {

        class ComponentModel
        {
        private:
            //Mesh* mesh {nullptr};
            //Shader* shader {nullptr};
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