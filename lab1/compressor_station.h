#ifndef COMPST_H
#define COMPST_H

#include <string>

typedef struct CS {
    std::string name;
    int workshop_count;
    int working_workshop;
    int station_class;
    int id;
} CS;

CS init_cs(const std::string &name, int workshop_count, int working_workshop, int station_class);
void print_cs_data(const CS &comp_st);
void mode_change(CS &comp_st, bool mode);
bool isEmpty(const CS &comp_st);
void cs_save_into_file(const CS &comp_st, std::ofstream &fout);
CS cs_read_from_file(std::ifstream &fin);

#endif