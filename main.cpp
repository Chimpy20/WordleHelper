#include "pch.h"
#include "WordleAnalyser.h"
#include "System.h"

int WINAPI WinMain( _In_ HINSTANCE instance, _In_opt_ HINSTANCE prevInstance, _In_ LPSTR cmdLine, _In_ int cmdShow )
{
	UNREFERENCED_PARAMETER( prevInstance );

	// Start up any platform related systems and begin the program
	if( system::Initialise( instance, cmdLine, cmdShow ) )
	{
		wa::WordleAnalyser wordleAnalyser;

		wordleAnalyser.Initialise();

		// Perform the main loop until we exit the program
		system::Run();

		wordleAnalyser.Run();

		wordleAnalyser.Shutdown();

		// Close down and clean up
		system::Shutdown();
	}

	return 0;
}

void WINAPI WinMainCRTStartup()
{
	STARTUPINFO				startupInfo = { sizeof( STARTUPINFO ),0 };

	GetStartupInfo( &startupInfo );

	char* commandLine = GetCommandLine();

	// Skip past program name (first token in command line).
	if( *commandLine == '"' ) // check for and handle quoted program name
	{
		// Scan, and skip over, subsequent characters until another
		// double-quote or a null is encountered
		while( *commandLine && ( *commandLine != '"' ) )
			commandLine++;

		// If we stopped on a double-quote (usual case), skip over it.
		if( *commandLine == '"' )
			commandLine++;
	}
	else
	{
		// First token wasn't a quote
		while( *commandLine > ' ' )
			commandLine++;
	}

	// Skip past any white space preceeding the second token.
	while( *commandLine && ( *commandLine <= ' ' ) )
		commandLine++;

	int result = WinMain( GetModuleHandle( NULL ), 0, commandLine, startupInfo.dwFlags & STARTF_USESHOWWINDOW
		? startupInfo.wShowWindow : SW_SHOWDEFAULT );

	ExitProcess( result );
}