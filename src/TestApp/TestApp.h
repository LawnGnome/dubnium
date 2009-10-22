///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb  1 2007)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __TestApp__
#define __TestApp__

// Define WX_GCH in order to support precompiled headers with GCC compiler.
// You have to create the header "wx_pch.h" and include all files needed
// for compile your gui inside it.
// Then, compile it and place the file "wx_pch.h.gch" into the same
// directory that "wx_pch.h".
#ifdef WX_GCH
#include <wx_pch.h>
#else
#include <wx/wx.h>
#endif

#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/html/htmlwin.h>
#include <wx/notebook.h>
#include <wx/button.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class TestFrame
///////////////////////////////////////////////////////////////////////////////
class TestFrame : public wxFrame 
{
	DECLARE_EVENT_TABLE()
	private:
		
		// Private event handlers
		void _wxFB_OnFileExit( wxCommandEvent& event ){ OnFileExit( event ); }
		void _wxFB_OnHelpAbout( wxCommandEvent& event ){ OnHelpAbout( event ); }
		
	
	protected:
		enum
		{
			ID_FILE_EXIT = 1000,
			ID_HELP_ABOUT,
			ID_RUN,
		};
		
		wxMenuBar* menuBar;
		wxStatusBar* statusBar;
		wxNotebook* m_notebook1;
		wxPanel* sourcePanel;
		wxTextCtrl* sourceTextCtrl;
		wxPanel* outputPanel;
		wxHtmlWindow* outputHTML;
		wxPanel* debugLogPanel;
		wxTextCtrl* debugTextCtrl;
		wxButton* run;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnFileExit( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnHelpAbout( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		TestFrame( wxWindow* parent, int id = wxID_ANY, wxString title = wxT("libDBGp Ad-Hoc Test Thingy"), wxPoint pos = wxDefaultPosition, wxSize size = wxSize( 500,300 ), int style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
	
};

#endif //__TestApp__
