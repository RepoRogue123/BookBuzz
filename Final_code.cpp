#include<bits/stdc++.h>

using namespace std;


// Class to represent a book
class Book {
public:
    int id;
    string title;
    int quantity;
    int rating;
    vector<int> currentBorrowers;
    int availableCopies;

    Book() : id(0), title(""), quantity(0), rating(0), availableCopies(0) {}

/*Constructor for the book object with the given pararmeters*/
    Book(int id, string title, int quantity, int rating) 
        : id(id), title(title), quantity(quantity), 
          rating((rating >= 0 && rating <= 10) ? rating : 0), 
          availableCopies(quantity) {}

/*This method returns a string representing the level of the book based on its rating*/
    string getRatingLevel() const {
        if (rating >= 8) return "Advanced";
        else if (rating >= 4) return "Intermediate";
        else return "Beginner";
    }

    bool isAvailable() const { return availableCopies > 0; }

    void decrementCopies() { if (availableCopies > 0) availableCopies--; }

    void incrementCopies() { if (availableCopies < quantity) availableCopies++; }

    void addBorrower(int userId) { currentBorrowers.push_back(userId); }

    void removeBorrower(int userId) {
        currentBorrowers.erase(
            remove(currentBorrowers.begin(), currentBorrowers.end(), userId),
            currentBorrowers.end());
    }
};

// User class
class User {
public:
    int id;
    string username;
    string password;
    string email;
    bool isAdmin;

    User() : id(0), username(""), password(""), email(""), isAdmin(false) {}

    User(int id, string username, string password, string email, bool isAdmin)
        : id(id), username(username), password(password), email(email), isAdmin(isAdmin) {}
};

// Loan class
class Loan {
public:
    int id;
    int userId;
    int bookId;
    string issueDate;
    string dueDate;
    string returnDate;
    bool isReturned;

    Loan() : id(0), userId(0), bookId(0), isReturned(false) {}

    

    Loan(int id, int userId, int bookId, string issueDate, string dueDate)
        : id(id), userId(userId), bookId(bookId), issueDate(issueDate), 
          dueDate(dueDate), isReturned(false), returnDate("") {}
};

// AVL Tree Node
class AVLNode {
public:
    Book book;
    AVLNode *left;
    AVLNode *right;
    int height;

    AVLNode(Book b) : book(b), left(NULL), right(NULL), height(1) {}
};

// AVL Tree Class
class AVLTree {
private:
    AVLNode *root;

    int height(AVLNode *node) { return node ? node->height : 0; }

    int getBalance(AVLNode *node) {
        return node ? height(node->left) - height(node->right) : 0;
    }

    AVLNode *rightRotate(AVLNode *y) {
        AVLNode *x = y->left;
        AVLNode *T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;
        return x;
    }

    AVLNode *leftRotate(AVLNode *x) {
        AVLNode *y = x->right;
        AVLNode *T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;
        return y;
    }

    AVLNode *insert(AVLNode *node, Book book) {
        if (node == NULL) return new AVLNode(book);
        
        if (book.id < node->book.id)
            node->left = insert(node->left, book);
        else if (book.id > node->book.id)
            node->right = insert(node->right, book);
        else {
            node->book.quantity += book.quantity;
            node->book.availableCopies += book.quantity;
            return node;
        }

        node->height = 1 + max(height(node->left), height(node->right));
        int balance = getBalance(node);

        if (balance > 1 && book.id < node->left->book.id)
            return rightRotate(node);
        if (balance < -1 && book.id > node->right->book.id)
            return leftRotate(node);
        if (balance > 1 && book.id > node->left->book.id) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && book.id < node->right->book.id) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }

    AVLNode *minValueNode(AVLNode *node) {
        AVLNode *current = node;
        while (current && current->left != NULL)
            current = current->left;
        return current;
    }

    AVLNode *deleteNode(AVLNode *root, int id) {
        if (root == NULL) return root;

        if (id < root->book.id)
            root->left = deleteNode(root->left, id);
        else if (id > root->book.id)
            root->right = deleteNode(root->right, id);
        else {
            if (root->book.quantity > 1) {
                root->book.quantity--;
                root->book.availableCopies--;
                return root;
            }
            if ((root->left == NULL) || (root->right == NULL)) {
                AVLNode *temp = root->left ? root->left : root->right;
                if (temp == NULL) {
                    temp = root;
                    root = NULL;
                } else
                    *root = *temp;
                delete temp;
            } else {
                AVLNode *temp = minValueNode(root->right);
                root->book = temp->book;
                root->right = deleteNode(root->right, temp->book.id);
            }
        }

        if (root == NULL) return root;

        root->height = 1 + max(height(root->left), height(root->right));
        int balance = getBalance(root);

        if (balance > 1 && getBalance(root->left) >= 0)
            return rightRotate(root);
        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
        if (balance < -1 && getBalance(root->right) <= 0)
            return leftRotate(root);
        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }

    

public:
    AVLTree() : root(NULL) {}

    void inorderTraversal(AVLNode *node, vector<Book> &books) const {
        if (node != NULL) {
            inorderTraversal(node->left, books);
            books.push_back(node->book);
            inorderTraversal(node->right, books);
        }
    }

    void insert(Book book) {
        root = insert(root, book);
    }

    void remove(int id) {
        root = deleteNode(root, id);
    }

    Book* search(int id) const {
        AVLNode *current = root;
        while (current != NULL) {
            if (id == current->book.id)
                return &(current->book);
            else if (id < current->book.id)
                current = current->left;
            else
                current = current->right;
        }
        return NULL;
    }
    AVLNode*getRoot() const{
        return root;
    }

    void displayAll() const {
        vector<Book> books;
        inorderTraversal(root, books);
        for (const auto &book : books) {
            cout << "ID: " << book.id 
                 << ", Title: " << book.title 
                 << ", Available: " << book.availableCopies 
                 << "/" << book.quantity << endl;
        }
    }
    

};

// Function prototypes
void loadBooks(AVLTree &library);
void loadUsers(vector<User> &users);
void loadLoans(vector<Loan> &loans);
User  login(const vector<User> &users);
bool signUp(vector<User> &users);
void addBook(AVLTree &library);
void removeBook(AVLTree &library);
void searchBook(const AVLTree &library);
void displayAllBooks(const AVLTree &library);
void manageUsers(vector<User> &users);
void manageLoans(vector<Loan> &loans, AVLTree &library, const vector<User> &users);
void borrowBook(const User &user, AVLTree &library, vector<Loan> &loans);
void returnBook(const User &user, AVLTree &library, vector<Loan> &loans);
void viewMyLoans(const User &user, const vector<Loan> &loans);
void saveBooks(const AVLTree &library);
void saveUsers(const vector<User> &users);
void saveLoans(const vector<Loan> &loans);

int main() {
    AVLTree library;
    vector<User> users;
    vector<Loan> loans;

    // Check if files exist, if not, create them with initial data
    ifstream booksFile("books.txt");
    if (!booksFile.good()) {
        ofstream newBooksFile("books.txt");
        newBooksFile << "1 Sample Book,5 7" << endl;
        newBooksFile.close();
    }
    booksFile.close();

    ifstream usersFile("users.txt");
    if (!usersFile.good()) {
        ofstream newUsersFile("users.txt");
        newUsersFile << "1 admin,admin123,admin@library.com,1" << endl;
        newUsersFile.close();
    }
    usersFile.close();

    ifstream loansFile("loans.txt");
    if (!loansFile.good()) {
        ofstream newLoansFile("loans.txt");
        newLoansFile.close();
    }
    loansFile.close();

    loadBooks(library);
    loadUsers(users);
    loadLoans(loans);

    int choice;
    do {
        cout << "\n=== Library Management System ===\n";
        cout << "1. Login\n";
        cout << "2. Sign Up\n";
        cout << "3. Exit\n";
        cout << "Choose an option: ";
        cin >> choice;

        switch(choice) {
            case 1: {
                User user = login(users);
                if (user.id != 0) {
                    if (user.isAdmin) {
                        // Admin Menu
                        int adminChoice;
                        do {
                            cout << "\n=== Admin Menu ===\n";
                            cout << "1. Add Book\n";
                            cout << "2. Remove Book\n";
                            cout << "3. Display All Books\n";
                            cout << "4. Manage Users\n";
                            cout << "5. Manage Loans\n";
                            cout << "6. Search Book\n";
                            cout << "7. Logout\n";
                            cout << "Choose an option: ";
                            cin >> adminChoice;

                            switch (adminChoice) {
                                case 1: 
                                    addBook(library); 
                                    break;
                                case 2: 
                                    removeBook(library); 
                                    break;
                                case 3: 
                                    displayAllBooks(library); 
                                    break;
                                case 4: 
                                    manageUsers(users); 
                                    break;
                                case 5: 
                                    manageLoans(loans, library, users); 
                                    break;
                                case 6: 
                                    searchBook(library); 
                                    break;
                                case 7: 
                                    cout << "Logged out successfully!" << endl; 
                                    break;
                                default: 
                                    cout << "Invalid choice!" << endl; 
                                    break;
                            }
                        } while (adminChoice != 7);
                    } else {
                        // Regular User Menu
                        int userChoice;
                        do {
                            cout << "\n=== User Menu ===\n";
                            cout << "1. Borrow Book\n";
                            cout << "2. Return Book\n";
                            cout << "3. View My Loans\n";
                            cout << "4. Search Book\n";
                            cout << "5. Display All Books\n";
                            cout << "6. Logout\n";
                            cout << "Choose an option: ";
                            cin >> userChoice;

                            switch (userChoice) {
                                case 1: 
                                    borrowBook(user, library, loans); 
                                    break;
                                case 2: 
                                    returnBook(user, library, loans); 
                                    break;
                                case 3: 
                                    viewMyLoans(user, loans); 
                                    break;
                                case 4: 
                                    searchBook(library); 
                                    break;
                                case 5: 
                                    displayAllBooks(library); 
                                    break;
                                case 6: 
                                    cout << "Logged out successfully!" << endl; 
                                    break;
                                default: 
                                    cout << "Invalid choice!" << endl; 
                                    break;
                            }
                        } while (userChoice != 6);
                    }
                }
                break;
            }
            case 2: {
                signUp(users);
                break;
            }
            case 3: {
                cout << "Thank you for using the Library Management System!\n";
                break;
            }
            default: {
                cout << "Invalid choice! Please try again.\n";
                break;
            }
        }
    } while (choice != 3);

    // Save all data before exiting
    saveBooks(library);
    saveUsers(users);
    saveLoans(loans);

    return 0;
}

void loadBooks(AVLTree &library) {
    ifstream file("books.txt");
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int id, quantity, rating;
        string title;
        ss >> id >> ws;
        getline(ss, title, ',');
        ss >> quantity >> rating;
        Book book(id, title, quantity, rating);
        library.insert(book);
    }
    file.close();
}

void loadUsers(vector<User> &users) {
    ifstream file("users.txt");
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int id;
        string username, password, email;
        bool isAdmin;
        ss >> id >> ws;
        getline(ss, username, ',');
        getline(ss, password, ',');
        getline(ss, email, ',');
        ss >> isAdmin;
        users.push_back(User(id, username, password, email, isAdmin));
    }
    file.close();
}

void loadLoans(vector<Loan> &loans) {
    ifstream file("loans.txt");
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int id, userId, bookId;
        string issueDate, dueDate;
        ss >> id >> userId >> bookId >> ws;
        getline(ss, issueDate, ',');
        getline(ss, dueDate, ',');
        loans.push_back( Loan(id, userId, bookId, issueDate, dueDate));
    }
    file.close();
}

User  login(const vector<User> &users) {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    for (const auto &user : users) {
        if (user.username == username && user.password == password) {
            return user;
        }
    }
    cout << "Invalid credentials!" << endl;
    return User(); // Return an empty user
}

bool signUp(vector<User> &users) {
    string username, password, email;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;
    cout << "Enter email: ";
    cin >> email;

    int newId = users.size() + 1; // Simple ID assignment
    users.push_back(User(newId, username, password, email, false));
    
    // Save users to file immediately after adding new user
    ofstream file("users.txt");
    for (const auto &user : users) {
        file << user.id << " " << user.username << "," << user.password << "," << user.email << "," << user.isAdmin << endl;
    }
    file.close();
    
    cout << "User registered successfully!" << endl;
    return true;
}

void addBook(AVLTree &library) {
    int id, quantity, rating;
    string title;
    cout << "Enter book ID: ";
    cin >> id;
    cout << "Enter book title: ";
    cin.ignore();
    getline(cin, title);
    cout << "Enter quantity: ";
    cin >> quantity;
    cout << "Enter rating (0-10): ";
    cin >> rating;

    Book book(id, title, quantity, rating);
    library.insert(book);
    
    // Save all books to file
    saveBooks(library);
    cout << "Book added successfully!" << endl;
}

void removeBook(AVLTree &library) {
    int id;
    cout << "Enter book ID to remove: ";
    cin >> id;
    library.remove(id);
    
    // Save changes to file
    saveBooks(library);
    cout << "Book removed successfully!" << endl;
}

void searchBook(const AVLTree &library) {
    int id;
    cout << "Enter book ID to search: ";
    cin >> id;
    Book *book = library.search(id);
    if (book) {
        cout << "Book found: " << book->title << ", Available: " << book->availableCopies << endl;
    } else {
        cout << "Book not found!" << endl;
    }
}

void displayAllBooks(const AVLTree &library) {
    library.displayAll();
}

void manageUsers(vector<User>& users) {
    int choice;
    do {
        cout << "\n=== User Management ===\n";
        cout << "1. View All Users\n";
        cout << "2. Add New User\n";
        cout << "3. Delete User\n";
        cout << "4. Back to Admin Menu\n";
        cout << "Choose an option: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                cout << "\nAll Users:\n";
                for (const auto& user : users) {
                    cout << "ID: " << user.id 
                         << ", Username: " << user.username 
                         << ", Email: " << user.email 
                         << ", Admin: " << (user.isAdmin ? "Yes" : "No") << endl;
                }
                break;
            }
            case 2: {
                string username, password, email;
                bool isAdmin;
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                cout << "Enter email: ";
                cin >> email;
                cout << "Make admin? (1 for yes, 0 for no): ";
                cin >> isAdmin;

                int newId = users.size() + 1;
                users.push_back(User(newId, username, password, email, isAdmin));
                cout << "User added successfully!" << endl;
                break;
            }
            case 3: {
                int userId;
                cout << "Enter user ID to delete: ";
                cin >> userId;
                
                auto it = remove_if(users.begin(), users.end(),
                    [userId](const User& user) { return user.id == userId; });
                
                if (it != users.end()) {
                    users.erase(it, users.end());
                    cout << "User deleted successfully!" << endl;
                } else {
                    cout << "User not found!" << endl;
                }
                break;
            }
            case 4:
                cout << "Returning to admin menu...\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 4);
}

void manageLoans(vector<Loan>& loans, AVLTree& library, const vector<User>& users) { 
    int choice; 
    do { 
        cout << "\n=== Loan Management ===\n"; 
        cout << "1. View All Loans\n"; 
        cout << "2. View Overdue Loans\n"; 
        cout << "3. Add New Loan\n"; 
        cout << "4. Mark Loan as Returned\n"; 
        cout << "5. Back to Admin Menu\n"; 
        cout << "Choose an option: "; 
        cin >> choice; 

        switch (choice) { 
            case 1: { 
                cout << "\nAll Loans:\n"; 
                for (const auto& loan : loans) { 
                    cout << "Loan ID: " << loan.id 
                         << ", User ID: " << loan.userId 
                         << ", Book ID: " << loan.bookId 
                         << ", Issue Date: " << loan.issueDate 
                         << ", Due Date: " << loan.dueDate 
                         << ", Returned: " << (loan.isReturned ? "Yes" : "No") << endl; 
                } 
                break; 
            } 
            case 2: { 
               
                cout << "\nOverdue Loans:\n"; 
                for (const auto& loan : loans) { 
                    if (!loan.isReturned) { 
                        cout << "Loan ID: " << loan.id 
                             << ", User ID: " << loan.userId 
                             << ", Book ID: " << loan.bookId 
                             << ", Due Date: " << loan.dueDate << endl; 
                    } 
                } 
                break; 
            } 
            case 3: { 
                int userId, bookId; 
                cout << "Enter user ID: "; 
                cin >> userId; 
                cout << "Enter book ID: "; 
                cin >> bookId; 

                // Check if book exists and is available 
                Book* book = library.search(bookId); 
                if (book && book->isAvailable()) { 
                    // Get current date 


                    auto now = chrono::system_clock::now(); 
                    time_t now_time = chrono::system_clock::to_time_t(now); 
                    tm *issueDate = localtime(&now_time); 

                    // Format issue date as dd--mm-yyyy 
                    ostringstream issueDateStream; 
                    issueDateStream << setfill('0') << setw(2) << issueDate->tm_mday << "--" 
                                    << setfill('0') << setw(2) << (issueDate->tm_mon + 1) << "--" 
                                    << (issueDate->tm_year + 1900); 
                    string issueDateStr = issueDateStream.str(); 

                    // Calculate due date (14 days later) 
                    issueDate->tm_mday += 14; // Add 14 days 
                    mktime(issueDate); // Normalize the date 
                    ostringstream dueDateStream; 
                    dueDateStream << setfill('0') << setw(2) << issueDate->tm_mday << "--" 
                                  << setfill('0') << setw(2) << (issueDate->tm_mon + 1) << "--" 
                                  << (issueDate->tm_year + 1900); 
                    string dueDateStr = dueDateStream.str(); 

                    // Create new loan ID 
                    int newLoanId = loans.size() + 1; 

                    // Create a new loan record 
                    Loan newLoan(newLoanId, userId, bookId, issueDateStr, dueDateStr); 
                    loans.push_back(newLoan); 
                    book->decrementCopies(); // Decrease available copies 

                    // Find the user object to access user ID 
                    auto it = std::find_if(users.begin(), users.end(), [userId](const User& user) { 
                        return user.id == userId; 
                    }); 

                    if (it != users.end()) { 
                        book->addBorrower(it->id); // Use the found user 
                        cout << "Book borrowed successfully!" << endl; 
                        cout << "Issue Date: " << issueDateStr << ", Due Date: " << dueDateStr << endl; 

                        // Save loans to file 
                        saveLoans(loans); 
                    } else { 
                        cout << "User  not found!" << endl; 
                    } 
                } else { 
                    cout << "Book not available or doesn't exist!" << endl; 
                } 
                break; 
            } 
            case 4: { 
                int loanId; 
                cout << "Enter loan ID to mark as returned: "; 
                cin >> loanId; 

                auto it = std::find_if(loans.begin(), loans.end(), [loanId](const Loan& loan) { 
                    return loan.id == loanId; 
                }); 

                if (it != loans.end()) { 
                    it->isReturned = true; 
                    cout << "Loan marked as returned!" << endl; 
                    // Optionally, you can increment the book's available copies here


                    Book* book = library.search(it->bookId);

                    if (book) {

                        book->incrementCopies(); // Increase available copies
                    }

                    // Save updated loans to file
                    saveLoans(loans);

                } else { 

                    cout << "Loan not found!" << endl; 
                } 
                break; 
            } 
            case 5: { 
                cout << "Returning to Admin Menu..." << endl; 
                break; 
            } 
            default: { 
                cout << "Invalid option. Please try again." << endl; 
                break; 
            } 
        } 
    } while (choice != 5); 
}

void borrowBook(const User &user, AVLTree &library, vector<Loan> &loans) {
    int bookId;
    cout << "Enter book ID to borrow: ";
    cin >> bookId;
    Book *book = library.search(bookId);

    if (book && book->isAvailable()) {
        // Get current date
        auto now = chrono::system_clock::now();
        time_t now_time = chrono::system_clock::to_time_t(now);
        tm *issueDate = localtime(&now_time);

        // Format issue date as dd--mm-yyyy
        ostringstream issueDateStream;
        issueDateStream << setfill('0') << setw(2) << issueDate->tm_mday << "--"
                        << setfill('0') << setw(2) << (issueDate->tm_mon + 1) << "--"
                        << (issueDate->tm_year + 1900);
        string issueDateStr = issueDateStream.str();

        // Calculate due date (14 days later)
        issueDate->tm_mday += 14; // Add 14 days
        mktime(issueDate); // Normalize the date
        ostringstream dueDateStream;
        dueDateStream << setfill('0') << setw(2) << issueDate->tm_mday << "--"
                      << setfill('0') << setw(2) << (issueDate->tm_mon + 1) << "--"
                      << (issueDate->tm_year + 1900);
        string dueDateStr = dueDateStream.str();

        // Create new loan ID
        int newLoanId = loans.size() + 1;

        // Create a new loan record
        Loan newLoan(newLoanId, user.id, bookId, issueDateStr, dueDateStr);
        loans.push_back(newLoan);
        book->decrementCopies(); // Decrease available copies
        book->addBorrower(user.id);

        cout << "Book borrowed successfully!" << endl;
        cout << "Issue Date: " << issueDateStr << ", Due Date: " << dueDateStr << endl;

        // Save loans to file
        saveLoans(loans);
    } else {
        cout << "Error: Book is not available or does not exist!" << endl;
    }
}

void returnBook(const User &user, AVLTree &library, vector<Loan> &loans) {
    int bookId;
    cout << "Enter book ID to return: ";
    cin >> bookId;

    for (auto &loan : loans) {
        if (loan.userId == user.id && loan.bookId == bookId && !loan.isReturned) {
            loan.isReturned = true; // Mark the loan as returned
            Book *book = library.search(bookId);

            if (book) {
                book->incrementCopies(); // Increment available copies
                book->removeBorrower(user.id); // Remove borrower
            }
            cout << "Book returned successfully!" << endl;

            // Save loans to file after updating the return status
            saveLoans(loans);
            return;
        }
    }
    cout << "No active loan found for this book!" << endl;
}

void viewMyLoans(const User &user, const vector<Loan> &loans) {
    cout << "Your loans:" << endl;

    for (const auto &loan : loans) {

        if (loan.userId == user.id) {
            cout << "Book ID: " << loan.bookId 
                 << ", Issue Date: " << loan.issueDate 
                 << ", Due Date: " << loan.dueDate 
                 << ", Returned: " << (loan.isReturned ? "Yes" : "No") << endl;
        }
    }
}

void saveBooks(const AVLTree &library) {
    ofstream file("books.txt");

    if (file.is_open()) {
        vector<Book> books;
        library.inorderTraversal(library.getRoot(), books);

        for (const auto &book : books) {
            file << book.id << " " << book.title << "," << book.quantity << " " << book.rating << endl;
        }
        file.close();
    } else {
        cout << "Error: Could not save books to file!" << endl;
    }
}


void saveUsers(const vector<User> &users) {
    ofstream file("users.txt");

    for (const auto &user : users) {
        file << user.id << " " << user.username << "," << user.password << "," << user.email << "," << user.isAdmin << endl;
    }
    file.close();
}

void saveLoans(const vector<Loan> &loans) {
    ofstream file("loans.txt");
    
    for (const auto &loan : loans) {
        file << loan.id << " " << loan.userId << " " << loan.bookId << " " << loan.issueDate << "," << loan.dueDate << endl;
    }
    file.close();
}