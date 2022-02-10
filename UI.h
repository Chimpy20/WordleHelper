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

private:
	class LetterInfo
	{
	public:
		wa::FilterLetterState	m_filterLetterState = wa::FilterLetterState::Incorrect;
	};

	HWND						m_dialogHandle;
	LetterInfo					m_letterInfo[ wa::Word::WordLength ];
};

} // namespace ui

} // namepace system
