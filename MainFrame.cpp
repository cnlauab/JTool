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
	auto mainSizer = new wxBoxSizer(wxVERTICAL);

	//Upper Panel
	auto upperPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200,100));
	upperPanel->SetFont(mainFont);

	auto upperSizer = new wxBoxSizer(wxVERTICAL);
	auto tableInfoSizer = new wxBoxSizer(wxHORIZONTAL);

	auto nameLable = new wxStaticText(upperPanel, wxID_ANY, "Table Name: ");
	nameField = new wxTextCtrl(upperPanel, wxID_ANY, "0", wxDefaultPosition, wxSize(200, 20), wxTE_READONLY);
	auto nameSizer = new wxBoxSizer(wxHORIZONTAL);
	auto nameButton = new wxButton(upperPanel, wxID_ANY, "Edit");
	nameButton->Bind(wxEVT_BUTTON, &MainFrame::OnEditNameClicked, this);
	nameSizer->Add(nameField);
	nameSizer->Add(nameButton);

	auto idLable = new wxStaticText(upperPanel, wxID_ANY, "Table ID: ");
	idField = new wxTextCtrl(upperPanel, wxID_ANY, "0", wxDefaultPosition, wxSize(200, 20), wxTE_READONLY);
	auto idSizer = new wxBoxSizer(wxHORIZONTAL);
	auto idButton = new wxButton(upperPanel, wxID_ANY, "Edit");
	idButton->Bind(wxEVT_BUTTON, &MainFrame::OnEditIdClicked, this);
	idSizer->Add(idField);
	idSizer->Add(idButton);

	auto sizeLable = new wxStaticText(upperPanel, wxID_ANY, "Table Size: ");
	sizeField = new wxTextCtrl(upperPanel, wxID_ANY, "0",wxDefaultPosition, wxSize(200, 20), wxTE_READONLY);

	upperSizer->Add(nameLable);
	upperSizer->Add(nameSizer);
	upperSizer->Add(idLable);
	upperSizer->Add(idSizer);
	upperSizer->Add(sizeLable);
	upperSizer->Add(sizeField);

	upperPanel->SetSizerAndFit(upperSizer);

	//Bottom Panel
	auto tabs = new wxNotebook(this, wxID_ANY);
	tabs->SetInternalBorder(0);
	auto schemaPanel = new wxPanel(tabs, wxID_ANY, wxDefaultPosition, wxSize(400, 400));
	auto dataPanel = new wxPanel(tabs, wxID_ANY, wxDefaultPosition, wxSize(400, 400));
	auto jsonPanel = new wxPanel(tabs, wxID_ANY, wxDefaultPosition, wxSize(400, 400));
	tabs->AddPage(schemaPanel, "Schema");
	tabs->AddPage(dataPanel, "Data");
	tabs->AddPage(jsonPanel, "Json");

	schemaListView = new wxListView(schemaPanel);
	schemaListView->AppendColumn("Name");
	schemaListView->AppendColumn("Type");

	dataListView = new wxListView(dataPanel);

	//Schema Tab
	auto addSchemaButton = new wxButton(schemaPanel, wxID_ANY, "Add");
	addSchemaButton->Bind(wxEVT_BUTTON, &MainFrame::OnAddFieldClicked, this);
	auto deleteSchemaButton = new wxButton(schemaPanel, wxID_ANY, "Delete");
	deleteSchemaButton->Bind(wxEVT_BUTTON, &MainFrame::OnDeleteFieldClicked, this);
	auto editSchemaButton = new wxButton(schemaPanel, wxID_ANY, "Edit");
	editSchemaButton->Bind(wxEVT_BUTTON, &MainFrame::OnEditFieldClicked, this);
	auto schemaSizer = new wxBoxSizer(wxVERTICAL);
	auto schemaButtonSizer = new wxBoxSizer(wxHORIZONTAL);
	schemaButtonSizer->Add(addSchemaButton);
	schemaButtonSizer->Add(deleteSchemaButton);
	schemaButtonSizer->Add(editSchemaButton);
	schemaSizer->Add(schemaButtonSizer);
	schemaSizer->Add(schemaListView,1,wxALL | wxEXPAND,0);
	schemaPanel->SetSizerAndFit(schemaSizer);

	//Data Tab
	auto addDataButton = new wxButton(dataPanel, wxID_ANY, "Add");
	addDataButton->Bind(wxEVT_BUTTON, &MainFrame::OnAddDataClicked, this);
	auto deleteDataButton = new wxButton(dataPanel, wxID_ANY, "Delete");
	deleteDataButton->Bind(wxEVT_BUTTON, &MainFrame::OnDeleteDataClicked, this);
	auto editDataButton = new wxButton(dataPanel, wxID_ANY, "Edit");
	editDataButton->Bind(wxEVT_BUTTON, &MainFrame::OnEditDataClicked, this);
	auto dataSizer = new wxBoxSizer(wxVERTICAL);
	auto dataButtonSizer = new wxBoxSizer(wxHORIZONTAL);
	dataButtonSizer->Add(addDataButton);
	dataButtonSizer->Add(deleteDataButton);
	dataButtonSizer->Add(editDataButton);
	dataSizer->Add(dataButtonSizer);
	dataSizer->Add(dataListView, 1, wxALL | wxEXPAND, 0);
	dataPanel->SetSizerAndFit(dataSizer);

	//Json Tab
	jsonField = new wxTextCtrl(jsonPanel, wxID_ANY, "", wxDefaultPosition, wxSize(500, 500), wxTE_READONLY);

	//Main Sizer
	mainSizer->Add(upperPanel, 1, wxEXPAND | wxALL, 10);
	mainSizer->Add(tabs, 5, wxEXPAND | wxALL, 10);
	this->SetSizerAndFit(mainSizer);
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

void MainFrame::UpdateInfoView()
{
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

void MainFrame::OnNewMenuClicked(wxCommandEvent& evt)
{
}

void MainFrame::OnOpenMenuClicked(wxCommandEvent& evt)
{
}

void MainFrame::OnSaveMenuClicked(wxCommandEvent& evt)
{
}

void MainFrame::OnSaveAsMenuClicked(wxCommandEvent& evt)
{
}

void MainFrame::OnEditNameClicked(wxCommandEvent& evt)
{
	auto result = wxMessageBox("Edit Name", "JTool", wxYES_NO | wxICON_INFORMATION);
}

void MainFrame::OnEditIdClicked(wxCommandEvent& evt)
{
	auto result = wxMessageBox("Edit ID", "JTool", wxYES_NO | wxICON_INFORMATION);
}

void MainFrame::OnAddFieldClicked(wxCommandEvent& evt)
{
	auto result = wxMessageBox("Add Field", "JTool", wxYES_NO | wxICON_INFORMATION);
}

void MainFrame::OnDeleteFieldClicked(wxCommandEvent& evt)
{
	auto result = wxMessageBox("Delete Field", "JTool", wxYES_NO | wxICON_INFORMATION);
}

void MainFrame::OnEditFieldClicked(wxCommandEvent& evt)
{
	auto result = wxMessageBox("Edit Field", "JTool", wxYES_NO | wxICON_INFORMATION);
}

void MainFrame::OnAddDataClicked(wxCommandEvent& evt)
{
	auto result = wxMessageBox("Add Data", "JTool", wxYES_NO | wxICON_INFORMATION);
}

void MainFrame::OnDeleteDataClicked(wxCommandEvent& evt)
{
	auto result = wxMessageBox("Delete Data", "JTool", wxYES_NO | wxICON_INFORMATION);
}

void MainFrame::OnEditDataClicked(wxCommandEvent& evt)
{
	auto result = wxMessageBox("Edit Data", "JTool", wxYES_NO | wxICON_INFORMATION);
}



