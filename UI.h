#pragma once

#include "FilterWord.h"

namespace system
{

namespace ui
{

class UI
{
public:
	static DWORD				LetterEditControlIDs[ wa::Word::WordLength ];
	static DWORD				ButtonFilterControlIDs[ wa::Word::WordLength ];
	static DWORD				ButtonFilterImageIDs[ wa::FilterLetterState::NumEntries ];

	UI( const HWND dialogHandle );
	~UI();

	bool						Initialise( const HINSTANCE instance );
	void						Shutdown();

	void						Reset();

	bool						OnCommand( const WPARAM wParam, const LPARAM lParam );

private:
	class LetterInfo
	{
	public:
		wa::FilterLetterState	m_filterLetterState = wa::FilterLetterState::Incorrect;
		CHAR					m_letter = '\0';
	};

	UINT						GetLetterIndexFromEditControlID( const UINT editControlID );
	void						RefreshFilterStateButtons();

	HWND						m_dialogHandle;
	HBITMAP						m_letterStateBitmapHandles[ wa::FilterLetterState::NumEntries ];
	LetterInfo					m_letterInfo[ wa::Word::WordLength ];
};

} // namespace ui

} // namepace system
