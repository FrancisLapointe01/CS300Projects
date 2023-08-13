//============================================================================
// Name        : Project2.cpp
// Author      : Francis Lapointe
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <time.h>
#include <fstream>
#include "CSVparser.hpp"

using namespace std;

struct Course {
    string courseNum;
    string courseName;
    vector<string> preReq;

    Course() {}
};

struct Node {
    Course course;
    Node* left;
    Node* right;

    Node() {
        left = nullptr;
        right = nullptr;
    }

    Node(Course aCourse) : Node() {
        this->course = aCourse;
    }
};


class BST {
private:
    Node* root;
    void addNode(Node* node, Course course);
    void printSampleSchedule(Node* node);
    void printCourseInformation(Node* node, string courseNum);
public:
    BST(); // Default Constructor
    virtual ~BST(); // Destructor
    void Delete(Node* node); // Deletes all Course information
    void Insert(Course course); // Inserts Courses
    int NumPrerequisiteCourses(Course course); // Returns number of Prerequisite Courses
    void PrintSampleSchedule(); // Prints a sample Schedule
    void PrintCourseInformation(string courseNum); //Prints Course Infromation
};
BST::BST()
{
    // set root to null
    root = nullptr;
}

BST::~BST()
{
    // delete all nodes recursively
    Delete(root);
}

void BST::Delete(Node* node)
{
    // resursively deletes all nodes
    if (node) {
        Delete(node->left);
        Delete(node->right);
        delete node;
    }
}

void BST::Insert(Course course)
{
    // if root is not initialied
    // initilize it with the current course
    // else send node to the add Course Method to be added to the BST
    if (root == nullptr) {
        root = new Node(course);
    }
    else {
        this->addNode(root, course);
    }
}

int BST::NumPrerequisiteCourses(Course course)
{
    // returns number of prerequisites required
    int count = 0;
    for (unsigned int i = 0; i < course.preReq.size(); i++) {
        if (course.preReq.at(i).length() > 0)
            count++;
    }
    return count;
}

void BST::PrintSampleSchedule()
{
    this->printSampleSchedule(root);
}

void BST::PrintCourseInformation(string courseNum)
{
    this->printCourseInformation(root, courseNum);
}
void BST::addNode(Node* node, Course course)
{
    // If Current CourseNUm is less than the current node's CourseNum
    // add it to the left
    // Current couseNum is Greater than Current node's CourseNum
    // add it to the right
    if (node->course.courseNum.compare(course.courseNum) > 0) {
        if (node->left == nullptr) {
            node->left = new Node(course);
        }
        else {
            this->addNode(node->left, course);
        }
    }
    else {
        if (node->right == nullptr) {
            node->right = new Node(course);
        }
        else {
            this->addNode(node->right, course);
        }
    }
}

void BST::printSampleSchedule(Node* node)
{
    //prints Schedule in Order
    if (node != nullptr) {
        printSampleSchedule(node->left);
        cout << node->course.courseNum << " ," << node->course.courseName << endl;
        printSampleSchedule(node->right);
    }
    return;
}

void BST::printCourseInformation(Node* current, string courseNum)
{
    // while Current is not null
    // Checks if courseNum matches Current CourseNum
    // If prerequisites exist, display them
    // If no prerequisites exist, display that there is none
    // if courseNum is smaller, traversal left
    // if courseNum is larger, traversal right
    // if not found, display it was not found
    while (current != nullptr) {
        unsigned int i;
        if (current->course.courseNum.compare(courseNum) == 0) {
            cout << endl << current->course.courseNum << ", " << current->course.courseName << endl;
            unsigned int size = NumPrerequisiteCourses(current->course);
            cout << "Prerequisite Course(s)";
            for (i = 0; i < size; i++) {
                cout << current->course.preReq.at(i);
                if (i != size - 1) {
                    cout << ", ";
                }
            }
            if (i == 0)
                cout << "No Prerequisites" << endl;
            return;
        }
        else if (courseNum.compare(current->course.courseNum) < 0) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }
    cout << "Course " << courseNum << " Not Found" << endl;

}


//Load Courses
bool loadCourses(string csvPath, BST* bst) {
    // open course file
    // uses a vecotr to hold information
    // gets a substring of each course
    // adds commas to vector and deletes commas from string
    // add the last course after the comma
    // Load the separated values into a course
    // insert into Binary Search Tree 
    // close file
    try {
        ifstream courseFile(csvPath);

        if (courseFile.is_open()) {
            while (!courseFile.eof()) {
                vector<string> courseInfo;
                string courseData;
                getline(courseFile, courseData);
                while (courseData.length() > 0) {
                    unsigned int comma = courseData.find(',');
                    if (comma < 100) {
                        courseInfo.push_back(courseData.substr(0, comma));
                        courseData.erase(0, comma + 1);
                    }
                    else {
                        courseInfo.push_back(courseData.substr(0, courseData.length()));
                        courseData = "";
                    }
                }
                Course course;
                course.courseNum = courseInfo[0];
                course.courseName = courseInfo[1];
                for (unsigned int i = 2; i < courseInfo.size(); i++) {
                    course.preReq.push_back(courseInfo[i]);
                }
                bst->Insert(course);

            }
            courseFile.close();
            return true;
        }

    }
    catch (csv::Error& e) {
        cerr << e.what() << endl;
    }
    return false;
}

int main(int argc, char* argv[]) {

    //Process Command Line Arguments
    string csvPath, courseId;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    case 3:
        csvPath = argv[1];
        courseId = argv[2];
        break;
    default:
        csvPath = "data.txt";
        break;
    }
    // Define Binary Search Tree 
    // Initilizes Menu
    // Checks for double digit Numbers
    BST* bst = nullptr;
    cout << "Welcome to the Course Planner" << endl;
    string userChoice = "0";
    int choice = userChoice[0] - '0';

    while (choice != 4) {
        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Print course" << endl;
        cout << "  4. Exit " << endl;
        cout << "Enter choice: ";
        cin >> userChoice;
        if (userChoice.length() == 1)
            choice = userChoice[0] - '0';
        else
            choice = 0;
        bool success = 1;
        //Menu Choices
        switch (choice) {

        case 1:
            if (bst == nullptr) {
                bst = new BST();
            }
            success = loadCourses(csvPath, bst);

            if (success) {
                cout << "Courses Loaded" << endl;
            }
            else
                cout << "File not found." << endl;
            break;
        case 2:
            cout << "Here is a sample Schedule: " << endl;
            bst->PrintSampleSchedule();
            cout << endl;
            break;
        case 3:
            cout << "What course do you want to know about? ";
            cin >> courseId;
     for (auto& userChoice : courseId) userChoice = toupper(userChoice);
         bst->PrintCourseInformation(courseId);
            cout << endl;
                break;

        default:
            if (choice != 4)
                cout << userChoice << " is not a valid option" << endl;
            break;
        }
    }

    cout << "Thank you for using the course planner!" << endl;
    return 0;
}