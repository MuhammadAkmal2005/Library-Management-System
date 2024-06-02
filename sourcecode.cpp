#include <iostream>
#include <cstring>
#include <limits>
#include <fstream>
#include <conio.h>

using namespace std;

// Constants for maximum number of books and members
const int MAX_BOOKS = 100;
const int MAX_MEMBERS = 100;

// File path for storing the password
const string PASSWORD_FILE = "password.txt";

// Structure to represent a book
struct Book
{
    char title[100];
    char author[50];
    int year;
    int id;
};

// Structure to represent a member
struct Member
{
    char name[50];
    int id;
};

// Global variable to track login attempts
unsigned short int loginAttempts = 5;

// Function prototypes
void logInSystem();
void menuOptions();
bool verifyPassword(const string &inputPassword);
void addBook(Book books[], int &bookCount);
void listBooks(const Book books[], int bookCount);
void findBook(const Book books[], int bookCount);
void deleteBook(Book books[], int &bookCount);
void addMember(Member members[], int &memberCount);
void listMembers(const Member members[], int memberCount);
void deleteMember(Member members[], int &memberCount);
void issueBook(int issuedBooks[MAX_BOOKS][MAX_MEMBERS], int bookCount, int memberCount);
void listIssuedBooks(const int issuedBooks[MAX_BOOKS][MAX_MEMBERS], const Book books[], int bookCount, const Member members[], int memberCount);
void passwordReset();
void savePassword(const string &newPassword);
void saveBooksToFile(const Book books[], int bookCount);
void saveMembersToFile(const Member members[], int memberCount);
void loadBooksFromFile(Book books[], int &bookCount);
void loadMembersFromFile(Member members[], int &memberCount);
void logOutSystem(const Book books[], int &bookCount);

int main()
{
    // Start of the program
    cout << "\n----- You are accessing Admin Portal -----\n";
    // Call login function
    logInSystem();
    return 0;
}

// Function for user login
void logInSystem()
{
    cout << "\n\nEnter password: ";
    string getPass;
    getline(cin, getPass);

    // Verify password
    if (verifyPassword(getPass))
    {
        cout << "\nSuccessfully Logged In!\n";
        // Display menu options after successful login
        menuOptions();
    }
    else
    {
        loginAttempts--;
        if (loginAttempts != 0)
        {
            cout << "You entered the wrong password.\nYou have " << loginAttempts << " attempts remaining!";
            cout << "\n\nTry again...";
            logInSystem();
        }
        else
        {
            cout << "\nSystem has been blocked! Contact your Organization.\n\n";
            return;
        }
    }
}

// Function to display menu options
void menuOptions()
{
    // Declare arrays for books, members, and issued books
    Book books[MAX_BOOKS];
    Member members[MAX_MEMBERS];
    int bookCount = 0;
    int memberCount = 0;
    int issuedBooks[MAX_BOOKS][MAX_MEMBERS] = {0};

    // Load data from files
    loadBooksFromFile(books, bookCount);
    loadMembersFromFile(members, memberCount);

    // Display menu options
    while (true)
    {
        cout << "\n\n----- Library Management System -----\n\n";
        cout << " 1. Add Book\n";
        cout << " 2. List Books\n";
        cout << " 3. Find Book\n";
        cout << " 4. Delete Book\n";
        cout << " 5. Add Member\n";
        cout << " 6. List Members\n";
        cout << " 7. Delete Member\n";
        cout << " 8. Issue Book\n";
        cout << " 9. List Issued Books\n";
        cout << "10. System Password Reset\n";
        cout << "11. Exit\n\n";
        cout << "Choose an option: ";

        // Input choice from user
        int choice;
        cin >> choice;

        // Switch statement for handling different menu options
        switch (choice)
        {
        case 1:
        {
            addBook(books, bookCount);
            break;
        }
        case 2:
        {
            listBooks(books, bookCount);
            break;
        }
        case 3:
        {
            findBook(books, bookCount);
            break;
        }
        case 4:
        {
            deleteBook(books, bookCount);
            break;
        }
        case 5:
        {
            addMember(members, memberCount);
            break;
        }
        case 6:
        {
            listMembers(members, memberCount);
            break;
        }
        case 7:
        {
            deleteMember(members, memberCount);
            break;
        }
        case 8:
        {
            issueBook(issuedBooks, bookCount, memberCount);
            break;
        }
        case 9:
        {
            listIssuedBooks(issuedBooks, books, bookCount, members, memberCount);
            break;
        }
        case 10:
            passwordReset();
            break;
        case 11:
        {
            cout << "\n----- System Logged out Successfully! -----\n\n";
            cout << "Press any key to close the program...\n\n";
            if (getch())
            {
                exit(0); // Exit the program
            }
        }
        default:
        {
            cout << "Invalid choice. Please try again." << endl;
            menuOptions();
        }
        }
        logOutSystem(books, bookCount);
    }
}

// Function to verify password
bool verifyPassword(const string &inputPassword)
{
    ifstream booksFile(PASSWORD_FILE);
    if (!booksFile)
    {
        cerr << "Error opening password file." << endl;
        return false;
    }

    string storedPassword;
    getline(booksFile, storedPassword);
    booksFile.close();

    return storedPassword == inputPassword;
}

// Function to add a book to library
void addBook(Book books[], int &bookCount)
{
    if (bookCount >= MAX_BOOKS)
    {
        cout << "Book limit reached." << endl;
        return;
    }

    Book newBook;

    // Clear the input buffer before reading the book details
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter book title: ";
    cin.getline(newBook.title, 100);

    cout << "Enter book author: ";
    cin.getline(newBook.author, 50);

    cout << "Enter book year: ";
    cin >> newBook.year;

    // Check if the input for the year was valid
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid year. Please enter a valid number." << endl;
        return;
    }

    // Assign an ID to the new book and add it to the array
    newBook.id = bookCount + 2;
    books[bookCount] = newBook;
    bookCount++;
    saveBooksToFile(books, bookCount);
    cout << "Book added successfully." << endl;
}

// Function to list all books in the library
void listBooks(const Book books[], int bookCount)
{
    cout << "----- List of Books -----\n\n";
    for (int i = 0; i < bookCount; i++)
    {
        cout << "Book ID: " << books[i].id << endl;
        cout << "Title: " << books[i].title << endl;
        cout << "Author: " << books[i].author << endl;
        cout << "Year: " << books[i].year << endl;
        cout << "-------------------------\n";
    }
}

// Function to find a book by ID
void findBook(const Book books[], int bookCount)
{
    int bookID;
    cout << "Enter book ID to search: ";
    cin >> bookID;

    for (int i = 0; i < bookCount; i++)
    {
        if (books[i].id == bookID)
        {
            cout << "Book found:\n";
            cout << "Title: " << books[i].title << endl;
            cout << "Author: " << books[i].author << endl;
            cout << "Year: " << books[i].year << endl;
            return;
        }
    }

    cout << "Book not found." << endl;
}

// Function to delete a book by ID
void deleteBook(Book books[], int &bookCount)
{
    int bookID;
    cout << "Enter book ID to delete: ";
    cin >> bookID;

    for (int i = 0; i < bookCount; i++)
    {
        if (books[i].id == bookID)
        {
            // Shift all books up by one position
            for (int j = i; j < bookCount - 1; j++)
            {
                books[j] = books[j + 1];
                books[j].id = j + 2; // Adjust the ID of the book
            }
            bookCount--;
            saveBooksToFile(books, bookCount);
            cout << "Book deleted successfully." << endl;
            return;
        }
    }

    cout << "Book not found." << endl;
}

// Function to add a member
void addMember(Member members[], int &memberCount)
{
    if (memberCount >= MAX_MEMBERS)
    {
        cout << "Member limit reached." << endl;
        return;
    }

    Member newMember;

    // Clear the input buffer before reading the member details
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter member name: ";
    cin.getline(newMember.name, 50);

    // Increment the id for the new member
    newMember.id = memberCount + 1;

    members[memberCount] = newMember;
    memberCount++;
    saveMembersToFile(members, memberCount);
    cout << "Member added successfully." << endl;
}

// Function to list all members
void listMembers(const Member members[], int memberCount)
{
    cout << "----- List of Members -----\n\n";
    for (int i = 0; i < memberCount; i++)
    {
        cout << "Member ID: " << members[i].id << endl;
        cout << "Name: " << members[i].name << endl;
        cout << "---------------------------\n";
    }
}

// Function to delete a member by ID
void deleteMember(Member members[], int &memberCount)
{
    int memberID;
    cout << "Enter member ID to delete: ";
    cin >> memberID;

    for (int i = 0; i < memberCount; i++)
    {
        if (members[i].id == memberID)
        {
            for (int j = i; j < memberCount - 1; j++)
            {
                members[j] = members[j + 1];
                members[j].id = members[j].id - 1;
            }

            memberCount--;
            saveMembersToFile(members, memberCount);
            cout << "Member deleted successfully." << endl;
            return;
        }
    }

    cout << "Member not found." << endl;
}

// Function to issue a book to a member
void issueBook(int issuedBooks[MAX_BOOKS][MAX_MEMBERS], int bookCount, int memberCount)
{
    int bookID, memberID;
    cout << "Enter book ID to issue: ";
    cin >> bookID;
    cout << "Enter member ID to issue to: ";
    cin >> memberID;

    if (bookID < 1 || bookID > bookCount || memberID < 1 || memberID > memberCount)
    {
        cout << "Invalid book ID or member ID." << endl;
        return;
    }

    issuedBooks[bookID - 1][memberID - 1] = 1;
    cout << "Book issued successfully." << endl;
}

// Function to list all issued books
void listIssuedBooks(const int issuedBooks[MAX_BOOKS][MAX_MEMBERS], const Book books[], int bookCount, const Member members[], int memberCount)
{
    cout << "----- List of Issued Books -----\n\n";
    for (int i = 0; i < bookCount; i++)
    {
        for (int j = 0; j < memberCount; j++)
        {
            if (issuedBooks[i][j] == 1)
            {
                cout << "Book ID: " << books[i].id << endl;
                cout << "Title: " << books[i].title << endl;
                cout << "Issued to Member ID: " << members[j].id << endl;
                cout << "Name: " << members[j].name << endl;
                cout << "-------------------------------\n";
            }
        }
    }
}

// Function to reset password
void passwordReset()
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "\nEnter Current Password: ";
    string givenOldPass;
    getline(cin, givenOldPass);

    if (verifyPassword(givenOldPass))
    {
        cout << "Enter New Password: ";
        string newPass;
        getline(cin, newPass);

        cout << "Enter New Password Again: ";
        string newPassAgain;
        getline(cin, newPassAgain);

        if (newPass == newPassAgain)
        {
            savePassword(newPass);
            cout << "Password changed successfully!\n";
            return;
        }
        else
        {
            cout << "Passwords do not match. Try again.\n";
            passwordReset();
        }
    }
    else
    {
        cout << "Entered wrong Old Password. Try again.\n";
        passwordReset();
    }
}

// Function to save the system password to a file
void savePassword(const string &newPassword)
{
    ofstream passwordFile(PASSWORD_FILE);
    if (!passwordFile)
    {
        cerr << "Error opening password file." << endl;
        return;
    }

    passwordFile << newPassword;
    passwordFile.close();
}

// Function to save books to file
void saveBooksToFile(const Book books[], int bookCount)
{
    ofstream outfile("books.txt", ios::out | ios::trunc);
    if (!outfile)
    {
        cout << "Error opening file!" << endl;
        return;
    }

    for (int i = 0; i < bookCount; i++)
    {
        outfile << books[i].id << "," << books[i].title << "," << books[i].author << "," << books[i].year << endl;
    }

    outfile.close();
}

// Function to load books from file
void loadBooksFromFile(Book books[], int &bookCount)
{
    ifstream booksFile("books.txt");
    if (!booksFile)
    {
        cout << "No saved book data found." << endl;
        return;
    }

    bookCount = 0;
    while (bookCount < MAX_BOOKS && booksFile >> books[bookCount].id)
    {
        booksFile.ignore(1, ',');
        booksFile.getline(books[bookCount].title, 100, ',');
        booksFile.getline(books[bookCount].author, 50, ',');
        booksFile >> books[bookCount].year;
        booksFile.ignore(numeric_limits<streamsize>::max(), '\n');
        bookCount++;
    }

    booksFile.close();
}

// Function to save members to file
void saveMembersToFile(const Member members[], int memberCount)
{
    ofstream outfile("members.txt", ios::out | ios::trunc);
    if (!outfile)
    {
        cout << "Error opening file!" << endl;
        return;
    }

    for (int i = 0; i < memberCount; i++)
    {
        outfile << members[i].id << "," << members[i].name << endl;
    }

    outfile.close();
}

// Function to load members from file
void loadMembersFromFile(Member members[], int &memberCount)
{
    ifstream membersFile("members.txt");
    if (!membersFile)
    {
        cout << "No saved member data found." << endl;
        return;
    }

    memberCount = 0;
    while (memberCount < MAX_MEMBERS && membersFile >> members[memberCount].id)
    {
        membersFile.ignore(1, ',');
        membersFile.getline(members[memberCount].name, 50, '\n');
        memberCount++;
    }

    membersFile.close();
}

// Function to log out and save data
void logOutSystem(const Book books[], int &bookCount)
{
    cout << "\nDo you want to enter the main menu again? (Y/n): ";
    char opt;
    cin >> opt;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (opt == 'n' || opt == 'N')
    {
        cout << "\n\n----- System Logged out Successfully! -----\n";
        cout << "Press any key to close the program...\n\n";
        if (getch())
        {
            exit(0); // Exit the program
        }
    }
}
