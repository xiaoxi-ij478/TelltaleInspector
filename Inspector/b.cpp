#include <iostream>
#include <string>
#include "ToolLibrary/TelltaleToolLibrary.h"
#include "ToolLibrary/Meta.hpp"
#include "ToolLibrary/Types/D3DMesh.h"
#include "ToolLibrary/Types/HandleObjectInfo.h"
#include "ToolLibrary/Types/Skeleton.h"

int main(int argc,char **argv)
{if(argc<2)return 1;
    TelltaleToolLib_Initialize("MC2");
	DataStreamFileDisc* db = _OpenDataStreamFromDisc("/home/xiaoxi/Desktop/sources/TelltaleInspector/Release Builds/Dist/Database/ToolLibrary.HashDB", READ);
	TelltaleToolLib_SetGlobalHashDatabaseFromStream(db);
	MetaStream ms;ms.Open(_OpenDataStreamFromDisc(argv[1],READ),MetaStreamMode::eMetaStream_Read,{});
	Skeleton sk;PerformMetaSerializeAsync<Skeleton>(&ms,&sk);
	std::cout<<"import bpy;from mathutils import *;from time import sleep;bpy.ops.object.armature_add(radius=0,enter_editmode=True);sleep(0.01)"<<std::endl;
	for(const Skeleton::Entry&i:sk.mEntries)
	{
		std::cout<<
		"cccc=bpy.context.blend_data.armatures[0].edit_bones.new('bone_"<<std::hex<<i.mJointName.GetCRC()<<std::dec<<"')"<<std::endl<<
		"cccc.use_deform=False"<<std::endl<<
		"cccc.parent=bpy.context.blend_data.armatures[0].edit_bones['bone_"<<std::hex<<i.mParentName.GetCRC()<<std::dec<<"']"<<std::endl<<
		"cccc.use_connect=True"<<std::endl<<
		"_quat="
		"Quaternion(cccc.parent['_transform_quat'])"
		"@"
		"Quaternion(("<<i.mLocalQuat.w<<','<<i.mLocalQuat.x<<','<<i.mLocalQuat.y<<','<<i.mLocalQuat.z<<"))"
		<<std::endl<<
		"cccc['_transform_quat']=(_quat.w,_quat.x,_quat.y,_quat.z)"<<std::endl<<
		"cccc.tail=cccc.head+("
		"_quat.to_matrix())"
		"@"
		"Vector(("<<i.mLocalPos.x<<','<<i.mLocalPos.y<<','<<i.mLocalPos.z<<"))"
		<<std::endl;
	}
    TelltaleToolLib_Free();
    return 0;
}
