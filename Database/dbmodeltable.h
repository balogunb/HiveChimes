#ifndef dbmodeltable_H
#define dbmodeltable_H
#include <iostream>
#include <string>
#include <sqlite3.h>

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "tool.h"
#include "dbtable.h"

class dbmodeltable: public DBTable
{
protected:
    std::string sql_select_all;
    std::string sql_update_row;
    std::string sql_select;
    int column =5;
public:

    dbmodeltable();
    dbmodeltable(Tool *db, std::string name);

    ~dbmodeltable();

    // An overloaded method to generate a new
    // create command for your child class.
    void store_create_sql();

    // An overloaded method to generate a new
    // insert command for your child class.
    virtual void store_add_row_sql();

    bool add_row_p(int id, std::string f_name,std::string l_name,
                 std::string game, std::string   score);

    bool update_row_p(int id, std::string game, std::string   score);
    char** select_row_p();
    bool select_all();
};


// This is a callback function that is sent to the library and used
// to parse the sql request being sent to the database.
int cb_add_row(void  *data,
               int    argc,
               char **argv,
               char **azColName);

int cb_update_row(void  *data,
               int    argc,
               char **argv,
               char **azColName);


// This is a callback function that is sent to the library and used
// to parse the sql request being sent to the database.
int cb_select_all(void  *data,
                  int    argc,
                  char **argv,
                  char **azColName);

#endif // dbmodeltable_H
