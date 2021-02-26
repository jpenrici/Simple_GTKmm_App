#include "controller.hpp"

/* 
 * GUI VIEW CONTROL
 */
int Controller::Gui()
{
    auto app = Gtk::Application::create("org.gtkmm.example");

    auto refBuilder = Gtk::Builder::create();
    try {
        refBuilder->add_from_file("view_gtkmm.glade");
    } catch(const Glib::FileError& ex) {
        cerr << "FileError: " << ex.what() << endl;
        return EXIT_FAILURE;
    } catch(const Glib::MarkupError& ex) {
        cerr << "MarkupError: " << ex.what() << endl;
        return EXIT_FAILURE;
    } catch(const Gtk::BuilderError& ex) {
        cerr << "BuilderError: " << ex.what() << endl;
        return EXIT_FAILURE;
    }

    pGui = nullptr;
    pBtn_insert = nullptr;
    pBtn_update = nullptr;
    pBtn_delete = nullptr;
    pBtn_clean = nullptr;
    pLbl_id = nullptr;
    pLbl_inform = nullptr;
    pLbl_status = nullptr;
    pTxt_first_name = nullptr;
    pTxt_last_name = nullptr;
    pTxt_email = nullptr;
    pTreeView = nullptr;

    refBuilder->get_widget_derived("appWindow", pGui);
    refBuilder->get_widget("btn_insert", pBtn_insert);
    refBuilder->get_widget("btn_update", pBtn_update);
    refBuilder->get_widget("btn_delete", pBtn_delete);
    refBuilder->get_widget("btn_clean", pBtn_clean);
    refBuilder->get_widget("lbl_id", pLbl_id);
    refBuilder->get_widget("lbl_inform", pLbl_inform);
    refBuilder->get_widget("lbl_status", pLbl_status);
    refBuilder->get_widget("txt_first_name", pTxt_first_name);
    refBuilder->get_widget("txt_last_name", pTxt_last_name);
    refBuilder->get_widget("txt_email", pTxt_email);
    refBuilder->get_widget("treeView", pTreeView);

    if (pBtn_insert) {
        pBtn_insert->signal_clicked().connect(sigc::mem_fun(*this,
            &Controller::gui_insert));
    }

    if (pBtn_update) {
        pBtn_update->signal_clicked().connect(sigc::mem_fun(*this,
            &Controller::gui_update));
    }

    if (pBtn_delete) {
        pBtn_delete->signal_clicked().connect(sigc::mem_fun(*this,
            &Controller::gui_delete));
    }

    if (pBtn_clean) {
        pBtn_clean->signal_clicked().connect(sigc::mem_fun(*this,
            &Controller::gui_clean));
    }

    m_refTreeModel = Gtk::ListStore::create(m_Columns);
    pTreeView->set_model(m_refTreeModel);

    m_TreeSelection = pTreeView->get_selection();
    m_TreeSelection->signal_changed().connect(sigc::mem_fun(*this,
        &Controller::gui_select_row_treeView));

    if(pGui) {
        cout << "GUI started." << endl;
        gui_clean();
        app->run(*pGui);
        cout << "GUI stoped." << endl;
    } 

    delete pBtn_insert;
    delete pBtn_update;
    delete pBtn_delete;
    delete pBtn_clean;
    delete pLbl_id;
    delete pLbl_inform;
    delete pLbl_status;
    delete pTxt_first_name;
    delete pTxt_last_name;
    delete pTxt_email;
    delete pTreeView;
    delete pGui;

    return EXIT_SUCCESS;
}

void Controller::gui_insert()
{
    string exp_name = "^[a-zA-ZãÃâÂáÁàÀäÄẽẼêÊéÉèÈëËĩĨîÎíÍìÌïÏôÔõÕóÓòÒöÖũŨúÚùÙûÛüÜçÇ' ']+$";
    string first_name = prepare(pTxt_first_name->get_text(), exp_name);
    string last_name = prepare(pTxt_last_name->get_text(), exp_name);

    string exp_email = "^[a-zA-Z0-9\\._-]+@[a-zA-Z0-9\\._-]+.([a-zA-Z]{2,4})$";
    string email = prepare(pTxt_email->get_text(), exp_email);

    if (first_name == "" || last_name == "" || email == "") {
        pLbl_status->set_text("Action canceled! There is invalid data!");
        return;
    }

    if (add_contact(first_name, last_name, email)) {
        gui_clean();
        pLbl_status->set_text("Contact added successfully.");
    } else {
        pLbl_status->set_text("Email already exists!");
    }
}

void Controller::gui_update()
{
    string id = pLbl_id->get_text();
    if (id == "") {
        return;
    }

    string exp_email = "^[a-zA-Z0-9\\._-]+@[a-zA-Z0-9\\._-]+.([a-zA-Z]{2,4})$";
    string email = prepare(pTxt_email->get_text(), exp_email);
    if (email == "") {
        pLbl_status->set_text("Action canceled! There is invalid data!");
        return;
    }

    if (update_contact(id, email)) {
        gui_clean();
        pLbl_status->set_text("Contact update successfully.");
    } else {
        pLbl_status->set_text("Email already exists!");
    }
}

void Controller::gui_delete()
{
    string id = pLbl_id->get_text();
    if (id == "") {
        return;
    }

    if (remove_contact(id)) {
        gui_clean();
        pLbl_status->set_text("Email removed successfully.");
    } else {
        pLbl_status->set_text("ID not found!");
    }
}

void Controller::gui_view()
{
    vector<vector<string> > result = read_contacts();
    vector<vector<string> > table {{"ID", "First Name", "Last Name", "Email"}};
    table.insert(end(table), begin(result), end(result));  
    gui_update_treeView(table);
}

void Controller::gui_clean()
{
    gui_view();

    pTxt_first_name->set_sensitive(true);
    pTxt_last_name->set_sensitive(true);
    pTxt_email->set_sensitive(true);

    pBtn_insert->set_sensitive(true);
    pBtn_update->set_sensitive(false);
    pBtn_delete->set_sensitive(false);
    
    pTxt_first_name->set_text("");
    pTxt_last_name->set_text("");
    pTxt_email->set_text("");

    pLbl_id->set_text("");
    pLbl_status->set_text("");
    pLbl_inform->set_text("Insert a new contact.");
}

void Controller::gui_select_row_treeView()
{
    pTxt_first_name->set_sensitive(false);
    pTxt_last_name->set_sensitive(false);
    pTxt_email->set_sensitive(true);

    pBtn_insert->set_sensitive(false);
    pBtn_update->set_sensitive(true);
    pBtn_delete->set_sensitive(true);

    auto iter = m_TreeSelection->get_selected();
    if(iter) {
        auto row = *iter;
        pLbl_id->set_text(row[m_Columns.m_col_0]);
        pTxt_first_name->set_text(row[m_Columns.m_col_1]);
        pTxt_last_name->set_text(row[m_Columns.m_col_2]);
        pTxt_email->set_text(row[m_Columns.m_col_3]);
    }

    pLbl_inform->set_text("Update email or Remove contact.");
}

void Controller::gui_update_treeView(vector<vector<string> > result)
{
    m_TreeSelection->unselect_all();
    m_refTreeModel->clear();

    if (result.size() < 2) {
        pLbl_status->set_text("No data to display.");
        return;
    }

    Gtk::TreeModel::Row row;
    for (unsigned int i = 1; i < result.size(); ++i)
    {
        row = *(m_refTreeModel->append());
        row[m_Columns.m_col_0] = result[i][0];
        row[m_Columns.m_col_1] = result[i][1];
        row[m_Columns.m_col_2] = result[i][2];
        row[m_Columns.m_col_3] = result[i][3];
    }

    pTreeView->remove_all_columns();
    if (result.size() != 0) {
        pTreeView->append_column(result[0][0], m_Columns.m_col_0);
        pTreeView->append_column(result[0][1], m_Columns.m_col_1);
        pTreeView->append_column(result[0][2], m_Columns.m_col_2);
        pTreeView->append_column(result[0][3], m_Columns.m_col_3);
    }
}

/* 
 * CONSOLE VIEW CONTROL
 */
int Controller::Console()
{
    string option;
    do {
        option = console.menu();
        vector<string> values;

        if (option == "ADD") {
            values = console.add_contact();
            if (add_contact(values[0], values[1], values[2])) {
                console.inform("Contact added successfully.");
            } else {
                console.inform("Email already exists!");
            }
        }

        if (option == "UPDATE") {
            values = console.update_contact();
            if (update_contact(values[0], values[1])) {
                console.inform("Email update successfully.");
            } else {
                console.inform("ID not found or Email already exists!");
            }           
        }

        if (option == "VIEW") {
            console.view_contacts(read_contacts());
        }       

        if (option == "DEL") {
            if (remove_contact(console.remove_contact())) {
                console.inform("Email removed successfully.");
            } else {
                console.inform("ID not found!");
            }           
        }

    } while (option != "QUIT");

    return EXIT_SUCCESS;
}

/* 
 * MODEL CONTROL
 */
bool Controller::add_contact(string first_name, string last_name, string email)
{
    return contacts.add(first_name, last_name, email);
}

vector<vector<string> > Controller::read_contacts()
{
    return contacts.read();
}

bool Controller::update_contact(string id, string email)
{
    return contacts.update(stoi(id), email);
}

bool Controller::remove_contact(string id)
{
    return contacts.remove(stoi(id));
}

/*
 * GENERIC FUNCTIONS
 */
string Controller::prepare(string value, string exp)
{
    if (!regex_match (value, regex(exp))) {
        return "";
    }

    char c = ' ';   // space
    string output = "";
    while(value.front() == c) {
        value.erase(0, 1);
    }
    while (value.back() == c) {
        value.erase(value.size() - 1, 1);
    }
    for (unsigned int i = 0; i < value.size(); ++i) {
        output += value[i];
        while(value[i] == c) {
            i = i + 1;
            if (value[i] != c) {
                output += value[i];
                break;
            }
        }
    }

    return output;
}