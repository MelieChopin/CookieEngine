#ifndef __PARTICLESSYSTEM_HPP__
#define __PARTICLESSYSTEM_HPP__


#include "ParticlesEmitter.hpp"
#include "ParticlesData.hpp"
#include "ComponentTransform.hpp"
#include "ParticlesPass.hpp"
#include "Resources/Mesh.hpp"
#include "Resources/Texture.hpp"
#include "Camera.hpp"
#include "DrawDataHandler.hpp"
#include "Resources/ResourcesManager.hpp"

namespace Cookie
{
	namespace Resources
	{
		namespace Particles
		{
			struct emit
			{
				std::string name;
				Cookie::Core::Math::Vec3 data[2];
				std::string nameData;
			};

			class ParticlesPrefab
			{
			public:
				std::string name;
				struct data
				{
					Mesh* mesh;
					Texture* texture;
					int size;
					int countFrame;
					int countAlive;
					bool isBillboard;
				};
				
				std::vector<data> data;
				std::vector<ParticlesEmitter> emitter;
				std::vector<std::vector<emit>> emit;
			};

			class ParticlesSystem
			{
			public :
				std::vector<ParticlesData> data;
				std::vector<ParticlesEmitter> particlesEmiter;

				Core::Math::Mat4 trs = Core::Math::Mat4::Identity();
				bool needToBeRemoved = false;

				Cookie::Render::ParticlesPass* shader;
				std::string name;

				ParticlesSystem() 
				{}

				ParticlesSystem(const ParticlesSystem& other): data(other.data), particlesEmiter(other.particlesEmiter), 
					trs(other.trs), needToBeRemoved(other.needToBeRemoved), shader(other.shader), name(other.name)
				{}

				ParticlesSystem(Cookie::Render::ParticlesPass* shader) : shader(shader)
				{}

				ParticlesSystem(int size, int sizeFrame)
				{
					data.push_back(ParticlesData());
					particlesEmiter.push_back(ParticlesEmitter());
					data[0].generate(size, sizeFrame);
				}
				~ParticlesSystem() {}

				void generate()
				{
					for (int j = 0; j < particlesEmiter.size(); j++)
						for (int i = 0; i < particlesEmiter[j].generators.size(); i++)
							particlesEmiter[j].generators[i]->generate(&data[j], 0, data[j].countAlive);
				}

				void Update()
				{
					needToBeRemoved = false;
					for (int j = 0; j < data.size(); j++)
					{
						if (particlesEmiter.size() > j)
						{
							for (int k = 0; k < particlesEmiter[j].updates.size(); k++)
								particlesEmiter[j].updates[k]->Update(&data[j]);
						}
						if (data[j].countAlive <= 0 && data[j].canRemoved)
							needToBeRemoved = true;
						else if (!data[j].canRemoved && needToBeRemoved && data[j].countAlive <= 0)
							needToBeRemoved = true;
						else
							needToBeRemoved = false;
					}
					
					if (needToBeRemoved)
					{
						data.clear();
						particlesEmiter.clear();
					}
				}

				void Draw(const Render::Camera& cam, Render::Frustrum& frustrum)
				{
					bool cull = false;
					for (int j = 0; j < data.size(); j++)
					{
						std::vector<Cookie::Render::InstancedData> newData;
						for (int i = 0; i < data[j].countAlive; i++)
						{
							Cookie::Core::Math::Vec4 pos = Cookie::Core::Math::Vec4(data[j].data[i].pos, 1);
							for (int j = 0; j < frustrum.planes.size(); j++)
								if ((frustrum.planes[j].Dot(pos) + frustrum.planes[j].w) < -Cookie::Core::Math::PI)
									cull = true;

							if (cull)
							{
								cull = false;
								continue;
							}

							Cookie::Render::InstancedData temp;
							temp.World = Cookie::Core::Math::Mat4::TRS(data[j].data[i].pos, data[j].data[i].rot, data[j].data[i].scale);
							temp.Color = data[j].data[i].col;
							temp.isBillboard = data[j].data[i].isBillboard;
							newData.push_back(temp);
						}

						if (newData.size() > 0)
							shader->Draw(cam, data[j].mesh, data[j].texture, newData);
					}
				}
			};
		}
	}
}

#endif // !__PARTICLESSYSTEM_HPP__
