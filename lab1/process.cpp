#include "process.h"
#include "compressor_station.h"
#include "pipe.h"

#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <limits>
#include <fstream>

int main_menu() {
    std::cout << "Menu" << std::endl;
    std::vector <std::string> operations = {"Create pipe", "Print pipe info", "Change repair status",
                "Create CS", "Print CS info", "Change CS working workshops", "Save", "Load", "Exit"};
    for (int i = 0; i < (int) operations.size(); i++) {
        std::cout << i + 1 << ". " << operations[i] << std::endl;
    }
    std::cout << "Enter the menu number" << std::endl;
    std::string str;
    try {
        std::cin >> str;
        int n = std::stoi(str);
        if (n <= (int) operations.size() && n >= 1) {
            return n - 1;
        } else {
            return -1;
        }
    } catch (...) {
        return -1;
    }
}

void tmp_mode_change(CS &comp_st) {
    std::cout << "Enter" << std::endl << "1. Turn on" << std::endl << "2. Turn off" << std::endl;
    int mode;
    std::cin >> mode;//!!!
    if (mode == 1 || mode == 2) {
        mode_change(comp_st, mode);
    } else {
        std::cout << "Error! Incorrect input, returning in main menu" << std::endl;
    }
}

void save_into_file(const Pipe &P, const CS &comp_st) {
    std::string filename;
    std::cout << "Enter filename" << std::endl;
    std::getline(std::cin>>std::ws, filename);
    std::ofstream fout(filename);
    if (!isEmpty(P)) {
        pipe_save_into_file(P, fout);
    }
    if (!isEmpty(comp_st)) {
        cs_save_into_file(comp_st, fout);
    }
    return;
}

void read_from_file(Pipe &P, CS &comp_st) {
    std::string filename;
    std::cout << "Enter filename" << std::endl;
    std::getline(std::cin>>std::ws, filename);
    std::ifstream fin(filename);
    if (fin.is_open()) {
        std::string struct_type;
        while (std::getline(fin, struct_type)) {
            if (struct_type == "Pipe") P = pipe_read_from_file(fin);
            else if (struct_type == "CS") comp_st = cs_read_from_file(fin);
            else {
                std::cout << "Error! Something went wrong while reading the file";
            }
        }
    } else {
        std::cout << "Error! Cannot open file" << std::endl;
    }
    return;
}

void pipe_recreation(Pipe &P) {
    std::cout << "Print pipe name" << std::endl;
    std::getline(std::cin>>std::ws, P.name);
    std::cout << "Print pipe lenght" << std::endl;
    std::cin >> P.len;
    std::cout << "Print pipe diameter" << std::endl;
    std::cin >> P.diameter;
    std::cout << "Print pipe repair status" << std::endl;
    std::cin >> P.repair;
    return;
}

void cs_recreation(CS &comp_st) {
    std::cout << "Print CS name" << std::endl;
    std::getchar();
    std::getline(std::cin>>std::ws, comp_st.name);
    std::cout << "Print CS workshops count" << std::endl;
    std::cin >> comp_st.workshop_count;
    std::cout << "Print CS number of working workshops" << std::endl;
    std::cin >> comp_st.working_workshop;
    std::cout << "Print CS station class" << std::endl;
    std::cin >> comp_st.station_class;
}

void process() {
    Pipe P = init_pipe("Empty pipe", 0, 0, true);
    CS comp_st = init_cs("Empty compressor station", 0, 0, -1);
    std::vector<std::pair<std::function<void()>, std::string>> actions = {
        {[&P]() {pipe_recreation(P);}, "Create pipe"},
        {[&P]() {print_pipe_data(P);}, "Print pipe info"},
        {[&P]() {repair_change(P);}, "Change repair status"},
        {[&comp_st]() {cs_recreation(comp_st);}, "Create CS"},
        {[&comp_st]() {print_cs_data(comp_st);}, "Print CS info"},
        {[&comp_st]() {tmp_mode_change(comp_st);}, "Change CS working workshops"},
        {[&P, &comp_st]() {save_into_file(P, comp_st);}, "Save"},
        {[&P, &comp_st]() {read_from_file(P, comp_st);}, "Load"},
    };
    while (true) {
        int n = main_menu();
        if (n != -1) {
            if (n == (int) actions.size()) {
                break;
            } else {
                std::cout << actions[n].second << std::endl;
                actions[n].first();
            }
        } else {
            std::cout << "Incorrect number, try again" << std::endl;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return;
}