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
#include <chrono>
#include <fmt/chrono.h> // sudo apt install libfmt-dev
#include <fmt/format.h>

int main_menu() {
    std::cout << "Menu" << std::endl;
    std::vector <std::string> operations = {"Add new pipe", "Print pipes info", "Change repair status",
                "Add new CS", "Print CSs info", "Change CS working workshops", "Filter", "Edit", "Save", "Load", "Exit"};
    for (int i = 0; i < (int) operations.size(); i++) {
        std::cout << i + 1 << ". " << operations[i] << std::endl;
    }
    std::cout << "Enter the menu number" << std::endl;
    std::string str;
    int n = get_correct_number<int>(1, (int) operations.size());
    return n - 1;
}

std::string get_str() {
    std::string str;
    std::getline(std::cin>>std::ws, str);
    std::cerr << str << std::endl;
    return str;
}

void execute_repair_change(std::unordered_map<int, Pipe> &P) {
    int id;
    std::cout << "Enter pipe ID" << std::endl;
    id = get_correct_number<int>(1, P.size());
    if (P.contains(id)) {
        P[id].repair_change();
    } else {
        std::cout << "Error! The CS with this ID was not found" << std::endl;
    }
    return;
}

void execute_mode_change(std::unordered_map<int, CS> &comp_st) {
    int id;
    int mode;
    std::cout << "Enter compressor station ID" << std::endl;
    id = get_correct_number<int>(1, comp_st.size());
    if (comp_st.contains(id)) {
        std::cout << "Enter" << std::endl << "1. Turn on" << std::endl << "2. Turn off" << std::endl;
        mode = get_correct_number<int>(1, 2);
        comp_st[id].mode_change(mode);
    } else {
        std::cout << "Error! The CS with this ID was not found" << std::endl;
    }
    return;
}

void save_into_file(const std::unordered_map<int, Pipe> &P, const std::unordered_map<int, CS> &comp_st) {
    std::string filename;
    std::cout << "Enter filename" << std::endl;
    filename = get_str();
    std::ofstream fout(filename);
    if (fout.is_open()) {
        for (const auto &[id, p] : P) {
            p.pipe_save_into_file(fout);
        }
        for (const auto &[id, cs] : comp_st) {
            cs.cs_save_into_file(fout);
        }
        fout.close();
    } else {
        std::cout << "Error! Cannot open file, returning to main menu" << std::endl;
    }
    return;
}

void read_from_file(std::unordered_map<int, Pipe> &P, std::unordered_map<int, CS> &comp_st) {
    std::string filename;
    std::cout << "Enter filename" << std::endl;
    filename = get_str();
    std::ifstream fin(filename);
    if (fin.is_open()) {
        std::string struct_type;
        while (std::getline(fin>>std::ws, struct_type)) {
            if (struct_type == "Pipe") {
                int id;
                fin >> id;
                if (P.contains(id)) {
                    P[id].pipe_read_from_file(fin);
                } else {
                    Pipe p;
                    p.pipe_read_from_file(fin);
                    P.emplace(p.get_id(), std::move(p));
                }
            }
            else if (struct_type == "CS") {
                int id;
                fin >> id;
                if (comp_st.contains(id)) {
                    comp_st[id].cs_read_from_file(fin);
                } else {
                    CS cs;
                    cs.cs_read_from_file(fin);
                    comp_st.emplace(cs.get_id(), cs);
                }
                CS cs;
                cs.cs_read_from_file(fin);
                comp_st.emplace(cs.get_id(), cs);
            }
            else {
                std::cout << "Error! Something went wrong while reading the file" << std::endl;
            }
        }
        fin.close();
    } else {
        std::cout << "Error! Cannot open file" << std::endl;
    }
    return;
}

void pipe_add(std::unordered_map<int, Pipe> &P) { //!!!!
    std::string name;
    double len;
    int diameter;
    bool repair;
    std::cout << "Print pipe name" << std::endl;
    name = get_str();
    std::cout << "Print pipe lenght" << std::endl;
    len = get_correct_number<double>(0, 2000);
    std::cout << "Print pipe diameter" << std::endl;
    diameter = get_correct_number<int>(0, 2000);
    std::cout << "Print pipe repair status" << std::endl;
    repair = get_correct_number<bool>(0, 1);
    Pipe p(name, len, diameter, repair);
    P.emplace(p.get_id(), std::move(p));
    return;
}

void cs_add(std::unordered_map<int, CS> &comp_st) { //!!!!
    std::string name;
    int workshop_count, working_workshop, station_class;
    std::cout << "Print CS name" << std::endl;
    name = get_str();
    std::cout << "Print CS workshops count" << std::endl;
    workshop_count = get_correct_number<int>(0, 2000);
    std::cout << "Print CS number of working workshops" << std::endl;
    working_workshop = get_correct_number<int>(0, workshop_count);
    std::cout << "Print CS station class" << std::endl;
    station_class = get_correct_number<int>(0, 2000);
    CS cs(name, workshop_count, working_workshop, station_class);
    comp_st.emplace(cs.get_id(), std::move(cs));
    return;
}

void pipe_info(const std::unordered_map<int, Pipe> &P) {
    for (const auto &p : P)
        std::cout << p.second;
    return;
}

void cs_info(const std::unordered_map<int, CS> &comp_st) {
    for (const auto &cs : comp_st)
        std::cout << cs.second;
    return;
}

bool check_by_repair(const Pipe &s, const bool &param) {
	return s.get_repair() == param;
}

bool check_by_occupancy(const CS &s, const double &param) {
	return s.get_percent() == param;
}

std::vector<int> filtration(std::unordered_map<int, Pipe> &p, std::unordered_map<int, CS> &cs, int struct_type) {
    std::vector<int> ids;
    if (!struct_type) {
        std::cout << "Choose struct: " << std::endl
            << "1. Pipe" << std::endl
            << "2. CS" << std::endl;
        struct_type = get_correct_number<int>(1, 2);
    }
    std::cout << "Filter by: " << std::endl
        << "1. Filter by name" << std::endl
        << "2. Filter by parameter" << std::endl;
    int filter_choice = get_correct_number<int>(1, 2);
    if (struct_type == 1) {
        if (filter_choice == 1) {
            std::string name;
            std::cout << "Enter name: ";
            name = get_str();
            ids = find_by_filter(p, check_by_name<Pipe>, name);
        } else {
            bool repair_status;
            std::cout << "Enter repair status (0 or 1): ";
            repair_status = get_correct_number<bool>(0, 1);
            ids = find_by_filter(p, check_by_repair, repair_status);
        }
    } else if (struct_type == 2) {
        if (filter_choice == 1) {
            std::string name;
            std::cout << "Enter name: ";
            name = get_str();
            ids = find_by_filter(cs, check_by_name<CS>, name);
        } else {
            double percent;
            std::cout << "Enter the percentage of occupancy: ";
            percent = get_correct_number<double>(0, 100.0);
            ids = find_by_filter(cs, check_by_occupancy, percent);
        }
    } else {
        std::cout << "Error! Incorrect struct type" << std::endl;
    }
    return ids;
}

void print_filtration(std::unordered_map<int, Pipe> &p, std::unordered_map<int, CS> &cs) {
    std::cout << "Choose struct: " << std::endl
        << "1. Pipe" << std::endl
        << "2. CS" << std::endl;
    int struct_type = get_correct_number<int>(1, 2);
    auto ids = filtration(p, cs, struct_type);
    if (struct_type == 1) {
        for (int id : ids) {
            if (p.contains(id))
                std::cout << p[id];
        }
    } else {
        for (int id : ids) {
            if (cs.contains(id))
                std::cout << cs[id];
        }
    }
    return;
}

std::vector<int> enter_id() {
    int id = -1;
    std::vector<int> ids;
    std::cout << "Enter the IDs, to stop enter 0: " << std::endl;
    while (id != 0) {
        id = get_correct_number<int>(0, INT32_MAX);
        if (id > 0) ids.push_back(id);
        else if (id != 0) std::cout << "ID must be >= 1, try again" << std::endl;
    }
    return ids;
}

void edit(std::unordered_map<int, Pipe> &P, std::unordered_map<int, CS> &comp_st) {
    std::vector<int> ids;
    std::cout << "Enter:" << std::endl
        << "1. Sort by filter" << std::endl
        << "2. Sort by IDs" << std::endl;
    int type = get_correct_number<int>(1, 2);
    std::cout << "Choose struct: " << std::endl
        << "1. Pipe" << std::endl
        << "2. CS" << std::endl;
    int struct_type = get_correct_number<int>(1, 2);
    if (type == 1)
        ids = filtration(P, comp_st, struct_type);
    else
        ids = enter_id();
    
    std::cout << "Enter:" << std::endl
        << "1. Change" << std::endl
        << "2. Delete" << std::endl;
    int mode = get_correct_number<int>(1, 2);
    if (struct_type == 1) {
        if (mode == 1) {
            std::cout << "Enter mode (1 - under repair, 0 - repaired): ";
            bool mode;
            mode = get_correct_number<bool>(0, 1);
            edit_by_ids(P, ids, &Pipe::repair_set, mode);
            std::cout << "Pipes edited" << std::endl;
        } else {
            delete_by_ids(P, ids);
            std::cout << "Pipes deleted" << std::endl;
        }
    } else {
        if (mode == 1) {
            std::cout << "Enter mode (1 to increase, 0 to decrease): ";
            bool mode;
            mode = get_correct_number<bool>(0, 1);
            edit_by_ids(comp_st, ids, &CS::mode_change, mode);
            std::cout << "CSs edited" << std::endl;
        } else {
            delete_by_ids(comp_st, ids);
            std::cout << "CSs deleted" << std::endl;
        }
    }
    return;
}

void process() {
    redirect_output_wrapper cerr_out(std::cerr);
    const std::string time = fmt::format("{:%d_%m_%Y %H_%M_%OS}", std::chrono::system_clock::now());
    std::ofstream logfile("logs/log_" + time);
    if (logfile)
        cerr_out.redirect(logfile);
    std::unordered_map<int, Pipe> P;
    std::unordered_map<int, CS> comp_st;
    std::vector<std::pair<std::function<void()>, std::string>> actions = {
        {[&P]() {pipe_add(P);}, "Create pipe"},
        {[&P]() {pipe_info(P);}, "Print pipe info"},
        {[&P]() {execute_repair_change(P);}, "Change repair status"},
        {[&comp_st]() {cs_add(comp_st);}, "Create CS"},
        {[&comp_st]() {cs_info(comp_st);}, "Print CS info"},
        {[&comp_st]() {execute_mode_change(comp_st);}, "Change CS working workshops"},
        {[&P, &comp_st]() {print_filtration(P, comp_st);}, "Filter"},
        {[&P, &comp_st]() {edit(P, comp_st);}, "Edit"},
        {[&P, &comp_st]() {save_into_file(P, comp_st);}, "Save"},
        {[&P, &comp_st]() {read_from_file(P, comp_st);}, "Load"},
    };
    while (true) {
        int n = main_menu();
        if (n > -1 && n <= (int) actions.size()) {
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
    logfile.close();
    return;
}