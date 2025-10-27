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

void tmp_repair_change(std::unordered_map<int, Pipe> &P) {
    int id;
    std::cout << "Enter pipe ID" << std::endl;
    std::cin >> id;//!!!
    if (auto search = P.find(id); search != P.end()) {
        P[id].repair_change();
    } else {
        std::cout << "Error! The CS with this ID was not found" << std::endl;
    }
    return;
}

void tmp_mode_change(std::unordered_map<int, CS> &comp_st) {
    int id;
    int mode;
    std::cout << "Enter compressor station ID" << std::endl;
    std::cin >> id;//!!!
    if (auto search = comp_st.find(id); search != comp_st.end()) {
        std::cout << "Enter" << std::endl << "1. Turn on" << std::endl << "2. Turn off" << std::endl;
        std::cin >> mode;//!!!
        if (mode == 1 || mode == 2) {
            comp_st[id].mode_change(mode);
        } else {
            std::cout << "Error! Incorrect input, returning in main menu" << std::endl;
        }
    } else {
        std::cout << "Error! The CS with this ID was not found" << std::endl;
    }
    return;
}

void save_into_file(const std::unordered_map<int, Pipe> &P, const std::unordered_map<int, CS> &comp_st) {
    std::string filename;
    std::cout << "Enter filename" << std::endl;
    std::getline(std::cin>>std::ws, filename);
    std::ofstream fout(filename);
    for (const auto &[id, p] : P) {
        if (!p.isEmpty()) {
            p.pipe_save_into_file(fout);
        }
    }
    for (const auto &[id, cs] : comp_st) {
        if (!cs.isEmpty()) {
            cs.cs_save_into_file(fout);
        }
    }
    
    return;
}

void read_from_file(std::unordered_map<int, Pipe> &P, std::unordered_map<int, CS> &comp_st) {
    std::string filename;
    std::cout << "Enter filename" << std::endl;
    std::getline(std::cin>>std::ws, filename);
    std::ifstream fin(filename);
    if (fin.is_open()) {
        std::string struct_type;
        while (std::getline(fin, struct_type)) {
            if (struct_type == "Pipe") {
                Pipe p;
                p.pipe_read_from_file(fin);
                P[p.get_id()] = p;
            }
            else if (struct_type == "CS") {
                CS cs;
                cs.cs_read_from_file(fin);
                comp_st[cs.get_id()] = cs;
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

void pipe_add(std::unordered_map<int, Pipe> &P) {
    Pipe p;
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
    if (repair == 1 || repair == 0) {
        p.set(name, len, diameter, repair);
        P.emplace(p.get_id(), std::move(p));
    }
    else std::cout << "Error! Incorrect repair status, returning in main menu" << std::endl;
    return;
}

void cs_add(std::unordered_map<int, CS> &comp_st) {
    CS cs;
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
    cs.set(name, workshop_count, working_workshop, station_class);
    comp_st.emplace(cs.get_id(), std::move(cs));
    return;
}

void pipe_info(std::unordered_map<int, Pipe> &P) {
    for (const auto &p : P)
        p.second.print_pipe_data();
    return;
}

void cs_info(std::unordered_map<int, CS> &comp_st) {
    for (const auto &cs : comp_st)
        cs.second.print_cs_data();
    return;
}

void process() {
    std::unordered_map<int, Pipe> P;
    std::unordered_map<int, CS> comp_st;
    std::vector<std::pair<std::function<void()>, std::string>> actions = {
        {[&P]() {pipe_add(P);}, "Create pipe"},
        {[&P]() {pipe_info(P);}, "Print pipe info"},
        {[&P]() {tmp_repair_change(P);}, "Change repair status"},
        {[&comp_st]() {cs_add(comp_st);}, "Create CS"},
        {[&comp_st]() {cs_info(comp_st);}, "Print CS info"},
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