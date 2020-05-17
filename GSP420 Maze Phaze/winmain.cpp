//==================================================================================
// winmain.cpp
// Donald Green
// Stuart Webster
//
// 
//
//==================================================================================

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9tex.h>
#include "DebugHeader.h"
#include "GameMain.h"

// Link in the DirectX libraries required to compile
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"dxerr.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"winmm.lib")


// global variables
HINSTANCE hInst;							// application instance
HWND wndHandle;								// application window handle

// camera variables
LPDIRECT3D9             pD3D;				// the Direct3D Object
LPDIRECT3DDEVICE9       pd3dDevice;			// the Direct3D Device

////////////////////////////////////////////// forward declarations
bool initWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Create the game main
GameMain *game = new GameMain();

/*********************************************************************
* WinMain
*********************************************************************/
int WINAPI WinMain(HINSTANCE hInstance, 
				   HINSTANCE hPrevInstance, 
				   LPTSTR lpCmdLine, int nCmdShow)
{
	if (!initWindow(hInstance))
	{
		MessageBox(NULL, "Unable to create window", "ERROR", MB_OK);
		return false;
	}
	
	
	// Initialize the game object
	if (game->init(wndHandle, hInstance) == false) 
	{
		return 0;
	}

	/*if (!initDirect3D())
	{
		MessageBox(NULL, "Unable to init Direct3D", "ERROR", MB_OK);
		return false;
	}*/
	
	// turn on the debug console
	debug_on;

	// Main message loop:
    MSG msg; 
    ZeroMemory( &msg, sizeof(msg) );
    while( msg.message!=WM_QUIT )
    {
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
        {
			TranslateMessage( &msg );
            DispatchMessage( &msg );
        }		
		else
		{
			game->update();
			//debug_con << "Hello World" << "";
		} 
    }

	// Delete the game object
	if (game){
		delete game;
	}

	//// release the device and the direct3D object
	//if( pd3dDevice != NULL) 
 //       pd3dDevice->Release();

 //   if( pD3D != NULL)
 //       pD3D->Release();
		
	return (int) msg.wParam;
}

/*********************************************************************
* initWindow
*********************************************************************/
bool initWindow(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= "DirectXExample";
	wcex.hIconSm		= 0;
	RegisterClassEx(&wcex);

	// create the window
	wndHandle = CreateWindowEx(0,
							"DirectXExample", 
							 "Main_Core", 
							 WS_BORDER | WS_SYSMENU | WS_CAPTION| WS_VISIBLE,
							 CW_USEDEFAULT, 
							 CW_USEDEFAULT, 
							 800, 
							 600, 
							 NULL, 
							 NULL, 
							 hInstance, 
							 NULL);
   if (!wndHandle)
      return false;
   
   //set the client area size
	RECT rcTemp;
	SetRect(&rcTemp,0,0,800,600);//1024x768 client area
	AdjustWindowRect(&rcTemp,WS_BORDER | WS_SYSMENU | WS_CAPTION| WS_VISIBLE,FALSE);//adjust the window size based on desired client area
	SetWindowPos(wndHandle,NULL,0,0,rcTemp.right-rcTemp.left,rcTemp.bottom-rcTemp.top,SWP_NOMOVE);//set the window width and height

	if(!wndHandle) return(false);

   ShowWindow(wndHandle, SW_SHOW);
   UpdateWindow(wndHandle);

   return true;
}

/*********************************************************************
* WndProc
*********************************************************************/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	
	game->CheckWndMsg(message, wParam, lParam);

	switch (message) 
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		// handle keyboard events
		case WM_KEYDOWN:
			// switch on which key was pressed
			switch( wParam )
			{
				// handle the up arrow key; moves camera up
			case VK_UP:
				
				break;
				// handle the down arrow key; moves camera down
			case VK_DOWN:
				
				break;
				// handle the left arrow key; moves camera left
			case VK_LEFT:
				
				break;
				// handle the right arrow key; moves camera right
			case VK_RIGHT:
				
				break;
			}
			break;
	}
	
			
	return DefWindowProc(hWnd, message, wParam, lParam);
}