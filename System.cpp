#include "pch.h"
#include <CommCtrl.h>
#include "System.h"
#include "resource.h"

INT_PTR DlgProc( HWND wnd, UINT message, WPARAM wParam, LPARAM lParam );

namespace system
{

HWND g_dlg = 0;

bool Initialise( HINSTANCE instance, LPSTR cmdLine, int cmdShow )
{
	if( !memory::Heap::Create() )
	{
		return false;
	}

	InitCommonControls();

	g_dlg = CreateDialogParam( instance, MAKEINTRESOURCE( IDD_DIALOG ), GetDesktopWindow(), DlgProc, 0 );
	DWORD lastError = GetLastError();
	ShowWindow( g_dlg, cmdShow );

	return( g_dlg != NULL );
}

void Run()
{
	MSG message;
	while( GetMessage( &message, NULL, 0, 0 ) > 0 )
	{
		if( !IsDialogMessage( g_dlg, &message ) )
		{
			TranslateMessage( &message );
			DispatchMessage( &message );
		}
	}
}

void Shutdown()
{
	memory::Heap::Destroy();
}

}

INT_PTR DlgProc( HWND wnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	BOOL result = FALSE;
	switch( message )
	{
/*		case WM_CREATE:
			// Initialize the window. 
			break;
		case WM_COMMAND:
			switch( LOWORD( wParam ) )
			{
				default:
					break;
			}*/
		case WM_CLOSE:
			DestroyWindow( system::g_dlg );
			result = TRUE;
			break;
		case WM_DESTROY:
			PostQuitMessage( 0 );
			result = TRUE;
			break;
		default:
			//return DefWindowProc( wnd, message, wParam, lParam );
			break;
	}

	return result;
}