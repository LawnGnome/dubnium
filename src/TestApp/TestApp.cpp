///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb  1 2007)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

#include "TestApp.h"

///////////////////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE( TestFrame, wxFrame )
	EVT_MENU( ID_FILE_EXIT, TestFrame::_wxFB_OnFileExit )
	EVT_MENU( ID_HELP_ABOUT, TestFrame::_wxFB_OnHelpAbout )
END_EVENT_TABLE()

TestFrame::TestFrame( wxWindow* parent, int id, wxString title, wxPoint pos, wxSize size, int style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	menuBar = new wxMenuBar( 0 );
	wxMenu* fileMenu;
	fileMenu = new wxMenu();
	wxMenuItem* exitItem = new wxMenuItem( fileMenu, ID_FILE_EXIT, wxString( _("&Exit") ) + wxT('\t') + wxT("CTRL+Q"), wxEmptyString, wxITEM_NORMAL );
	fileMenu->Append( exitItem );
	menuBar->Append( fileMenu, _("&File") );
	wxMenu* helpMenu;
	helpMenu = new wxMenu();
	wxMenuItem* aboutItem = new wxMenuItem( helpMenu, ID_HELP_ABOUT, wxString( _("&About...") ) + wxT('\t') + wxT("F1"), wxEmptyString, wxITEM_NORMAL );
	helpMenu->Append( aboutItem );
	menuBar->Append( helpMenu, _("&Help") );
	this->SetMenuBar( menuBar );
	
	statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_notebook1 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	sourcePanel = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	sourceTextCtrl = new wxTextCtrl( sourcePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxTE_DONTWRAP|wxTE_MULTILINE|wxTE_READONLY );
	sourceTextCtrl->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Courier New") ) );
	
	bSizer2->Add( sourceTextCtrl, 1, wxALL|wxEXPAND, 5 );
	
	sourcePanel->SetSizer( bSizer2 );
	sourcePanel->Layout();
	bSizer2->Fit( sourcePanel );
	m_notebook1->AddPage( sourcePanel, _("Source"), false );
	outputPanel = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	outputHTML = new wxHtmlWindow( outputPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO);
	bSizer3->Add( outputHTML, 1, wxALL|wxEXPAND, 5 );
	
	outputPanel->SetSizer( bSizer3 );
	outputPanel->Layout();
	bSizer3->Fit( outputPanel );
	m_notebook1->AddPage( outputPanel, _("Output"), false );
	debugLogPanel = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	debugTextCtrl = new wxTextCtrl( debugLogPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxTE_DONTWRAP|wxTE_MULTILINE|wxTE_READONLY );
	debugTextCtrl->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Courier New") ) );
	
	bSizer4->Add( debugTextCtrl, 1, wxALL|wxEXPAND, 5 );
	
	debugLogPanel->SetSizer( bSizer4 );
	debugLogPanel->Layout();
	bSizer4->Fit( debugLogPanel );
	m_notebook1->AddPage( debugLogPanel, _("Debug Log"), false );
	
	bSizer1->Add( m_notebook1, 1, wxEXPAND | wxALL, 0 );
	
	run = new wxButton( this, ID_RUN, _("&Run"), wxDefaultPosition, wxDefaultSize, 0 );
	run->Enable( false );
	
	bSizer1->Add( run, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
}
