﻿#include "CRD11CompiledShader.h"
#include "../../Utility/CRLog.h"
#include <d3dcompiler.h>


//---------------------------------------------------------------------------------------------------------------------
/// Destructor
//---------------------------------------------------------------------------------------------------------------------
CRD11CompiledShader::~CRD11CompiledShader()
{
    if ( CompiledShader )
    {
        CompiledShader->Release();
        CompiledShader = nullptr;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Create compiled shader.
//---------------------------------------------------------------------------------------------------------------------
void CRD11CompiledShader::Create( const CWRString& Path, const CRString& EntryPoint, const CRString& ShaderModel )
{
    ID3DBlob* error = nullptr;
	
    HRESULT hr = D3DCompileFromFile( Path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, EntryPoint.c_str(), ShaderModel.c_str(), D3DCOMPILE_ENABLE_STRICTNESS, 0, &CompiledShader, &error );

    if ( FAILED( hr ) )
    {
        GLog.AddErrorLog( hr );

        if ( error )
        {
            GLog.AddLog( (char*)error->GetBufferPointer() );
        }
    }

    if ( error )
    {
        error->Release();
    }
}
