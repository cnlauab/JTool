#include "MainFrame.h"

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title){
	IntroDialogue();
	CreateControls();
	BindEventHandlers();
	//AddSavedTasks();
}

void MainFrame::IntroDialogue() {
	auto result = wxMessageBox("Load table from files?", "JTool", wxYES_NO | wxICON_INFORMATION);
	if (result == wxYES) {
		wxFileDialog dialog(this, "Open File", "", "", "Json Files (*.json;*.txt)|*.json;*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
		if (dialog.ShowModal() == wxID_CANCEL) {
			return;
		}
		LoadTable(dialog.GetPath().ToStdString());
	}
}

void MainFrame::LoadTable(std::string path) {
	Json::Value json;
	Json::Reader reader;
	reader.parse(path, content);
	wxMessageBox(path, "Message", wxOK | wxICON_INFORMATION);

}

void MainFrame::CreateControls() {

	wxFont mainFont(wxFontInfo(wxSize(0, 12)));

	auto upperPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200,100));
	upperPanel->SetFont(mainFont);
	upperPanel->SetBackgroundColour(wxColour(100, 100, 200));
	auto labelPanel = new wxPanel(upperPanel, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
	labelPanel->SetFont(mainFont);
	labelPanel->SetBackgroundColour(wxColour(100, 200, 100));
	auto labelPanel2 = new wxPanel(upperPanel, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
	labelPanel2->SetFont(mainFont);
	labelPanel2->SetBackgroundColour(wxColour(100, 200, 100));
	//auto tabPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
	//tabPanel->SetFont(mainFont);
	//tabPanel->SetBackgroundColour(wxColour(200, 100, 100));

	auto mainSizer = new wxBoxSizer(wxVERTICAL);
	auto tableInfoSizer = new wxBoxSizer(wxHORIZONTAL);
	auto tableInfoLabelSizer = new wxBoxSizer(wxHORIZONTAL);
	auto tableInfoButtonSizer = new wxBoxSizer(wxHORIZONTAL);

	auto idLabel = new wxStaticText(labelPanel, wxID_ANY, "Table ID: 000");
	auto nameLabel = new wxStaticText(labelPanel, wxID_ANY, "Table Name: Students");
	auto sizeLabel = new wxStaticText(labelPanel, wxID_ANY, "Size: 12");
	auto changeIdButton = new wxButton(labelPanel2, wxID_ANY, "Change ID");
	auto changeNameButton = new wxButton(labelPanel2, wxID_ANY, "Change Name");
	
	auto tabs = new wxNotebook(this, wxID_ANY);
	tabs->SetInternalBorder(0);
	tabs->AddPage(new wxPanel(tabs, wxID_ANY, wxDefaultPosition, wxDefaultSize), "Schema");
	tabs->AddPage(new wxPanel(tabs, wxID_ANY, wxDefaultPosition, wxDefaultSize), "Data");
	tabs->AddPage(new wxPanel(tabs, wxID_ANY, wxDefaultPosition, wxDefaultSize), "Json");

	tableInfoSizer->Add(labelPanel, 0, wxEXPAND | wxALL, 10);
	tableInfoSizer->Add(labelPanel2, 0, wxEXPAND | wxALL, 10);
	
	upperPanel->SetSizerAndFit(tableInfoSizer);
	mainSizer->Add(upperPanel, 0, wxEXPAND | wxALL, 10);
	mainSizer->Add(tabs, 5, wxEXPAND | wxALL, 10);

	this->SetSizerAndFit(mainSizer);
	/*
	wxFont headlineFont(wxFontInfo(wxSize(0, 36)).Bold());
	wxFont mainFont(wxFontInfo(wxSize(0, 24)));

	panel = new wxPanel(this);
	panel->SetFont(mainFont);

	//Panel1
	headlineText = new wxStaticText(panel, wxID_ANY, "To-Do List", wxPoint(0, 22), wxSize(800, -1), wxALIGN_CENTER_HORIZONTAL);
	headlineText->SetFont(headlineFont);
	
	inputField = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(100, 80), wxSize(395, 35), wxTE_PROCESS_ENTER);
	addButton = new wxButton(panel, wxID_ANY, "Add", wxPoint(600, 80), wxSize(100, 35));
	testButton = new wxButton(panel, wxID_ANY, "Test", wxPoint(500, 80), wxSize(100, 35));
	checkListBox = new wxCheckListBox(panel, wxID_ANY, wxPoint(100, 120), wxSize(600, 400));
	clearButton = new wxButton(panel, wxID_ANY, "Clear", wxPoint(100, 525), wxSize(100, 35));
	*/

}

void MainFrame::BindEventHandlers()
{
	/*
	addButton->Bind(wxEVT_BUTTON, &MainFrame::OnAddButtonClicked, this);
	testButton->Bind(wxEVT_BUTTON, &MainFrame::OnTestButtonClicked, this);
	inputField->Bind(wxEVT_TEXT_ENTER, &MainFrame::OnInputEnter, this);
	checkListBox->Bind(wxEVT_KEY_DOWN, &MainFrame::OnListKeyDown, this);
	clearButton->Bind(wxEVT_BUTTON, &MainFrame::OnClearButtonClicked, this);
	this->Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnWindowClosed, this);
	*/
}

void MainFrame::AddSavedTasks()
{
	std::vector<Task> tasks = loadTasksFromFile("tasks.txt");
	for (const Task& task : tasks) {
		int index = checkListBox->GetCount();
		checkListBox->Insert(task.description, index);
		checkListBox->Check(index, task.done);
	}
}

void MainFrame::OnTestButtonClicked(wxCommandEvent& evt)
{
	if (test) {
		headlineText->Hide();
	}
	else {
		headlineText->Show();
	}
	test = !test;
}

void MainFrame::OnAddButtonClicked(wxCommandEvent& evt)
{
	AddTaskFromInput();
}

void MainFrame::OnInputEnter(wxCommandEvent& evt)
{
	AddTaskFromInput();
}

void MainFrame::OnListKeyDown(wxKeyEvent& evt)
{
	switch (evt.GetKeyCode()) {
	case WXK_DELETE:
		DeleteSelectedTask();
		break;
	case WXK_UP:
		MoveSelectedTask(-1);
		break;
	case WXK_DOWN:
		MoveSelectedTask(1);
		break;
	}
}

void MainFrame::OnClearButtonClicked(wxCommandEvent& evt)
{
	if (checkListBox->IsEmpty()) {
		return;
	}

	wxMessageDialog dialog(this, "Are you sure you want to clear all tasks?", "Clear", wxYES_NO | wxCANCEL);
	int result = dialog.ShowModal();
	if (result == wxID_YES) {
		checkListBox->Clear();
	}
}

void MainFrame::OnWindowClosed(wxCloseEvent& evt)
{
	std::vector<Task>tasks;
	for (int i = 0; i < checkListBox->GetCount(); i++) {
		Task task;
		task.description = checkListBox->GetString(i);
		task.done = checkListBox->IsChecked(i);
		tasks.push_back(task);
	}

	saveTaskToFile(tasks, "tasks.txt");
	evt.Skip();
}

void MainFrame::AddTaskFromInput()
{
	wxString description = inputField->GetValue();

	if (!description.IsEmpty()) {
		checkListBox->Insert(description, checkListBox->GetCount());
		inputField->Clear();
	}
	inputField->SetFocus();
}

void MainFrame::DeleteSelectedTask()
{
	int selectedIndex = checkListBox->GetSelection();
	if (selectedIndex == wxNOT_FOUND) {
		return;
	}
	checkListBox->Delete(selectedIndex);
}

void MainFrame::MoveSelectedTask(int offset)
{
	int selectedIndex = checkListBox->GetSelection();
	if (selectedIndex == wxNOT_FOUND) return;

	int newIndex = selectedIndex + offset;
	if (newIndex >= 0 && newIndex < checkListBox->GetCount()) {
		SwapTasks(selectedIndex, newIndex);
		checkListBox->SetSelection(newIndex, true);
	}
}

void MainFrame::SwapTasks(int i, int j)
{
	
	Task taskI{ checkListBox->GetString(i).ToStdString(), checkListBox->IsChecked(i) };
	Task taskJ{ checkListBox->GetString(j).ToStdString(), checkListBox->IsChecked(j) };

	checkListBox->SetString(i, taskJ.description);
	checkListBox->Check(i, taskJ.done);

	checkListBox->SetString(j, taskI.description);
	checkListBox->Check(j, taskI.done);
}

