// 1. Условие задачи:
//   18. Организовать  в  основной  памяти с помощью указателей
// стек из очередей. Обеспечить   операции   ведения  очереди из
// вершины   стека,   расширения   и  сокращения  стека,  выдачи
// содержимого стека (9).
//
// 2. Автор: Никита Черемин
// 3. Среда выполнения: CLang version 7.0.1-8

#include<iostream>
#include "DataStructures/Stack.h"
#include "DataStructures/Queue.h"
#include <memory>

bool isNumber(std::string inputStr);

int main() {
    using namespace std;

    const std::string PROMPT_STRING = ">>> ";
    string currInput;
    unique_ptr<Stack<Queue<int>*>> mainStack(new Stack<Queue<int>*>());

    cout << "Stack was created. Enter commands. Type 'help' for more information.\n";
    while (currInput != "exit" ) {
        cout << PROMPT_STRING;
        getline(cin, currInput);
        if (currInput == "help") {
            cout << "List of possible commands:" << endl
                 << "\thelp:                  Display this text" << endl
                 << "\texit:                  Stop program" << endl
                 << "\tadd_to_stack:          Add new queue to stack" << endl
                 << "\tadd_to_queue:          Add number to current queue" << endl
                 << "\tget:                   Get top element from current queue" << endl
                 << "\tget_all_from_queue:    Get all elements from current queue" << endl
                 << "\tget_all_from_stack:    Get all elements from stack" << endl
                 << "\tremove:                Remove top element from current queue" << endl
                 << "\tremove_queue:          Remove current queue from stack" << endl;
        } else if (currInput == "exit") {
            cout << "Terminating the program." << endl;
        } else if (currInput == "add_to_stack") {
            mainStack -> Push(new Queue<int>());
        } else if (currInput == "add_to_queue") {
            if (mainStack -> GetLength() > 0) {
                string newQueueTopValue;

                cout << "Enter the number: ";
                getline(cin, newQueueTopValue);
                if (isNumber(newQueueTopValue)) {
                    mainStack->Peek()->Enqueue(stoi(newQueueTopValue));
                } else {
                    cout << "Input string is not a number" << endl;
                }
            } else {
                cout << "Stack is empty" << endl;
            }
        } else if (currInput == "get") {
            if ((mainStack -> GetLength() > 0) && (mainStack -> Peek() -> GetLength() > 0)) {
                cout << "Current queue top value: " << mainStack->Peek()->Peek() << endl;
            } else if (mainStack -> GetLength() == 0) {
                cout << "Stack is empty" << endl;
            } else if (mainStack -> Peek() -> GetLength() == 0) {
                cout << "Current queue is empty"  << endl;
            }
        } else if (currInput == "get_all_from_queue") {
            if ((mainStack -> GetLength() > 0) && (mainStack -> Peek() -> GetLength() > 0)) {
                int* currQueueArray = mainStack -> Peek() -> ToArray();
                cout << "Current queue: ";
                for (int i = 0; i < mainStack -> Peek() -> GetLength(); ++i) {
                    cout << currQueueArray[i] << " ";
                }
                cout << endl;
            } else if (mainStack -> GetLength() == 0) {
                cout << "Stack is empty" << endl;
            } else if (mainStack -> Peek() -> GetLength() == 0) {
                cout << "Current queue is empty"  << endl;
            }
        } else if (currInput == "get_all_from_stack") {
            if (mainStack -> GetLength() > 0) {
                Queue<int>** allQueues = mainStack -> ToArray();
                for (int i = 0; i < mainStack->GetLength(); ++i) {
                    int* currQueueArray = allQueues[i] -> ToArray();

                    cout << "Queue #" << i + 1 << ": ";
                    if (allQueues[i] ->GetLength() != 0) {
                        for (int j = 0; j < mainStack->Peek()->GetLength(); ++j) {
                            cout << currQueueArray[j] << " ";
                        }
                    } else {
                        cout << "Empty";
                    }
                    cout << endl;
                }
            } else if (mainStack -> GetLength() == 0) {
                cout << "Stack is empty" << endl;
            }
        } else if (currInput == "remove") {
            if ((mainStack -> GetLength() > 0) && (mainStack -> Peek() -> GetLength() > 0)) {
                mainStack -> Peek() -> Dequeue();
                cout << "Element has been removed from queue. Now queue length is " << mainStack -> Peek() ->GetLength();
            } else if (mainStack -> GetLength() == 0) {
                cout << "Stack is empty";
            } else if (mainStack -> Peek() -> GetLength() == 0) {
                cout << "Current queue is empty";
            }
            cout << endl;
        } else if (currInput == "remove_queue") {
            if (mainStack -> GetLength() > 0) {
                mainStack -> Pop();
                cout << "Queue has been removed from stack. Now stack length is " << mainStack ->GetLength();
            } else {
                cout << "Stack is empty";
            }
            cout << endl;
        } else {
            cout << "Incorrect command. Type 'help' for more information." << endl;
        }
    }
    return 0;
}

bool isNumber(std::string inputStr) {
    const char digits[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
    bool isAllSymbolsAreDigits = true;

    for (char currSymbol: inputStr) {
        bool isDigit = false;
        for (char digit : digits) {
            isDigit = (currSymbol == digit);
            if (isDigit) {
                break;
            }
        }
        isAllSymbolsAreDigits = isDigit;
        if (!isDigit) {
            break;
        }
    }
    return (isAllSymbolsAreDigits && !inputStr.empty());
}
