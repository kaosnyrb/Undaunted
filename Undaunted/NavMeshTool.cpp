#include "NavmeshTool.h"
#include "SKSELink.h"
namespace Undaunted
{
	std::string floattostring(float value)
	{
		char hex[9];
		sprintf(hex, "%08X", *(unsigned long int*) & value);
		//	_MESSAGE("float value: %s", hex);
		std::string output = "";
		output += hex[6];
		output += hex[7];
		output += ' ';
		output += hex[4];
		output += hex[5];
		output += ' ';
		output += hex[2];
		output += hex[3];
		output += ' ';
		output += hex[0];
		output += hex[1];
		return output;
	}

	std::string uint32tostring(UInt32 value)
	{
		char hex[9];
		sprintf(hex, "%04X", *(unsigned long int*) & value);
		std::string output = "";
		output += hex[2];
		output += hex[3];
		output += ' ';
		output += hex[0];
		output += hex[1];
		return output;
	}

	void AddVertex(Vert vertex)
	{
		_MESSAGE("verts := NewArrayElement(nvnm, 'Vertices');seev(nvnm, 'Vertices\\[%i]', '%s %s %s');", vertex.index, floattostring(vertex.x), floattostring(vertex.y), floattostring(vertex.z));
	}

	void AddTriangle(Triangle tri)
	{
		//Not bothering with cover right now, which is the last 4 values.
		_MESSAGE("NewArrayElement(nvnm, 'Triangles');seev(nvnm, 'Triangles\\[%i]', '%s %s %s %s %s %s 00 00 00 00');",
			tri.index, 
			uint32tostring(tri.vert1), 
			uint32tostring(tri.vert2),
			uint32tostring(tri.vert3),
			uint32tostring(tri.edge1),
			uint32tostring(tri.edge2),
			uint32tostring(tri.edge3));
	}

	// Remember to edit the navmesh slightly in the Creation Kit. Otherwise it seems to lead to an unending memory leak :D
	// I believe this is due to us not setting up the NavMeshGrid in the code, howeer making any change in the CK and saving it generates it.
	void ExportNavmesh()
	{
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim Special Edition\\SKSE\\Navmesh.pas");
		_MESSAGE("unit userscript;uses SkyrimUtils;uses mteFunctions;");
		_MESSAGE("function NewArrayElement(rec: IInterface; path: String): IInterface; var a: IInterface; begin a := ElementByPath(rec, path); if Assigned(a) then begin Result := ElementAssign(a, HighInteger, nil, false); end else begin a := Add(rec, path, true);Result := ElementByIndex(a, 0);end;end;");
		_MESSAGE("function Process(e: IInterface): integer; var cell: IInterface; navm: IInterface; nvnm: IInterface; verts: IInterface; begin Result := 0;  if not (Signature(e) = 'CELL') then begin Exit; end; AddMessage('Processing: ' + FullPath(e));");
		_MESSAGE("navm := Add(e,'NAVM',true);nvnm := Add(navm,'NVNM',true);seev(nvnm, 'Version', 12);seev(nvnm, 'Parent Cell', HexFormID(e));seev(nvnm, 'NavMeshGrid', '00');");

		//Float - Little Endian (DCBA) - x y z
		AddVertex(Vert(0, -32.0411339, -41.5492, -0.999985));
		AddVertex(Vert(1, -25.0411339, -75.5492, -0.999985));
		AddVertex(Vert(2, 10.0411339, -41.5492, -0.999985));

		// vert index UINT32 - vert index  UINT32 - vert index  UINT32 - edge neighbour triangle index UINT32 - cover flag - 
		Triangle tri = Triangle(0,0,1,2,-1,-1,-1);
		AddTriangle(tri);

		_MESSAGE("end;end.");
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim Special Edition\\SKSE\\Undaunted.log");
	}
}