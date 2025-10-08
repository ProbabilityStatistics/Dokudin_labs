#include "compressor_station.h"

#include <iostream>
#include <fstream>

CS init_cs(const std::string &name, int workshop_count, int working_workshop, int station_class) {
    CS comp_st;
    comp_st.name = name;
    comp_st.station_class = station_class;
    comp_st.working_workshop = working_workshop;
    comp_st.workshop_count = workshop_count;
    return comp_st;
}

void print_cs_data(const CS &comp_st) {
    std::cout 
    << "Name: " << comp_st.name 
    << ", station class: " << comp_st.station_class 
    << ", working workshops: " << comp_st.working_workshop 
    << ", workshop count: " << comp_st.workshop_count 
    << std::endl;
    return;
}

void mode_change(CS &comp_st, bool mode) {
    if (mode) {
        if (comp_st.workshop_count > comp_st.working_workshop) comp_st.working_workshop++;
        else std::cout << "Operation canceled, all workshops are working now" << std::endl;
    } else {
        if (comp_st.working_workshop > 0) comp_st.working_workshop--;
        else std::cout << "Operation canceled, all workshops are stoped" << std::endl;
    }
    return;
}

bool isEmpty(const CS &comp_st) {
    if (comp_st.workshop_count == 0 && comp_st.station_class == -1) return 1;
    else return 0;
}

void cs_save_into_file(const CS &comp_st, const std::string &filename) {
    std::ofstream fout(filename, std::ios::app);
    if (fout.is_open()) {
        fout << "CS" << std::endl;
        fout << comp_st.name << std::endl << comp_st.workshop_count << std::endl << comp_st.working_workshop << 
            std::endl << comp_st.station_class << std::endl;
        fout.close();
    } else {
        std::cout << "Error! Cannot open file" << std::endl;
    }
}

CS cs_read_from_file(const std::string &filename) {
    std::ifstream fin(filename);
    CS comp_st = init_cs("Empty compressor station", 0, 0, -1);
    if (fin.is_open()) {
        std::string struct_type;
        while(std::getline(fin, struct_type)) {
            if (!struct_type.compare("CS")) {
                std::getline(fin, comp_st.name);
                fin >> comp_st.workshop_count >> comp_st.working_workshop >> comp_st.station_class;
            }
        }
        fin.close();
    } else {
        std::cout << "Error! Cannot open file" << std::endl;
    }
    return comp_st;
}