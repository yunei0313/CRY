﻿#pragma once


#include "CRD11Include.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11Device
//---------------------------------------------------------------------------------------------------------------------
class CRD11Device
{
private:
	ID3D11Device*        DevicePtr        = nullptr;
	ID3D11DeviceContext* DeviceContextPtr = nullptr;
	IDXGISwapChain*      SwapChainPtr     = nullptr;

public:
	/// Constructor
	CRD11Device() = default;
	
	/// Create Dx11 device ojects.
	bool Create( HWND hWnd );
	
	//-----------------------------------------------------------------------------------------------------------------
	/// Getter
	//-----------------------------------------------------------------------------------------------------------------
	/// Get Dx11 device object.
	ID3D11Device* GetDevice() const { return DevicePtr; }

	/// Get Dx11 device context object.
	ID3D11DeviceContext* GetDeviceContext() const { return DeviceContextPtr; }

	/// Get Dx11 swap chain object.
	IDXGISwapChain* GetSwapChain() const { return SwapChainPtr; }
};
