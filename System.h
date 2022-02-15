#pragma once

namespace wa
{
class WordleHelper;
}

namespace system
{

bool Initialise( HINSTANCE instance, LPSTR cmdLine, int cmdShow );
void LinkHelper( wa::WordleHelper& helper );
void UnlinkHelper();
UINT Run();
void Shutdown();

}
