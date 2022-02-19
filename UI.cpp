#include "pch.h"
#include "UI.h"
#include "resource.h"
#include "WordleHelper.h"

namespace system
{

namespace ui
{

UINT UI::LetterEditControlIDs[ wh::Word::WordLength ] =
{
	IDC_LETTER0,
	IDC_LETTER1,
	IDC_LETTER2,
	IDC_LETTER3,
	IDC_LETTER4
};

UINT UI::ButtonFilterControlIDs[ wh::Word::WordLength ] =
{
	IDB_FILTERSTATE0,
	IDB_FILTERSTATE1,
	IDB_FILTERSTATE2,
	IDB_FILTERSTATE3,
	IDB_FILTERSTATE4
};

UINT UI::ButtonFilterImageIDs[ wh::FilterLetterState::NumEntries ] =
{
	IDB_BLOCKGREY,
	IDB_BLOCKYELLOW,
	IDB_BLOCKGREEN
};

UI::UI( const HWND dialogHandle ):
	m_dialogHandle( dialogHandle ),
	m_letterStateBitmapHandles{ NULL },
	m_helper( nullptr ),
	m_messageLogText( nullptr )
{
	m_messageLogText = new CHAR[ MessageEditControlMaxChars ];
}

UI::~UI()
{
	if( m_messageLogText != nullptr )
	{
		delete m_messageLogText;
	}
}

// Called while other objects are being initialised - other objects may not exist at
// this stage
// @param instance the program instance
bool UI::Initialise( const HINSTANCE instance )
{
	// Load the bitmap for the coloured buttons
	for( UINT filterLetterState = 0; filterLetterState < wh::FilterLetterState::NumEntries; ++filterLetterState )
	{
		FindResource( instance, MAKEINTRESOURCE( ButtonFilterImageIDs[ filterLetterState ] ), RT_BITMAP );
		m_letterStateBitmapHandles[ filterLetterState ] = LoadBitmap( instance, MAKEINTRESOURCE( ButtonFilterImageIDs[ filterLetterState ] ) );
	}
	
	RefreshFilterStateButtons();

	// Make each edit box for the letters only take one character
	for( UINT letterIndex = 0; letterIndex < wh::Word::WordLength; ++letterIndex )
	{
		const HWND editControl = GetDlgItem( m_dialogHandle, LetterEditControlIDs[ letterIndex ] );
		if( editControl != NULL )
		{
			SendMessage( editControl, EM_LIMITTEXT, 1, 0 );
		}

	}

	return true;
}

// Called after other objects have been created
void UI::PostInitialise()
{
	// Do an initial guess when everything is set up
	if( m_helper != nullptr )
	{
		m_helper->Guess();
	}

	UpdateOutoutLog();
}

void UI::LinkHelper( wh::WordleHelper& helper )
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

// Reset the UI state
void UI::OnReset()
{
	ClearInputControls();

	// Clear everything and make a new initial guess
	if( m_helper != nullptr )
	{
		m_helper->Reset();
		m_helper->Guess();
	}

	UpdateOutoutLog();
}

// Handles commands for the dialog passed here from the main WndProc
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
			}
		}
	}

	for( UINT stateButtonControlIndex = 0; stateButtonControlIndex < wh::Word::WordLength; ++stateButtonControlIndex )
	{
		if( LOWORD( wParam ) == ButtonFilterControlIDs[ stateButtonControlIndex ] )
		{
			UINT filterLetterState = m_letterInfo[ stateButtonControlIndex ].m_filterLetterState;
			++filterLetterState;
			if( filterLetterState >= wh::FilterLetterState::NumEntries )
			{
				filterLetterState = 0;
			}
			m_letterInfo[ stateButtonControlIndex ].m_filterLetterState = static_cast<wh::FilterLetterState>( filterLetterState );
			RefreshFilterStateButtons();
			break;
		}
	}

	switch( LOWORD( wParam ) )
	{
		case IDB_RESET:
			OnReset();
			break;
		case IDB_GUESS:
			OnGuess();
			break;
		default:
			break;
	}

	return true;
}

// Given an edit control ID, find the letter index
UINT UI::GetLetterIndexFromEditControlID( const UINT editControlID )
{
	UINT foundLetterIndex = 0;
	for( UINT letterIndex = 0; letterIndex < wh::Word::WordLength; ++letterIndex )
	{
		if( editControlID == LetterEditControlIDs[ letterIndex ] )
		{
			foundLetterIndex = letterIndex;
			break;
		}
	}

	return foundLetterIndex;
}

// Update the visual state of the coloured letter state buttons
void UI::RefreshFilterStateButtons()
{
	for( UINT letterIndex = 0; letterIndex < wh::Word::WordLength; ++letterIndex )
	{
		const HWND buttonHandle = GetDlgItem( m_dialogHandle, ButtonFilterControlIDs[ letterIndex ] );
		if( buttonHandle != NULL )
		{
			const wh::FilterLetterState letterFilterState = m_letterInfo[ letterIndex ].m_filterLetterState;
			SendMessage( buttonHandle, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_letterStateBitmapHandles[ letterFilterState ] );
		}
	}
}

// When 'guess' has been clicked
void UI::OnGuess()
{
	CHAR letters[ wh::Word::WordLength ];
	wh::FilterLetterState letterFilterStates[ wh::Word::WordLength ];

	for( UINT letterIndex = 0; letterIndex < wh::Word::WordLength; ++letterIndex )
	{
		letters[ letterIndex ] = m_letterInfo[ letterIndex ].m_letter;
		letterFilterStates[ letterIndex ] = m_letterInfo[ letterIndex ].m_filterLetterState;
	}
	
	wh::FilterWord filterWord( letters, letterFilterStates );
	if( m_helper != nullptr )
	{
		m_helper->Filter( filterWord );
		m_helper->Guess();
	}

	ClearInputControls();

	UpdateOutoutLog();
}

// Reset the state of the controls to blank
void UI::ClearInputControls()
{
	for( UINT controlIndex = 0; controlIndex < wh::Word::WordLength; ++controlIndex )
	{
		m_letterInfo[ controlIndex ].Reset();
		SetDlgItemText( m_dialogHandle, LetterEditControlIDs[ controlIndex ], "" );
	}

	RefreshFilterStateButtons();
}

// Put all the messages for this run in the output edit control
void UI::UpdateOutoutLog()
{
	wh::MessageLog* const messageLog = m_helper->GetMessageLog();
	messageLog->GetCombinedMessages( m_messageLogText, MessageEditControlMaxChars );

	const HWND editOutputHandle = GetDlgItem( m_dialogHandle, IDC_OUTPUT );
	SetWindowText( editOutputHandle, m_messageLogText );
	SendMessage( editOutputHandle, EM_SCROLLCARET, 0, 0 );
}

} // namespace ui

} // namepace system