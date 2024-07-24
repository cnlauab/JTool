#include "MainFrame.h"

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title){
	IntroDialogue();
	SetupMainMenu();
	CreateControls();
	BindEventHandlers();
	UpdateSchemaView();
	UpdateDataView();
}

void MainFrame::SetupMainMenu()
{
	wxMenuBar* menuBar = new wxMenuBar();

	wxMenu* fileMenu = new wxMenu();
	fileMenu->Append(wxID_NEW);
	fileMenu->Append(wxID_OPEN);
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_SAVE);
	fileMenu->Append(wxID_SAVEAS);

	wxMenu* editMenu = new wxMenu();

	editMenu->Append(wxID_UNDO);
	editMenu->Append(wxID_REDO);
	editMenu->AppendSeparator();
	editMenu->Append(wxID_CUT);
	editMenu->Append(wxID_COPY);
	editMenu->Append(wxID_PASTE);

	menuBar->Append(fileMenu, "&File");
	menuBar->Append(editMenu, "&Edit");

	SetMenuBar(menuBar);
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
	std::ifstream istream(path);
	Json::Reader reader;
	if (!reader.parse(istream, json)) {
		wxMessageBox("Could not parse JSON", "Message", wxOK | wxICON_INFORMATION);
	}
	else {
		table = Table(json);
		std::string s = table.toString();
		wxMessageBox(s, "Message", wxOK | wxICON_INFORMATION);
	}

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
	auto schemaPanel = new wxPanel(tabs, wxID_ANY, wxDefaultPosition, wxSize(400, 400));
	auto dataPanel = new wxPanel(tabs, wxID_ANY, wxDefaultPosition, wxSize(400, 400));
	auto jsonPanel = new wxPanel(tabs, wxID_ANY, wxDefaultPosition, wxSize(400, 400));
	tabs->AddPage(schemaPanel, "Schema");
	tabs->AddPage(dataPanel, "Data");
	tabs->AddPage(jsonPanel, "Json");

	tableInfoSizer->Add(labelPanel, 0, wxEXPAND | wxALL, 10);
	tableInfoSizer->Add(labelPanel2, 0, wxEXPAND | wxALL, 10);
	
	upperPanel->SetSizerAndFit(tableInfoSizer);
	mainSizer->Add(upperPanel, 0, wxEXPAND | wxALL, 10);
	mainSizer->Add(tabs, 5, wxEXPAND | wxALL, 10);

	this->SetSizerAndFit(mainSizer);

	schemaListView = new wxListView(schemaPanel);
	schemaListView->AppendColumn("Name");
	schemaListView->AppendColumn("Type");

	dataListView = new wxListView(dataPanel);

	auto schemaSizer = new wxBoxSizer(wxVERTICAL);
	schemaSizer->Add(schemaListView,1,wxALL | wxEXPAND,0);
	schemaPanel->SetSizerAndFit(schemaSizer);

	auto dataSizer = new wxBoxSizer(wxVERTICAL);
	dataSizer->Add(dataListView, 1, wxALL | wxEXPAND, 0);
	dataPanel->SetSizerAndFit(dataSizer);

}

void MainFrame::CreateSchemaTab() {
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

void MainFrame::UpdateSchemaView()
{
	
	int counter = 0;
	for (auto field : table.schema) {
		dataListView->AppendColumn(field.first);
		schemaListView->InsertItem(counter, field.first);
		schemaListView->SetItem(counter, 1, field.second);
		table.columnIndex[field.first] = counter;
		counter++;
	}
}

void MainFrame::UpdateDataViewColumn()
{
	for (auto field : table.schema) {
		dataListView->AppendColumn(field.first);
	}
}

void MainFrame::UpdateDataView()
{
	for (int i = 0; i < table.data.size(); i++) {
		dataListView->InsertItem(i, "");
		for (auto pair : table.data[i]) {
			std::string field = pair.first;
			std::string data = pair.second;
			dataListView->SetItem(i, table.columnIndex[field], data);
		}
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

