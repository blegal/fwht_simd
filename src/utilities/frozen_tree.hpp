//
// Created by legal on 29/06/2025.
//

#ifndef FROZEN_TREE_H
#define FROZEN_TREE_H

#include <cstdlib>
#include <vector>
#include <iostream>

#include "utilities/NodeType.hpp"

class frozen_tree {
public:
    frozen_tree(const int N);
    frozen_tree()  = delete;
    ~frozen_tree() = default;

    std::vector<NodeType> next_node_status;

    void analyze(const int * frozen, const int size, std::ostream & sink = std::cout);

    void dump(std::ostream & sink = std::cout);

private:
    int execute(
        const int * frozen,
        const int   curr_frozen,
        NodeType * array,
        int         curr_elmnt,
        const int   size,
        const int   level = 1,
		std::ostream & sink = std::cout);
};

#endif // FROZEN_TREE_H
