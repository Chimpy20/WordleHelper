#include "pch.h"
#include "System.h"
#include "resource.h"

INT_PTR DlgProc( HWND wnd, UINT message, WPARAM wParam, LPARAM lParam );

namespace system
{

bool Initialise( HINSTANCE instance, LPSTR cmdLine, int cmdShow )
{
	if( !memory::Heap::Create() )
	{
		return false;
	}

	DialogBox( instance, MAKEINTRESOURCE(IDD_DIALOG), GetDesktopWindow(), DlgProc );

	return true;
}

void Run()
{
	MSG message;
	while( GetMessage( &message, NULL, 0, 0 ) > 0 )
	{
		TranslateMessage( &message );
		DispatchMessage( &message );
	}
}

void Shutdown()
{
	memory::Heap::Destroy();
}

}

INT_PTR DlgProc( HWND wnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		case WM_CREATE:
			// Initialize the window. 
			break;
		case WM_COMMAND:
			switch( LOWORD( wParam ) )
			{
				default:
					break;
			}
		default:
			//return DefWindowProc( wnd, message, wParam, lParam );
			break;
	}

	return 0;
}