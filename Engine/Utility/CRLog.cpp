﻿#include "CRLog.h"
#include <comdef.h>

CRLog GLog;


//---------------------------------------------------------------------------------------------------------------------
/// Add log.
//---------------------------------------------------------------------------------------------------------------------
void CRLog::AddLog( const CRString& Log )
{
	Logs.Add( Log );
}

//---------------------------------------------------------------------------------------------------------------------
/// Add error log.
//---------------------------------------------------------------------------------------------------------------------
void CRLog::AddErrorLog( HRESULT HR )
{
	_com_error comError( HR );

	ErrorLogs.Add( comError.ErrorMessage() );
}