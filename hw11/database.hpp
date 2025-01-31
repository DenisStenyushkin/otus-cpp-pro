#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <sqlite3.h>


class Database {
public:
    using JoinRow = std::vector<std::tuple<int, std::string, std::string>>;

    Database() {
        char* zErrMsg = nullptr;
        int rc;

        rc = sqlite3_open("", &db_);
        if (rc) {
            throw std::runtime_error("Unable to open database");
        }

        const char* tableA_sql = "CREATE TABLE A(" \
                                 "ID INT PRIMARY KEY  NOT NULL," \
                                 "NAME           TEXT NOT NULL);";
        const char* tableB_sql = "CREATE TABLE B(" \
                                 "ID INT PRIMARY KEY  NOT NULL," \
                                 "NAME           TEXT NOT NULL);";
        
        rc = sqlite3_exec(db_, tableA_sql, nullptr, 0, &zErrMsg);
        if (rc) {
            throw std::runtime_error("Unable to create table A: " + std::string(zErrMsg));
        }

        rc = sqlite3_exec(db_, tableB_sql, nullptr, 0, &zErrMsg);
        if (rc) {
            throw std::runtime_error("Unable to create table B: " + std::string(zErrMsg));
        }
    }

    void insert(const std::string& table, int id, const std::string& name) {
        std::stringstream sql;
        int rc;
        char* zErrMsg = nullptr;

        sql << "INSERT INTO " << table << " (ID, NAME) ";
        sql << "VALUES (" << id << ", '" << name << "');";
        std::cout << "SQL: " << sql.str() << std::endl;

        rc = sqlite3_exec(db_, sql.str().c_str(), nullptr, 0, &zErrMsg);
        if (rc) {
            throw std::runtime_error("Unable to insert: " + std::string(zErrMsg));
        }
    }

    void truncate(const std::string& table) {
        std::stringstream sql;
        int rc;
        char* zErrMsg = nullptr;

        sql << "DELETE FROM " << table << ";";
        std::cout << "SQL: " << sql.str() << std::endl;

        rc = sqlite3_exec(db_, sql.str().c_str(), nullptr, 0, &zErrMsg);
        if (rc) {
            throw std::runtime_error("Unable to truncate: " + std::string(zErrMsg));
        }
    }

    JoinRow intersection() {
        return exec_join_sql("SELECT A.id, A.name, B.name "
                             "FROM A INNER JOIN B "
                             "ON A.id = B.id "
                             "ORDER BY A.ID ASC;");
    }

    JoinRow symmetric_difference() {
        std::stringstream sql;
        sql << "SELECT A.ID, A.name, B.name "
               "FROM A LEFT JOIN B "
               "ON A.id = B.id "
               "UNION ALL "
               "SELECT B.id, A.name, B.name "
               "FROM B LEFT JOIN A "
               "ON B.id = A.id "
               "EXCEPT "
               "SELECT A.id, A.name, B.name "
               "FROM A INNER JOIN B "
               "ON A.id = B.id;";
        return exec_join_sql(sql.str().c_str());
    }

    ~Database() {
        sqlite3_close(db_);
    }

private:
     JoinRow exec_join_sql(const char* sql) {
        sqlite3_stmt* stmt = nullptr;
        int rc;
        JoinRow result;

        rc = sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr);
        if (rc) {
            throw std::runtime_error("Unable to execute " + std::string(sql));
        }

        rc = sqlite3_step(stmt);
        while (rc != SQLITE_DONE && rc != SQLITE_OK) {
            int id = sqlite3_column_int(stmt, 0);
            const unsigned char* name1 =  sqlite3_column_text(stmt, 1);
            const unsigned char* name2 =  sqlite3_column_text(stmt, 2);

            std::string name1_s = name1 != nullptr ? std::string((const char*)name1) : "";
            std::string name2_s = name2 != nullptr ? std::string((const char*)name2) : "";

            result.emplace_back(id, name1_s, name2_s);
            rc = sqlite3_step(stmt);
        }

        rc = sqlite3_finalize(stmt);

        return result;
     }

    sqlite3* db_;
};
