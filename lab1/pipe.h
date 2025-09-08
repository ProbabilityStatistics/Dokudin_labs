#ifndef PIPE_H
#define PIPE_H

#include <string>

typedef struct Pipe {
    std::string name;
    int len;
    int diameter;
    bool repair;
} Pipe;

Pipe init_pipe(std::string name = "Empty pipe", int len = 0, int diameter = 0, bool repair = true);
void print_pipe_data(Pipe P);
void repair_change(Pipe P);
void pipe_save_into_file(Pipe pipe, std::string filename = "output.txt");
Pipe pipe_read_from_file(std::string filename = "input.txt", Pipe pipe = init_pipe());

#endif