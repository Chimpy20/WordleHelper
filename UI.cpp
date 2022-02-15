#include "pch.h"
#include "UI.h"
#include "resource.h"
#include "WordleHelper.h"

namespace system
{

namespace ui
{

UINT UI::LetterEditControlIDs[ wa::Word::WordLength ] =
{
	IDC_LETTER0,
	IDC_LETTER1,
	IDC_LETTER2,
	IDC_LETTER3,
	IDC_LETTER4
};

UINT UI::ButtonFilterControlIDs[ wa::Word::WordLength ] =
{
	IDB_FILTERSTATE0,
	IDB_FILTERSTATE1,
	IDB_FILTERSTATE2,
	IDB_FILTERSTATE3,
	IDB_FILTERSTATE4
};

UINT UI::ButtonFilterImageIDs[ wa::FilterLetterState::NumEntries ] =
{
	IDB_BLOCKGREY,
	IDB_BLOCKYELLOW,
	IDB_BLOCKGREEN
};

UI::UI( const HWND dialogHandle ):
	m_dialogHandle( dialogHandle ),
	m_letterStateBitmapHandles{NULL},
	m_helper(nullptr)
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
	
	RefreshFilterStateButtons();

	for( UINT letterIndex = 0; letterIndex < wa::Word::WordLength; ++letterIndex )
	{
		const HWND editControl = GetDlgItem( m_dialogHandle, LetterEditControlIDs[ letterIndex ] );
		if( editControl != NULL )
		{
			SendMessage( editControl, EM_LIMITTEXT, 1, 0 );
		}

	}

	return true;
}

void UI::PostInitialise()
{
	if( m_helper != nullptr )
	{
		m_helper->Guess();
	}
}

void UI::LinkHelper( wa::WordleHelper& helper )
{
	m_helper = &helper;
}

void UI::UnlinkHelper()
{
	m_helper = nullptr;
}

void UI::Shutdown()
{
}

void UI::Reset()
{
	for( UINT controlIndex = 0; controlIndex < wa::Word::WordLength; ++controlIndex )
	{
		m_letterInfo[ controlIndex ].Reset();
		SetDlgItemText( m_dialogHandle, LetterEditControlIDs[ controlIndex ], "" );
	}

	RefreshFilterStateButtons();

	if( m_helper != nullptr )
	{
		m_helper->Reset();
		m_helper->Guess();
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

	switch( LOWORD( wParam ) )
	{
		case IDB_RESET:
			Reset();
			break;
		case IDB_GUESS:
			Guess();
			break;
		default:
			break;
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

void UI::Guess()
{
	CHAR letters[ wa::Word::WordLength ];
	wa::FilterLetterState letterFilterStates[ wa::Word::WordLength ];

	for( UINT letterIndex = 0; letterIndex < wa::Word::WordLength; ++letterIndex )
	{
		letters[ letterIndex ] = m_letterInfo[ letterIndex ].m_letter;
		letterFilterStates[ letterIndex ] = m_letterInfo[ letterIndex ].m_filterLetterState;
	}
	
	wa::FilterWord filterWord( letters, letterFilterStates );
	if( m_helper != nullptr )
	{
		m_helper->Filter( filterWord );
		m_helper->Guess();
	}
}

} // namespace ui

} // namepace system