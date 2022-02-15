#pragma once

#include "FilterWord.h"

namespace wa
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
	static UINT					LetterEditControlIDs[ wa::Word::WordLength ];
	static UINT					ButtonFilterControlIDs[ wa::Word::WordLength ];
	static UINT					ButtonFilterImageIDs[ wa::FilterLetterState::NumEntries ];

	UI( const HWND dialogHandle );
	~UI();

	bool						Initialise( const HINSTANCE instance );
	void						PostInitialise();
	void						LinkHelper( wa::WordleHelper& helper );
	void						UnlinkHelper();
	void						Shutdown();

	void						Reset();

	bool						OnCommand( const WPARAM wParam, const LPARAM lParam );

	void						Guess();

private:
	class LetterInfo
	{
	public:
		void Reset()
		{
			m_filterLetterState = wa::FilterLetterState::Incorrect;
			m_letter = '\0';
		}
		wa::FilterLetterState	m_filterLetterState = wa::FilterLetterState::Incorrect;
		CHAR					m_letter = '\0';
	};

	UINT						GetLetterIndexFromEditControlID( const UINT editControlID );
	void						RefreshFilterStateButtons();

	HWND						m_dialogHandle;
	HBITMAP						m_letterStateBitmapHandles[ wa::FilterLetterState::NumEntries ];
	LetterInfo					m_letterInfo[ wa::Word::WordLength ]{};
	wa::WordleHelper*			m_helper;
};

} // namespace ui

} // namepace system
