﻿#pragma once


#include <fbxsdk.h>


#ifdef _DEBUG

#pragma comment ( lib, "Fbx/debug/libfbxsdk.lib" )

#else

#pragma comment ( lib, "Fbx/release/libfbxsdk.lib" )

#endif
