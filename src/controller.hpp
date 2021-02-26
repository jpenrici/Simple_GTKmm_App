#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "view_console.hpp"
#include "view_gtkmm.hpp"
#include "model.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <regex>

using namespace std;

class Controller {

    public:
        // VIEW CONTROL
        int Gui();
        int Console();

    private:
        // REGEX
        const string EXP_NAME {
            "^[a-zA-ZãÃâÂáÁàÀäÄẽẼêÊéÉèÈëËĩĨîÎíÍìÌïÏôÔõÕóÓòÒöÖũŨúÚùÙûÛüÜçÇ' ']+$"
        };

        const string EXP_EMAIL {
            "^[a-zA-Z0-9\\._-]+@[a-zA-Z0-9\\._-]+.([a-zA-Z]{2,4})$"
        };

        const string EXP_NUM {"^[0-9]+$"};

        // INFORM
        const vector<string> INFO {
            "There's something wrong! Check data.",     // ERROR_UNDEFINED
            "Successful action!",                       // OK            
            "There's something wrong! Check name.",     // ERROR_NAME
            "There's something wrong! Check email.",    // ERROR_EMAIL
            "There's something wrong! Check id.",       // ERROR_ID
        };        

        enum Error {
            ERROR_UNDEFINED,
            NO_ERROR,
            ERROR_NAME,
            ERROR_EMAIL,
            ERROR_ID,
        };

        // GENERIC FUNCTIONS
        string prepare(string value, string exp);
        
        // MODEL CONTROL
        Contacts contacts;
        int add_contact(string first_name, string last_name, string email);
        int update_contact(string id, string email);
        int remove_contact(string id);
        vector<vector<string> > read_contacts();

        // CONSOLE VIEW CONTROL
        ConsoleApp console; 

        // GUI VIEW CONTROL
        GUIApp* pGui;
        Gtk::Button* pBtn_insert;
        Gtk::Button* pBtn_update;
        Gtk::Button* pBtn_delete;
        Gtk::Button* pBtn_clean;          
        Gtk::Label* pLbl_id;
        Gtk::Label* pLbl_inform;
        Gtk::Label* pLbl_status;
        Gtk::Entry* pTxt_first_name;
        Gtk::Entry* pTxt_last_name;
        Gtk::Entry* pTxt_email;
        Gtk::TreeView* pTreeView;
        Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
        Glib::RefPtr<Gtk::TreeSelection> m_TreeSelection;       

        void gui_insert();
        void gui_update();
        void gui_delete();
        void gui_view();
        void gui_clean();
        void gui_select_row_treeView();
        void gui_update_treeView(vector<vector<string> > result);       

        class ModelColumns : public Gtk::TreeModel::ColumnRecord {

            public:
                ModelColumns() {
                    add(m_col_0);
                    add(m_col_1);
                    add(m_col_2);
                    add(m_col_3);
                };
                virtual ~ModelColumns(){};

                Gtk::TreeModelColumn<Glib::ustring> m_col_0;
                Gtk::TreeModelColumn<Glib::ustring> m_col_1;
                Gtk::TreeModelColumn<Glib::ustring> m_col_2;
                Gtk::TreeModelColumn<Glib::ustring> m_col_3;
        };

        ModelColumns m_Columns;
};

#endif // CONTROLLER_H