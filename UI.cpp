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
	IDB_BLOCKGREY,
	IDB_BLOCKYELLOW,
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
	for( UINT filterLetterState = 0; filterLetterState < wa::FilterLetterState::NumEntries; ++filterLetterState )
	{
		FindResource( instance, MAKEINTRESOURCE( ButtonFilterImageIDs[ filterLetterState ] ), RT_BITMAP );
		m_letterStateBitmapHandles[ filterLetterState ] = LoadBitmap( instance, MAKEINTRESOURCE( ButtonFilterImageIDs[ filterLetterState ] ) );
	}

	for( UINT letterIndex = 0; letterIndex < wa::Word::WordLength; ++letterIndex )
	{
		const HWND editControl = GetDlgItem( m_dialogHandle, LetterEditControlIDs[ letterIndex ] );
		if( editControl != NULL )
		{
			SendMessage( editControl, EM_LIMITTEXT, 1, 0 );
		}

		RefreshFilterStateButtons();
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

bool UI::OnCommand( const WPARAM wParam, const LPARAM lParam )
{
	if( HIWORD( wParam ) == EN_CHANGE )
	{
		UINT editControlID = LOWORD( wParam );

		CHAR letter[ 2 ] = { 0 };
		if( GetDlgItemText( m_dialogHandle, editControlID, letter, 2 ) != 0 )
		{
			const UINT letterIndex = GetLetterIndexFromEditControlID( editControlID );
			if( m_letterInfo[ letterIndex ].m_letter != letter[ 0 ] )
			{
				m_letterInfo[ letterIndex ].m_letter = letter[ 0 ];
				DEBUG_MESSAGE( "Setting letter %u to %c\n", letterIndex, letter[ 0 ] );
			}
		}
	}

	for( UINT stateButtonControlIndex = 0; stateButtonControlIndex < wa::Word::WordLength; ++stateButtonControlIndex )
	{
		if( LOWORD( wParam ) == ButtonFilterControlIDs[ stateButtonControlIndex ] )
		{
			UINT filterLetterState = m_letterInfo[ stateButtonControlIndex ].m_filterLetterState;
			++filterLetterState;
			if( filterLetterState >= wa::FilterLetterState::NumEntries )
			{
				filterLetterState = 0;
			}
			m_letterInfo[ stateButtonControlIndex ].m_filterLetterState = static_cast<wa::FilterLetterState>( filterLetterState );
			RefreshFilterStateButtons();
			break;
		}
	}

	return true;
}

UINT UI::GetLetterIndexFromEditControlID( const UINT editControlID )
{
	UINT foundLetterIndex = 0;
	for( UINT letterIndex = 0; letterIndex < wa::Word::WordLength; ++letterIndex )
	{
		if( editControlID == LetterEditControlIDs[ letterIndex ] )
		{
			foundLetterIndex = letterIndex;
			break;
		}
	}

	return foundLetterIndex;
}

void UI::RefreshFilterStateButtons()
{
	for( UINT letterIndex = 0; letterIndex < wa::Word::WordLength; ++letterIndex )
	{
		const HWND buttonHandle = GetDlgItem( m_dialogHandle, ButtonFilterControlIDs[ letterIndex ] );
		if( buttonHandle != NULL )
		{
			const wa::FilterLetterState letterFilterState = m_letterInfo[ letterIndex ].m_filterLetterState;
			SendMessage( buttonHandle, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_letterStateBitmapHandles[ letterFilterState ] );
		}
	}
}

} // namespace ui

} // namepace system