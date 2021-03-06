
#include "settings.h"
#include "enums.h"
#include "util.h"

#include "globalvars.h" 

IMPLEMENT_CLASS(Settings_Page1, wxWindow)
IMPLEMENT_CLASS(Settings_Page2, wxWindow)
IMPLEMENT_CLASS(SettingsControl, wxWindow)

BEGIN_EVENT_TABLE(Settings_Page1, wxWindow)
	EVT_BUTTON(ID_SETTINGS_UP, Settings_Page1::OnButton)
	EVT_BUTTON(ID_SETTINGS_DOWN, Settings_Page1::OnButton)
	EVT_BUTTON(ID_SETTINGS_ADD, Settings_Page1::OnButton)
	EVT_BUTTON(ID_SETTINGS_REMOVE, Settings_Page1::OnButton)
	EVT_BUTTON(ID_SETTINGS_CLEAR, Settings_Page1::OnButton)
	EVT_CHECKBOX(ID_SETTINGS_RANDOMSKIN, Settings_Page1::OnCheck)
	EVT_CHECKBOX(ID_SETTINGS_LOCALFILES, Settings_Page1::OnCheck)
END_EVENT_TABLE()


BEGIN_EVENT_TABLE(Settings_Page2, wxWindow)
	EVT_BUTTON(ID_SETTINGS_APPLY, Settings_Page2::OnButton)
END_EVENT_TABLE()


BEGIN_EVENT_TABLE(SettingsControl, wxWindow)
	
END_EVENT_TABLE()


Settings_Page1::Settings_Page1(wxWindow* parent, wxWindowID id)
{
	if (Create(parent, id, wxPoint(0,0), wxSize(400,400), 0, _T("Settings_Page1")) == false) {
		wxLogMessage(_T("GUI Error: Settings_Page1"));
		return;
	}

	chkbox[CHECK_RANDOMCHAR] = new wxCheckBox(this, ID_SETTINGS_RANDOMCHAR, _T("Random Characters"), wxPoint(5,50), wxDefaultSize, 0);
	chkbox[CHECK_RANDOMSKIN] = new wxCheckBox(this, ID_SETTINGS_RANDOMSKIN, _T("Random Skins"), wxPoint(150,50), wxDefaultSize, 0);
	chkbox[CHECK_LOCALFILES] = new wxCheckBox(this, ID_SETTINGS_LOCALFILES, _T("Use Local Files"), wxPoint(5,75), wxDefaultSize, 0);

	chkbox[CHECK_INITSTARTUP] = new wxCheckBox(this, ID_SETTINGS_INITSTARTUP, _T("Init on Startup"), wxPoint(5,100), wxDefaultSize, 0);
	chkbox[CHECK_SAVELAYOUT] = new wxCheckBox(this, ID_SETTINGS_SAVELAYOUT, _T("Save Layout on Exit"), wxPoint(150,100), wxDefaultSize, 0);
	
	wxStaticText *lbl2 = new wxStaticText(this, wxID_ANY, _T("MPQ Archives order and files to load"), wxPoint(0,140), wxDefaultSize, 0);
	mpqList = new wxListBox(this, wxID_ANY, wxPoint(0,160), wxSize(380, 190), mpqArchives, wxLB_SINGLE|wxLB_HSCROLL, wxDefaultValidator);
	wxButton *btn1 = new wxButton(this, ID_SETTINGS_UP, _T("Move Up"), wxPoint(5,355), wxSize(70,25), 0);
	wxButton *btn2 = new wxButton(this, ID_SETTINGS_DOWN, _T("Move Down"), wxPoint(80,355), wxSize(70,25), 0);
	wxButton *btn3 = new wxButton(this, ID_SETTINGS_ADD, _T("Add"), wxPoint(155,355), wxSize(70,25), 0);
	wxButton *btn4 = new wxButton(this, ID_SETTINGS_REMOVE, _T("Remove"), wxPoint(230,355), wxSize(70,25), 0);
	wxButton *btn5 = new wxButton(this, ID_SETTINGS_CLEAR, _T("Clear"), wxPoint(305,355), wxSize(70,25), 0);
}


void Settings_Page1::OnButton(wxCommandEvent &event)
{
	int id = event.GetId();

	if (id==ID_SETTINGS_UP) {
		int sel = mpqList->GetSelection();
		if (sel > 0) {
			wxString temp = mpqArchives[sel];
			mpqArchives[sel] = mpqArchives[sel-1];
			mpqArchives[sel-1] = temp;

			mpqList->Set(mpqArchives);
			mpqList->Select(sel-1);
		}
	} else if (id==ID_SETTINGS_DOWN) {
		int sel = mpqList->GetSelection();
		if (sel>-1 && sel < int(mpqList->GetCount()-1)) {
			wxString temp = mpqArchives[sel];
			mpqArchives[sel] = mpqArchives[sel+1];
			mpqArchives[sel+1] = temp;

			mpqList->Set(mpqArchives);
			mpqList->Select(sel+1);
		}
		
	} else if (id==ID_SETTINGS_ADD) {
		wxFileDialog file(this, _T("Choose a MPQ file to add"), _T(""), _T(""), "MPQ Archives (*.mpq)|*.mpq", wxFD_OPEN|wxFD_FILE_MUST_EXIST, wxDefaultPosition);

		if (file.ShowModal()==wxID_OK) {
			if (wxFile::Exists(file.GetPath())) {
				for (size_t i=0; i<mpqArchives.GetCount(); i++) {
					if (mpqArchives[i] == file.GetPath())
						return;
				}

				mpqArchives.Insert(file.GetPath(), 0);
				mpqList->Set(mpqArchives);
			}
		}
		
	} else if (id==ID_SETTINGS_REMOVE) {
		int sel = mpqList->GetSelection();
		if (sel>-1 && sel < int(mpqList->GetCount())) {
			mpqArchives.RemoveAt(sel);
			mpqList->Set(mpqArchives);
			mpqList->Select(0);
		}

	} else if (id==ID_SETTINGS_CLEAR) {
		mpqArchives.Clear();
		mpqList->Clear();
	}
}

void Settings_Page1::OnCheck(wxCommandEvent &event)
{
	int id = event.GetId();

	if (id==ID_SETTINGS_RANDOMCHAR) {
		
	} else if (id==ID_SETTINGS_RANDOMSKIN) {
		useRandomLooks = event.IsChecked();
	} else if (id==ID_SETTINGS_LOCALFILES) {
		useLocalFiles = event.IsChecked();
	} else if (id==ID_SETTINGS_INITSTARTUP) {

	} else if (id==ID_SETTINGS_SAVELAYOUT) {

	}
}

void Settings_Page1::Update()
{
	chkbox[CHECK_RANDOMSKIN]->SetValue(useRandomLooks);
	chkbox[CHECK_LOCALFILES]->SetValue(useLocalFiles);
}

Settings_Page2::Settings_Page2(wxWindow* parent, wxWindowID id)
{
	if (Create(parent, id, wxPoint(0,0), wxSize(400,400), 0, _T("Settings_Page2")) == false) {
		wxLogMessage(_T("GUI Error: Settings_Page2"));
		return;
	}

	wxFlexGridSizer *top = new wxFlexGridSizer(1);
	top->AddGrowableCol(0);
	top->SetFlexibleDirection(wxBOTH);

	top->Add(new wxStaticText(this, wxID_ANY, _T("OpenGL Display Mode:"), wxDefaultPosition, wxDefaultSize, 0), 1, wxEXPAND|wxALL, 10);	
	top->Add(oglMode = new wxComboBox(this, wxID_ANY, _T("Default"), wxDefaultPosition, wxSize(360, 25), 0, 0, wxCB_READONLY), 1, wxEXPAND, 10);
	
	top->Add(new wxStaticText(this, wxID_ANY, _T("Field of View:"), wxDefaultPosition, wxDefaultSize, 0), 1, wxEXPAND|wxALL, 10);	
	top->Add(txtFov = new wxTextCtrl(this, wxID_ANY, _T("45.000000"), wxDefaultPosition, wxSize(100, 20)), 1, 0, 10);

	wxFlexGridSizer *gs = new wxFlexGridSizer(3, 4, 4);
	
	#define ADD_CONTROLS(index, id, caption) \
	gs->Add(chkBox[index] = new wxCheckBox(this, id, caption, wxDefaultPosition, wxDefaultSize, 0), wxSizerFlags(0).Align(wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL));
	
	ADD_CONTROLS(CHECK_COMPRESSEDTEX, wxID_ANY, _T("Compressed Textures"))
	ADD_CONTROLS(CHECK_MULTITEX, wxID_ANY, _T("Multi-Textures"))
	ADD_CONTROLS(CHECK_VBO, wxID_ANY, _T("Vertex Buffer"))
	ADD_CONTROLS(CHECK_FBO, wxID_ANY, _T("Frame Buffer"))
	ADD_CONTROLS(CHECK_PBO, wxID_ANY, _T("Pixel Buffer"))
	ADD_CONTROLS(CHECK_DRAWRANGEELEMENTS, wxID_ANY, _T("Draw Range Elements"))
	ADD_CONTROLS(CHECK_ENVMAPPING, wxID_ANY, _T("Environmental Mapping"))
	ADD_CONTROLS(CHECK_NPOT, wxID_ANY, _T("Non-Power of two"))
	ADD_CONTROLS(CHECK_PIXELSHADERS, wxID_ANY, _T("Pixel Shaders"))
	ADD_CONTROLS(CHECK_VERTEXSHADERS, wxID_ANY, _T("Vertex Shaders"))
	ADD_CONTROLS(CHECK_GLSLSHADERS, wxID_ANY, _T("GLSL Shaders"))
	#undef ADD_CONTROLS

	top->Add(gs,wxSizerFlags().Proportion(1).Expand().Border(wxALL, 10));

	top->Add(new wxButton(this, ID_SETTINGS_APPLY, _("Apply Settings"), wxDefaultPosition, wxDefaultSize, 0), wxSizerFlags()/*.Expand()*/.Border(wxALL, 10).Align(wxALIGN_RIGHT|wxALIGN_BOTTOM));
	
	top->SetMinSize(350, 350);
	//top->SetMaxSize(400, 400);
	top->SetSizeHints(this);
	SetSizer(top);
	SetAutoLayout(true);
	Layout();
}

void Settings_Page2::Update()
{
	oglMode->Clear();

	for (size_t i=0; i<g_videoSetting.capsList.size(); i++) {
		wxString mode = wxString::Format(_T("Colour:%i Depth:%i Alpha:%i "), g_videoSetting.capsList[i].colour, g_videoSetting.capsList[i].zBuffer, g_videoSetting.capsList[i].alpha);
		
		if (g_videoSetting.capsList[i].sampleBuffer)
			mode.Append(wxString::Format(_T("FSAA:%i "), g_videoSetting.capsList[i].aaSamples));
		
		if (g_videoSetting.capsList[i].doubleBuffer)
			mode.Append(_T("DoubleBuffer "));

		if (g_videoSetting.capsList[i].hwAcc == WGL_FULL_ACCELERATION_ARB)
			mode.Append(_T("Hardware mode"));
		else if (g_videoSetting.capsList[i].hwAcc == WGL_GENERIC_ACCELERATION_ARB)
			mode.Append(_T("Emulation mode"));
		else //WGL_NO_ACCELERATION_ARB
			mode.Append(_T("Software mode"));

		oglMode->Append(mode);
	}

	oglMode->SetSelection(g_videoSetting.capIndex);

	txtFov->SetValue(wxString::Format(_T("%f"), g_videoSetting.fov));

	// Toggle all the video options
	if (g_videoSetting.supportCompression)
		chkBox[CHECK_COMPRESSEDTEX]->SetValue(g_videoSetting.useCompression);
	else
		chkBox[CHECK_COMPRESSEDTEX]->Disable();

	if (g_videoSetting.supportMultiTex) {
		chkBox[CHECK_MULTITEX]->SetValue(true);
		chkBox[CHECK_MULTITEX]->Disable();
	} else
		chkBox[CHECK_MULTITEX]->Disable();

	if (g_videoSetting.supportVBO)
		chkBox[CHECK_VBO]->SetValue(g_videoSetting.useVBO);
	else
		chkBox[CHECK_VBO]->Disable();

	if (g_videoSetting.supportFBO)
		chkBox[CHECK_FBO]->SetValue(g_videoSetting.useFBO);
	else
		chkBox[CHECK_FBO]->Disable();

	if (g_videoSetting.supportPBO)
		chkBox[CHECK_PBO]->SetValue(g_videoSetting.usePBO);
	else
		chkBox[CHECK_PBO]->Disable();

	if (g_videoSetting.supportDrawRangeElements) {
		chkBox[CHECK_DRAWRANGEELEMENTS]->SetValue(true);
		chkBox[CHECK_DRAWRANGEELEMENTS]->Disable();
	} else
		chkBox[CHECK_DRAWRANGEELEMENTS]->Disable();

	chkBox[CHECK_ENVMAPPING]->SetValue(g_videoSetting.useEnvMapping);

	if (g_videoSetting.supportNPOT) {
		chkBox[CHECK_NPOT]->SetValue(true);
		chkBox[CHECK_NPOT]->Disable();
	} else
		chkBox[CHECK_NPOT]->Disable();

	if (g_videoSetting.supportFragProg)
		chkBox[CHECK_PIXELSHADERS]->SetValue(true);
	else
		chkBox[CHECK_PIXELSHADERS]->Disable();

	if (g_videoSetting.supportVertexProg)
		chkBox[CHECK_VERTEXSHADERS]->SetValue(true);
	else
		chkBox[CHECK_VERTEXSHADERS]->Disable();

	if (g_videoSetting.supportGLSL)
		chkBox[CHECK_GLSLSHADERS]->SetValue(true);
	else
		chkBox[CHECK_GLSLSHADERS]->Disable();
}

void Settings_Page2::OnButton(wxCommandEvent &event)
{
	int id = event.GetId();
	
	if (id == ID_SETTINGS_APPLY) {
		if ((oglMode->GetSelection() != g_videoSetting.capIndex) && g_videoSetting.GetCompatibleWinMode(g_videoSetting.capsList[oglMode->GetSelection()])) {
			wxLogMessage("Info: Graphics display mode changed.  Requires restart to take effect.");
			wxMessageBox("Graphics display settings changed.\nWoW Model Viewer requires restarting to take effect.", "Settings Changed", wxICON_INFORMATION);
		}
		
		double fov;
		txtFov->GetValue().ToDouble(&fov);
		if ((fov > 0) && (fov < 270.0))
			g_videoSetting.fov = (float) fov;

		g_modelViewer->SaveSession();
		g_modelViewer->interfaceManager.GetPane(this->GetParent()).Show(false);
		g_modelViewer->interfaceManager.Update();
	}
}

SettingsControl::SettingsControl(wxWindow* parent, wxWindowID id)
{
	wxLogMessage(_T("Creating Settings Control..."));
	
	if (Create(parent, id, wxDefaultPosition, wxSize(405,440), wxDEFAULT_FRAME_STYLE, _T("SettingsControlFrame")) == false) {
		wxLogMessage(_T("GUI Error: Failed to create the window for our SettingsControl!"));
		return;
	}

	//
	notebook = new wxNotebook(this, ID_SETTINGS_TABS, wxPoint(0,0), wxSize(400,420), wxNB_TOP|wxNB_FIXEDWIDTH|wxNB_NOPAGETHEME);
	
	page1 = new Settings_Page1(notebook, ID_SETTINGS_PAGE1);
	page2 = new Settings_Page2(notebook, ID_SETTINGS_PAGE2);

	notebook->AddPage(page1, _T("Options"), false, -1);
	notebook->AddPage(page2, _T("Display"), false);
}


SettingsControl::~SettingsControl()
{
	page1->Destroy();
	page2->Destroy();
	notebook->Destroy();
}


void SettingsControl::Open()
{
	Show(true);

	page1->Update();
	page2->Update();
}

void SettingsControl::Close()
{
	
}

// --
