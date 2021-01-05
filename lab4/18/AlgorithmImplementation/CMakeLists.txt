add_library(algorithm-implementation-library
        DijkstraAlgorithm.cpp DijkstraAlgorithm.h
        Entities/NodeLabel.h
        Entities/AlgorithmState.h Entities/AlgorithmHistory.h)
set_target_properties(algorithm-implementation-library PROPERTIES LINKER_LANGUAGE CXX)
