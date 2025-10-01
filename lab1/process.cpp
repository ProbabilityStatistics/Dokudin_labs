#include "process.h"
#include "compressor_station.h"
#include "pipe.h"

#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <limits>
#include <fstream>

void main_menu() {
    std::cout << "Menu" << std::endl;
    std::vector <std::string> operations = {"Create pipe", "Print pipe info", "Change repair status",
                "Create CS", "Print CS info", "Change CS working workshops", "Save", "Load", "Exit"};
    for (int i = 0; i < (int) operations.size(); i++) {
        std::cout << i + 1 << ". " << operations[i] << std::endl;
    }
    std::cout << "Enter the menu number" << std::endl;
    return;
}

int comand_num(std::string str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] < '1' || str[i] > '9' || str.size() > 1) return -1;
    }
    return str[0] - '0';
}

void tmp_mode_change(CS &comp_st) {
    std::cout << "Enter" << std::endl << "1. Turn on" << std::endl << "2. Turn off" << std::endl;
    int mode;
    std::cin >> mode;
    if (mode == 1 || mode == 2) {
        mode_change(comp_st, mode);
    } else {
        std::cout << "Error! Incorrect input, returning in main menu" << std::endl;
    }
}

void save_into_file(const Pipe &P, const CS &comp_st) {
    std::string filename;
    std::cout << "Enter filename" << std::endl;
    std::cin >> filename;
    std::ofstream fout(filename);
    if (fout.is_open()) {
        fout.clear();
        if (!isEmpty(P)) {
            fout << "Pipe" << std::endl;
            fout << P.name << std::endl << P.len << std::endl << P.diameter << 
                std::endl << P.repair << std::endl;
        }
        if (!isEmpty(comp_st)) {
            fout << "CS" << std::endl;
            fout << comp_st.name << std::endl << comp_st.workshop_count << std::endl << comp_st.working_workshop << 
                std::endl << comp_st.station_class << std::endl;
        }
    } else {
        std::cout << "Error! Cannot open file" << std::endl;
    }
    fout.close();
    return;
}

void read_from_file(Pipe &P, CS &comp_st) {
    std::string filename;
    std::cout << "Enter filename" << std::endl;
    std::cin >> filename;
    std::ifstream fin(filename);
    if (fin.is_open()) {
        fin.seekg(0, fin.beg);
        std::string struct_type;
        std::getline(fin, struct_type);
        if (!struct_type.compare("Pipe")) {
            std::string name;
            int len;
            int diameter;
            bool repair;
            fin >> name >> len >> diameter >> repair;
            P = init_pipe(name, len, diameter, repair);
            fin >> struct_type;
        }
        if (!struct_type.compare("CS")) {
            std::string name;
            int workshop_count;
            int working_workshop;
            bool station_class;
            fin >> name >> workshop_count >> working_workshop >> station_class;
            comp_st = init_cs(name, workshop_count, working_workshop, station_class);
        }
    } else {
        std::cout << "Error! Cannot open file" << std::endl;
    }
    fin.close();
    return;
}

void change_pipe_data(Pipe &P) {
    std::string name;
    int len;
    int diameter;
    bool repair;
    std::cout << "Print pipe name" << std::endl;
    std::cin >> name;
    std::cout << "Print pipe lenght" << std::endl;
    std::cin >> len;
    std::cout << "Print pipe diameter" << std::endl;
    std::cin >> diameter;
    std::cout << "Print pipe repair status" << std::endl;
    std::cin >> repair;
    P = init_pipe(name, len, diameter, repair);
    return;
}

void change_cs_data(CS &comp_st) {
    std::string name;
    int workshop_count;
    int working_workshop;
    bool station_class;
    std::cout << "Print CS name" << std::endl;
    std::cin >> name;
    std::cout << "Print CS workshops count" << std::endl;
    std::cin >> workshop_count;
    std::cout << "Print CS number of working workshops" << std::endl;
    std::cin >> working_workshop;
    std::cout << "Print CS station class" << std::endl;
    std::cin >> station_class;
    comp_st = init_cs(name, workshop_count, working_workshop, station_class);
}

void process() {
    bool flag = 1;
    Pipe P = init_pipe("Empty pipe", 0, 0, true);
    CS comp_st = init_cs("Empty compressor station", 0, 0, -1);
    std::vector<std::pair<std::function<void()>, std::string>> actions = {
        {[&P]() {change_pipe_data(P);}, "Create pipe"},
        {[&P]() {print_pipe_data(P);}, "Print pipe info"},
        {[&P]() {repair_change(P);}, "Change repair status"},
        {[&comp_st]() {change_cs_data(comp_st);}, "Create CS"},
        {[&comp_st]() {print_cs_data(comp_st);}, "Print CS info"},
        {[&comp_st]() {tmp_mode_change(comp_st);}, "Change CS working workshops"},
        {[&P, &comp_st]() {save_into_file(P, comp_st);}, "Save"},
        {[&P, &comp_st]() {read_from_file(P, comp_st);}, "Load"},
    };
    while (flag) {
        main_menu();
        std::string op;
        std::cin >> op;
        int n = comand_num(op);
        n--;
        if (n != -1) {
            if (n == 8) {
                flag = !flag;
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