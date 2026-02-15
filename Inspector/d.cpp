#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include "ToolLibrary/TelltaleToolLibrary.h"
#include "ToolLibrary/Meta.hpp"
#include "ToolLibrary/Types/D3DMesh.h"
#include "ToolLibrary/Types/HandleObjectInfo.h"
#include "ToolLibrary/Types/Skeleton.h"
#include "ToolLibrary/HashDB/HashDB.h"

int main(int argc,char **argv)
{
    TelltaleToolLib_Initialize("MC2");
	DataStreamFileDisc* db = _OpenDataStreamFromDisc("/home/xiaoxi/Desktop/sources/TelltaleInspector/Release Builds/Dist/Database/ToolLibrary.HashDB", READ);
	DataStreamFileDisc* ndb = _OpenDataStreamFromDisc("/home/xiaoxi/Desktop/sources/TelltaleInspector/Inspector/rundir/_Dev/ToolLibrary.HashDB", WRITE);
	HashDatabase::Create("/home/xiaoxi/Desktop/sources/TelltaleInspector/Inspector/rundir/_Dev/LIBTT_DB.txt",ndb,true,true,false);
    TelltaleToolLib_Free();
    return 0;
}
