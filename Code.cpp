#include <iostream>
#include <string>
using namespace std;

// Forward declaration for the files struct
struct files;

// Folder struct (Outer Linked List)
struct folders {
    string name;
    folders* next;
    files* sublist; // Pointer to the files linked list (Inner Linked List)

    folders(string val) : name(val), next(nullptr), sublist(nullptr) {}
};

// File struct (Inner Linked List)
struct files {
    string name;
    files* next;

    files(string val) : name(val), next(nullptr) {}
};

// Global pointers for folder and file management
folders* headC = nullptr;  // Head for C: drive
folders* headD = nullptr;  // Head for D: drive
folders* tempFolder = nullptr; // Temporary pointer for folders
folders* prevFolder = nullptr; // Previous pointer for folders
files* tempFile = nullptr;     // Temporary pointer for files
files* prevFile = nullptr;     // Previous pointer for files

// Function to append a folder to the folders list
void appendFolder(string drive, string name) {
    folders*& head = (drive == "C:") ? headC : headD;

    folders* newFolder = new folders(name);
    if (head == nullptr) {
        head = newFolder;
    }
    else {
        tempFolder = head;
        while (tempFolder->next != nullptr) {
            tempFolder = tempFolder->next;
        }
        tempFolder->next = newFolder;
    }
}

// Function to append a file to a folder's file list
void appendFile(string drive, string folderName, string fileName) {
    folders*& head = (drive == "C:") ? headC : headD;

    tempFolder = head;
    while (tempFolder != nullptr && tempFolder->name != folderName) {
        tempFolder = tempFolder->next;
    }

    if (tempFolder != nullptr) {
        files* newFile = new files(fileName);
        if (tempFolder->sublist == nullptr) {
            tempFolder->sublist = newFile;
        }
        else {
            tempFile = tempFolder->sublist;
            while (tempFile->next != nullptr) {
                tempFile = tempFile->next;
            }
            tempFile->next = newFile;
        }
    }
}

// Function to delete a folder
void deleteFolder(string drive, string folderName) {
    folders*& head = (drive == "C:") ? headC : headD;

    tempFolder = head;
    prevFolder = nullptr;

    while (tempFolder != nullptr && tempFolder->name != folderName) {
        prevFolder = tempFolder;
        tempFolder = tempFolder->next;
    }

    if (tempFolder == nullptr) {
        cout << "Folder not found.\n";
        return;
    }

    // Delete all files in the folder
    tempFile = tempFolder->sublist;
    while (tempFile != nullptr) {
        files* toDelete = tempFile;
        tempFile = tempFile->next;
        delete toDelete;
    }

    // Delete the folder node
    if (prevFolder == nullptr) {
        head = tempFolder->next;
    }
    else {
        prevFolder->next = tempFolder->next;
    }

    delete tempFolder;
    cout << "Folder " << folderName << " deleted successfully.\n";
}

// Function to delete a file from a folder
void deleteFile(string drive, string folderName, string fileName) {
    folders*& head = (drive == "C:") ? headC : headD;

    tempFolder = head;
    while (tempFolder != nullptr && tempFolder->name != folderName) {
        tempFolder = tempFolder->next;
    }

    if (tempFolder == nullptr) {
        cout << "Folder not found.\n";
        return;
    }

    tempFile = tempFolder->sublist;
    prevFile = nullptr;

    while (tempFile != nullptr && tempFile->name != fileName) {
        prevFile = tempFile;
        tempFile = tempFile->next;
    }

    if (tempFile == nullptr) {
        cout << "File not found.\n";
        return;
    }

    if (prevFile == nullptr) {
        tempFolder->sublist = tempFile->next;
    }
    else {
        prevFile->next = tempFile->next;
    }

    delete tempFile;
    cout << "File " << fileName << " deleted successfully from folder " << folderName << ".\n";
}

// Function to display folders and files of a drive
void displayDrive(string drive) {
    folders*& head = (drive == "C:") ? headC : headD;

    cout << drive << "\\" << endl;

    tempFolder = head;
    int folderIndex = 1;
    while (tempFolder != nullptr) {
        cout << folderIndex++ << ". " << tempFolder->name << endl;
        tempFolder = tempFolder->next;
    }

    int folderChoice;
    cout << "\nEnter folder number to view files, 0 to add new folder, -1 to go back, or -2 to delete a folder: ";
    cin >> folderChoice;

    if (folderChoice == -1) {
        return;
    }
    else if (folderChoice == 0) {
        string newFolderName;
        cout << "Enter new folder name: ";
        cin >> newFolderName;
        appendFolder(drive, newFolderName);
        cout << "Folder " << newFolderName << " added successfully.\n";
        return;
    }
    else if (folderChoice == -2) {
        string folderName;
        cout << "Enter the name of the folder to delete: ";
        cin >> folderName;
        deleteFolder(drive, folderName);
        return;
    }

    tempFolder = head;
    int index = 1;
    while (tempFolder != nullptr && index != folderChoice) {
        tempFolder = tempFolder->next;
        index++;
    }

    if (tempFolder != nullptr) {
        cout << "\nFiles in folder " << tempFolder->name << ":\n";
        tempFile = tempFolder->sublist;
        int fileIndex = 1;
        while (tempFile != nullptr) {
            cout << fileIndex++ << ". " << tempFile->name << endl;
            tempFile = tempFile->next;
        }

        int fileChoice;
        cout << "\nEnter 0 to add new file, -1 to go back, or -2 to delete a file: ";
        cin >> fileChoice;

        if (fileChoice == 0) {
            string newFileName;
            cout << "Enter new file name: ";
            cin >> newFileName;
            appendFile(drive, tempFolder->name, newFileName);
            cout << "File " << newFileName << " added to folder " << tempFolder->name << " successfully.\n";
        }
        else if (fileChoice == -2) {
            string fileName;
            cout << "Enter the name of the file to delete: ";
            cin >> fileName;
            deleteFile(drive, tempFolder->name, fileName);
        }
    }
}

// Main function
int main() {
    int choice;

    // Create default folders and files for C: and D:
    appendFolder("C:", "Folder1");
    appendFolder("C:", "Folder2");
    appendFile("C:", "Folder1", "File1");
    appendFile("C:", "Folder1", "File2");
    appendFile("C:", "Folder2", "File1");
    appendFile("C:", "Folder2", "File2");

    appendFolder("D:", "Folder1");
    appendFolder("D:", "Folder2");
    appendFile("D:", "Folder1", "File1");
    appendFile("D:", "Folder1", "File2");
    appendFile("D:", "Folder2", "File1");
    appendFile("D:", "Folder2", "File2");

    // Menu loop
    do {
        cout << "\nWelcome to the File Management System" << endl;
        cout << "C:\\ " << endl;
        cout << "D:\\ " << endl;
        cout << "[1] for C: " << endl;
        cout << "[2] for D: " << endl;
        cout << "[0] to Exit" << endl;
        cout << ">>> ";
        cin >> choice;

        if (choice == 1) {
            displayDrive("C:");
        }
        else if (choice == 2) {
            displayDrive("D:");
        }
    } while (choice != 0);

    cout << "Exiting the File Management System. Goodbye!" << endl;

    return 0;
}