/*
    Group       : Group8
    Name		: Kyeongsoo Lee, Hyungchul You, Heeyoung Yang
    Section		: NCC
    Date		: Aug. 03. 2021
    Description : Project MS2
*/

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <occi.h>
#include <iomanip>
#include <cstring>

using oracle::occi::Environment;
using oracle::occi::Connection;
using namespace oracle::occi;
using namespace std;
using std::setw;
using std::left;

struct Employee {
    int  employeeNumber;
    char lastName[50];
    char firstName[50];
    char extension[10];
    char email[100];
    char officecode[10];
    int  reportsTo;
    char jobTitle[50];
};

int menu();
int findEmployee(Connection* conn, int employeeNumber, struct Employee* emp);
void displayEmployee(Connection* conn, struct Employee emp);
void displayAllEmployees(Connection* conn);
void getEmployee(struct Employee* emp);
void insertEmployee(Connection* conn, struct Employee emp);
void updateEmployee(Connection* conn, int employeeNumber);
void deleteEmployee(Connection* conn, int employeeNumber);


int main(void)
{
    /* OCCI Variables */
    Environment* env = nullptr;
    Connection* conn = nullptr;
        
    /* Used Variables */
    string user = "dbs211_212c39";
    string pass = "23719284";
    string constr = "myoracle12c.senecacollege.ca:1521/oracle12c";

    int option, findResult;
    int employeeNum;
    Employee employee;

    try {
        env = Environment::createEnvironment(Environment::DEFAULT);
        conn = env->createConnection(user, pass, constr);

        do {
        option = menu();
        
            switch (option)
            {
            case(0):
                break;
            case(1):
                cout << "Enter Employee Number: ";
                cin >> employeeNum;
                while (cin.fail()) {
                    cout << "Invalid number, Try again: ";
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cin >> employeeNum;
                }
                cin.ignore(10000, '\n');
                
                findResult = findEmployee(conn, employeeNum, &employee);
                if (findResult) {
                    cout << endl;
                    displayEmployee(conn, employee);
                }
                else
                    cout << "Employee " << employeeNum << " does not exist." << endl << endl;
                break;
            case(2):
                displayAllEmployees(conn);
                break;
            case(3):
                cout << endl;
                getEmployee(&employee);
                insertEmployee(conn, employee);
                cout << endl;
                break;
            case(4):
                cout << "Enter Employee Number: ";
                cin >> employeeNum;
                while (cin.fail()) {
                    cout << "Invalid number, Try again: ";
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cin >> employeeNum;
                }
                cin.ignore(10000, '\n');
                updateEmployee(conn, employeeNum);
                cout << endl;
                break;
            case(5):
                cout << "Enter Employee Number: ";
                cin >> employeeNum;
                while (cin.fail()) {
                    cout << "Invalid number, Try again: ";
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cin >> employeeNum;
                }
                cin.ignore(10000, '\n');
                deleteEmployee(conn, employeeNum);
                cout << endl;
                break;
            }
        } while (option);

        env->terminateConnection(conn);
        Environment::terminateEnvironment(env);
    }
    catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }

    return 0;
}

int menu() {
    int option;

    cout << "********************* HR Menu *********************" << endl;
    cout << "1) Find Employee" << endl;
    cout << "2) Employees Report" << endl;
    cout << "3) Add Employee" << endl;
    cout << "4) Update Employee" << endl;
    cout << "5) Remove Employee" << endl;
    cout << "0) Exit" << endl;
    cout << "Enter an option (0-5): ";
    cin >> option;
    while (cin.fail() || option < 0 || option > 5) {
        if (option < 0 || option > 5) cout << "Invalid option. Try again (0-5): ";
        else cout << "Invalid number, Try again: ";
        cin.clear();
        cin.ignore(10000, '\n');
        cin >> option;
    };
    cin.ignore(10000, '\n');

    return option;
}

int findEmployee(Connection* conn, int employeeNumber, struct Employee* emp) {
    int result; 
    Statement* stmt = nullptr;
    ResultSet* rs = nullptr;

    stmt = conn->createStatement("SELECT * FROM dbs211_employees WHERE employeenumber = :1");
    stmt->setInt(1, employeeNumber);
    rs = stmt->executeQuery();  
    
    if(rs->next()) {
        result = 1;

        emp->employeeNumber = rs->getInt(1);
        string const* lastName = new string(rs->getString(2));
        strcpy_s(emp->lastName, lastName->c_str());
        string const* firstName = new string(rs->getString(3));
        strcpy_s(emp->firstName, firstName->c_str());
        string const* extension = new string(rs->getString(4));
        strcpy_s(emp->extension, rs->getString(4).c_str());
        string const* email = new string(rs->getString(5));
        strcpy_s(emp->email, email->c_str());
        string const* officecode = new string(rs->getString(6));
        strcpy_s(emp->officecode, officecode->c_str());
        emp->reportsTo = rs->getInt(7);
        string const* jobTitle = new string(rs->getString(8));
        strcpy_s(emp->jobTitle, jobTitle->c_str());

        lastName = nullptr;
        firstName = nullptr;
        extension = nullptr;
        email = nullptr;
        officecode = nullptr;
        jobTitle = nullptr;
    }
    else {
        result= 0;
    }

    conn->terminateStatement(stmt);
    return result;
}

void displayEmployee(Connection* conn, struct Employee emp) {
    cout << "--------------Employee Information-------------" << endl;
    cout << "Employee Number: " << emp.employeeNumber << endl;
    cout << "Last Name: " << emp.lastName << endl;
    cout << "First Name: " << emp.firstName << endl;
    cout << "Extension: " << emp.extension << endl;
    cout << "Email: " << emp.email << endl;
    cout << "Office Code: " << emp.officecode << endl;
    cout << "Manager ID: " << emp.reportsTo << endl;
    cout << "Job Title: " << emp.jobTitle << endl << endl;
}

void displayAllEmployees(Connection* conn) {

    cout << endl;
    cout.fill('-');
    cout << setw(7) << "  " << setw(20) << "  " << setw(40) << "  " << setw(20) << "  " << setw(12) << "  " << setw(20) << "  " << endl;
   
    cout.fill(' ');
    cout << left << setw(7) << "ID" << setw(20) << "Employee Name" << setw(40) << "Email" << setw(20) << "Phone" << setw(12) << "Extension" << "Manager Name" << endl;

    cout.fill('-'); 
    cout << setw(5) << "" << setw(20) << "  " << setw(40) << "  " << setw(20) << "  " << setw(12) << "  " << setw(20) << "  " << endl;
    cout.fill(' ');

    Statement* stmt = conn->createStatement("SELECT employeenumber, name, email, phone, extension, mgrName FROM (SELECT a.employeenumber, a.firstname || ' ' || a.lastname AS name, a.extension, a.email, a.officecode, b.firstname || ' ' || b.lastname as mgrName FROM dbs211_employees a LEFT JOIN dbs211_employees b ON a.reportsto = b.employeenumber) e JOIN dbs211_offices o ON e.officecode = o.officecode ORDER BY employeenumber");
     ResultSet* rs = stmt->executeQuery();

    if (rs->next()) {
        
        do {
            int employeeNum = rs->getInt(1);
            string const* name = new string(rs->getString(2));
            string const* email = new string(rs->getString(3));
            string const* phone = new string(rs->getString(4));
            string const* extension = new string(rs->getString(5));
            string const* mgrName = new string(rs->getString(6));

        cout << left << setw(7) << employeeNum << setw(20) << *name << setw(40) << *email << setw(20) << *phone << setw(12) << *extension << *mgrName << endl;
        
        name = nullptr;
        email = nullptr;
        phone = nullptr;
        extension = nullptr;
        mgrName = nullptr;

        } while (rs->next());
    }
    else {
        cout << "There is no employees¡¯ information to be displayed." << endl; 
    }
    cout << endl;

    conn->terminateStatement(stmt);
}

void getEmployee(struct Employee* emp) {
    cout << "-------------- New Employee Information -------------" << endl;
    cout << "Employee Number: ";
    cin >> emp->employeeNumber;
    cin.ignore(1000, '\n');
    cout << "Last Name: ";
    cin.getline(emp->lastName, 50, '\n');
    cout << "First Name: ";
    cin.getline(emp->firstName, 50, '\n');
    cout << "Extension: ";
    cin.getline(emp->extension, 10, '\n');
    cout << "Email: ";
    cin.getline(emp->email, 100, '\n');
    cout << "Office Code: 1" << endl;
    strcpy_s(emp->officecode, "1");
    cout << "Manager ID: 1002" << endl;
    emp->reportsTo = 1002;
    cout << "Job Title: ";
    cin.getline(emp->jobTitle, 50, '\n');
    cout << endl;
}

void insertEmployee(Connection* conn, struct Employee emp) {
    Statement* stmt = conn->createStatement();
    
    if (!findEmployee(conn, emp.employeeNumber, &emp)) {
        
        stmt->setSQL("INSERT INTO dbs211_employees VALUES (:1,:2,:3,:4,:5,:6,:7,:8) ");
        stmt->setInt(1, emp.employeeNumber);
        stmt->setString(2, emp.lastName);
        stmt->setString(3, emp.firstName);
        stmt->setString(4, emp.extension);
        stmt->setString(5, emp.email);
        stmt->setString(6, emp.officecode);
        stmt->setInt(7, emp.reportsTo);
        stmt->setString(8, emp.jobTitle);
        stmt->executeUpdate();
        cout << "The new employee is added successfully." << endl;
    }
    else cout << "An employee with the same employee number exists." << endl;

    conn->commit();
    conn->terminateStatement(stmt);
}

void updateEmployee(Connection* conn, int employeeNumber) {
    Statement* stmt = conn->createStatement();
    Employee employee;
    if (findEmployee(conn, employeeNumber, &employee)) {
        cout << "Last Name: " << employee.lastName << endl;
        cout << "First Name: " << employee.firstName << endl;
        cout << "Extension: ";
        cin.getline(employee.extension, 10, '\n');

        stmt->setSQL("UPDATE dbs211_employees SET extension = :1 WHERE employeeNumber = :2");
        stmt->setString(1, employee.extension);
        stmt->setInt(2, employeeNumber);
        stmt->executeUpdate();

        cout << "The employee's extension is updated successfully." << endl;
    }
    else cout << "The employee with ID " << employeeNumber << " does not exist." << endl;

    conn->commit();
    conn->terminateStatement(stmt);

}

void deleteEmployee(Connection* conn, int employeeNumber) {

    Employee employee;

    if (findEmployee(conn, employeeNumber, &employee)) {
        Statement* stmt = conn->createStatement();
        stmt->setSQL("DELETE FROM dbs211_employees WHERE employeeNumber = :1");
        stmt->setInt(1, employeeNumber);
        stmt->executeUpdate();
        cout << "The employee with ID " << employeeNumber << " is deleted successfully." << endl;

        conn->commit();
        conn->terminateStatement(stmt);
    }
    else cout << "The employee with ID " << employeeNumber << " does not exist." << endl;
}