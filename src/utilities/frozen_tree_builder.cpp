#include "definitions/const_config_GF64_N64.hpp"
#include "utilities/frozen_tree.hpp"

#include <iostream>
#include <iomanip>

using std::cerr, std::endl, std::setw;

int main(int, char*[]) {
	int frozen_symbols[N];
	std::fill(frozen_symbols, frozen_symbols + N, 1);
	for (const auto * ptr_seq = reliab_seq; ptr_seq < (reliab_seq + K); ptr_seq++) {
		frozen_symbols[*ptr_seq] = 0;
	}

	frozen_tree f_tree(N);
	f_tree.analyze(frozen_symbols, N, cerr);  
	f_tree.dump(cerr);

	std::cout << "#include \"utilities/NodeType.hpp\"\n";
	std::cout << "constexpr NodeType next_node_status[" << f_tree.next_node_status.size() << "] = {\n    ";
	for (auto it = f_tree.next_node_status.begin(); it < f_tree.next_node_status.end() - 1; it++) {
		std::cout << string_next_node(*it) << ",\n    ";
	}
	std::cout << string_next_node(f_tree.next_node_status.back()) << "\n};" << std::endl;

	return 0;	
}
