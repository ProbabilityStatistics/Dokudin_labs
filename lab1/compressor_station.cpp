#include "compressor_station.h"

#include <iostream>
#include <fstream>

CS init_cs(std::string name = "Empty compressor station", int workshop_count = 0, int working_workshop = 0, int station_class = -1) {
    CS comp_st;
    comp_st.name = name;
    comp_st.station_class = station_class;
    comp_st.working_workshop = working_workshop;
    comp_st.workshop_count = workshop_count;
    return comp_st;
}

void print_cs_data(CS comp_st) {
    std::cout << "Name: " << comp_st.name << ", station class: " << comp_st.station_class << ", working workshops: " << comp_st.working_workshop << 
        ", workshop count: " << comp_st.workshop_count << std::endl;
}

void mode_change(CS comp_st, std::string mode) {
    if (!mode.compare("on")) {
        if (comp_st.workshop_count > comp_st.working_workshop) comp_st.working_workshop++;
        else std::cout << "Operation canceled, all workshops are working now" << std::endl;
    } else if (!mode.compare("off")) {
        if (comp_st.working_workshop > 0) comp_st.working_workshop--;
        else std::cout << "Operation canceled, all workshops are stoped" << std::endl;
    } else {
        std::cout << "Error! Incorrect mode" << std::endl;
    }
}

void cs_save_into_file(CS comp_st, std::string filename = "output.txt") {
    std::ofstream fout(filename);
    if (fout.is_open()) {
        fout << "Name: " << comp_st.name << ", station class: " << comp_st.station_class << ", working workshops: " << comp_st.working_workshop << 
            ", workshop count: " << comp_st.workshop_count << std::endl;
    } else {
        std::cout << "Error! Cannot open file" << std::endl;
    }
}

CS cs_read_from_file(std::string filename = "input.txt", CS comp_st = init_cs()) {
    std::ifstream fin(filename);
    if (fin.is_open()) {
        
    } else {
        std::cout << "Error! Cannot open file";
    }
}   