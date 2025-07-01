//
// Created by legal on 29/06/2025.
//

#ifndef FROZEN_TREE_H
#define FROZEN_TREE_H

#include <cstdio>
#include <cstdlib>
#include <vector>

enum next_node { RATE_0,
                 RATE_1_FROM_F,
                 RATE_1_FROM_G,
                 REP_FROM_F,
                 REP_FROM_G,
                 LEAF_RATE_0,
                 LEAF_RATE_1_FROM_F,
                 LEAF_RATE_1_FROM_G,
                 MID_NODE_FROM_F,
                 MID_NODE_FROM_G };

class frozen_tree {
public:
    frozen_tree(const int N) {
        next_node_status.resize(2 * N);
    }

    ~frozen_tree() {
        // nothing
    }

    std::vector<next_node> next_node_status;

    void analyze(const int * frozen, const int size) {
        printf("\nFrozen matrix:\n");
        for (int i = 0; i < size; i += 1) {
            if ((i % 8) == 0)
                printf(" | ");
            if ((i % 16) == 0)
                printf("\n | ");
            printf("%2d ", frozen[i]);
        }
        printf(" |\n");

        const int n_elmnt = execute(frozen, 0, next_node_status.data(), 0, size);
        next_node_status.resize(n_elmnt);
        printf("-> #elements : %d\n", n_elmnt);
        printf("-> #elements : %zu\n", next_node_status.size());
    }

    void dump() {
        printf("################################################\n");
        int level = 0;
        for (int i = 0; i < int(next_node_status.size()); i++) {
            add_space(level);
            switch (next_node_status[i]) {
                case RATE_0:
                    printf("> Execute RATE_0\n");
                    break;
                case RATE_1_FROM_F:
                    printf("> Execute RATE_1_FROM_F");
                    break;
                case RATE_1_FROM_G:
                    printf("> Execute RATE_1_FROM_G");
                    break;
                case REP_FROM_F:
                    printf("> Execute REP_FROM_F");
                    break;
                case REP_FROM_G:
                    printf("> Execute REP_FROM_G");
                    break;
                case LEAF_RATE_0:
                    printf("> Execute LEAF_RATE_0");
                    break;
                case LEAF_RATE_1_FROM_F:
                    printf("> Execute LEAF_RATE_1_FROM_F");
                    break;
                case LEAF_RATE_1_FROM_G:
                    printf("> Execute LEAF_RATE_1_FROM_G");
                    break;
                case MID_NODE_FROM_F:
                    printf("> Execute MID_NODE_FROM_F");
                    break;
                case MID_NODE_FROM_G:
                    printf("> Execute MID_NODE_FROM_G");
                    break;
                default:
                    printf("> Execute ?????? > FAILURE !");
                    exit(EXIT_FAILURE);
                    break;
            }
        }
        printf("################################################\n");
    }

private:
    void add_space(const int level) {
        for (int i = 0; i < level; i += 1) {
            printf(" ");
        }
    }

    int execute(
        const int * frozen,
        const int   curr_frozen,
        next_node * array,
        int         curr_elmnt,
        const int   size,
        const int   level = 1) {
        const int n = size / 2; // Assuming size is the number of symbols
        for (int z = 0; z < level; z += 1)
            printf("+  ");
        printf("NODE LEVEL (%d)\n", size);
        for (int z = 0; z < level; z += 1)
            printf("+  ");
        printf("F edge : ");
        for (int i = 0; i < n; i++) {
            if (i == n)
                printf(" | ");
            printf("%d", frozen[curr_frozen + i]);
        }
        printf("\n");

        //
        // Analyse de la branche gauche
        //
        int sum = 0;
        for (int i = 0; i < n; i++)
            sum += frozen[curr_frozen + i];

        //
        //
        //
        int next_elmnt;
        if (sum == n) {
            if (n == 1) {
                for (int z = 0; z < level; z += 1)
                    printf("+  ");
                printf("> Leaf rate-0 node found (%d)\n", n);
                array[curr_elmnt] = LEAF_RATE_0;
            } else {
                for (int z = 0; z < level; z += 1)
                    printf("+  ");
                printf("> Rate-0 node found (%d)\n", n);
                array[curr_elmnt] = RATE_0;
            }
            next_elmnt = curr_elmnt + 1;
        } else if (sum == 0) {
            if (n == 1) {
                for (int z = 0; z < level; z += 1)
                    printf("+  ");
                printf("> Leaf rate-1 node found (%d)\n", n);
                array[curr_elmnt] = LEAF_RATE_1_FROM_F;
            } else {
                for (int z = 0; z < level; z += 1)
                    printf("+  ");
                printf("> Rate-1 node found (%d)\n", n);
                array[curr_elmnt] = RATE_1_FROM_F;
            }
            next_elmnt = curr_elmnt + 1;
        } else {
            for (int z = 0; z < level; z += 1)
                printf("+  ");
            printf("> Normal (f) node found (%d)\n", n);
            array[curr_elmnt] = MID_NODE_FROM_F;
            next_elmnt        = execute(
                frozen,
                curr_frozen,
                array,
                curr_elmnt + 1,
                n, level + 1);
        }

        //
        // Analyse de la branche droite
        //
        for (int z = 0; z < level; z += 1)
            printf("+  ");
        printf("G edge : ");
        for (int i = 0; i < n; i++)
            printf("%d", frozen[curr_frozen + n + i]);
        printf("\n");
        //
        //
        //
        sum = 0;
        for (int i = 0; i < n; i++)
            sum += frozen[curr_frozen + n + i];
        //
        //
        //

        if (sum == n) {
            if (n == 1) {
                for (int z = 0; z < level; z += 1)
                    printf("+  ");
                printf("> Leaf rate-0 node found (size = %d)\n", n);
                array[next_elmnt] = LEAF_RATE_0;
            } else {
                for (int z = 0; z < level; z += 1)
                    printf("+  ");
                printf("> Rate-0 node found (size = %d)\n", n);
                array[next_elmnt] = RATE_0;
            }
            return next_elmnt + 1;
        } else if (sum == 0) {
            if (n == 1) {
                for (int z = 0; z < level; z += 1)
                    printf("+  ");
                printf("> Leaf rate-1 node found (size = %d)\n", n);
                array[next_elmnt] = LEAF_RATE_1_FROM_G;
            } else {
                for (int z = 0; z < level; z += 1)
                    printf("+  ");
                printf("> Rate-1 node found (size = %d)\n", n);
                array[next_elmnt] = RATE_1_FROM_G;
            }
            return next_elmnt + 1;
        } else {
            for (int z = 0; z < level; z += 1)
                printf("+  ");
            printf(" >Normal (g) node found (size = %d)\n", n);

            array[next_elmnt] = MID_NODE_FROM_G;
            int final_elmnt   = execute(
                frozen,
                curr_frozen + n,
                array,
                next_elmnt + 1,
                n, level + 1);
            return final_elmnt;
        }
    }
};

#endif // FROZEN_TREE_H
