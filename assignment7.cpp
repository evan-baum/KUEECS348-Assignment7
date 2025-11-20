#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

// file to load sql file into given database using sql statement pointer
void setSQL(std::string fileName, sql::Statement *stmt) {
    std::ifstream tableFile(fileName); // gets file stream of sql file
    std::string line;                  // creates a line to read each line of the file
    std::string sqlCommand = "";       // creates empty string to hold commands to run

    if (tableFile.is_open()) // checks is the file can be opened or is available
    {
        while(getline(tableFile, line)){ // iterates across lines of the file
            if(!line.empty() && line.substr(0,2) != "--") { // checks that the line is not empty and is not a comment
                if(line.find(';') != std::string::npos){ // checks if there is a semicolon in the line
                    sqlCommand += line + " " ; // adds line and space to the sql command
                    stmt->execute(sqlCommand); // executes the concatenated sql command
                    sqlCommand = ""; // resets sql command
                }
                else{
                    sqlCommand += line + " "; // adds line to sql command if there is not semicolon
                }
            }
        }
    }
}

int main() {
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con; // to establish connection
    sql::Statement *stmt; // to execute SQL statement
    sql::ResultSet *res;  // a place to store the query results

    driver = sql::mysql::get_mysql_driver_instance(); // sets driver
    con = driver->connect("mysql.eecs.ku.edu", "348f25_e724b371", "Jah7EWek"); // connects to database with username and password

    stmt = con->createStatement(); // sets stmt to createStatement to run SQL commands

    stmt->execute("USE 348f25_e724b371"); // entering database

    setSQL("student7.SQL", stmt); // loads Student7 table
    setSQL("enrollment7.SQL", stmt); // loads Enrollment7 table
    setSQL("faculty7.SQL", stmt); // loads Faculty7 table
    setSQL("offering7.SQL", stmt); // loads Offering7 table
    setSQL("course7.SQL", stmt); // loads Course7 table

    std::cout << "1. Retrieve all students majoring in 'IS':" << std::endl; // prints question prompt
    res = stmt->executeQuery("SELECT * FROM Student7 WHERE StdMajor LIKE 'IS'"); // gets response from shown statement
    // prints column headers
    std::cout << std::setw(14) << "StdNo";
    std::cout << std::setw(14) << "StdFirstName";
    std::cout << std::setw(14) << "StdLastName";
    std::cout << std::setw(14) << "StdCity";
    std::cout << std::setw(10) << "StdState";
    std::cout << std::setw(14) << "StdZip";
    std::cout << std::setw(14) << "StdMajor";
    std::cout << std::setw(10) << "StdClass";
    std::cout << std::setw(10) << "StdGPA" << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------------------------" << std::endl;
    while (res->next()) // loops arcross all rows of returned response
    {
        // prints each column in the row
        std::cout << std::setw(14) << res->getString("StdNo");
        std::cout << std::setw(14) << res->getString("StdFirstName");
        std::cout << std::setw(14) << res->getString("StdLastName");
        std::cout << std::setw(14) << res->getString("StdCity");
        std::cout << std::setw(10) << res->getString("StdState");
        std::cout << std::setw(14) << res->getString("StdZip");
        std::cout << std::setw(14) << res->getString("StdMajor");
        std::cout << std::setw(10) << res->getString("StdClass");
        std::cout << std::setw(10) << res->getString("StdGPA") << std::endl;
        std::cout << "--------------------------------------------------------------------------------------------------------------------" << std::endl;
    }

    std::cout << std:: endl << "2. Find the names of students who have enrolled in more than two courses:" << std::endl; // prints question prompt
    res = stmt->executeQuery("SELECT StdNo FROM Enrollment7 GROUP BY StdNo HAVING COUNT(StdNo) > 1"); // gets response from shown statement
    // prints column headers
    std::cout << std::setw(14) << "StdNo" << std::endl;
    std::cout << "-----------------" << std::endl;
    while (res->next()) // loops arcross all rows of returned response
    {
        // prints each column in the row
        std::cout << std::setw(14) << res->getString("StdNo") << std::endl;
        std::cout << "-----------------" << std::endl;
    }

    std::cout << std:: endl << "3. All professors employed more than 15 years:" << std::endl; // prints question prompt
    res = stmt->executeQuery("SELECT FacFirstName, FacLastName FROM Faculty7 WHERE YEAR(FacHireDate) < 2011"); // gets response from shown statement
    // prints column headers
    std::cout << std::setw(14) << "FacFirstName";
    std::cout << std::setw(14) << "FacLastName" << std::endl;
    std::cout << "------------------------------" << std::endl;
    while (res->next()) // loops arcross all rows of returned response
    {
        // prints each column in the row
        std::cout << std::setw(14) << res->getString("FacFirstName");
        std::cout << std::setw(14) << res->getString("FacLastName") << std::endl;
        std::cout << "------------------------------" << std::endl;
    }

    std::cout << std:: endl << "4. Course numbers of courses offered Summer 2020:" << std::endl; // prints question prompt
    res = stmt->executeQuery("SELECT OfferNo FROM Offering7 WHERE OffTerm LIKE 'SUMMER' AND OffYear = 2020"); // gets response from shown statement
    // prints column headers
    std::cout << std::setw(8) << "OfferNo" << std::endl;
    std::cout << "---------" << std::endl;
    while (res->next()) // loops arcross all rows of returned response
    {
        // prints each column in the row
        std::cout << std::setw(8) << res->getString("OfferNo") << std::endl;
        std::cout << "---------" << std::endl;
    }

    std::cout << std:: endl << "5. First and last names of professors in ZIP code 98114:" << std::endl; // prints question prompt
    res = stmt->executeQuery("SELECT FacFirstName, FacLastName FROM Faculty7 WHERE FacZipCode LIKE '98114%'"); // gets response from shown statement
    // prints column headers
    std::cout << std::setw(14) << "FacFirstName";
    std::cout << std::setw(14) << "FacLastName" << std::endl;
    std::cout << "------------------------------" << std::endl;
    while (res->next()) // loops arcross all rows of returned response
    {
        // prints each column in the row
        std::cout << std::setw(14) << res->getString("FacFirstName");
        std::cout << std::setw(14) << res->getString("FacLastName") << std::endl;
        std::cout << "------------------------------" << std::endl;
    }

    std::cout << std:: endl << "6. Retrieve the second-highest GPA from the Student table:" << std::endl; // prints question prompt
    res = stmt->executeQuery("SELECT StdGPA FROM Student7 ORDER BY StdGPA DESC LIMIT 1 OFFSET 1"); // gets response from shown statement
    // prints column headers
    std::cout << std::setw(7) << "StdGPA" << std::endl;
    std::cout << "--------" << std::endl;
    while (res->next()) // loops arcross all rows of returned response
    {
        // prints each column in the row
        std::cout << std::setw(7) << res->getString("StdGPA") << std::endl;
        std::cout << "--------" << std::endl;
    }

    std::cout << std:: endl << "7. Individuals in the student and faculty tables:" << std::endl; // prints question prompt
    res = stmt->executeQuery("SELECT s.StdFirstName AS FirstName, s.StdLastName AS LastName FROM Student7 AS s INNER JOIN Faculty7 AS f ON s.StdNo=f.FacNo"); // gets response from shown statement
    // prints column headers
    std::cout << std::setw(14) << "FirstName";
    std::cout << std::setw(14) << "LastName" << std::endl;
    std::cout << "------------------------------" << std::endl;
    while (res->next()) // loops arcross all rows of returned response
    {
        // prints each column in the row
        std::cout << std::setw(14) << res->getString("FirstName");
        std::cout << std::setw(14) << res->getString("LastName") << std::endl;
        std::cout << "------------------------------" << std::endl;
    }

    std::cout << std:: endl << "8. Student number, first and last names, and the number of selected courses for each student:" << std::endl; // prints question prompt
    res = stmt->executeQuery("SELECT e.StdNo AS StdNo, s.StdFirstName AS FirstName, s.StdLastName AS LastName, COUNT(*) AS NumCourses FROM Enrollment7 AS e INNER JOIN Student7 AS s ON e.StdNo=s.StdNo GROUP BY e.StdNo"); // gets response from shown statement
    // prints column headers
    std::cout << std::setw(14) << "StdNo";
    std::cout << std::setw(14) << "StdFirstName";
    std::cout << std::setw(14) << "StdLastName";
    std::cout << std::setw(12) << "NumCourses" << std::endl;
    std::cout << "---------------------------------------------------------" << std::endl;
    while (res->next()) // loops arcross all rows of returned response
    {
        // prints each column in the row
        std::cout << std::setw(14) << res->getString("StdNo");
        std::cout << std::setw(14) << res->getString("FirstName");
        std::cout << std::setw(14) << res->getString("LastName");
        std::cout << std::setw(12) << res->getString("NumCourses") << std::endl;
        std::cout << "---------------------------------------------------------" << std::endl;
    }

    
    std::cout << std:: endl << "9. First and last names of professors, along with their salaries of top 3 highest paid professors:" << std::endl; // prints question prompt
    res = stmt->executeQuery("SELECT FacFirstName, FacLastName, FacSalary FROM Faculty7 ORDER BY FacSalary DESC LIMIT 3"); // gets response from shown statement
    // prints column headers
    std::cout << std::setw(14) << "FacFirstName";
    std::cout << std::setw(14) << "FacLastName";
    std::cout << std::setw(12) << "FacSalary" << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
    while (res->next()) // loops arcross all rows of returned response
    {
        // prints each column in the row
        std::cout << std::setw(14) << res->getString("FacFirstName");
        std::cout << std::setw(14) << res->getString("FacLastName");
        std::cout << std::setw(12) << res->getString("FacSalary") << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
    }

    std::cout << std:: endl << "10. Students who do not have any enrollment records:" << std::endl; // prints question prompt
    res = stmt->executeQuery("SELECT * FROM Enrollment7 AS e Right JOIN Student7 AS s ON e.StdNo=s.StdNo WHERE e.OfferNo IS NULL"); // gets response from shown statement
    // prints column headers
    std::cout << std::setw(14) << "StdNo";
    std::cout << std::setw(14) << "StdFirstName";
    std::cout << std::setw(14) << "StdLastName";
    std::cout << std::setw(14) << "StdCity";
    std::cout << std::setw(10) << "StdState";
    std::cout << std::setw(14) << "StdZip";
    std::cout << std::setw(14) << "StdMajor";
    std::cout << std::setw(10) << "StdClass";
    std::cout << std::setw(10) << "StdGPA" << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------------------------" << std::endl;
    while (res->next()) // loops arcross all rows of returned response
    {
        // prints each column in the row
        std::cout << std::setw(14) << res->getString("StdNo");
        std::cout << std::setw(14) << res->getString("StdFirstName");
        std::cout << std::setw(14) << res->getString("StdLastName");
        std::cout << std::setw(14) << res->getString("StdCity");
        std::cout << std::setw(10) << res->getString("StdState");
        std::cout << std::setw(14) << res->getString("StdZip");
        std::cout << std::setw(14) << res->getString("StdMajor");
        std::cout << std::setw(10) << res->getString("StdClass");
        std::cout << std::setw(10) << res->getString("StdGPA") << std::endl;
        std::cout << "--------------------------------------------------------------------------------------------------------------------" << std::endl;
    }
    

    std::cout << std:: endl << "11. Add Student Alice Smith to Student table and displaying whole table:" << std::endl; // prints question prompt
    stmt->execute("INSERT INTO Student7 (StdNo, StdFirstName, StdLastName, StdCity, StdState, StdMajor, StdClass, StdGPA, StdZip) VALUES ('888-88-8888','ALICE','SMITH','TOPEKA','KS','CS','JR',2.85,'66610')"); // gets response from shown statement
    res = stmt->executeQuery("SELECT * FROM Student7");
    // prints column headers
    std::cout << std::setw(14) << "StdNo";
    std::cout << std::setw(14) << "StdFirstName";
    std::cout << std::setw(14) << "StdLastName";
    std::cout << std::setw(14) << "StdCity";
    std::cout << std::setw(10) << "StdState";
    std::cout << std::setw(14) << "StdZip";
    std::cout << std::setw(14) << "StdMajor";
    std::cout << std::setw(10) << "StdClass";
    std::cout << std::setw(10) << "StdGPA" << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------------------------" << std::endl;
    while (res->next()) // loops arcross all rows of returned response
    {
        // prints each column in the row
        std::cout << std::setw(14) << res->getString("StdNo");
        std::cout << std::setw(14) << res->getString("StdFirstName");
        std::cout << std::setw(14) << res->getString("StdLastName");
        std::cout << std::setw(14) << res->getString("StdCity");
        std::cout << std::setw(10) << res->getString("StdState");
        std::cout << std::setw(14) << res->getString("StdZip");
        std::cout << std::setw(14) << res->getString("StdMajor");
        std::cout << std::setw(10) << res->getString("StdClass");
        std::cout << std::setw(10) << res->getString("StdGPA") << std::endl;
        std::cout << "--------------------------------------------------------------------------------------------------------------------" << std::endl;
    }

    std::cout << std:: endl << "12. Update student Bob Norbert to live in Overland Park and display student table:" << std::endl; // prints question prompt
    stmt->execute("UPDATE Student7 SET StdCity='Overland Park', StdState='KS', StdZip='66210' WHERE StdFirstName LIKE 'Bob' AND StdLastName LIKE 'Norbert'"); // gets response from shown statement
    res = stmt->executeQuery("SELECT * FROM Student7");
    // prints column headers
    std::cout << std::setw(14) << "StdNo";
    std::cout << std::setw(14) << "StdFirstName";
    std::cout << std::setw(14) << "StdLastName";
    std::cout << std::setw(14) << "StdCity";
    std::cout << std::setw(10) << "StdState";
    std::cout << std::setw(14) << "StdZip";
    std::cout << std::setw(14) << "StdMajor";
    std::cout << std::setw(10) << "StdClass";
    std::cout << std::setw(10) << "StdGPA" << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------------------------" << std::endl;
    while (res->next()) // loops arcross all rows of returned response
    {
        // prints each column in the row
        std::cout << std::setw(14) << res->getString("StdNo");
        std::cout << std::setw(14) << res->getString("StdFirstName");
        std::cout << std::setw(14) << res->getString("StdLastName");
        std::cout << std::setw(14) << res->getString("StdCity");
        std::cout << std::setw(10) << res->getString("StdState");
        std::cout << std::setw(14) << res->getString("StdZip");
        std::cout << std::setw(14) << res->getString("StdMajor");
        std::cout << std::setw(10) << res->getString("StdClass");
        std::cout << std::setw(10) << res->getString("StdGPA") << std::endl;
        std::cout << "--------------------------------------------------------------------------------------------------------------------" << std::endl;
    }
    
    delete res; // deletes sql response variable
    delete stmt; // deletes sql statement variable
    delete con; // deletes sql connection variable

    return 0;
}