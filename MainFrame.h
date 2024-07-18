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
	void IntroDialogue();
	void SetupMainMenu();
	void LoadTable(std::string path);
	void CreateControls();
	void BindEventHandlers();
	void AddSavedTasks();

	void OnAddButtonClicked(wxCommandEvent& evt);
	void OnInputEnter(wxCommandEvent& evt);
	void OnListKeyDown(wxKeyEvent& evt);
	void OnClearButtonClicked(wxCommandEvent& evt);
	void OnWindowClosed(wxCloseEvent& evt);

	void OnTestButtonClicked(wxCommandEvent& evt);

	void AddTaskFromInput();
	void DeleteSelectedTask();
	void MoveSelectedTask(int offset);
	void SwapTasks(int i, int j);

	wxPanel* panel;
	wxStaticText* headlineText;
	wxTextCtrl* inputField;
	wxButton* addButton;
	wxCheckListBox* checkListBox;
	wxButton* clearButton;

	bool test = true;
	wxButton* testButton;

	Json::Value content;
};

