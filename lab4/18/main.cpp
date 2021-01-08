// 1 Условие задачи:
//   18. Реализовать алгоритм поиска кратчайших путей Дейкстры и
// проиллюстрировать по шагам этапы его выполнения (10).
//
// 2. Автор: Никита Черемин
// 3. Среда выполнения: "Linux" + "g++ (SUSE Linux) 7.5.0"  + CMake version 3.17.0
// 4. Формат входного файла:
//       В первой строке через пробел перечисляются все узлы графа
//     На каждой следующих строках по одному перечисляются все рёбра
//     графа. Через пробел пишутся имя начального узла, имя конечного узла,
//     вес ребра (вес не может быть отрицательным или равен нулю). Граф создается направленным
//
//       Пример:
//           A B C D E
//           A B 5
//           B C 4
//           C D 3
//           D A 2
//           B E 8
//           C F 1
//           E F 12
//
//       Вид получившегося графа:
//
//          A ---- 5 ---> B ---- 8 ---> E
//          ^             |             |
//          |             4             12
//          2             |             |
//          |             v             v
//          D <--- 3 ---- C ---- 1 ---> F
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
        cout << "Brief information:" << endl
             << "The program searches the graph for the shortest path and writes the step-by-step execution of the algorithm to a file." << endl
             << endl
             << "Parameters:" << endl
             << "\t - Path to input file" << endl
             << "\t - Path to output file" << endl
             << "\t - Start node name (cannot contain spaces)" << endl
             << endl
             << "Example:" << endl
             << "\t DikstraAlgoritm input.txt output.txt A" << endl;
    } else if (argc == 4) {
        std::string inputFileName = argv[1];
        std::string outputFileName = argv[2];
        std::string startNodeName = argv[3];
        try {
            ifstream inputFile;

            inputFile.open(inputFileName);
            if (!inputFile) {
                throw invalid_argument("Input file not found");
            }
            std::string inputFileContent(
                (std::istreambuf_iterator<char>(inputFile)),
            (std::istreambuf_iterator<char>()));

            std::unique_ptr<Graph> graph(&(GraphParser::Parse(inputFileContent)));

            AlgorithmHistory history = DijkstraAlgorithm::FindAllShortPathsWithHistory(*(graph.get()), startNodeName);
            DijkstraAlgorithm::PrintHistoryToFile(history, outputFileName, startNodeName);

            inputFile.close();

            cout << "Algorithm completed successfully" << endl;
        } catch (const std::exception& e) {
            cout << e.what() << endl;
        }
    } else {
        cout << "Incorrect number of arguments. Enter \"DijkstraAlgorithm help \"for help" << endl;
    }
}