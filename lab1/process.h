#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include "pipe.h"
#include "compressor_station.h"

class redirect_output_wrapper // https://github.com/papilinatm/cpp_lessons_2020/blob/master/cpp_lessons/utils.h
{
private:
    std::ostream &stream;
    std::streambuf *const old_buf;
public:
    redirect_output_wrapper(std::ostream &src)
        : stream(src), old_buf(src.rdbuf()) {
    }
    ~redirect_output_wrapper() {
        stream.rdbuf(old_buf);
    }
    void redirect(const std::ostream &dest) const {
        stream.rdbuf(dest.rdbuf());
    }
};

void process();
int main_menu();
void execute_repair_change(std::unordered_map<int, Pipe> &P);
void execute_mode_change(std::unordered_map<int, CS> &comp_st);
void save_into_file(const std::unordered_map<int, Pipe> &P, const std::unordered_map<int, CS> &comp_st);
void read_from_file(Pipe &P, CS &comp_st);
void pipe_add(std::unordered_map<int, Pipe> &P);
void cs_add(std::unordered_map<int, CS> &comp_st);
void pipe_info(const std::unordered_map<int, Pipe> &P);
void cs_info(const std::unordered_map<int, CS> &comp_st);
std::vector<int> filters(std::unordered_map<int, Pipe> &P, std::unordered_map<int, CS> &comp_st);
int parse_filter();
int get_num();
std::string get_str();
double get_double();
std::vector<int> enter_id();
void pipe_edit(std::unordered_map<int, Pipe> &P, std::vector<int> &ids);
void pipe_delete(std::unordered_map<int, Pipe> &P, std::vector<int> &ids);
void cs_edit(std::unordered_map<int, CS> &comp_st, std::vector<int> &ids, bool mode);
void cs_delete(std::unordered_map<int, CS> &comp_st, std::vector<int> &ids);
void edit(std::unordered_map<int, Pipe> &P, std::unordered_map<int, CS> &comp_st);

#endif