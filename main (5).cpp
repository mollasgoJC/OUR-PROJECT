#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <unordered_map> 

using namespace std;

class Employee {
public:
    string employee_id;
    string full_name;
    int age;
    string position;
    string department;
    string contact_number;
    string date_hired;
    double salary;
    string role_category; 
    
     Employee* next; 
     Employee(string id, string name, int a, string pos, string dept, string contact, string hired, double sal, string role)
        : employee_id(id), full_name(name), age(a), position(pos), department(dept), contact_number(contact), date_hired(hired), salary(sal), role_category(role), next(nullptr) {}
     string toString() const {
        stringstream ss;
        ss << left << setw(10) << employee_id
           << left << setw(100) << full_name
           << left << setw(5) << age
           << left << setw(15) << position
           << left << setw(25) << department
           << left << setw(15) << contact_number
           << left << setw(12) << date_hired
           << left << fixed << setprecision(2) << setw(15) << salary
           << left << setw(20) << role_category;
        return ss.str();
    }
    
    string toFileString() const {
        return employee_id + "|" + full_name + "|" + to_string(age) + "|" + position + "|" + department + "|" +
               contact_number + "|" + date_hired + "|" + to_string(salary) + "|" + role_category;
    }
};

class EmployeeManagementSystem {
private:
    Employee* head; 
    const string FILENAME = "employee_data.txt";
    const vector<string> ROLE_CATEGORIES = {"Manager", "Staff", "Intern", "Other"};

     struct BSTNode {
        string emp_id;
        BSTNode* left; 
        BSTNode* right; 
        BSTNode(string id) : emp_id(id), left(nullptr), right(nullptr) {}
    };

    BSTNode* bst_root = nullptr; 

public:
    EmployeeManagementSystem() : head(nullptr) {
        load_data();
    }

   
  ~EmployeeManagementSystem() {
        Employee* current = head;
        while (current) {
            Employee* next = current->next;
            delete current; 
            current = next;
        }
        head = nullptr;
    
        clear_bst(bst_root);
    }

    void save_data() {
        ofstream outFile(FILENAME);
        if (outFile.is_open()) {
            Employee* current = head;
            while (current) {
                outFile << current->toFileString() << "\n";
                current = current->next;
            }
            outFile.close();
            cout << "\nData automatically saved to " << FILENAME << ".\n";
        } else {
            cerr << "\nError: Unable to open file for saving.\n";
        }
    }

    void load_data() {
        ifstream inFile(FILENAME);
        if (inFile.is_open()) {
            string line;
            while (getline(inFile, line)) {
                stringstream ss(line);
                string segment;
                vector<string> data;

                while (getline(ss, segment, '|')) {
                    data.push_back(segment);
                }

                if (data.size() == 9) {
                    try {
                        add_employee_node(new Employee(data[0], data[1], stoi(data[2]), data[3], data[4], data[5], data[6], stod(data[7]), data[8]));
                    } catch (const exception& e) {
                        cerr << "Error parsing data on line: " << line << ". Skipping record.\n";
                    }
                }
            }
            inFile.close();
            cout << "Data automatically loaded from " << FILENAME << ".\n";
        }
    }
    
    void add_employee_node(Employee* newNode) {
        if (!head) {
            head = newNode; 
        } else {
            Employee* current = head;
            while (current->next) {
                current = current->next; 
            }
            current->next = newNode; 
        }
    }


    void add_employee(string id, string name, int age, string pos, string dept, string contact, string hired, double sal, string role) {
        if (find_employee_by_id(id, true)) {
            cout << "\nEmployee with ID " << id << " already exists. Cannot add.\n";
            return;
        }

        Employee* newEmployee = new Employee(id, name, age, pos, dept, contact, hired, sal, role);
        add_employee_node(newEmployee);
        cout << "\nEmployee " << name << " added successfully.\n";
        save_data();
    }

    void view_all_employees() const {
        if (!head) {
            cout << "\n SOrry The employee list is currently empty.\n";
            return;
        }

        cout << "\n---  All the Employees ---\n";
        cout << left << setw(10) << "ID" << left << setw(100) << "Full Name" << left << setw(5) << "Age"
             << left << setw(15) << "Position" << left << setw(25) << "Dept" << left << setw(15) << "Contact"
             << left << setw(12) << "Hired Date" << left << setw(15) << "Salary" << left << setw(20) << "Role\n";
        cout << string(217, '-') << endl;

        Employee* current = head;
        int count = 0;
        while (current) {
            cout << current->toString() << "\n";
            current = current->next;
            count++;
        }
        cout << string(217, '-') << endl;
        cout << "--- Total Employees: " << count << " ---\n";
    }

     Employee* find_employee_by_id(const string& id, bool silent = false) const {
        Employee* current = head;
        
        while (current) {
            if (current->employee_id == id) {
                if (!silent) {
                    cout << "\nEmployee Found: " << current->toString() << "\n";
                }
                return current; 
            }
            current = current->next;
        }
        
        if (!silent) {
            cout << "\nEmployee with ID " << id << " not found.\n";
        }
        return nullptr;
    }

    void delete_employee(const string& id) {
        Employee* current = head;
        Employee* prev = nullptr;

        if (current != nullptr && current->employee_id == id) {
            head = current->next;
            cout << "\nEmployee ID " << id << " (" << current->full_name << ") deleted successfully.\n";
            delete current; 
            save_data();
            return;
        }

        while (current != nullptr && current->employee_id != id) {
            prev = current; 
            current = current->next;
        }

        if (current == nullptr) {
            cout << "\nEmployee with ID " << id << " not found. Cannot delete.\n";
            return;
        }

        prev->next = current->next; 
        cout << "\nEmployee ID " << id << " (" << current->full_name << ") deleted successfully.\n";
        delete current;
        save_data();
    }

   vector<Employee*> convert_to_vector() const {
        vector<Employee*> vec;
        Employee* current = head;
        while (current) {
            vec.push_back(current);
            current = current->next;
        }
        return vec;
    }
    void rebuild_linked_list(const vector<Employee*>& vec) {
        if (vec.empty()) {
            head = nullptr;
            return;
        }

        head = vec[0];
        Employee* current = head;
        for (size_t i = 1; i < vec.size(); ++i) {
            current->next = vec[i];
            current = current->next;
        }
        current->next = nullptr; 
    }

    void bubble_sort_employees(const string& key) {
        vector<Employee*> employee_vec = convert_to_vector();
        size_t n = employee_vec.size();

        if (n <= 1) {
            cout << "\n Not enough employees to sort.\n";
            return;
        }

             for (size_t i = 0; i < n - 1; ++i) {
            bool swapped = false;
            for (size_t j = 0; j < n - i - 1; ++j) {
                bool should_swap = false;

                        if (key == "id" && employee_vec[j]->employee_id > employee_vec[j+1]->employee_id) {
                    should_swap = true;
                } else if (key == "name") {
                    string name1 = employee_vec[j]->full_name;
                    string name2 = employee_vec[j+1]->full_name;
                    transform(name1.begin(), name1.end(), name1.begin(), ::tolower);
                    transform(name2.begin(), name2.end(), name2.begin(), ::tolower);
                    if (name1 > name2) should_swap = true;
                } else if (key == "role" && employee_vec[j]->role_category > employee_vec[j+1]->role_category) {
                    should_swap = true;
                }

                if (should_swap) {
                    swap(employee_vec[j], employee_vec[j+1]);
                    swapped = true;
                }
            }
            if (!swapped) break; 
        }

        rebuild_linked_list(employee_vec);
        cout << "\nEmployees sorted by " << key << " using BS.\n";
        view_all_employees();
    }

     void categorize_by_role() const {
        unordered_map<string, vector<string>> role_hash_table;
        
        Employee* current = head;
        while (current) {
            string details = "ID: " + current->employee_id + ", Name: " + current->full_name;
            role_hash_table[current->role_category].push_back(details);
            current = current->next;
        }

        cout << "\n--- Employee Categorie ---\n";
        for (const auto& pair : role_hash_table) {
            cout << "\n**" << pair.first << " (" << pair.second.size() << ")**\n";
            if (!pair.second.empty()) {
                for (const auto& emp_details : pair.second) {
                    cout << "  - " << emp_details << "\n";
                }
            } else {
                cout << "  - No employees in this category.\n";
            }
        }
        cout << "===-----------------------------------===\n";
    }

    BSTNode* insert_id_bst(BSTNode* node, const string& emp_id) {
        if (node == nullptr) {
            return new BSTNode(emp_id);
        }
        if (emp_id < node->emp_id) {
            node->left = insert_id_bst(node->left, emp_id);
        } else if (emp_id > node->emp_id) {
            node->right = insert_id_bst(node->right, emp_id);
        }
        return node;
    }

    void build_id_bst() {
        clear_bst(bst_root); 
        bst_root = nullptr;
        
        Employee* current = head;
        while (current) {
            bst_root = insert_id_bst(bst_root, current->employee_id);
            current = current->next;
        }
    }
    
    void clear_bst(BSTNode* node) {
        if (node != nullptr) {
            clear_bst(node->left);
            clear_bst(node->right);
            delete node;
        }
    }

    BSTNode* search_id_bst(BSTNode* node, const string& emp_id) const {
        if (node == nullptr || node->emp_id == emp_id) {
            return node; 
        }
        
        if (emp_id < node->emp_id) {
            return search_id_bst(node->left, emp_id);
        }
        return search_id_bst(node->right, emp_id);
    }

    void search_bst_by_id(const string& emp_id) {
        build_id_bst();
        
        if (bst_root == nullptr) {
            cout << "\n Cannot search: No employees in the system.\n";
            return;
        }

        BSTNode* node = search_id_bst(bst_root, emp_id);
        
        if (node) {
            cout << "\nID " << emp_id << " found in the BST";
             find_employee_by_id(emp_id, false); 
        } else {
            cout << "\nID " << emp_id << " NOT found in BST.\n";
        }
    }
};

string get_input(const string& prompt, bool allow_empty = false) {
    string input;
    cout << prompt;
    getline(cin, input);
    if (!allow_empty && input.empty()) {
        while (input.empty()) {
            cout << " cannot be empty. " << prompt;
            getline(cin, input);
        }
    }
    return input;
}

int get_int_input(const string& prompt, int min_val = 0, int max_val = 200) {
    int value;
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        stringstream ss(input);
        if (ss >> value && ss.eof()) { 
            if (value >= min_val && value <= max_val) {
                return value;
            } else {
                cout << "Value must be between " << min_val << " and " << max_val << ".\n";
            }
        } else {
            cout << "Invalid input. Please enter a valid number.\n";
        }
    }
}

double get_double_input(const string& prompt) {
    double value;
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (input.empty()) {
            return -1.0; 
        }
        stringstream ss(input);
        if (ss >> value && ss.eof()) {
            if (value >= 0) {
                return value;
            } else {
                cout << "Salary must be a positive number (or zero).\n";
            }
        } else {
            cout << "Invalid input. Please enter a valid number.\n";
        }
    }
}



void show_menu() {
    cout << "\n" << string(50, '=') << "\n";
    cout << "\n==== Employee Management System ====\n";
    cout << "1. Add Employee\n";
    cout << "2. View All Employees\n";
    cout << "3. Search Employee by ID\n";
    cout << "4. Delete Employee\n";
    cout << "5. Sort Employees\n";
    cout << "6. Categorize Employees by Role\n";
    cout << "7. Exit Program\n";
    cout << string(50, '-') << "\n";
    cout << "Enter your choice: ";
}

void run_menu() {
    EmployeeManagementSystem ems;
    string choice;

    while (true) {
        show_menu();
        getline(cin, choice); 

        if (choice == "1") {
            cout << "\n--- Enter Employee Details ---\n";
            string id = get_input("Employee ID: ");
            string name = get_input("Full Name: ");
            int age = get_int_input("Age: ", 18, 50);
            string pos = get_input("Position / Job Title: ");
            string dept = get_input("Department: ");
            string contact = get_input("Contact Number: ");
            string hired = get_input("Date Hired (YYYY-MM-DD): ");
            double sal = get_double_input("Salary: ");
            string role = get_input("Role Category (e.g., Manager, Staff, Intern): ");

            ems.add_employee(id, name, age, pos, dept, contact, hired, sal, role);
        } 
        
        else if (choice == "2") { ems.view_all_employees(); } 
        
        else if (choice == "3") {
            string id = get_input("Enter Employee ID to search: ");
            ems.find_employee_by_id(id);
        } 
        
        else if (choice == "4") {
            string id = get_input("Enter Employee ID to DELETE: ");
            ems.delete_employee(id);
        } 
        
        else if (choice == "5") {
            string sort_by = get_input("Sort by: (1) ID, (2) Name, or (3) Role? ");
            if (sort_by == "1") ems.bubble_sort_employees("id");
            else if (sort_by == "2") ems.bubble_sort_employees("name");
            else if (sort_by == "3") ems.bubble_sort_employees("role");
            else cout << "Invalid sort option.\n";
        } 
        
        else if (choice == "6") { ems.categorize_by_role(); } 
        
        else if (choice == "") {
            cout << "\nExiting program. Goodbye!\n";
            break;
        } 
        
        else {
            cout << "\nInvalid choice. Please enter a number from the menu.\n";
        }
    }
}

int main() {
    run_menu();
    return 0;
}