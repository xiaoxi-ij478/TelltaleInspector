#include <iostream>
#include <string>
#include <iterator>
#include "ToolLibrary/TelltaleToolLibrary.h"
#include "ToolLibrary/Meta.hpp"
#include "ToolLibrary/Types/D3DMesh.h"
#include "ToolLibrary/Types/HandleObjectInfo.h"
#include "ToolLibrary/Types/Skeleton.h"

std::string lookup_crc(uint64_t crc,const std::string &default_val){
std::string name;TelltaleToolLib_GetGlobalHashDatabase()->FindEntry(nullptr,crc,&name);
return name.length()?name:default_val;
}

#define lookup_bone_crc(crc) lookup_crc(crc,"bone_"+std::to_string(crc))

int main(int argc,char **argv)
{
    if(argc<2)return 1;
    TelltaleToolLib_Initialize("MC2");
	DataStreamFileDisc* db = _OpenDataStreamFromDisc("/home/xiaoxi/Desktop/sources/TelltaleInspector/Inspector/rundir/_Dev/ToolLibrary.HashDB", READ);
	TelltaleToolLib_SetGlobalHashDatabaseFromStream(db);
	MetaStream ms;ms.Open(_OpenDataStreamFromDisc(argv[1],READ),MetaStreamMode::eMetaStream_Read,{});
    D3DMesh mesh;
    PerformMetaSerializeAsync<D3DMesh>(&ms,&mesh);
    DCArray<T3MeshBoneEntry>&bones=mesh.mMeshData.mBones;
	T3MeshLOD& lod = mesh.mMeshData.mLODs[0];
	T3GFXVertexState& vertexState = *mesh.mMeshData.mVertexStates[lod.mVertexStateIndex];
	std::shared_ptr<T3GFXBuffer> weightBuffer = vertexState.mpVertexBuffer[get_params(vertexState, eGFXPlatformAttribute_BlendWeight)->mBufferIndex];
	std::shared_ptr<T3GFXBuffer> boneBuffer = vertexState.mpVertexBuffer[get_params(vertexState, eGFXPlatformAttribute_BlendIndex)->mBufferIndex];
if(!weightBuffer||!boneBuffer)return 1;
if(get_params(vertexState,eGFXPlatformAttribute_BlendIndex)->mFormat!=eGFXPlatformFormat_U8x4)return 1;
if(get_params(vertexState,eGFXPlatformAttribute_BlendWeight)->mFormat!=eGFXPlatformFormat_UN16x4)return 1;
std::cout<<"import bpy;from mathutils import *"<<std::endl;
for(const T3MeshBoneEntry &i:bones){
    std::cout<< "bpy.context.scene.objects['"<<argv[1]<<"'].vertex_groups.new(name='"<<lookup_bone_crc(i.mBoneName.GetCRC())<<"')"<<std::endl<<
    "bpy.context.scene.objects['骨架'].data.edit_bones['"<<lookup_bone_crc(i.mBoneName.GetCRC())<<"'].use_deform=True"<<std::endl;
}
for (unsigned i=0;i<weightBuffer->mCount;i++)
    {
    for(unsigned j=0;j<4;j++){
        unsigned short bone_weight_non_normalized=reinterpret_cast<unsigned short*>(
            static_cast<char*>(weightBuffer->mpCPUBuffer)+
            weightBuffer->mStride*i+
            get_params(vertexState,eGFXPlatformAttribute_BlendWeight)->mBufferOffset
        )[j];
        float bone_weight=bone_weight_non_normalized/65535.f;
        unsigned char bone_index=reinterpret_cast<unsigned char*>(
            static_cast<char*>(boneBuffer->mpCPUBuffer)+
            boneBuffer->mStride*i+
            get_params(vertexState,eGFXPlatformAttribute_BlendIndex)->mBufferOffset
        )[j];
        if(bone_weight_non_normalized)
        {
        std::cout<<"bpy.context.scene.objects['"<<argv[1]<<"'].vertex_groups['"<<std::hex<<lookup_bone_crc(bones[bone_index].mBoneName.GetCRC())<<std::dec<<"'].add(["<<i<<"],"<<bone_weight<<",'ADD')"<<std::endl;
        }
    }
}
    TelltaleToolLib_Free();
    return 0;
}
