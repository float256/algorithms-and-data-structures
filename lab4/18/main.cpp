//
// Created by user on 01.01.2021.
//

#include <cstring>
#include <iostream>
#include <fstream>
#include "DataStructures/Graph/Graph.h"
#include "DataStructures/GraphParser/GraphParser.h"
#include "AlgorithmImplementation/DijkstraAlgorithm.h"
#include "AlgorithmImplementation/Entities/NodeLabel.h"
#include "AlgorithmImplementation/Entities/AlgorithmState.h"
#include "AlgorithmImplementation/Entities/AlgorithmHistory.h"


int main(int argc, char *argv[]) {
    using namespace std;

    if ((argc == 2) && (strcmp(argv[1], "help") == 0)) {
        cout << "Краткая справка:" << endl
             << "\t Программа выполняет поиск кратчайшего пути в графе и записывает " << endl
             << "пошаговое выполнение алгоритма в файл. Также стоит упомянуть, что " << endl
             << "построенный граф является НЕНАПРАВЛЕННЫМ, т.е. запись ребра \"A B 1\" " << endl
             << "идентична \"B A 1\"" << endl
             << endl
             << "Параметры:" << endl
             << "\t - Путь ко входному файлу" << endl
             << "\t - Путь к выходному файлу" << endl
             << "\t - Название стартовой вершины(не может содержать пробелы)" << endl
             << endl
             << "Пример:" << endl
             << "\t DikstraAlgoritm input.txt output.txt A" << endl;
    } else if (argc == 4) {
        std::string inputFileName = argv[1];
        std::string outputFileName = argv[2];
        std::string startNodeName = argv[3];
        try {
            ifstream inputFile;

            inputFile.open(inputFileName);
            if (!inputFile) {
                throw invalid_argument("Входной файл не найден");
            }
            std::string inputFileContent(
                (std::istreambuf_iterator<char>(inputFile)),
            (std::istreambuf_iterator<char>()));

            std::unique_ptr<Graph> graph(&(GraphParser::Parse(inputFileContent)));

            AlgorithmHistory history = DijkstraAlgorithm::FindAllShortPathsWithHistory(*(graph.get()), startNodeName);
            DijkstraAlgorithm::PrintHistoryToFile(history, outputFileName, startNodeName);

            inputFile.close();
        } catch (const std::exception& e) {
            cout << e.what() << endl;
        }
    } else {
        cout << "Некорректное количество аргументов. Введите \"DijkstraAlgorithm help\" для получения справки" << endl;
    }
}