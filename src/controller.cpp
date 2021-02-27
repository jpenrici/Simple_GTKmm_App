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
    pBtn_search = nullptr;
    pBtn_clean = nullptr;
    pLbl_id = nullptr;
    pLbl_inform = nullptr;
    pLbl_status = nullptr;
    pTxt_search = nullptr;
    pTxt_first_name = nullptr;
    pTxt_last_name = nullptr;
    pTxt_email = nullptr;
    pTreeView = nullptr;

    refBuilder->get_widget_derived("appWindow", pGui);
    refBuilder->get_widget("btn_insert", pBtn_insert);
    refBuilder->get_widget("btn_update", pBtn_update);
    refBuilder->get_widget("btn_delete", pBtn_delete);
    refBuilder->get_widget("btn_search", pBtn_search);
    refBuilder->get_widget("btn_clean", pBtn_clean);
    refBuilder->get_widget("lbl_id", pLbl_id);
    refBuilder->get_widget("lbl_inform", pLbl_inform);
    refBuilder->get_widget("lbl_status", pLbl_status);
    refBuilder->get_widget("txt_search", pTxt_search);
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

    if (pBtn_search) {
        pBtn_search->signal_clicked().connect(sigc::mem_fun(*this,
            &Controller::gui_search));
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
    delete pBtn_search;
    delete pBtn_clean;
    delete pLbl_id;
    delete pLbl_inform;
    delete pLbl_status;
    delete pTxt_search;
    delete pTxt_first_name;
    delete pTxt_last_name;
    delete pTxt_email;
    delete pTreeView;
    delete pGui;

    return EXIT_SUCCESS;
}

void Controller::gui_insert()
{
    string first_name = prepare(pTxt_first_name->get_text(), EXP_NAME);
    string last_name = prepare(pTxt_last_name->get_text(), EXP_NAME);
    string email = prepare(pTxt_email->get_text(), EXP_EMAIL);
    int result = add_contact(first_name, last_name, email);

    if (result == 1) {
        gui_clean();
    }
    pLbl_status->set_text(INFO[result]);
}

void Controller::gui_update()
{
    string id = pLbl_id->get_text();
    string email = prepare(pTxt_email->get_text(), EXP_EMAIL);
    int result = update_contact(id, email);

    if (result == 1) {
        gui_clean();
    }
    pLbl_status->set_text(INFO[result]);
}

void Controller::gui_search()
{
    string search = pTxt_search->get_text();
    vector<vector<string> > result = search_contact("all", search);
    vector<vector<string> > table {{"ID", "First Name", "Last Name", "Email"}};
    table.insert(end(table), begin(result), end(result));  
    gui_update_treeView(table);
}

void Controller::gui_view()
{
    vector<vector<string> > result = read_contacts();
    vector<vector<string> > table {{"ID", "First Name", "Last Name", "Email"}};
    table.insert(end(table), begin(result), end(result));  
    gui_update_treeView(table);
}

void Controller::gui_delete()
{
    string id = pLbl_id->get_text();
    int result = remove_contact(id);

    if (result == 1) {
        gui_clean();
    }
    pLbl_status->set_text(INFO[result]);
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
    } else {
        pLbl_status->set_text(to_string(result.size() - 1) + " values found.");
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
    int result;
    string option;
    do {
        option = console.menu();
        vector<string> values;

        if (option == "ADD") {
            values = console.add_contact();
            result = add_contact(prepare(values[0], EXP_NAME),
                prepare(values[1], EXP_NAME), prepare(values[2], EXP_EMAIL));
            console.inform(INFO[result]);
        }

        if (option == "UPDATE") {
            values = console.update_contact();
            result = update_contact(prepare(values[0], EXP_NUM),
                prepare(values[1], EXP_EMAIL));
            console.inform(INFO[result]);         
        }

        if (option == "SEARCH") {
            console.view_contacts(search_contact("all", console.search_contact()));
        }          

        if (option == "VIEW") {
            console.view_contacts(read_contacts());
        }       

        if (option == "DEL") {
            result = remove_contact(prepare(console.remove_contact(), EXP_NUM));
            console.inform(INFO[result]);         
        }

    } while (option != "QUIT");

    return EXIT_SUCCESS;
}

/* 
 * MODEL CONTROL
 */
int Controller::add_contact(string first_name, string last_name, string email)
{
    if (first_name == "" || last_name == "") {
        return ERROR_NAME;
    }

    if (email == "") {
        return ERROR_EMAIL;
    }

    return contacts.add(first_name, last_name, email);
}

vector<vector<string> > Controller::search_contact(string field, string value)
{
    vector<vector<string> > result;
    if (field == "" || value == "") {
        result.clear();
        return result;
    }

    return contacts.search(field, value);
}

vector<vector<string> > Controller::read_contacts()
{
    return contacts.read();
}

int Controller::update_contact(string id, string email)
{
    if (email == "") {
        return ERROR_EMAIL;
    }

    if (id == "") {
        return ERROR_ID;
    }    

    return contacts.update(stoi(id), email);
}

int Controller::remove_contact(string id)
{
    if (id == "") {
        return ERROR_ID;
    }

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