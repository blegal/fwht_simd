//
// Created by legal on 29/06/2025.
//
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <array>
#include <string>
#include <string_view>

#include "utilities/frozen_tree.hpp"

static void add_space(const int level, std::ostream & sink) {
    for (int i = 0; i < level; i += 1) {
        sink << " ";
    }
}

static const std::array<std::string, int(MID_NODE_FROM_G) + 1> next_node_str = {
    "RATE_0",
    "RATE_1_FROM_F",
    "RATE_1_FROM_G",
    "REP_FROM_F",
    "REP_FROM_G",
    "SPC_FROM_F",
    "SPC_FROM_G",
    "LEAF_RATE_0",
    "LEAF_RATE_1_FROM_F",
    "LEAF_RATE_1_FROM_G",
    "MID_NODE_FROM_F",
    "MID_NODE_FROM_G"};

const std::string_view string_next_node(NodeType node) {
    return next_node_str.at(int(node));
}

frozen_tree::frozen_tree(const int N)
    : next_node_status(2 * N) {}

void frozen_tree::analyze(const int * frozen, const int size, std::ostream & sink) {
    sink << "\nFrozen matrix:\n";
    for (int i = 0; i < size; i += 1) {
        if ((i % 8) == 0) {
            sink << " | ";
        }
        if ((i % 16) == 0) {
            sink << "\n | ";
        }
        sink << std::setw(2) << frozen[i];
    }
    sink << " |\n";

    const int n_elmnt = execute(frozen, 0, next_node_status.data(), 0, size, 1, sink);
    next_node_status.resize(n_elmnt);
    sink << "-> #elements : " << (int) n_elmnt << "\n";
    sink << "-> #elements : " << (size_t) next_node_status.size() << std::endl;
}

void frozen_tree::dump(std::ostream & sink) {
    sink << "################################################\n";
    int level = 0;
    for (int i = 0; i < int(next_node_status.size()); i++) {
        add_space(level, sink);
		sink << "> Execute " << string_next_node(next_node_status[i]) << "\n";
    }
    sink << "################################################" << std::endl;
}

int frozen_tree::execute(
    const int * frozen,
    const int   curr_frozen,
    NodeType * array,
    int         curr_elmnt,
    const int   size,
    const int   level,
	std::ostream & sink) {
    const int n = size / 2; // Assuming size is the number of symbols
    for (int z = 0; z < level; z += 1) {
        sink << "+  ";
    }
    sink << "NODE LEVEL (" << (int) size << ")\n";
    for (int z = 0; z < level; z += 1) {
        sink << "+  ";
    }
    sink << "F edge : ";
    for (int i = 0; i < n; i++) {
        if (i == n) {
            sink << " | ";
        }
        sink << (int) frozen[curr_frozen + i];
    }
    sink << "\n";

    //
    // Analyse de la branche gauche
    //
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += frozen[curr_frozen + i];
    }

    //
    //
    //
    int next_elmnt;
    if (sum == n) {
        if (n == 1) {
            for (int z = 0; z < level; z += 1) {
                sink << "+  ";
            }
            sink << "> Leaf rate-0 node found (" << (int) n << ")\n";
            array[curr_elmnt] = LEAF_RATE_0;
        } else {
            for (int z = 0; z < level; z += 1) {
                sink << "+  ";
            }
            sink << "> Rate-0 node found (" << (int) n << ")\n";
            array[curr_elmnt] = RATE_0;
        }
        next_elmnt = curr_elmnt + 1;
    } else if (sum == 0) {
        if (n == 1) {
            for (int z = 0; z < level; z += 1) {
                sink << "+  ";
            }
            sink << "> Leaf rate-1 node found (" << (int) n << ")\n";
            array[curr_elmnt] = LEAF_RATE_1_FROM_F;
        } else {
            for (int z = 0; z < level; z += 1) {
                sink << "+  ";
            }
            sink << "> Rate-1 node found (" << (int) n << ")\n";
            array[curr_elmnt] = RATE_1_FROM_F;
        }
        next_elmnt = curr_elmnt + 1;
    } else if ((sum == (n - 1)) && (frozen[curr_frozen + n - 1] == false)) {
        for (int z = 0; z < level; z += 1) {
            sink << "+  ";
        }
        sink << "> Leaf REP_F node found (" << (int) n << ")\n";
        array[curr_elmnt] = REP_FROM_F;
        next_elmnt        = curr_elmnt + 1;
    } else {
        for (int z = 0; z < level; z += 1) {
            sink << "+  ";
        }
        sink << "> Normal (f) node found (" << (int) n << ")\n";
        array[curr_elmnt] = MID_NODE_FROM_F;
        next_elmnt        = execute(
            frozen,
            curr_frozen,
            array,
            curr_elmnt + 1,
            n, level + 1,
			sink);
    }

    //
    // Analyse de la branche droite
    //
    for (int z = 0; z < level; z += 1) {
        sink << "+  ";
    }
    sink << "G edge : ";
    for (int i = 0; i < n; i++) {
        sink << (int) frozen[curr_frozen + n + i];
    }
    sink << "\n";

    sum = 0;
    for (int i = 0; i < n; i++) {
        sum += frozen[curr_frozen + n + i];
    }

    if (sum == n) {
        if (n == 1) {
            for (int z = 0; z < level; z += 1)
                sink << "+  ";
            sink << "> Leaf rate-0 node found (size = " << (int) n << ")\n";
            array[next_elmnt] = LEAF_RATE_0;
        } else {
            for (int z = 0; z < level; z += 1)
                sink << "+  ";
            sink << "> Rate-0 node found (size = " << (int) n << ")\n";
            array[next_elmnt] = RATE_0;
        }
        return next_elmnt + 1;
    } else if (sum == 0) {
        if (n == 1) {
            for (int z = 0; z < level; z += 1)
                sink << "+  ";
            sink << "> Leaf rate-1 node found (size = " << (int) n << ")\n";
            array[next_elmnt] = LEAF_RATE_1_FROM_G;
        } else {
            for (int z = 0; z < level; z += 1)
                sink << "+  ";
            sink << "> Rate-1 node found (size = " << (int) n << ")\n";
            array[next_elmnt] = RATE_1_FROM_G;
        }
        return next_elmnt + 1;
    } else if ((sum == (n - 1)) && (frozen[curr_frozen + size - 1] == false)) {
        for (int z = 0; z < level; z += 1)
            sink << "+  ";
        sink << "> Node REP_G node found (" << (int) n << ")\n";
        array[curr_elmnt] = REP_FROM_G;
        return next_elmnt + 1;
    } else {
        for (int z = 0; z < level; z += 1)
            sink << "+  ";
        sink << " >Normal (g) node found (size = " << (int) n << ")\n";

        array[next_elmnt] = MID_NODE_FROM_G;
        int final_elmnt   = execute(
            frozen,
            curr_frozen + n,
            array,
            next_elmnt + 1,
            n, level + 1,
			sink);
        return final_elmnt;
    }
}
