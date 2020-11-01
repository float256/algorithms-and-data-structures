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
                 << "\tcd:                    Checkout to child node" << endl;
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
            fileSystemTree -> PrintTree();
        } else if (currInput == "ls") {
            fileSystemTree -> PrintFolderContent();
        } else if (currInput == "cd") {
            int folderIdx;
            TreeNode* childNode;

            if (fileSystemTree -> GetNumberOfElementsInCurrentFolder() == 0) {
                cout << "Current folder is empty" << endl;
                continue;
            }

            cout << "All nodes in current folder:" << endl << endl;
            fileSystemTree -> PrintFolderContent();
            cout << endl << "Enter index of folder you want to switch: ";
            cin >> folderIdx;
            getchar();

            if (folderIdx > fileSystemTree -> GetNumberOfElementsInCurrentFolder() - 1) {
                cout << "Index is too large";
            } else if (folderIdx < 0) {
                cout << "Index is incorrect";
            } else {
                childNode = fileSystemTree -> GetChildNode(folderIdx);

                if (childNode -> ElementType == FileSystemFolder) {
                    fileSystemTree -> CheckoutToChildNode(folderIdx);
                } else {
                    cout << "Selected item is not a folder";
                }
            }

            cout << endl;
        } else {
            cout << "Incorrect command. Type 'help' for more information." << endl;
        }
    }
    return 0;
}