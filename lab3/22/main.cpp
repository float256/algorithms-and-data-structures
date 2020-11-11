// 1 Условие задачи:
//   Информация  о  файлах  на  жестких  дисках   компьютера
// записана  с  помощью  дерева.  Обеспечить выполнение следующих
// операций:
//   1) загрузку дерева в память из файла;
//   2) обход дерева папок в  режиме  диалога  (раскрытие папок,
//      подъем на уровень и т. п.);
//   3) корректировку  дерева при создании новых папок и файлов,
// их переименовании, копировании, переносе и удалении.
//   4) сохранение дерева в файле (13).
//
// 2. Автор: Никита Черемин
// 3. Среда выполнения: Linux + CLang version 10.0.1-1 + CMake version 3.18.2-1

#include <iostream>
#include <memory>
#include <variant>
#include "FileSystem/FileSystemTree.h"

int main(){
    using namespace std;

    const std::string PROMPT_STRING = ">>> ";
    string currInput;
    unique_ptr<FileSystemTree> fileSystemTree(new FileSystemTree());

    cout << "File system tree was created. Enter commands. Type 'help' for more information.\n";
    while (currInput != "exit" ) {
        cout << PROMPT_STRING;
        getline(cin, currInput);
        if (currInput == "help") {
            cout << "List of possible commands:" << endl
                 << "\thelp:                  Display this text" << endl
                 << "\texit:                  Stop program" << endl
                 << "\tmkfile:                Add file to tree" << endl
                 << "\tmkdir:                 Add folder to tree" << endl
                 << "\tls /:                  Print tree" << endl
                 << "\tls:                    Print all folders and files from current folder" << endl
                 << "\tcd:                    Checkout to child node" << endl
                 << "\tcd ../:                Checkout to parent node" << endl
                 << "\trm:                    Remove element from folder" << endl
                 << "\tpwd:                   Get path to current folder" << endl
        } else if (currInput == "exit") {
            cout << "Terminating the program." << endl;
        } else if (currInput == "mkfile") {
            string fileName, fileText;

            cout << "Enter file name: ";
            getline(cin, fileName);

            cout << "Enter file text: ";
            getline(cin, fileText);

            fileSystemTree->AddTextFile(fileName, fileText);
        } else if (currInput == "mkdir") {
            string folderName;

            cout << "Enter folder name: ";
            getline(cin, folderName);

            fileSystemTree->AddFolder(folderName);
        } else if (currInput == "ls /") {
            fileSystemTree->PrintTree(fileSystemTree->GetRootFolder());
        } else if (currInput == "ls") {
            int numberOfElements = fileSystemTree->GetNumberOfElementsInCurrentFolder();
            if (numberOfElements > 0) {
                fileSystemTree->PrintTree(fileSystemTree->GetCurrentFolder());
            } else {
                cout << "Folder '" << get<FileSystemFolder>(fileSystemTree->GetCurrentFolder()->Data).Name
                     << "' is empty" << endl;
            }
        } else if (currInput == "cd") {
            int folderIdx;
            TreeNode *childNode;

            if (fileSystemTree->GetNumberOfElementsInCurrentFolder() == 0) {
                cout << "Current folder is empty" << endl;
                continue;
            }

            cout << "All nodes in current folder:" << endl << endl;
            fileSystemTree->PrintFolderContent(fileSystemTree->GetCurrentFolder(),
                                               nullptr, 0, true);
            cout << endl << "Enter index of folder you want to switch: ";
            cin >> folderIdx;
            getchar();

            if (folderIdx > fileSystemTree->GetNumberOfElementsInCurrentFolder() - 1) {
                cout << "Index is too large";
            } else if (folderIdx < 0) {
                cout << "Index is incorrect";
            } else {
                childNode = fileSystemTree->GetChildNode(folderIdx);

                if (childNode->ElementType == FileSystemFolder) {
                    fileSystemTree->CheckoutToChildNode(folderIdx);
                } else {
                    cout << "Selected item is not a folder";
                }
            }

            cout << endl;
        } else if (currInput == "cd ../") {
            if (fileSystemTree->GetCurrentFolder() != fileSystemTree->GetRootFolder()) {
                fileSystemTree->CheckoutToParentNode();
            } else {
                cout << "Current folder is root folder" << endl;
            }
        } else if (currInput == "rm") {
            int nodeIndexInFolder;

            if (fileSystemTree->GetNumberOfElementsInCurrentFolder() == 0) {
                cout << "Current folder is empty" << endl;
                continue;
            }

            cout << "All nodes in current folder:" << endl << endl;
            fileSystemTree->PrintFolderContent(fileSystemTree->GetCurrentFolder(),
                                               nullptr, 0, true);
            cout << endl << "Enter index of element you want to remove: ";
            cin >> nodeIndexInFolder;
            getchar();

            if (nodeIndexInFolder > fileSystemTree->GetNumberOfElementsInCurrentFolder() - 1) {
                cout << "Index is too large" << endl;
            } else if (nodeIndexInFolder < 0) {
                cout << "Index is incorrect" << endl;
            } else {
                fileSystemTree->RemoveChildNode(nodeIndexInFolder);

                cout << "Element was deleted successfully" << endl;
            }
        } else if (currInput == "pwd") {
            fileSystemTree -> PrintPathToCurrentFolder();
            cout << endl;
        } else {
            cout << "Incorrect command. Type 'help' for more information." << endl;
        }
    }
    return 0;
}