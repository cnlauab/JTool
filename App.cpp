#include "App.h"
#include "MainFrame.h"

wxIMPLEMENT_APP(App);

bool App::OnInit() {
	MainFrame* mainFrame = new MainFrame("JTool Version 1.0");
	//mainFrame->SetClientSize(400, 300);
	mainFrame->Center();
	mainFrame->Show();
	return true;
}