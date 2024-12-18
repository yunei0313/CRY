﻿#pragma once


#include "CRD11Include.h"
#include "CRD11Types.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11Renderer
//---------------------------------------------------------------------------------------------------------------------
class CRD11Renderer
{
private:
    CRD11VertexBufferWPtr VertexBuffers[ D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT ];
    CRD11InputLayoutWPtr  InputLayout  [ D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT ];

    CRD11IndexBufferWPtr  IndexBuffer;
    CRD11VertexShaderWPtr VertexShader;
    CRD11PixelShaderWPtr  PixelShader;

    CRD11ShaderResourceViewWPtr ShaderResourceViews[ D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT ];
	CRD11SamplerStateWPtr       SamplerStates      [ D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT ];

    ID3D11RenderTargetView* RenderTargetView = nullptr;
    
public:
    /// Initialize renderer.
    void Initialize( unsigned int Width, unsigned int Height );

    /// Set vertex buffer.
    void SetVertexBuffer( const CRD11VertexBufferSPtr& CRVertexBuffer, unsigned int Slot );

    /// Set index buffer.
    void SetIndexBuffer( const CRD11IndexBufferSPtr& CRIndexBuffer );

    /// Set vertex shader.
    void SetVertexShader( const CRD11VertexShaderSPtr& CRVertexShader );

    /// Set input layout.
    void SetInputLayout( const CRD11InputLayoutSPtr& CRInputLayout, unsigned int Slot );

    /// Set pixel shader.
    void SetPixelShader( const CRD11PixelShaderSPtr& CRPixelShader );

    /// Set shader resource.
    void SetShaderResource( const CRD11ShaderResourceViewSPtr& CRShaderResourceView, unsigned int Slot );

	/// Set sampler state.
	void SetSamplerState( const CRD11SamplerStateSPtr& CRSamplerState, unsigned int Slot );

    /// Draw.
    void Draw( unsigned int Slot ) const;

    /// Clear render target.
    void ClearRenderTarget() const;

    /// Present.
    void Present() const;
private:
    /// Initialize render target.
    void _InitializeRenderTarget();
    
    /// Initialize viewport.
    void _InitializeViewport( float Width, float Height ) const;
};
