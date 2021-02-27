#ifndef MODELUSER_H
#define MODELUSER_H

#include "connect_db.hpp"
#include <vector>
#include <string>

using namespace std;

class Contacts {

    public:
        Contacts();
        ~Contacts();

        bool add(string first_name, string last_name, string email);
        vector<vector<string> > search(string field, string value);
        vector<vector<string> > read(string field, string value);
        vector<vector<string> > read(int id);
        vector<vector<string> > read();
        bool update(int id, string email);
        bool remove(int id);

    private:
        Connect::SqliteDB conn;
        const char* database = "contacts.db";

};

#endif // MODELUSER_H