#pragma once

#include <wx/wx.h>
#include <vector>
#include <string>

class InputDialog : public wxDialog
{
public:
    InputDialog(std::vector<std::string>& labels,
        std::vector<std::string>& prefill,
        wxWindow* parent,
        wxWindowID id = wxID_ANY,
        const wxString& title = "Input",
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_DIALOG_STYLE,
        const wxString& name = wxASCII_STR(wxDialogNameStr)) : wxDialog(parent, id, title, pos, size, style) {

        auto mainSizer = new wxBoxSizer(wxVERTICAL);

        int prefillCounter = 0;
        for (auto label : labels) {
            std::string prefillString = "";
            if (prefillCounter < prefill.size()) {
                prefillString = prefill[prefillCounter];
                prefillCounter++;
            }
            auto sizer = new wxBoxSizer(wxHORIZONTAL);
            auto labelText = new wxStaticText(this, wxID_ANY, label);
            auto textCtrl = new wxTextCtrl(this, wxID_ANY, prefillString, wxDefaultPosition, wxSize(200, 20));
            textCtrls.push_back(textCtrl);
            sizer->Add(labelText);
            sizer->Add(textCtrl);
            mainSizer->Add(sizer);
        }
        mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALIGN_CENTER | wxALL, FromDIP(10));
        this->SetSizerAndFit(mainSizer);
    }

    std::vector<std::string> GetInputs() {
        std::vector<std::string> result;
        for (auto textCtrl : textCtrls) {
            result.push_back(textCtrl->GetValue().ToStdString());
        }
        return result;
    }

    std::string test() { return "There is Dialog " + std::to_string(textCtrls.size()); }

private:
    std::vector<wxTextCtrl*> textCtrls;
};

