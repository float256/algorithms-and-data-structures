add_library(data-structures-library
        Graph/Graph.cpp Graph/Graph.h
        GraphParser/GraphParser.cpp GraphParser/GraphParser.h
        Queue.h)
set_target_properties(data-structures-library PROPERTIES LINKER_LANGUAGE CXX)
