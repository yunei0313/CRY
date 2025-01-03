﻿#pragma once


#include "CRD11BindingConstantBuffer.h"
#include "CRD11Include.h"
#include "CRD11Mesh.h"
#include "CRD11Types.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11Renderer
//---------------------------------------------------------------------------------------------------------------------
class CRD11Renderer
{
public:
    struct CRViewProjection
    {
        CRMatrix View;
        CRMatrix Projection;
    };
    
private:
    unsigned int ViewportWidth  = 1920;
    unsigned int ViewportHeight = 1080;

    CRD11Mesh Mesh;

    CRD11BindingConstantBuffer< CRMatrix         > TransformBuffer;
    CRD11BindingConstantBuffer< CRViewProjection > ViewProjectionBuffer;
    CRD11BindingConstantBuffer< CRVector4D       > LightDirectionBuffer;
    CRD11BindingConstantBuffer< CRVector4D       > LightColorBuffer;    
    
    CRD11RenderTargetViewSPtr RenderTargetView;
    
public:
    /// Initialize renderer.
    void Initialize( unsigned int Width, unsigned int Height );

    /// Update transform buffer.
    void UpdateTransformBuffer( const CRMatrix& Matrix );

    /// Update view projection buffer.
    void UpdateViewProjectionBuffer( const CRMatrix& ViewMatrix, const CRMatrix& ProjectionMatrix );

    /// Draw.
    void Draw() const;

    /// Clear render target.
    void ClearRenderTarget() const;

    /// Present.
    void Present() const;

    /// Get viewport width.
    unsigned int GetViewportWidth() const { return ViewportWidth; }

    /// Get viewport height.
    unsigned int GetViewportHeight() const { return ViewportHeight; }
    
private:
    /// Initialize render target.
    void _InitializeRenderTarget();

    /// Initialize viewport.
    void _InitializeViewport( float Width, float Height ) const;
};
