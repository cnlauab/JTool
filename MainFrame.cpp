#include "MainFrame.h"

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title){
	SetupMainMenu();
	CreateControls();
	BindEventHandlers();
	IntroDialogue();
	FullUpdate();
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
		currPath = path;
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
	nameButton = new wxButton(upperPanel, wxID_ANY, "Edit");
	nameSizer->Add(nameField);
	nameSizer->Add(nameButton);

	auto idLable = new wxStaticText(upperPanel, wxID_ANY, "Table ID: ");
	idField = new wxTextCtrl(upperPanel, wxID_ANY, "0", wxDefaultPosition, wxSize(200, 20), wxTE_READONLY);
	auto idSizer = new wxBoxSizer(wxHORIZONTAL);
	idButton = new wxButton(upperPanel, wxID_ANY, "Edit");
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
	auto logPanel = new wxPanel(tabs, wxID_ANY, wxDefaultPosition, wxSize(400, 400));
	tabs->AddPage(schemaPanel, "Schema");
	tabs->AddPage(dataPanel, "Data");
	tabs->AddPage(jsonPanel, "Json");
	tabs->AddPage(logPanel, "Log");

	schemaListView = new wxListView(schemaPanel);
	schemaListView->AppendColumn("SEQ");
	schemaListView->AppendColumn("NAME");
	schemaListView->AppendColumn("TYPE");

	dataListView = new wxListView(dataPanel);

	//Schema Tab
	addSchemaButton = new wxButton(schemaPanel, wxID_ANY, "Add");
	deleteSchemaButton = new wxButton(schemaPanel, wxID_ANY, "Delete");
	editSchemaButton = new wxButton(schemaPanel, wxID_ANY, "Edit");
	auto schemaSizer = new wxBoxSizer(wxVERTICAL);
	auto schemaButtonSizer = new wxBoxSizer(wxHORIZONTAL);
	schemaButtonSizer->Add(addSchemaButton);
	schemaButtonSizer->Add(deleteSchemaButton);
	schemaButtonSizer->Add(editSchemaButton);
	schemaSizer->Add(schemaButtonSizer);
	schemaSizer->Add(schemaListView,1,wxALL | wxEXPAND,0);
	schemaPanel->SetSizerAndFit(schemaSizer);

	//Data Tab
	addDataButton = new wxButton(dataPanel, wxID_ANY, "Add");
	deleteDataButton = new wxButton(dataPanel, wxID_ANY, "Delete");
	editDataButton = new wxButton(dataPanel, wxID_ANY, "Edit");
	auto dataSizer = new wxBoxSizer(wxVERTICAL);
	auto dataButtonSizer = new wxBoxSizer(wxHORIZONTAL);
	dataButtonSizer->Add(addDataButton);
	dataButtonSizer->Add(deleteDataButton);
	dataButtonSizer->Add(editDataButton);
	dataSizer->Add(dataButtonSizer);
	dataSizer->Add(dataListView, 1, wxALL | wxEXPAND, 0);
	dataPanel->SetSizerAndFit(dataSizer);

	//Json Tab
	auto jsonSizer = new wxBoxSizer(wxVERTICAL);
	jsonField = new wxTextCtrl(jsonPanel, wxID_ANY, "", wxDefaultPosition, wxSize(500, 500), wxTE_READONLY | wxTE_MULTILINE);
	jsonSizer->Add(jsonField, 1, wxALL | wxEXPAND, 0);
	jsonPanel->SetSizerAndFit(jsonSizer);

	//Log Tab
	auto logSizer = new wxBoxSizer(wxVERTICAL);
	logField = new wxTextCtrl(logPanel, wxID_ANY, "", wxDefaultPosition, wxSize(500, 500), wxTE_READONLY | wxTE_MULTILINE);
	logSizer->Add(logField, 1, wxALL | wxEXPAND, 0);
	logPanel->SetSizerAndFit(logSizer);

	//Main Sizer
	mainSizer->Add(upperPanel, 1, wxEXPAND | wxALL, 10);
	mainSizer->Add(tabs, 5, wxEXPAND | wxALL, 10);
	this->SetSizerAndFit(mainSizer);
}

void MainFrame::BindEventHandlers()
{
	nameButton->Bind(wxEVT_BUTTON, &MainFrame::OnEditNameClicked, this);
	idButton->Bind(wxEVT_BUTTON, &MainFrame::OnEditIdClicked, this);

	addSchemaButton->Bind(wxEVT_BUTTON, &MainFrame::OnAddFieldClicked, this);
	deleteSchemaButton->Bind(wxEVT_BUTTON, &MainFrame::OnDeleteFieldClicked, this);
	editSchemaButton->Bind(wxEVT_BUTTON, &MainFrame::OnEditFieldClicked, this);

	addDataButton->Bind(wxEVT_BUTTON, &MainFrame::OnAddDataClicked, this);
	deleteDataButton->Bind(wxEVT_BUTTON, &MainFrame::OnDeleteDataClicked, this);
	editDataButton->Bind(wxEVT_BUTTON, &MainFrame::OnEditDataClicked, this);

	this->Bind(wxEVT_MENU, &MainFrame::OnNewMenuClicked, this, wxID_NEW);
	this->Bind(wxEVT_MENU, &MainFrame::OnOpenMenuClicked, this, wxID_OPEN);
	this->Bind(wxEVT_MENU, &MainFrame::OnSaveMenuClicked, this, wxID_SAVE);
	this->Bind(wxEVT_MENU, &MainFrame::OnSaveAsMenuClicked, this, wxID_SAVEAS);
}

void MainFrame::UpdateInfoView()
{
	nameField->SetValue(table.name);
	idField->SetValue(std::to_string(table.id));
	sizeField->SetValue(std::to_string(table.data.size()));
}

void MainFrame::UpdateSchemaView()
{
	for (int i = 0; i < table.schema.size(); i++) {
		schemaListView->InsertItem(i, "");
	}
	for (auto field : table.schema) {
		int i = table.fieldIndex[field.first];
		dataListView->AppendColumn(field.first);
		schemaListView->SetItem(i, 0, std::to_string(i));
		schemaListView->SetItem(i, 1, field.first);
		schemaListView->SetItem(i, 2, field.second);
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
		for (auto pair : table.data[i].GetRow()) {
			std::string field = pair.first;
			std::string data = pair.second;
			int col = table.fieldIndex[field];
			dataListView->SetItem(i, col, data);
		}
	}
}

void MainFrame::UpdateJson()
{
	Json::StyledWriter styledWriter;
	json = table.toJson();
	jsonField->SetValue(styledWriter.write(json));
}

void MainFrame::FullUpdate()
{
	dataListView->ClearAll();
	schemaListView->DeleteAllItems();
	UpdateInfoView();
	UpdateSchemaView();
	UpdateDataView();
	UpdateJson();
}

void MainFrame::OnNewMenuClicked(wxCommandEvent& evt)
{
	table = Table();
	currPath = "";
	FullUpdate();
}

void MainFrame::OnOpenMenuClicked(wxCommandEvent& evt)
{
	wxFileDialog dialog(this, "Open File", "", "", "Json Files (*.json;*.txt)|*.json;*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (dialog.ShowModal() == wxID_CANCEL) {
		return;
	}
	currPath = dialog.GetPath().ToStdString();
	LoadTable(currPath);
	FullUpdate();
}

void MainFrame::OnSaveMenuClicked(wxCommandEvent& evt)
{
	if (currPath.size() <= 0) {
		wxFileDialog dialog(this, "Save File", "", "", "Json Files (*.json;*.txt)|*.json;*.txt", wxFD_SAVE);
		if (dialog.ShowModal() == wxID_CANCEL) {
			return;
		}
		currPath = dialog.GetPath().ToStdString();
	}
	Json::StyledWriter styledWriter;
	std::ofstream file(currPath);
	file << styledWriter.write(json);
	file.close();
}

void MainFrame::OnSaveAsMenuClicked(wxCommandEvent& evt)
{
	wxFileDialog dialog(this, "Save File", "", "", "Json Files (*.json;*.txt)|*.json;*.txt", wxFD_SAVE);
	if (dialog.ShowModal() == wxID_CANCEL) {
		return;
	}
	currPath = dialog.GetPath().ToStdString();
	Json::StyledWriter styledWriter;
	std::ofstream file(currPath);
	file << styledWriter.write(json);
	file.close();
}

void MainFrame::OnEditNameClicked(wxCommandEvent& evt)
{
	std::vector<std::string> labels = { "Name" };
	std::vector<std::string> prefill = { table.name };
	auto input = new InputDialog(labels, prefill, this);
	if (input->ShowModal() == wxID_OK) {
		auto values = input->GetInputs();
		if (values.size() > 0) {
			table.name = values[0];
			UpdateInfoView();
			UpdateJson();
		}
	}
}

void MainFrame::OnEditIdClicked(wxCommandEvent& evt)
{
	std::vector<std::string> labels = { "ID" };
	std::vector<std::string> prefill = { std::to_string(table.id) };
	auto input = new InputDialog(labels, prefill, this);
	if (input->ShowModal() == wxID_OK) {
		auto values = input->GetInputs();
		if (values.size() > 0) {
			table.id = stoi(values[0]);
			UpdateInfoView();
			UpdateJson();
		}
	}
}

void MainFrame::OnAddFieldClicked(wxCommandEvent& evt)
{
	std::vector<std::string> labels = { "Name", "Type"};
	std::vector<std::string> prefill;
	std::string key;
	std::string value;
	auto input = new InputDialog(labels, prefill, this);
	if (input->ShowModal() == wxID_OK) {
		auto values = input->GetInputs();
		if (values.size() > 1) {
			key = values[0];
			value = values[1];
		}
		else {
			return;
		}
	}
	else {
		return;
	}
	bool result = table.AddField(key, value);
	if (result) {
		dataListView->ClearAll();
		schemaListView->DeleteAllItems();
		UpdateSchemaView();
		UpdateDataView();
		UpdateJson();
	}
}

void MainFrame::OnDeleteFieldClicked(wxCommandEvent& evt)
{
	long selected = schemaListView->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if (selected == -1) return;
	std::string key = table.schema[selected].first;
	bool result = table.DeleteField(key);
	if (result) {
		dataListView->ClearAll();
		schemaListView->DeleteAllItems();
		UpdateSchemaView();
		UpdateDataView();
		UpdateJson();
	}
}

void MainFrame::OnEditFieldClicked(wxCommandEvent& evt)
{
	long selected = schemaListView->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if (selected == -1) return;
	std::vector<std::string> labels = {"Type"};
	std::vector<std::string> prefill = { table.schema[selected].second };
	auto input = new InputDialog(labels, prefill, this);
	if (input->ShowModal() == wxID_OK) {
		auto values = input->GetInputs();
		if (values.size() > 0) {
			table.schema[selected].second = values[0];
		}
	}
	dataListView->ClearAll();
	schemaListView->DeleteAllItems();
	UpdateSchemaView();
	UpdateDataView();
	UpdateJson();
}

void MainFrame::OnAddDataClicked(wxCommandEvent& evt)
{
	std::vector<std::string> labels;
	std::vector<std::string> prefill;
	for (auto field : table.schema) {
		labels.push_back(field.first);
	}
	auto input = new InputDialog(labels, prefill, this);
	if (input->ShowModal() == wxID_OK) {
		std::map<std::string, std::string> row;
		auto values = input->GetInputs();
		if (values.size() > 0) {
			for (int i = 0; i < values.size(); i++) {
				row.insert(std::pair(labels[i], values[i]));
			}
		}
		else {
			return;
		}
		bool result = table.AddRow(row);
		if (result) {
			dataListView->DeleteAllItems();
			UpdateInfoView();
			UpdateDataView();
			UpdateJson();
		}
	}
}

void MainFrame::OnDeleteDataClicked(wxCommandEvent& evt)
{
	long selected = dataListView->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if (selected == -1) return;
	bool result = table.DeleteRow(selected);
	if (result) {
		dataListView->DeleteAllItems();
		UpdateInfoView();
		UpdateDataView();
		UpdateJson();
	}
}

void MainFrame::OnEditDataClicked(wxCommandEvent& evt)
{
	int selected = dataListView->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if (selected == -1) return;
	std::vector<std::string> labels;
	std::vector<std::string> prefill;
	for (auto field : table.schema) {
		labels.push_back(field.first);
		prefill.push_back(table.data[selected].GetCell(field.first));
	}
	auto input = new InputDialog(labels, prefill, this);
	if (input->ShowModal() == wxID_OK) {
		auto values = input->GetInputs();
		if (values.size() > 0) {
			for (int i = 0; i < values.size(); i++) {
				table.EditRow(selected, { labels[i], values[i] });
			}
		}
		else {
			return;
		}
		bool result = true;
		if (result) {
			dataListView->DeleteAllItems();
			UpdateInfoView();
			UpdateDataView();
			UpdateJson();
		}
	}
}

void MainFrame::PrintLog(std::string message)
{
	log.AppendLog(message);
	logField->SetValue("");
	*logField << log.PrintLog();
}



