#include "pch.h"
#include <CommCtrl.h>
#include "System.h"
#include "UI.h"
#include "resource.h"
#include "WordleHelper.h"

namespace system
{

ui::UI* g_userInterface = nullptr;
HWND g_dialog = NULL;

INT_PTR DlgProc( HWND wnd, UINT message, WPARAM wParam, LPARAM lParam );

bool Initialise( HINSTANCE instance, LPSTR cmdLine, int cmdShow )
{
	if( !memory::Heap::Create() )
	{
		return false;
	}

	InitCommonControls();

	const HRSRC dialogResourceInfo = FindResource( instance, MAKEINTRESOURCE( IDD_DIALOG1 ), RT_DIALOG );
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

	if( g_dialog != NULL )
	{
		ShowWindow( g_dialog, cmdShow );
		UpdateWindow( g_dialog );
	}
#ifdef _DEBUG
	else
	{
		const DWORD lastError = GetLastError();
		io::OutputMessage( "Unable to create dialog, error 0x%08x\n", lastError );
	}
#endif

	g_userInterface = new ui::UI( g_dialog );
	g_userInterface->Initialise( instance );

	return( g_dialog != NULL );
}

void LinkHelper( wa::WordleHelper& helper )
{
	g_userInterface->LinkHelper( helper );
}

void UnlinkHelper()
{
	g_userInterface->UnlinkHelper();
}

UINT Run()
{
	g_userInterface->PostInitialise();

	MSG message;
	while( GetMessage( &message, NULL, 0, 0 ) > 0 )
	{
		if( !IsDialogMessage( g_dialog, &message ) )
		{
			TranslateMessage( &message );
			DispatchMessage( &message );
		}
	}

	return LOWORD( message.wParam );
}

void Shutdown()
{
	if( g_userInterface != nullptr )
	{
		g_userInterface->Shutdown();
		delete g_userInterface;
	}

	DestroyWindow( g_dialog );
	g_dialog = NULL;

	memory::Heap::Destroy();
}

INT_PTR DlgProc( HWND wnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	BOOL result = FALSE;
	switch( message )
	{
		case WM_CREATE:
			break;
		case WM_INITDIALOG:
			result = TRUE;
			break;
		case WM_COMMAND:
		{
			WORD param = LOWORD( wParam );
			//DEBUG_MESSAGE( "WM_COMMAND wParam = %u, lParam = %u\n", LOWORD( wParam ), lParam );
			switch( param )
			{
				case IDEXIT:
					CloseWindow( wnd );
					DestroyWindow( wnd );
					break;
				default:
					break;
			}
			if( g_userInterface != nullptr )
				g_userInterface->OnCommand( wParam, lParam );
			break;
		}
		case WM_CLOSE:
			EndDialog( g_dialog, 0 );
			DestroyWindow( g_dialog );
			break;
		case WM_DESTROY:
			PostQuitMessage( 0 );
			result = TRUE;
			break;
		default:
			break;
	}

	return result;
}

}

#ifdef __cplusplus
extern "C"
{
	#pragma function(memset)
	void* __cdecl memset( _Out_writes_bytes_all_( count ) void* dest, _In_ int value, _In_ size_t count )
	{
		char* bytes = (char*)dest;
		while( count-- )
		{
			*bytes++ = (char)value;
		}
		return dest;
	}

	int _fltused = 0;
}
#endif //__cplusplus