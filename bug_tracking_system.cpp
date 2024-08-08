#include <iostream>
#include <string>
#include <mysql/mysql.h>

// MySQL connection settings
const char* host = "localhost";
const char* user = "root";
const char* password = "password";
const char* database = "bug_tracking_system";

// Function to connect to MySQL database
MYSQL* connectToDatabase() {
    MYSQL* conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, host, user, password, database, 0, NULL, 0)) {
        std::cerr << "Error connecting to database: " << mysql_error(conn) << std::endl;
        exit(1);
    }
    return conn;
}

// Function to create a new bug report
void createBugReport(MYSQL* conn, std::string title, std::string description, std::string priority) {
    std::string query = "INSERT INTO bugs (title, description, priority) VALUES (?, ?, ?)";
    MYSQL_STMT* stmt = mysql_stmt_init(conn);
    mysql_stmt_prepare(stmt, query.c_str(), query.length());
    mysql_stmt_bind_param(stmt, 0, title.c_str());
    mysql_stmt_bind_param(stmt, 1, description.c_str());
    mysql_stmt_bind_param(stmt, 2, priority.c_str());
    mysql_stmt_execute(stmt);
    mysql_stmt_close(stmt);
}

// Function to retrieve all bug reports
void getBugReports(MYSQL* conn) {
    std::string query = "SELECT * FROM bugs";
    MYSQL_STMT* stmt = mysql_stmt_init(conn);
    mysql_stmt_prepare(stmt, query.c_str(), query.length());
    mysql_stmt_execute(stmt);
    MYSQL_RES* result = mysql_stmt_result(stmt);
    while (MYSQL_ROW row = mysql_fetch_row(result)) {
        std::cout << "ID: " << row[0] << ", Title: " << row[1] << ", Description: " << row[2] << ", Priority: " << row[3] << std::endl;
    }
    mysql_stmt_close(stmt);
}

// Function to update a bug report
void updateBugReport(MYSQL* conn, int id, std::string title, std::string description, std::string priority) {
    std::string query = "UPDATE bugs SET title = ?, description = ?, priority = ? WHERE id = ?";
    MYSQL_STMT* stmt = mysql_stmt_init(conn);
    mysql_stmt_prepare(stmt, query.c_str(), query.length());
    mysql_stmt_bind_param(stmt, 0, title.c_str());
    mysql_stmt_bind_param(stmt, 1, description.c_str());
    mysql_stmt_bind_param(stmt, 2, priority.c_str());
    mysql_stmt_bind_param(stmt, 3, &id);
    mysql_stmt_execute(stmt);
    mysql_stmt_close(stmt);
}

// Function to delete a bug report
void deleteBugReport(MYSQL* conn, int id) {
    std::string query = "DELETE FROM bugs WHERE id = ?";
    MYSQL_STMT* stmt = mysql_stmt_init(conn);
    mysql_stmt_prepare(stmt, query.c_str(), query.length());
    mysql_stmt_bind_param(stmt, 0, &id);
    mysql_stmt_execute(stmt);
    mysql_stmt_close(stmt);
}

int main() {
    MYSQL* conn = connectToDatabase();

    // Create a new bug report
    createBugReport(conn, "Bug 1", "This is a bug report", "High");

    // Retrieve all bug reports
    getBugReports(conn);

    // Update a bug report
    updateBugReport(conn, 1, "Bug 1 Updated", "This is an updated bug report", "Medium");

    // Delete a bug report
    deleteBugReport(conn, 1);

    mysql_close(conn);
    return 0;
}
