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