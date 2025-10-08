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

void pipe_save_into_file(const Pipe &P, const std::string &filename) {
    std::ofstream fout(filename, std::ios::app);
    if (fout.is_open()) {
        fout << "Pipe" << std::endl;
        fout << P.name << std::endl << P.len << std::endl << P.diameter << 
            std::endl << P.repair << std::endl;
        fout.close();
    } else {
        std::cout << "Error! Cannot open file" << std::endl;
    }
}

Pipe pipe_read_from_file(const std::string &filename) {
    std::ifstream fin(filename);
    Pipe P = init_pipe("Empty pipe", 0, 0, true);
    if (fin.is_open()) {
        std::string struct_type;
        while(std::getline(fin, struct_type)) {
            if (!struct_type.compare("Pipe")) {
                std::getline(fin, P.name);
                fin >> P.len >> P.diameter >> P.repair;
            }
        }
        fin.close();
    } else {
        std::cout << "Error! Cannot open file" << std::endl;
    }
    return P;
}