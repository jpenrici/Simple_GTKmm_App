#include "model.hpp"

Contacts::Contacts()
{
    conn.command(database,
                "CREATE TABLE IF NOT EXISTS Contacts ("  \
                "ID INTEGER PRIMARY KEY AUTOINCREMENT," \
                "first_name TEXT NOT NULL," \
                "last_name TEXT NOT NULL," \
                "email TEXT NOT NULL);");
}

Contacts::~Contacts()
{
}

bool Contacts::add(string first_name, string last_name, string email)
{
    if (read("email", email).size() != 0) {
        return false;
    }

    string sql = "INSERT INTO Contacts (first_name, last_name, email) VALUES ";
    sql += "('" + first_name + "','"  + last_name + "','" + email + "');";
    conn.command(database, sql.c_str());

    return true;
}

vector<vector<string> > Contacts::read(string field, string value)
{
    string sql = "SELECT * FROM Contacts WHERE " + field + "='" + value + "';";
    return conn.command(database, sql.c_str());
}

vector<vector<string> > Contacts::read(int id)
{
    return read("ID", to_string(id));
}

vector<vector<string> > Contacts::read()
{
    return conn.command(database, "SELECT * FROM Contacts;");
}

bool Contacts::update(int id, string email)
{
    if (read("email", email).size() != 0) {
        return false;
    }

    if (read(id).size() == 0) {
        return false;
    }

    string sql = "UPDATE Contacts set email='" + email + "'";
    sql += " WHERE ID=" + to_string(id) + ";";
    conn.command(database, sql.c_str());

    return true;
}

bool Contacts::remove(int id)
{
    if (read(id).size() == 0) {
        return false;
    }   

    string sql = "DELETE from Contacts where ID=" + to_string(id) + ";";
    conn.command(database, sql.c_str());

    return true;
}
