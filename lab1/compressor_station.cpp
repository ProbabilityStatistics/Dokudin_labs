#include "compressor_station.h"

#include <iostream>
#include <fstream>
#include <string>

std::atomic<int> CS::next_id{0};

CS::CS() {
    id = CS::next_id.fetch_add(1, std::memory_order_relaxed);
    name = "Empty compressor station";
    station_class = 0;
    working_workshop = 0;
    workshop_count = -1;
}

CS::CS(std::string n, int count, int working, int st_cl) : name{n}, workshop_count{count}, working_workshop{working}, station_class{st_cl} {
    id = CS::next_id.fetch_add(1, std::memory_order_relaxed);
}

CS::~CS() {}

void CS::print_cs_data() const {
    std::cout 
    << "ID: " << id
    << ", name: " << name 
    << ", station class: " << station_class 
    << ", working workshops: " << working_workshop 
    << ", workshop count: " << workshop_count 
    << std::endl;
    return;
}

void CS::mode_change(bool mode) {
    if (mode) {
        if (workshop_count > working_workshop) working_workshop++;
        else std::cout << "Operation canceled, all workshops are working now" << std::endl;
    } else {
        if (working_workshop > 0) working_workshop--;
        else std::cout << "Operation canceled, all workshops are stoped" << std::endl;
    }
    return;
}

bool CS::isEmpty() const {
    if (workshop_count == 0 && station_class == -1) return 1;
    else return 0;
}

void CS::cs_save_into_file(std::ofstream &fout) const {
    fout << "CS" << std::endl;
    fout << name << std::endl 
        << workshop_count << std::endl 
        << working_workshop << std::endl 
        << station_class << std::endl;
}

void CS::cs_read_from_file(std::ifstream &fin) {
    std::getline(fin>>std::ws, name);
    fin >> workshop_count >> working_workshop >> station_class;
    return;
}

void CS::set(std::string n, int count, int working, int st_cl) {
    name = n;
    station_class = st_cl;
    working_workshop = working;
    workshop_count = count;
}

int CS::get_id() const {
    return id;
}

std::string CS::get_name() const {
    return name;
}

double CS::get_percent() const {
    return working_workshop / workshop_count;
}

std::ostream& operator<<(std::ostream& os, const CS& comp_st) {
    return os << "ID: " << comp_st.id
        << ", name: " << comp_st.name 
        << ", station class: " << comp_st.station_class 
        << ", working workshops: " << comp_st.working_workshop 
        << ", workshop count: " << comp_st.workshop_count
        << std::endl;
}