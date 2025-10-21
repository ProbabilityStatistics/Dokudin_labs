#include "process.h"
#include "compressor_station.h"
#include "pipe.h"

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <functional>
#include <limits>
#include <fstream>

int main_menu() {
    std::cout << "Menu" << std::endl;
    std::vector <std::string> operations = {"Add new pipe", "Print pipes info", "Change repair status",
                "Add new CS", "Print CSs info", "Change CS working workshops", "Save", "Load", "Exit"};
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
        comp_st.mode_change(mode);
    } else {
        std::cout << "Error! Incorrect input, returning in main menu" << std::endl;
    }
}

void save_into_file(const Pipe &P, const CS &comp_st) {
    std::string filename;
    std::cout << "Enter filename" << std::endl;
    std::getline(std::cin>>std::ws, filename);
    std::ofstream fout(filename);
    if (P.isEmpty()) {
        P.pipe_save_into_file(fout);
    }
    if (comp_st.isEmpty()) {
        comp_st.cs_save_into_file(fout);
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
            if (struct_type == "Pipe") {
                //Pipe p;
                P.pipe_read_from_file(fin);
                
            }
            else if (struct_type == "CS") {
                //CS cs;
                comp_st.cs_read_from_file(fin);
            }
            else {
                std::cout << "Error! Something went wrong while reading the file";
            }
        }
    } else {
        std::cout << "Error! Cannot open file" << std::endl;
    }
    return;
}

void pipe_add(Pipe &P) {
    std::string name;
    int len, diameter, repair;
    std::cout << "Print pipe name" << std::endl;
    std::getline(std::cin>>std::ws, name);
    std::cout << "Print pipe lenght" << std::endl;
    std::cin >> len;
    std::cout << "Print pipe diameter" << std::endl;
    std::cin >> diameter;
    std::cout << "Print pipe repair status" << std::endl;
    std::cin >> repair;
    if (repair == 1 || repair == 0) P.set(name, len, diameter, repair);
    else std::cout << "Error! Incorrect repair status, returning in main menu" << std::endl;
    return;
}

void cs_add(CS &comp_st) {
    std::string name;
    int workshop_count, working_workshop, station_class;
    std::cout << "Print CS name" << std::endl;
    std::getline(std::cin>>std::ws, name);
    std::cout << "Print CS workshops count" << std::endl;
    std::cin >> workshop_count;
    std::cout << "Print CS number of working workshops" << std::endl;
    std::cin >> working_workshop;
    std::cout << "Print CS station class" << std::endl;
    std::cin >> station_class;
    comp_st.set(name, workshop_count, working_workshop, station_class);
}

void process() {
    std::unordered_map<int, Pipe> P;
    std::unordered_map<int, CS> comp_st;
    int id;
    std::vector<std::pair<std::function<void()>, std::string>> actions = {
        {[&P]() {pipe_add(P[]);}, "Create pipe"},
        {[&P, id]() {P[id].print_pipe_data();}, "Print pipe info"},
        {[&P]() {P[].repair_change();}, "Change repair status"},
        {[&comp_st]() {cs_add(comp_st[]);}, "Create CS"},
        {[&comp_st]() {comp_st[].print_cs_data();}, "Print CS info"},
        {[&comp_st]() {tmp_mode_change(comp_st[]);}, "Change CS working workshops"},
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