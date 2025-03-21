﻿#pragma once


#include "CRD11BindingConstantBuffer.h"
#include "CRD11DepthStencilBuffer.h"
#include "CRD11Include.h"
#include "CRD11Types.h"
#include "Source/RHI/CRRHITypes.h"
#include "Source/RHI/ICRRHIRenderer.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11Renderer
//---------------------------------------------------------------------------------------------------------------------
class CRD11Renderer : public ICRRHIRenderer
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

    CRArray< ICRRHIMeshWPtr > RenderMeshes;

    CRD11BindingConstantBuffer< CRMatrix         > TransformBuffer;
    CRD11BindingConstantBuffer< CRViewProjection > ViewProjectionBuffer;
    CRD11BindingConstantBuffer< CRVector4D       > LightDirectionBuffer;
    CRD11BindingConstantBuffer< CRVector4D       > LightColorBuffer;    
    
    CRD11RenderTargetViewSPtr RenderTargetView;
    CRD11RasterizerStateWPtr  RasterizerState;

    CRD11DepthStencilBuffer DepthStencilBuffer;
    
public:
    /// Constructor
    CRD11Renderer() = default;

    /// Destructor
    virtual ~CRD11Renderer() override = default;
    
    /// Initialize renderer.
    virtual void Initialize( u32 Width, u32 Height ) override;

    /// Add render mesh.
    virtual void AddRenderMesh( const ICRRHIMeshWPtr& Mesh ) override;

    /// Remove render mesh.
    virtual void RemoveRenderMesh( const ICRRHIMeshWPtr& Mesh ) override;

    /// Draw.
    virtual void Draw() override;

    /// Clear render target.
    virtual void ClearRenderTarget() const override;

    /// Present.
    virtual void Present() const override;

    /// Get viewport width.
    virtual unsigned int GetViewportWidth() const override { return ViewportWidth; }

    /// Get viewport height.
    virtual unsigned int GetViewportHeight() const override { return ViewportHeight; }

public:
    /// Update transform buffer.
    virtual void UpdateTransformBuffer( const CRMatrix& Matrix ) override;

    /// Update view projection buffer.
    virtual void UpdateViewProjectionBuffer( const CRMatrix& ViewMatrix, const CRMatrix& ProjectionMatrix ) override;
    
private:
    /// Initialize render target.
    void _InitializeRenderTarget();

    /// Initialize viewport.
    void _InitializeViewport( f32 Width, f32 Height ) const;
};
