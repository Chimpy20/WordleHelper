#include "pch.h"
#include <CommCtrl.h>
#include "System.h"
#include "resource.h"

namespace system
{

HWND g_dialog = NULL;

INT_PTR DlgProc( HWND wnd, UINT message, WPARAM wParam, LPARAM lParam );

bool Initialise( HINSTANCE instance, LPSTR cmdLine, int cmdShow )
{
	if( !memory::Heap::Create() )
	{
		return false;
	}

	InitCommonControls();

	const HRSRC dialogResourceInfo = FindResource( instance, MAKEINTRESOURCE( IDD_DIALOG1 ), "RT_DIALOG" );
	if( dialogResourceInfo != NULL )
	{
		const HGLOBAL dialogResource = LoadResource( instance, dialogResourceInfo );
		if( dialogResource == NULL )
		{
#ifdef _DEBUG
			const DWORD lastError = GetLastError();
			io::OutputMessage( "Unable to find dialog resource, error 0x%08x\n", lastError );
#endif
		}
	}
#ifdef _DEBUG
	else
	{
		const DWORD lastError = GetLastError();
		io::OutputMessage( "Unable to find dialog resource, error 0x%08x\n", lastError );
	}
#endif

	g_dialog = CreateDialog( instance, MAKEINTRESOURCE( IDD_DIALOG1 ), GetDesktopWindow(), DlgProc );

#ifdef _DEBUG
	if( g_dialog == NULL )
	{
		const DWORD lastError = GetLastError();
		io::OutputMessage( "Unable to create dialog, error 0x%08x\n", lastError );
	}
#endif

	ShowWindow( g_dialog, cmdShow );
	UpdateWindow( g_dialog );

	return( g_dialog != NULL );
}

void Run()
{
	MSG message;
	while( GetMessage( &message, NULL, 0, 0 ) > 0 )
	{
		if( !IsDialogMessage( g_dialog, &message ) )
		{
			TranslateMessage( &message );
			DispatchMessage( &message );
		}
	}
}

void Shutdown()
{
	DestroyWindow( g_dialog );
	g_dialog = NULL;
	memory::Heap::Destroy();
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
			DestroyWindow( g_dialog );
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

}

#ifdef __cplusplus
extern "C"
{
#pragma function(memset)
	void* memset( void* dest, int c, size_t count )
	{
		char* bytes = (char*)dest;
		while( count-- )
		{
			*bytes++ = (char)c;
		}
		return dest;
	}

	int _fltused = 0;
}
#endif //__cplusplus