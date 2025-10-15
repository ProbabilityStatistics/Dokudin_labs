#ifndef PIPE_H
#define PIPE_H

#include <string>

class Pipe {
private:
    std::string name;
    int len;
    int diameter;
    bool repair;
    int id;
public:
    Pipe();
    ~Pipe();
    void print_pipe_data(const Pipe &P);
    void repair_change(Pipe &P);
    bool isEmpty (const Pipe &P);
    void pipe_save_into_file(const Pipe &P, std::ofstream &fout);
    Pipe pipe_read_from_file(std::ifstream &fin);
};
/*
typedef struct Pipe {
    std::string name;
    int len;
    int diameter;
    bool repair;
    int id;
} Pipe;

Pipe init_pipe(const std::string &name, int len, int diameter, bool repair);
void print_pipe_data(const Pipe &P);
void repair_change(Pipe &P);
bool isEmpty (const Pipe &P);
void pipe_save_into_file(const Pipe &P, std::ofstream &fout);
Pipe pipe_read_from_file(std::ifstream &fin);
*/
#endif