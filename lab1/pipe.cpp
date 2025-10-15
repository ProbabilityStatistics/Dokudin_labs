#include "pipe.h"

#include <iostream>
#include <fstream>

Pipe::Pipe() {
    name = "Empty pipe";
    len = 0;
    diameter = 0;
    repair = 0;
    id = -1;
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

void pipe_save_into_file(const Pipe &P, std::ofstream &fout) {
    fout << "Pipe" << std::endl;
    fout << P.name << std::endl 
        << P.len << std::endl 
        << P.diameter << std::endl 
        << P.repair << std::endl;
}

Pipe pipe_read_from_file(std::ifstream &fin) {
    Pipe P = init_pipe("Empty pipe", 0, 0, true);
    std::getline(fin, P.name);
    fin >> P.len >> P.diameter >> P.repair;
    return P;
}