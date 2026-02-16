#include <iostream>
#include <string>
#include "ToolLibrary/TelltaleToolLibrary.h"
#include "ToolLibrary/Meta.hpp"
#include "ToolLibrary/Types/D3DMesh.h"
#include "ToolLibrary/Types/HandleObjectInfo.h"

int main(int argc, char **argv)
{
    if(argc<2)return 1;
    TelltaleToolLib_Initialize("MC2");
	DataStreamFileDisc* db = _OpenDataStreamFromDisc("/home/xiaoxi/Desktop/sources/TelltaleInspector/Release Builds/Dist/Database/ToolLibrary.HashDB", READ);
	TelltaleToolLib_SetGlobalHashDatabaseFromStream(db);
    MetaStream ms/*,ms2,ms3*/,ms4;
    D3DMesh mesh;/*
    PropertySet *ps1=new PropertySet,*ps2=new PropertySet;*/
    ms.Open(_OpenDataStreamFromDisc(argv[1],READ), MetaStreamMode::eMetaStream_Read, { 0 });
    /*ms2.Open(_OpenDataStreamFromDisc("/home/xiaoxi/Desktop/New Folder 1/skM1_admin_skM1_admin_clothes_M.prop",READ), MetaStreamMode::eMetaStream_Read, { 0 });
    ms3.Open(_OpenDataStreamFromDisc("/home/xiaoxi/Desktop/New Folder 1/skM1_admin_skM1_admin_skin_M.prop",READ), MetaStreamMode::eMetaStream_Read, { 0 });*/
    PerformMetaSerializeAsync<D3DMesh>(&ms,&mesh);
    /*PerformMetaSerializeAsync<PropertySet>(&ms2,ps1);
    PerformMetaSerializeAsync<PropertySet>(&ms3,ps2);*//*
    HandleUncached ha(TelltaleToolLib_FindMetaClassDescription("PropertySet",true),ps1);
    HandleUncached ha2(TelltaleToolLib_FindMetaClassDescription("PropertySet",true),ps2);*//*
    ha.SetObjectName("skM1_admin_skM1_admin_clothes_M.prop");
    ha2.SetObjectName("skM1_admin_skM1_admin_skin_M.prop");
    mesh.mInternalResources.AddElement(0,nullptr,&ha);
    mesh.mInternalResources.AddElement(0,nullptr,&ha2);*/

    for(T3MeshMaterial*i=mesh.mMeshData.mMaterials.mpStorage;i!=mesh.mMeshData.mMaterials.mpStorage+mesh.mMeshData.mMaterials.mSize;i++)
    {
        if(i->mhMaterial.GetObjectName()=="<NotFound>")continue;
        MetaStream ms_;
        PropertySet*ps=new PropertySet;
        ms_.SwitchToMode(MetaStreamMode::eMetaStream_Read,_OpenDataStreamFromDisc((std::string("/home/xiaoxi/Desktop/New Folder 1/")+i->mhMaterial.GetObjectName()).c_str(),READ));
        PerformMetaSerializeAsync<PropertySet>(&ms_,ps);
        HandleUncached ha(TelltaleToolLib_FindMetaClassDescription("PropertySet",true),ps);
        ha.SetObjectName(static_cast<HandleBase>(i->mhMaterial).GetObjectName());
        mesh.mInternalResources.AddElement(0,nullptr,&ha);
    }
    ms4.Open(_OpenDataStreamFromDisc((std::string(argv[1])+".new.d3dmesh").c_str(),WRITE), MetaStreamMode::eMetaStream_Write, { 0 });
    PerformMetaSerializeAsync<D3DMesh>(&ms4,&mesh);
    TelltaleToolLib_Free();
    return 0;
}
