﻿#pragma once


#include "../CRTypes.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRVertex
//---------------------------------------------------------------------------------------------------------------------
struct CRVertex
{
    CRVector Position;
    CRVector Color;
};


extern CRVertex GCRVTriangle[ 3 ];
extern CRVertex GCRVRect    [ 4 ];