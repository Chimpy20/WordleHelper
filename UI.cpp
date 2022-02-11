#include "pch.h"
#include "UI.h"
#include "resource.h"

namespace system
{

namespace ui
{

DWORD UI::LetterEditControlIDs[ wa::Word::WordLength ] =
{
	IDC_LETTER0,
	IDC_LETTER1,
	IDC_LETTER2,
	IDC_LETTER3,
	IDC_LETTER4
};

DWORD UI::ButtonFilterControlIDs[ wa::Word::WordLength ] =
{
	IDB_FILTERSTATE0,
	IDB_FILTERSTATE1,
	IDB_FILTERSTATE2,
	IDB_FILTERSTATE3,
	IDB_FILTERSTATE4
};

DWORD UI::ButtonFilterImageIDs[ wa::FilterLetterState::NumEntries ] =
{
	IDB_BLOCKGREEN,
	IDB_BLOCKGREEN,
	IDB_BLOCKGREEN
};

UI::UI( const HWND dialogHandle ):
	m_dialogHandle( dialogHandle )
{
}

UI::~UI()
{
}

bool UI::Initialise( const HINSTANCE instance )
{
	FindResource( instance, MAKEINTRESOURCE( IDB_BLOCKGREEN ), RT_BITMAP );
	HBITMAP bitmap = LoadBitmap( instance, MAKEINTRESOURCE( IDB_BLOCKGREEN ) );

	for( UINT filterButtonIndex = 0; filterButtonIndex < wa::Word::WordLength; ++filterButtonIndex )
	{
		const HWND buttonHandle = GetDlgItem( m_dialogHandle, ButtonFilterControlIDs[ filterButtonIndex ] );
		if( buttonHandle != NULL )
		{
			SendMessage( buttonHandle, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bitmap );
		}
	}

	Reset();
	return true;
}

void UI::Shutdown()
{
}

void UI::Reset()
{
	for( UINT letterEditControlIndex = 0; letterEditControlIndex < wa::Word::WordLength; ++letterEditControlIndex )
	{
		SetDlgItemText( m_dialogHandle, LetterEditControlIDs[ letterEditControlIndex ], "" );
	}
}

} // namespace ui

} // namepace system