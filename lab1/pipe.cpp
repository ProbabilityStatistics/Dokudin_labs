#include "pipe.h"

#include <iostream>
#include <fstream>

Pipe init_pipe(const std::string &name, int len, int diameter, bool repair) {
    Pipe pipe;
    pipe.name = name;
    pipe.len = len;
    pipe.diameter = diameter;
    pipe.repair = repair;
    return pipe;
}

void print_pipe_data(const Pipe &P) {
    std::cout 
    << "Name: " << P.name 
    << ", len: " << P.len 
    << ", diameter: " << P.diameter 
    << ", repair status: "<< (P.repair ? "under repair" : "repaired")
    << std::endl;
    return;
}

void repair_change(Pipe &P) {
    P.repair = !P.repair;
}

bool isEmpty (const Pipe &P) {
    if (P.len == 0 && P.diameter == 0) return 1;
    else return 0;
}