#ifndef CONSOLEAPP_H
#define CONSOLEAPP_H

#include <iostream>
#include <vector>

using namespace std;

class ConsoleApp
{
    public:
        string menu();
        vector<string> add_contact();
        void view_contacts(vector<vector<string> > result);
        vector<string> update_contact();
        string remove_contact();
        void inform(string text);

    private:
        void view(vector<vector<string> > result);
};

#endif // CONSOLEAPP_H
