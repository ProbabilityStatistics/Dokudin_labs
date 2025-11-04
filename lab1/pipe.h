#ifndef PIPE_H
#define PIPE_H

#include <string>
#include <atomic>

class Pipe {
private:
    std::string name;
    double len;
    int diameter;
    bool repair;
    int id;
    static std::atomic<int> next_id;
public:
    Pipe();
    ~Pipe();
    void print_pipe_data() const;
    void repair_change();
    bool isEmpty() const;
    void pipe_save_into_file(std::ofstream &fout) const;
    void pipe_read_from_file(std::ifstream &fin);
    void set(std::string n, double l, int d, bool r);//!!!
    int get_id() const;
    std::string get_name() const;
    int get_repair() const;
    friend std::ostream& operator<<(std::ostream& os, const Pipe& P);
};

#endif
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
