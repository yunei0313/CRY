﻿#include "framework.h"
#include "Engine.h"
#include "RHI/CRRHI.h"


#define MAX_LOADSTRING 100


//---------------------------------------------------------------------------------------------------------------------
/// global variables
//---------------------------------------------------------------------------------------------------------------------
HINSTANCE hInst;                           // 현재 인스턴스입니다.
WCHAR     szTitle      [ MAX_LOADSTRING ]; // 제목 표시줄 텍스트입니다.
WCHAR     szWindowClass[ MAX_LOADSTRING ]; // 기본 창 클래스 이름입니다.


//---------------------------------------------------------------------------------------------------------------------
/// function forward declaration
//---------------------------------------------------------------------------------------------------------------------
ATOM MyRegisterClass( HINSTANCE hInstance );
BOOL InitInstance   ( HINSTANCE, int );

LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );
INT_PTR CALLBACK About  ( HWND, UINT, WPARAM, LPARAM );


void RenderFrame();


//---------------------------------------------------------------------------------------------------------------------
/// wWinMain
//---------------------------------------------------------------------------------------------------------------------
int APIENTRY wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow )
{
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );
	
	LoadStringW( hInstance, IDS_APP_TITLE, szTitle,       MAX_LOADSTRING );
	LoadStringW( hInstance, IDC_ENGINE,    szWindowClass, MAX_LOADSTRING );
	
	MyRegisterClass( hInstance );

	if ( !InitInstance( hInstance, nCmdShow ) ) return false;

	HACCEL hAccelTable = LoadAccelerators( hInstance, MAKEINTRESOURCE( IDC_ENGINE ) );
	MSG    msg;

	while( true )
	{
		if ( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage ( &msg );
    
			if( msg.message == WM_QUIT ) break;
		}

		RenderFrame();
	}

	return (int)msg.wParam;
}


//---------------------------------------------------------------------------------------------------------------------
/// RenderFrame
//---------------------------------------------------------------------------------------------------------------------
void RenderFrame()
{
	CRRHI::RenderFrame();
}


//---------------------------------------------------------------------------------------------------------------------
/// MyRegisterClass
//---------------------------------------------------------------------------------------------------------------------
ATOM MyRegisterClass( HINSTANCE hInstance )
{
	WNDCLASSEXW wcex;
	wcex.cbSize        = sizeof( WNDCLASSEX );
	wcex.style         = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc   = WndProc;
	wcex.cbClsExtra    = 0;
	wcex.cbWndExtra    = 0;
	wcex.hInstance     = hInstance;
	wcex.hIcon         = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_ENGINE ) );
	wcex.hCursor       = LoadCursor( nullptr, IDC_ARROW );
	wcex.hbrBackground = (HBRUSH)( COLOR_WINDOW + 1 );
	wcex.lpszMenuName  = MAKEINTRESOURCEW( IDC_ENGINE );
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm       = LoadIcon( wcex.hInstance, MAKEINTRESOURCE( IDI_SMALL ) );
	
	return RegisterClassExW( &wcex );
}


//---------------------------------------------------------------------------------------------------------------------
/// InitInstance
//---------------------------------------------------------------------------------------------------------------------
BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	constexpr int width  = 1400;
	constexpr int height = 900;

	HWND hWnd = CreateWindowW( szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, width, height, nullptr, nullptr, hInstance, nullptr );
	if ( !hWnd ) return false;

	CRRHI::Initialize( hWnd, width,height );

	ShowWindow( hWnd, nCmdShow );
	UpdateWindow( hWnd );

	return TRUE;
}


//---------------------------------------------------------------------------------------------------------------------
/// WndProc
//---------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch ( message )
	{
	case WM_COMMAND:
		{
			switch ( LOWORD( wParam ) )
			{
			case IDM_ABOUT: DialogBox( hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About ); break;
			case IDM_EXIT:  DestroyWindow( hWnd ); break;
			default:        return DefWindowProc( hWnd, message, wParam, lParam );
			}
		}
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint( hWnd, &ps );
			
			EndPaint( hWnd, &ps );
		}
		break;
	case WM_DESTROY: PostQuitMessage( 0 ); break;
	default:         return DefWindowProc( hWnd, message, wParam, lParam );
	}
	return 0;
}

INT_PTR CALLBACK About( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	UNREFERENCED_PARAMETER( lParam );
	switch ( message )
	{
	case WM_INITDIALOG: return (INT_PTR)TRUE;
	case WM_COMMAND:
		{
			if ( LOWORD( wParam ) == IDOK || LOWORD( wParam ) == IDCANCEL )
			{
				EndDialog( hDlg, LOWORD( wParam ) );
				return (INT_PTR)TRUE;
			}
			break;
		}
	}
	return (INT_PTR)FALSE;
}