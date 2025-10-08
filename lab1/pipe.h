#ifndef PIPE_H
#define PIPE_H

#include <string>

typedef struct Pipe {
    std::string name;
    int len;
    int diameter;
    bool repair;
} Pipe;

Pipe init_pipe(const std::string &name, int len, int diameter, bool repair);
void print_pipe_data(const Pipe &P);
void repair_change(Pipe &P);
bool isEmpty (const Pipe &P);
void pipe_save_into_file(const Pipe &P, const std::string &filename);
Pipe pipe_read_from_file(const std::string &filename);

#endif