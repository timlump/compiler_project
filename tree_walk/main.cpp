#include <iostream>
#include "tree_walk.h"

int main(int num_args, char ** args) {
    if (num_args == 2) {
        lox::tree_walk::run_file(args[1]);
    }
    else if (num_args == 1) {
        lox::tree_walk::run_prompt();
    }
    else {
        std::cout << "Usage: lox [script]" << std::endl;
    }

    return 0;
}