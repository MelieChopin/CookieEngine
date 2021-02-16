#ifndef __MESH_HPP__
#define __MESH_HPP__

struct ID3D11Buffer;

namespace Cookie
{
	namespace Resources
	{
		class Mesh
		{
			private:
				ID3D11Buffer* VBuffer = nullptr;

			public:
				Mesh();
				~Mesh();
		};
	}
}

#endif /* __MESH_HPP__ */