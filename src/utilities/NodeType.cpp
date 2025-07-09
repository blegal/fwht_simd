#include "utilities/NodeType.hpp"

#include <string>
#include <array>

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

