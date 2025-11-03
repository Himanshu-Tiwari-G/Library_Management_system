#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

// ---------- Book Class ----------
class Book
{
public:
    int id;
    string title;
    string author;
    bool isIssued;
    string issuedTo;
    int issuedDays;

    Book()
    {
        id = 0;
        isIssued = false;
        issuedDays = 0;
    }

    void input()
    {
        cout << "Enter Book ID: ";
        cin >> id;
        cin.ignore();
        cout << "Enter Book Title: ";
        getline(cin, title);
        cout << "Enter Book Author: ";
        getline(cin, author);
        isIssued = false;
        issuedTo = "None";
        issuedDays = 0;
    }

    void display()
    {
        cout << left << setw(10) << id
             << setw(25) << title
             << setw(20) << author
             << setw(15) << (isIssued ? "Issued" : "Available")
             << setw(15) << issuedTo
             << issuedDays << endl;
    }
};

// ---------- Library Class ----------
class Library
{
private:
    vector<Book> books;
    const int FINE_RATE = 5; // Rs 5 per day

public:
    // Load data from file on start
    Library()
    {
        loadData();
    }

    // Save data permanently in file
    void saveData()
    {
        ofstream file("books.txt");
        for (auto &b : books)
        {
            file << b.id << "," << b.title << "," << b.author << ","
                 << b.isIssued << "," << b.issuedTo << "," << b.issuedDays << "\n";
        }
        file.close();
    }

    // Load data when program starts
    void loadData()
    {
        ifstream file("books.txt");
        if (!file)
            return;

        books.clear();
        Book b;
        while (file >> b.id)
        {
            file.ignore();
            getline(file, b.title, ',');
            getline(file, b.author, ',');
            file >> b.isIssued;
            file.ignore();
            getline(file, b.issuedTo, ',');
            file >> b.issuedDays;
            books.push_back(b);
        }
        file.close();
    }

    // Add Book
    void addBook()
    {
        Book b;
        b.input();
        books.push_back(b);
        saveData();
        cout << "\n✅ Book Added Successfully!\n";
    }

    // View Book List
    void viewBooks()
    {
        if (books.empty())
        {
            cout << "\n❌ No Books Available!\n";
            return;
        }
        cout << "\n---------------------------------------------------------------------------------------\n";
        cout << left << setw(10) << "ID"
             << setw(25) << "Title"
             << setw(20) << "Author"
             << setw(15) << "Status"
             << setw(15) << "Issued To"
             << "Days\n";
        cout << "---------------------------------------------------------------------------------------\n";
        for (auto &b : books)
            b.display();
    }

    // Search Book by Title
    void searchBook()
    {
        cin.ignore();
        string key;
        cout << "Enter Book Title to Search: ";
        getline(cin, key);

        bool found = false;
        for (auto &b : books)
        {
            if (b.title.find(key) != string::npos)
            {
                cout << "\n✅ Book Found:\n";
                b.display();
                found = true;
            }
        }
        if (!found)
            cout << "\n❌ Book Not Found!\n";
    }

    // Issue Book
    void issueBook()
    {
        int id;
        cout << "Enter Book ID to Issue: ";
        cin >> id;

        for (auto &b : books)
        {
            if (b.id == id)
            {
                if (!b.isIssued)
                {
                    cin.ignore();
                    cout << "Enter Student Name: ";
                    getline(cin, b.issuedTo);
                    cout << "Enter No. of Days: ";
                    cin >> b.issuedDays;
                    b.isIssued = true;
                    saveData();
                    cout << "\n✅ Book Issued Successfully!\n";
                }
                else
                {
                    cout << "\n❌ Book Already Issued!\n";
                }
                return;
            }
        }
        cout << "\n❌ Book ID Not Found!\n";
    }

    // Return Book + Fine Calculation
    void returnBook()
    {
        int id, returnDays, fine = 0;
        cout << "Enter Book ID to Return: ";
        cin >> id;

        for (auto &b : books)
        {
            if (b.id == id && b.isIssued)
            {
                cout << "Enter Actual Return Days: ";
                cin >> returnDays;

                if (returnDays > b.issuedDays)
                {
                    fine = (returnDays - b.issuedDays) * FINE_RATE;
                }
                b.isIssued = false;
                b.issuedTo = "None";
                b.issuedDays = 0;
                saveData();

                cout << "\n✅ Book Returned Successfully!\n";
                cout << "📌 Fine: Rs " << fine << "\n";
                return;
            }
        }
        cout << "\n❌ Book ID Not Found or Not Issued!\n";
    }

    // Sort Books by Title
    void sortBooks()
    {
        sort(books.begin(), books.end(), [](Book &a, Book &b)
             { return a.title < b.title; });
        saveData();
        cout << "\n✅ Books Sorted Alphabetically!\n";
    }

    // Delete Book by ID
    void deleteBook()
    {
        int id;
        cout << "Enter Book ID to Delete: ";
        cin >> id;

        for (auto it = books.begin(); it != books.end(); it++)
        {
            if (it->id == id)
            {
                books.erase(it);
                saveData();
                cout << "\n✅ Book Deleted Successfully!\n";
                return;
            }
        }
        cout << "\n❌ Book Not Found!\n";
    }
};

// Menu
int main()
{
    Library lib;
    int choice;

    while (true)
    {
        cout << "\n================= Smart Library System =================\n";
        cout << "1. Add Book\n2. View Books\n3. Search Book\n4. Issue Book";
        cout << "\n5. Return Book\n6. Sort Books\n7. Delete Book\n8. Exit\n";
        cout << "Enter Choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            lib.addBook();
            break;
        case 2:
            lib.viewBooks();
            break;
        case 3:
            lib.searchBook();
            break;
        case 4:
            lib.issueBook();
            break;
        case 5:
            lib.returnBook();
            break;
        case 6:
            lib.sortBooks();
            break;
        case 7:
            lib.deleteBook();
            break;
        case 8:
            lib.saveData();
            cout << "\n👋 Exiting... Goodbye!";
            exit(0);
        default:
            cout << "❌ Invalid Choice!\n";
        }
    }
}
