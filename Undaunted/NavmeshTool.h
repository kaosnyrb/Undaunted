namespace Undaunted
{
#ifndef NavMeshTool
#define NavMeshTool
	class Vert {
	public:
		UInt32 index;
		float x, y, z;
	};

	class Triangle {
	public:
		UInt32 index;
		UInt32 vert1, vert2, vert3;
		UInt32 edge1, edge2, edge3;
	};

	void ExportNavmesh();
#endif
}