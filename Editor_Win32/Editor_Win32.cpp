﻿#include "framework.h"
#include "Editor_Win32.h"
#include "Input/CRInputProcessorCamera.h"
#include "UI/CREditorUI.h"
#include "UI/CRUIManager.h"
#include <Extras/ImGUI/imgui.h>
#include <Source/RHI/CRRHI.h>
#include <Engine.h>


#define MAX_LOADSTRING 100


//---------------------------------------------------------------------------------------------------------------------
/// global variables
//---------------------------------------------------------------------------------------------------------------------
HINSTANCE hInst;                           // 현재 인스턴스입니다.
WCHAR     szTitle      [ MAX_LOADSTRING ]; // 제목 표시줄 텍스트입니다.
WCHAR     szWindowClass[ MAX_LOADSTRING ]; // 기본 창 클래스 이름입니다.


DirectX::Keyboard GKeyboard;
DirectX::Mouse    GMouse;

CRUIManager GUIManager;
CRInputProcessorCamera GInputProcessorCamera;

//---------------------------------------------------------------------------------------------------------------------
/// function forward declaration
//---------------------------------------------------------------------------------------------------------------------
ATOM MyRegisterClass( HINSTANCE hInstance );
BOOL InitInstance   ( HINSTANCE, int );

LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );
INT_PTR CALLBACK About  ( HWND, UINT, WPARAM, LPARAM );


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

    float deltaSeconds = 0.f;
    
    while( true )
    {
        if ( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
	    {
		    TranslateMessage( &msg );
		    DispatchMessage ( &msg );

		    if( msg.message == WM_QUIT ) break;
	    }

        
        GFrameTime.Start();

        CREngine::Tick( deltaSeconds );

        GInputProcessorCamera.Tick( deltaSeconds );

        if ( CREngine::PreRender( deltaSeconds ) )
        {
            CREngine::Render( deltaSeconds );

            GUIManager.Draw();
            
            CREngine::PostRender( deltaSeconds );
        }
        
        deltaSeconds = GFrameTime.Finish();
    }

    return (int)msg.wParam;
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

    GMouse.SetWindow( hWnd );

    CREngine::Initialize( hWnd, width, height );

    GUIManager.AddUI( new CREditorUI() );

    ShowWindow( hWnd, nCmdShow );
    UpdateWindow( hWnd );

    return TRUE;
}


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );


//---------------------------------------------------------------------------------------------------------------------
/// WndProc
//---------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    if ( ImGui_ImplWin32_WndProcHandler( hWnd, message, wParam, lParam ) )
        return true;
    
    switch ( message )
    {
    case WM_ACTIVATEAPP:
        {
            DirectX::Keyboard::ProcessMessage( message, wParam, lParam );
            DirectX::Mouse   ::ProcessMessage( message, wParam, lParam );
        }
        break;
    case WM_ACTIVATE:
    case WM_INPUT:
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_MOUSEWHEEL:
    case WM_XBUTTONDOWN:
    case WM_XBUTTONUP:
    case WM_MOUSEHOVER:
        {
            DirectX::Mouse::ProcessMessage( message, wParam, lParam );
        }
        break;
    case WM_MOUSEACTIVATE:
        return MA_ACTIVATEANDEAT;
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYUP:
        {
            DirectX::Keyboard::ProcessMessage( message, wParam, lParam );
        }
        break;
    case WM_SYSKEYDOWN:
        {
            DirectX::Keyboard::ProcessMessage( message, wParam, lParam );
    
            if ( wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000 )
            {
            }
        }
        break;
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

//---------------------------------------------------------------------------------------------------------------------
/// About
//---------------------------------------------------------------------------------------------------------------------
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
