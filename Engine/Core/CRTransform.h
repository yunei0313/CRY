﻿#pragma once


#include "../RHI/DX11/CRD11Include.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRTransform
//---------------------------------------------------------------------------------------------------------------------
class CRTransform
{
private:
    CRVector     Location = CRVector::Zero;
    CRQuaternion Rotation = CRQuaternion::Identity;
    CRVector     Scale    = CRVector::One;

public:
    static CRTransform Identity;

public:
    /// Constructor.
    CRTransform() = default;
    
    /// Return converted matrix. 
    CRMatrix ToLocalMatrix() const;

    /// Get location.
    const CRVector& GetLocation() const { return Location; }

    /// Get Rotation.
    const CRQuaternion& GetRotation() const { return Rotation; }

    /// Get scale.
    const CRVector& GetScale() const { return Scale; }

    /// Translate.
    void Translate( const CRVector& InTranslation ) { Location += InTranslation; }

    /// Set location.
    void SetLocation( const CRVector& InLocation ) { Location = InLocation; }

    /// Set location.
    void SetLocation( float X, float Y, float Z ) { Location = CRVector( X, Y, Z ); }

    /// Set rotation.
    void SetRotation( const CRQuaternion& InRotation ) { Rotation = InRotation; }

    /// Rotate.
    void Rotate( const CRQuaternion& InRotation ) { Rotation *= InRotation; }

    /// Rotate.
    void Rotate( const CRVector& Axis, float Angle ) { Rotation *= CRQuaternion::CreateFromAxisAngle( Axis, Angle ); }

    /// Set scale.
    void SetScale( const CRVector& InScale ) { Scale = InScale; }
};
