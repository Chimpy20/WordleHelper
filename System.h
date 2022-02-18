#pragma once

namespace wh
{
class WordleHelper;
}

namespace system
{

bool Initialise( HINSTANCE instance, LPSTR cmdLine, int cmdShow );
void LinkHelper( wh::WordleHelper& helper );
void UnlinkHelper();
const CHAR* OpenTextFile( INT resourceID, UINT& dataSize );
void CloseTextFile( const INT resourceID );
UINT Run();
void Shutdown();

}
