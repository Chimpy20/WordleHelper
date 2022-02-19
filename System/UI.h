#pragma once

#include "FilterWord.h"

namespace wh
{
class WordleHelper;
}

namespace system
{

namespace ui
{

class UI
{
public:
	static UINT					LetterEditControlIDs[ wh::Word::WordLength ];
	static UINT					ButtonFilterControlIDs[ wh::Word::WordLength ];
	static UINT					ButtonFilterImageIDs[ wh::FilterLetterState::NumEntries ];

	UI( const HWND dialogHandle );
	~UI();

	bool						Initialise( const HINSTANCE instance );
	void						PostInitialise();
	void						LinkHelper( wh::WordleHelper& helper );
	void						UnlinkHelper();
	void						Shutdown();

	void						OnReset();

	bool						OnCommand( const WPARAM wParam, const LPARAM lParam );
	void						OnKeyUp( const DWORD keyCode );

	bool						OnGuess();

private:
	static const UINT			MessageEditControlMaxChars = 2048;
	class LetterInfo
	{
	public:
		void Reset()
		{
			m_filterLetterState = wh::FilterLetterState::Incorrect;
			m_letter = '\0';
		}
		wh::FilterLetterState	m_filterLetterState = wh::FilterLetterState::Incorrect;
		CHAR					m_letter = '\0';
	};

	UINT						GetLetterIndexFromEditControlID( const UINT editControlID );
	void						RefreshFilterStateButtons();
	void						ClearInputControls();
	void						UpdateOutoutLog();

	HWND						m_dialogHandle;
	HBITMAP						m_letterStateBitmapHandles[ wh::FilterLetterState::NumEntries ];
	LetterInfo					m_letterInfo[ wh::Word::WordLength ]{};
	wh::WordleHelper*			m_helper;
	CHAR*						m_messageLogText;
};

} // namespace ui

} // namepace system
