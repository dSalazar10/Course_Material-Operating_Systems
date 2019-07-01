//
//  sqlite3.c
//  programs
//
//  Created by DANIEL SALAZAR on 4/6/18.
//
// http://zetcode.com/db/sqlitec/
//  Be sure to compile with the following: clang -o sqlite3 sqlite3.c -lsqlite3

#include <sqlite3.h>
#include <stdio.h>

int main(void)
{
    char* cars = {"AMG","Acura","Aston_Martin","Audi","BMW","Bentley",
    "Buick","Cadillac","Chevrolet","Chrysler","Dodge","Ferrari","Ford",
    "GMC","Grand_Cherokee","Honda","Hyundai","Infiniti","Jaguar","Jeep",
    "Kia","Lamborghini","Land_Rover","Lexus","Lincoln","Lotus","MINI",
    "Make","Maserati","Maybach","Mazda","Mercedes","Mercedes_Benz",
    "Mercury","Mitsubishi","Nissan","Porsche","Rolls_Royce","Saab",
    "Scion","Subaru","Suzuki","Toyota","Volkswagen","Volvo"};

    //  defines a database handle
    sqlite3 *db;
    // holds an error message number
    char *err_msg = 0;
    // SQL commands for building the Cars db
    char *sql = "DROP TABLE IF EXISTS Cars;"
                "CREATE TABLE Cars(Id INT, Name TEXT, Price INT);"
                "INSERT INTO Cars VALUES(1, 'Audi', 52642);"
                "INSERT INTO Cars VALUES(2, 'Mercedes', 57127);"
                "INSERT INTO Cars VALUES(3, 'Skoda', 9000);"
                "INSERT INTO Cars VALUES(4, 'Volvo', 29000);"
                "INSERT INTO Cars VALUES(5, 'Bentley', 350000);"
                "INSERT INTO Cars VALUES(6, 'Citroen', 21000);"
                "INSERT INTO Cars VALUES(7, 'Hummer', 41400);"
                "INSERT INTO Cars VALUES(8, 'Volkswagen', 21600);";
    // opens a new database connection.
    int rc = sqlite3_open("cars.db", &db);
    // if the connection wasn't successfully established
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }
    // combines prepare, step, and finalize
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    // if compilation wasn't successfully completed
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return -2;
    }
    // close the database connection
    sqlite3_close(db);
    
    return 0;
}
