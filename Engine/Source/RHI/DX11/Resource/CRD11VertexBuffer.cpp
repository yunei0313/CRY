﻿#include "CRD11VertexBuffer.h"
#include "CRD11Device.h"
#include "Source/RHI/DX11/CRD11.h"
#include "Source/Utility/Generic/CRGeneric.h"


//---------------------------------------------------------------------------------------------------------------------
/// Create vertex buffer
//---------------------------------------------------------------------------------------------------------------------
void CRD11VertexBuffer::Create( D3D11_USAGE Usage, u32 CpuAccess, const void* BlobPtr, u32 InStride, u32 InCount )
{
    Stride = InStride;
    Count  = InCount;

    D3D11_BUFFER_DESC bd;
    ZeroMemory( &bd, sizeof( D3D11_BUFFER_DESC ) );

    bd.Usage          = Usage;  
    bd.ByteWidth      = Stride * Count;  
    bd.BindFlags      = D3D11_BIND_VERTEX_BUFFER;  
    bd.CPUAccessFlags = CpuAccess;

    D3D11_SUBRESOURCE_DATA sd;
    ZeroMemory( &sd, sizeof( D3D11_SUBRESOURCE_DATA ) );

    sd.pSysMem = BlobPtr;

    HRESULT hr = GD11.GetDevice()->CreateBuffer( &bd, &sd, &ObjectPtr );
    CRGeneric::CheckError( hr );
}
