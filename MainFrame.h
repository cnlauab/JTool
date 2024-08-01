#pragma once
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/notebook.h>
#include <json/json.h>

#include <vector>
#include <string>
#include <filesystem>
#include <fstream>

#include "Task.h"
#include "Table.h"

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);

private:
	//Setup
	void IntroDialogue();
	void SetupMainMenu();
	void LoadTable(std::string path);
	void CreateControls();
	void BindEventHandlers();
	//Update
	void UpdateInfoView();
	void UpdateSchemaView();
	void UpdateDataViewColumn();
	void UpdateDataView();
	//Menu
	void OnNewMenuClicked(wxCommandEvent& evt);
	void OnOpenMenuClicked(wxCommandEvent& evt);
	void OnSaveMenuClicked(wxCommandEvent& evt);
	void OnSaveAsMenuClicked(wxCommandEvent& evt);
	//Binding
	void OnEditNameClicked(wxCommandEvent& evt);
	void OnEditIdClicked(wxCommandEvent& evt);

	void OnAddFieldClicked(wxCommandEvent& evt);
	void OnDeleteFieldClicked(wxCommandEvent& evt);
	void OnEditFieldClicked(wxCommandEvent& evt);

	void OnAddDataClicked(wxCommandEvent& evt);
	void OnDeleteDataClicked(wxCommandEvent& evt);
	void OnEditDataClicked(wxCommandEvent& evt);
	//Cache
	wxTextCtrl* nameField;
	wxTextCtrl* idField;
	wxTextCtrl* sizeField;
	wxListView* schemaListView;
	wxListView* dataListView;
	wxTextCtrl* jsonField;

	bool test = true;
	wxButton* testButton;

	Json::Value json;
	Table table;
};

