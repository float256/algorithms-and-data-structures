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
                 << "\tmv:                    Move file" << endl
                 << "\tcp:                    Copy file" << endl
                 << "\tchange:                Change node data" << endl
                 << "\tinfo:                  Print node info" << endl;
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
                cout << "Index is too large" << endl;
            } else if (folderIdx < 0) {
                cout << "Index is incorrect" << endl;
            } else {
                childNode = fileSystemTree->GetChildNode(folderIdx);

                if (childNode->ElementType == FileSystemFolder) {
                    fileSystemTree->CheckoutToChildNode(folderIdx);
                } else {
                    cout << "Selected item is not a folder" << endl;
                }
            }
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
                fileSystemTree->RemoveNodeFromTree(nodeIndexInFolder);

                cout << "Element was deleted successfully" << endl;
            }
        } else if (currInput == "pwd") {
            fileSystemTree->PrintPathToCurrentFolder();
            cout << endl;
        } else if (currInput == "mv") {
            int movedFolderIdx, newParentFolderIdx;

            if (fileSystemTree->GetNumberOfElementsInCurrentFolder() == 0) {
                cout << "Current folder is empty" << endl;
                continue;
            }

            cout << "All nodes in current folder:" << endl << endl;
            fileSystemTree->PrintFolderContent(fileSystemTree->GetCurrentFolder(),
                                               nullptr, 0, true);
            cout << endl << "Enter index of folder you want to move: ";
            cin >> movedFolderIdx;
            getchar();

            if (movedFolderIdx > fileSystemTree->GetNumberOfElementsInCurrentFolder() - 1) {
                cout << "Index is too large" << endl;
                continue;
            } else if (movedFolderIdx < 0) {
                cout << "Index is incorrect" << endl;
                continue;
            }
            cout << endl
                 << "Enter name of the folder you want to move the selected folder (enter '-1' if you want move to parent folder): ";
            cin >> newParentFolderIdx;
            getchar();

            if (newParentFolderIdx > fileSystemTree->GetNumberOfElementsInCurrentFolder() - 1) {
                cout << "Index is too large" << endl;
                continue;
            } else if ((newParentFolderIdx < 0) && (newParentFolderIdx != -1)) {
                cout << "Index is incorrect" << endl;
                continue;
            } else if (fileSystemTree -> GetChildNode(newParentFolderIdx) -> ElementType != FileSystemFolder){
                cout << "Selected node is not a folder" << endl;
                continue;
            }  else if (newParentFolderIdx == movedFolderIdx) {
                cout << "You can't move the node into itself" << endl;
                continue;
            }

            if (newParentFolderIdx == -1) {
                if (fileSystemTree->GetCurrentFolder() == fileSystemTree->GetRootFolder()) {
                    cout << "Current folder hasn't parent folder." << endl;
                } else {
                    fileSystemTree->MoveChildNode(fileSystemTree->GetCurrentFolder()->ParentNode, movedFolderIdx);
                }
            } else {
                fileSystemTree->MoveChildNode(fileSystemTree->GetChildNode(newParentFolderIdx), movedFolderIdx);
            }
        } else if (currInput == "cp") {
            int movedFolderIdx, parentFolderIdx;

            if (fileSystemTree->GetNumberOfElementsInCurrentFolder() == 0) {
                cout << "Current folder is empty" << endl;
                continue;
            }

            cout << "All nodes in current folder:" << endl << endl;
            fileSystemTree->PrintFolderContent(fileSystemTree->GetCurrentFolder(),
                                               nullptr, 0, true);
            cout << endl << "Enter index of node you want to copy: ";
            cin >> movedFolderIdx;
            getchar();

            if (movedFolderIdx > fileSystemTree->GetNumberOfElementsInCurrentFolder() - 1) {
                cout << "Index is too large" << endl;
                continue;
            } else if (movedFolderIdx < 0) {
                cout << "Index is incorrect" << endl;
                continue;
            }
            cout << endl
                 << "Enter index of the folder you want to move the selected node (enter '-1' if you want move to parent folder): ";
            cin >> parentFolderIdx;
            getchar();

            if (parentFolderIdx > fileSystemTree->GetNumberOfElementsInCurrentFolder() - 1) {
                cout << "Index is too large" << endl;
                continue;
            } else if ((parentFolderIdx < 0) && (parentFolderIdx != -1)) {
                cout << "Index is incorrect" << endl;
                continue;
            } else if (fileSystemTree -> GetChildNode(parentFolderIdx) -> ElementType != FileSystemFolder){
                cout << "Selected node is not a folder" << endl;
                continue;
            }

            if (parentFolderIdx == -1) {
                if (fileSystemTree->GetCurrentFolder() == fileSystemTree->GetRootFolder()) {
                    cout << "Current folder hasn't parent folder." << endl;
                } else {
                    fileSystemTree->MoveChildNode(fileSystemTree->GetCurrentFolder()->ParentNode, parentFolderIdx);
                }
            } else {
                fileSystemTree->CopyChildFolder(
                    fileSystemTree -> GetChildNode(parentFolderIdx),
                    fileSystemTree -> GetChildNode(movedFolderIdx));
            }
        } else if(currInput == "change"){
            int nodeForChangeIdx;
            TreeNode* nodeForChange;

            if (fileSystemTree->GetNumberOfElementsInCurrentFolder() == 0) {
                cout << "Current folder is empty" << endl;
                continue;
            }

            cout << "All nodes in current folder:" << endl << endl;
            fileSystemTree->PrintFolderContent(fileSystemTree->GetCurrentFolder(),
                                               nullptr, 0, true);
            cout << endl << "Enter index of node you want to change: ";
            cin >> nodeForChangeIdx;
            getchar();

            if (nodeForChangeIdx > fileSystemTree->GetNumberOfElementsInCurrentFolder() - 1) {
                cout << "Index is too large" << endl;
                continue;
            } else if (nodeForChangeIdx < 0) {
                cout << "Index is incorrect" << endl;
                continue;
            }

            nodeForChange = fileSystemTree -> GetChildNode(nodeForChangeIdx);
            if (nodeForChange -> ElementType == FileSystemFolder) {
                string newName;
                FolderInfo nodeInfo = std::get<FolderInfo>(nodeForChange -> Data);

                cout << "Enter new folder name: ";
                getline(cin, newName);

                nodeInfo.Name = newName;
                nodeForChange -> Data = nodeInfo;
            } else {
                string newName, newFileText;
                TextFileInfo nodeInfo = std::get<TextFileInfo>(nodeForChange -> Data);

                cout << "Enter new file name: ";
                getline(cin, newName);

                cout << "Enter new file text: ";
                getline(cin, newFileText);

                nodeInfo.Name = newName;
                nodeInfo.Text = newFileText;
                nodeForChange -> Data = nodeInfo;
            }
        } else if (currInput == "info"){
            int printingNodeIdx;
            TreeNode* printingNode;

            if (fileSystemTree->GetNumberOfElementsInCurrentFolder() == 0) {
                cout << "Current folder is empty" << endl;
                continue;
            }

            cout << "All nodes in current folder:" << endl << endl;
            fileSystemTree->PrintFolderContent(fileSystemTree->GetCurrentFolder(),
                                               nullptr, 0, true);
            cout << endl << "Enter index of node you want to print: ";
            cin >> printingNodeIdx;
            getchar();
            cout << endl;

            if (printingNodeIdx > fileSystemTree->GetNumberOfElementsInCurrentFolder() - 1) {
                cout << "Index is too large" << endl;
                continue;
            } else if (printingNodeIdx < 0) {
                cout << "Index is incorrect" << endl;
                continue;
            }
            printingNode = fileSystemTree -> GetChildNode(printingNodeIdx);

            if (printingNode -> ElementType == FileSystemFolder) {
                FolderInfo nodeData = std::get<FolderInfo>(printingNode -> Data);

                cout << "Folder name: " << nodeData.Name << endl;
                if (printingNode -> FirstChildNode == nullptr) {
                    cout << "Folder is empty" << endl;
                } else {
                    cout << "Folder content: " << endl;
                }
                fileSystemTree -> PrintFolderContent(printingNode);
            } else {
                TextFileInfo nodeData = std::get<FileSystemFile>(printingNode -> Data);

                cout << "File name: " << nodeData.Name << endl;
                if (nodeData.Text == ""){
                    cout << "File is empty" << endl;
                } {
                    cout << "File text: " << nodeData.Text << endl;
                }
            }
        } else {
            cout << "Incorrect command. Type 'help' for more information." << endl;
        }
    }
    return 0;
}