#ifndef COMPST_H
#define COMPST_H

#include <string>

typedef struct CS {
    std::string name;
    int workshop_count;
    int working_workshop;
    int station_class;
} CS;

CS init_cs(std::string name = "Empty compressor station", int workshop_count = 0, int working_workshop = 0, int station_class = -1);
void print_cs_data(CS comp_st);
void mode_change(CS comp_st, std::string mode);
void cs_save_into_file(CS comp_st, std::string filename = "output.txt");
CS cs_read_from_file(std::string filename, CS comp_st = init_cs());

#endif