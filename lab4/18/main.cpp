// 1 Условие задачи:
//   18. Реализовать алгоритм поиска кратчайших путей Дейкстры и
// проиллюстрировать по шагам этапы его выполнения (10).
//
// 2. Автор: Никита Черемин
// 3. Среда выполнения: "Linux" + "g++ (SUSE Linux) 7.5.0"  + CMake version 3.17.0
// 4. Формат входного файла:
//       В первой строке через пробел перечисляются все узлы графа
//     На каждой следующих строках по одному перечисляются все рёбра
//     графа. Через пробел пишутся имя первого узла, имя второго узла,
//     вес ребра (вес не может быть отрицательным или равен нулю).
//     Также стоит учесть, что граф ненаправленный, т.е. не имеет
//     значения порядок, в котором указаны узлы при описании ребра.
//
//       Пример:
//           A B C D
//           A B 4
//           B C 30
//           C D 5
//           D A 100

#include <cstring>
#include <iostream>
#include "DataStructures/Graph/Graph.h"
#include "DataStructures/GraphParser/GraphParser.h"
#include "AlgorithmImplementation/DijkstraAlgorithm.h"
#include "AlgorithmImplementation/Entities/NodeLabel.h"
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

            cout << "Алгоритм выполнен успешно" << endl;
        } catch (const std::exception& e) {
            cout << e.what() << endl;
        }
    } else {
        cout << "Некорректное количество аргументов. Введите \"DijkstraAlgorithm help\" для получения справки" << endl;
    }
}