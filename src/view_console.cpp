#include "view_console.hpp"

string ConsoleApp::menu()
{
    string menu = "Contacts: [1] Add [2] Update [3] View [4] Remove [0] Qui";

    int answer;
    string option = "";

    cout << menu << endl;
    cout << "Select option: ";
    cin >> answer;

    switch (answer)
    {
        case 1:
            option = "ADD";
            break;
        case 2:
            option = "UPDATE";
            break;          
        case 3:
            option = "VIEW";
            break;
        case 4:
            option = "DEL";
            break;              
        case 0:
            option = "QUIT";
            break;          
        default:
            inform("Invalid option.");
    }

    return option;
}

vector<string> ConsoleApp::add_contact()
{
    string value;
    vector<string> data;

    cout << "First Name: ";
    cin >> value;
    data.push_back(value);

    cout << "Last Name: ";
    cin >> value;
    data.push_back(value);

    cout << "Email: ";
    cin >> value;
    data.push_back(value);  

    return data;
}

void ConsoleApp::view_contacts(vector<vector<string> > result)
{
    vector<vector<string> > table {{"ID","First Name", "Last Name", "Email"}};
    table.insert(end(table), begin(result), end(result));
    view(table);
}

vector<string> ConsoleApp::update_contact()
{
    string value;
    vector<string> data;

    cout << "ID: ";
    cin >> value;
    data.push_back(value);

    cout << "Email: ";
    cin >> value;
    data.push_back(value);

    return data;
}

string ConsoleApp::remove_contact()
{
    string value;

    cout << "ID: ";
    cin >> value;

    return value;
}

void ConsoleApp::inform(string text)
{
    cout << text << endl;
}

void ConsoleApp::view(vector<vector<string> > result)
{
    unsigned int len = 15;
    for (unsigned int row = 0; row < result.size(); ++row) {
        for (unsigned int col = 0; col < result[row].size(); ++col) {
            string v = result[row][col];
            cout << v.substr(0, v.size() > len ? len : v.size());
            cout << string(len - v.size(), ' ');
        }
        cout << endl;
    }
}
