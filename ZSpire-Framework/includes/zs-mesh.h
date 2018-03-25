#ifndef ZSMESH
#define ZSMESH
#endif

#ifndef ZSRESOURCE
#include "../includes/zs-resource.h"
#endif

#define MESH_NO_INDICES -1

namespace ZSpire {
	class Mesh {
	protected:
		unsigned int meshVAO;
		unsigned int meshVBO;
		unsigned int meshIBO;

		unsigned int IndicesCount;
		int VerticesCount = MESH_NO_INDICES;
	public:
		ZSResourceDesc resource_desc;
		void InitializeMesh();
		void Release();
		void Draw();
		void processMesh(ZSVERTEX* vertsf, unsigned int* indices, unsigned int verticesCount, unsigned int indicesCount);
		void processMesh(ZSVERTEX* vertsf, unsigned int verticesCount );
	};
}