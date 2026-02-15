#include <iostream>
#include <string>
#include <iterator>
#include "ToolLibrary/TelltaleToolLibrary.h"
#include "ToolLibrary/Meta.hpp"
#include "ToolLibrary/Types/D3DMesh.h"
#include "ToolLibrary/Types/HandleObjectInfo.h"
#include "ToolLibrary/Types/Skeleton.h"

int main(int argc,char **argv)
{
    if(argc<2)return 1;
    TelltaleToolLib_Initialize("MC2");
	DataStreamFileDisc* db = _OpenDataStreamFromDisc("/home/xiaoxi/Desktop/sources/TelltaleInspector/Release Builds/Dist/Database/ToolLibrary.HashDB", READ);
	TelltaleToolLib_SetGlobalHashDatabaseFromStream(db);
	MetaStream ms;ms.Open(_OpenDataStreamFromDisc(argv[1],READ),MetaStreamMode::eMetaStream_Read,{});
    D3DMesh mesh;
    PerformMetaSerializeAsync<D3DMesh>(&ms,&mesh);
    DCArray<T3MeshBoneEntry>&bones=mesh.mMeshData.mBones;
	T3MeshLOD& lod = mesh.mMeshData.mLODs[0];
    TelltaleToolLib_Free();
    return 0;
}
