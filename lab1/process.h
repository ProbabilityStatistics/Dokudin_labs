#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <unordered_map>
#include "pipe.h"
#include "compressor_station.h"

void process();
int main_menu();
void tmp_repair_change(std::unordered_map<int, Pipe> &P);
void tmp_mode_change(std::unordered_map<int, CS> &comp_st);
void save_into_file(const std::unordered_map<int, Pipe> &P, const std::unordered_map<int, CS> &comp_st);
void read_from_file(Pipe &P, CS &comp_st);
void pipe_add(std::unordered_map<int, Pipe> &P);
void cs_add(std::unordered_map<int, CS> &comp_st);
void pipe_info(std::unordered_map<int, Pipe> &P);
void cs_info(std::unordered_map<int, CS> &comp_st);
void filters(std::unordered_map<int, Pipe> &P, std::unordered_map<int, CS> &comp_st);

#endif