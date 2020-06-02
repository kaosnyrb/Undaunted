#include "NavmeshTool.h"
#include "SKSELink.h"

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

void Undaunted::ExportNavmesh()
{
	std::string output = floattostring(-32.0411339);
	_MESSAGE("output value: %s", output.c_str());
	/*
	gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim Special Edition\\SKSE\\Navmesh.pas");
	_MESSAGE("unit userscript;uses SkyrimUtils;uses mteFunctions;");
	_MESSAGE("function NewArrayElement(rec: IInterface; path: String): IInterface; var a: IInterface; begin a := ElementByPath(rec, path); if Assigned(a) then begin Result := ElementAssign(a, HighInteger, nil, false); end else begin a := Add(rec, path, true);Result := ElementByIndex(a, 0);end;end;");
	_MESSAGE("function Process(e: IInterface): integer; var cell: IInterface; navm: IInterface; nvnm: IInterface; verts: IInterface; begin Result := 0;  if not (Signature(e) = 'CELL') then begin Exit; end; AddMessage('Processing: ' + FullPath(e));");
	_MESSAGE("navm := Add(e,'NAVM',true);nvnm := Add(navm,'NVNM',true);seev(nvnm, 'Version', 12);seev(nvnm, 'Parent Cell', HexFormID(e));seev(nvnm, 'NavMeshGrid', '00');");

	
	//Float - Little Endian (DCBA) - x y z
	_MESSAGE("verts := NewArrayElement(nvnm, 'Vertices');seev(nvnm, 'Vertices\\[0]', '1F 2A 00 C2 6C 32 26 C2 00 FF 7F BF');");
	_MESSAGE("verts := NewArrayElement(nvnm, 'Vertices');seev(nvnm, 'Vertices\\[1]', '37 04 13 42 CC B6 78 C1 00 00 80 BF');");
	_MESSAGE("verts := NewArrayElement(nvnm, 'Vertices');seev(nvnm, 'Vertices\\[2]', '88 E3 E2 41 EC D1 99 C2 00 FF 7F BF');");

	// vert index UINT32 - vert index  UINT32 - vert index  UINT32 - edge neighbour triangle index UINT32 - cover flag - 
	_MESSAGE("NewArrayElement(nvnm, 'Triangles');seev(nvnm, 'Triangles\\[0]', '00 00 02 00 01 00 FF FF FF FF FF FF 00 00 00 00');");


	_MESSAGE("end;end.");
	gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim Special Edition\\SKSE\\Undaunted.log");
	*/
}
