#pragma once

namespace wa
{
class WordleAnalyser;
}

namespace system
{

bool Initialise( HINSTANCE instance, LPSTR cmdLine, int cmdShow );
void LinkHelper( wa::WordleAnalyser& helper );
void UnlinkHelper();
UINT Run();
void Shutdown();

}
