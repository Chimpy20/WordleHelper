#include "pch.h"
#include "WordleAnalyser.h"
#include "System.h"

void WINAPI WinMainCRTStartup()
{
	STARTUPINFO				startupInfo = { sizeof( STARTUPINFO ),0 };

	GetStartupInfo( &startupInfo );

	int Result = WinMain( GetModuleHandle( NULL ), 0, 0, startupInfo.dwFlags & STARTF_USESHOWWINDOW
		? startupInfo.wShowWindow : SW_SHOWDEFAULT );

	ExitProcess( Result );
}

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow )
{
	UNREFERENCED_PARAMETER( prevInstance );

	if( system::Initialise( instance, cmdLine, cmdShow ) )
	{
		wa::WordleAnalyser wordleAnalyser;

		wordleAnalyser.Initialise();

		system::Run();

		wordleAnalyser.Run();

		wordleAnalyser.Shutdown();

		system::Shutdown();
	}

	return 0;
}