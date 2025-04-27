//============================================================================
// Name        : Project2.cpp
// Author      : Justin Lennox
// Version     : 1.0
// Description : Project 2 ABCU
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;
// define a structure to hold course information

struct Course {
	string courseNumber;
	string courseTitle;
	vector<string> prerequisites;

	// default constructor
	Course() : courseNumber(""), courseTitle("") {}

	// Parameter constructor
	Course(string number, string title, vector<string> prereqs) :
		courseNumber(number), courseTitle(title), prerequisites(prereqs) {
	}
};

// Internal structure for tree node
struct Node {
	Course course;
	Node* left;
	Node* right;

	// default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}

	// initialize with a course
	Node(Course c) :course(c), left(nullptr), right(nullptr) {}

};

// Binary Search Tree class to store courses
class storeCoursesInTree {
private:
	Node* root;

	//Helper to destory the tree
	void destroyTree(Node* node) {
		if (node) {
			destroyTree(node->left);
			destroyTree(node->right);
			delete node;
		}
	}

	//Insert to input a new node into the tree
	void insert(Node*& node, Course course) {
		if (!node) {
			node = new Node(course);
		}
		else if (course.courseNumber < node->course.courseNumber) {
			insert(node->left, course);
		}
		else {
			insert(node->right, course);
		}
	}

	//Private call for in order print to print the classes in alphanumeric order
	void inOrderPrint(Node* node) {
		if (node) {
			inOrderPrint(node->left);
			cout << node->course.courseNumber << ": " << node->course.courseTitle << endl;
			inOrderPrint(node->right);
		}
	}
	//Private search to search the nodes for a course number
	Node* search(Node* node, string courseNumber) {
		if (!node || node->course.courseNumber == courseNumber) {
			return node;
		}
		if (courseNumber < node->course.courseNumber) {
			return search(node->left, courseNumber);
		}
		return search(node->right, courseNumber);
	}

public:
	storeCoursesInTree() : root(nullptr) {}

	
	~storeCoursesInTree() {
		destroyTree(root); // Call the helper to free memory
		// Destructor to free memory
	}

	//Public insert course call
	void insertCourse(Course course) { 
		insert(root, course); 
	}

	void printAllCourses() { //Print all courses or inform the person that no courses are available.
		if (!root) {
			cout << "No courses available." << endl;
			return;
		}
		
		cout << "Courses in alphanumeric order:" << endl;
		inOrderPrint(root);
	}

	Node* searchCourse(string courseNumber) { //Public call to search for a course
		return search(root, courseNumber);
	}

	void loadCoursesFromFile(string fileName) { // load all courses by file
		ifstream file(fileName);
		if (!file.is_open()) { //if file is not open, print error
			cout << "Error: Could not open file!" << endl;
			return;
		}
		else if (file.is_open()) { //if file is open, print file opened successfully
			cout << "File opened successfully!" << endl;
		}

		string line;
		while (getline(file, line)) {
			if (line.empty()) {
				continue; // Skip empty lines
			}
			stringstream ss(line);
			string courseNumber, courseTitle, prereq; //read the file by course number, course tiltle, and prereqs
			vector<string> prerequisites;

			if (!getline(ss, courseNumber, ',') || !getline(ss, courseTitle, ',')) { // if the line doesn't have a 
				//course number or a course title, print out an error
				cout << "Error: Invalid line format!" << endl;
				continue;
			}

			while (getline(ss, prereq, ',')) { //if file has prereqs, pushback the prereq
				prerequisites.push_back(prereq);
			}
			Course newCourse(courseNumber, courseTitle, prerequisites);
			insertCourse(newCourse); //insert new course
		}

		cout << "Courses successfully loaded." << endl;
		file.close(); //close file
	}
	Node* getRoot() { return root;}
};

void menu() { //function for menu
	cout << "1 Load courses into data structure." << endl;
	cout << "2 Print all courses in alphanumeric order." << endl;
	cout << "3 Print course title and prerequisites." << endl;
	cout << "9 Exit program." << endl;
}
int main() {

	storeCoursesInTree coursesTree; // Create an instance of the BST class
	int option = 0; // Variable to store user input for menu option
	string courseNumber; // Variable to store course number input by user
	
	cout << "Welcome to the ABCU Advising Program!" << endl;
	cout << "This program will help you find courses and their prerequisites." << endl;
	while (option != 9) { // Loop until user chooses to exit
		cout << "Please select an option from the menu below." << endl;
		menu(); // Call the menu function to start the program
		cin >> option; // Get user input for menu option
		Node* result = nullptr; // Initialize result pointer to nullptr
		
		switch (option) {
			case 1:
				coursesTree.loadCoursesFromFile("CS 300 ABCU_Advising_Program_Input.csv");
				// Load courses into data structure
				break;
			case 2:
				coursesTree.printAllCourses();
				// Print all courses in alphanumeric order
				break;
			case 3:
				cout << "Enter course number: ";
				cin >> courseNumber; // Get user input for course number

				if (courseNumber.empty()) {
					cout << "Error! Course number cannot be empty." << endl;
					break; // Exit the function if course number is empty
				}

				result = coursesTree.searchCourse(courseNumber); // Search for the course in the tree
				if (result) {
					cout << "Course Found: " << result->course.courseTitle << endl;
					if (!result->course.prerequisites.empty()) {
						cout << "Prerequisites: ";
						for (const string& prereq : result->course.prerequisites) {
							cout << prereq << " ";
						}
						cout << endl;
					}
				}
				else {
					cout << "Error! Course not found." << endl;
				}

				break; // Exit the function if course is found
			case 9:
				cout << "Exiting program." << endl;
				return 0; // Exit the menu loop
			default:
				cout << "Invalid option. Please try again." << endl;
				break; // Handle invalid option
			}

	}
	return 0; // Exit the program
}
