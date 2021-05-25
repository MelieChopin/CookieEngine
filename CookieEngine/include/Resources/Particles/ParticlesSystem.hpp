#ifndef __PARTICLESSYSTEM_HPP__
#define __PARTICLESSYSTEM_HPP__

#include "ParticlesEmitter.hpp"
#include "ParticlesData.hpp"
#include "ComponentTransform.hpp"
#include "ParticlesPass.hpp"
#include "Resources/Mesh.hpp"
#include "Resources/Texture.hpp"
#include "Camera.hpp"
#include "ResourcesManager.hpp"

namespace Cookie
{
	namespace Resources
	{
		namespace Particles
		{
			class ParticlesSystem
			{
			public :
				std::vector<ParticlesData> data;
				std::vector<ParticlesEmitter> particlesEmiter;

				ECS::ComponentTransform trs;

				Cookie::Render::ParticlesPass shader;

				ParticlesSystem() {}

				ParticlesSystem(const ParticlesSystem& other) : data(other.data), particlesEmiter(other.particlesEmiter)
				{
					//shader.InitShader();
				}

				ParticlesSystem(int size, int sizeFrame)
				{
					//shader.InitShader();
					data.push_back(ParticlesData());
					particlesEmiter.push_back(ParticlesEmitter());
					data[0].generate(size, sizeFrame);
				}
				~ParticlesSystem() {}

				void Update()
				{
					for (int j = 0; j < data.size(); j++)
						for (int k = 0; k < particlesEmiter[j].updates.size(); k++)
							particlesEmiter[j].updates[k]->Update(&data[j]);
				}

				void Draw(const Render::Camera& cam, Cookie::Resources::ResourcesManager& resources)
				{
					for (int j = 0; j < data.size(); j++)
					{
						std::vector<Cookie::Render::InstancedData> newData;
						for (int i = 0; i < data[j].countAlive; i++)
						{
							Cookie::Render::InstancedData temp;
							temp.World = (trs.TRS * data[j].trs[i]);
							temp.Color = Cookie::Core::Math::Vec4(data[j].col[i].ToVec3(), 0.25f);
							newData.push_back(temp);
						}

						shader.Draw(cam, data[j].mesh, data[j].texture, newData);
					}
				}
			};
		}
	}
}

#endif // !__PARTICLESSYSTEM_HPP__
