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
                "Add new CS", "Print CSs info", "Change CS working workshops", "Find", "Edit", "Save", "Load", "Exit"};
    for (int i = 0; i < (int) operations.size(); i++) {
        std::cout << i + 1 << ". " << operations[i] << std::endl;
    }
    std::cout << "Enter the menu number" << std::endl;
    std::string str;
    int n = get_num();
    if (n <= (int) operations.size() && n >= 1) {
        return n - 1;
    } else {
        return -1;
    }
}

int get_num() {
    std::string str;
    int n;
    bool flag = 1;
    while (flag) {
        try {
            std::cin >> str;
            std::cerr << str << std::endl;
            n = std::stoi(str);
            flag = 0;
        } catch (...) {
            flag = 1;
            std::cout << "Error! Incorrect input, try again" << std::endl;
        }
    }
    return n;
}

std::string get_str() {
    std::string str;
    std::getline(std::cin>>std::ws, str);
    std::cerr << str << std::endl;
    return str;
}

double get_double() {
    std::string str;
    double n;
    bool flag = 1;
    while (flag) {
        try {
            std::cin >> str;
            std::cerr << str << std::endl;
            n = std::stod(str);
            flag = 0;
        } catch (...) {
            flag = 1;
            std::cout << "Error! Incorrect input, try again" << std::endl;
        }
    }
    return n;
}

void execute_repair_change(std::unordered_map<int, Pipe> &P) {
    int id;
    std::cout << "Enter pipe ID" << std::endl;
    id = get_num();
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
    id = get_num();
    if (comp_st.contains(id)) {
        std::cout << "Enter" << std::endl << "1. Turn on" << std::endl << "2. Turn off" << std::endl;
        mode = get_num();
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
    int len, diameter, repair;
    std::cout << "Print pipe name" << std::endl;
    name = get_str();
    std::cout << "Print pipe lenght" << std::endl;
    len = get_double();
    std::cout << "Print pipe diameter" << std::endl;
    diameter = get_num();
    std::cout << "Print pipe repair status" << std::endl;
    repair = get_num();
    if ((repair == 1 || repair == 0) && len > -1 && diameter > -1) {
        Pipe p(name, len, diameter, repair);
        P.emplace(p.get_id(), std::move(p));
    } else std::cout << "Error! Incorrect input, returning in main menu" << std::endl;
    return;
}

void cs_add(std::unordered_map<int, CS> &comp_st) { //!!!!
    std::string name;
    int workshop_count, working_workshop, station_class;
    std::cout << "Print CS name" << std::endl;
    name = get_str();
    std::cout << "Print CS workshops count" << std::endl;
    workshop_count = get_num();
    std::cout << "Print CS number of working workshops" << std::endl;
    working_workshop = get_num();
    std::cout << "Print CS station class" << std::endl;
    station_class = get_num();
    if (working_workshop > -1 && workshop_count > -1 && station_class > -1) {
        CS cs(name, workshop_count, working_workshop, station_class);
        comp_st.emplace(cs.get_id(), std::move(cs));
    } else {
        std::cout << "Error! Incorrect input, returning in main menu" << std::endl;
    }
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

int choose_filter() {
    std::string str;
    int struct_type, filter, ans = 0;
    std::cout << "Enter: " << std::endl
        << "1. Pipe" << std::endl
        << "2. CS" << std::endl;
    struct_type = get_num();
    if (struct_type < 1 || struct_type > 2) return -1;
    std::cout << "Enter: " << std::endl
        << "1. Filter by name" << std::endl
        << "2. Filter by parameter" << std::endl;
    filter = get_num();
    if (filter < 1 || filter > 2) return -1;
    if (struct_type == 1) ans = filter;
    else if (struct_type == 2) ans = filter + 2;
    return ans;
}

std::pair<int, std::vector<int>> filters(std::unordered_map<int, Pipe> &P, std::unordered_map<int, CS> &comp_st) { //!!!! lambda func?
    int filter;
    std::pair<int, std::vector<int>> id;
    filter = choose_filter();    
    if (filter == 1) {
        id.first = 1;
        std::string name;
        std::cout << "Enter the pipe name: " << std::endl;
        name = get_str();
        for (const auto &p : P) {
            if (p.second.get_name().find(name) != std::string::npos) {
                id.second.push_back(p.first);
                std::cout << p.second;
            }
        }
    } else if (filter == 2) {
        id.first = 1;
        int repair;        
        repair = get_num();
        for (const auto &p : P) {
            if (p.second.get_repair() == repair) {
                id.second.push_back(p.first);
                std::cout << p.second;
            }
        }
    } else if (filter == 3) {
        id.first = 2;
        std::string name;
        std::cout << "Enter the cs name: " << std::endl;
        name = get_str();
        for (const auto &cs : comp_st) {
            if (cs.second.get_name().find(name) != std::string::npos) {
                id.second.push_back(cs.first);
                std::cout << cs.second;
            }
        }
    } else if (filter == 4) {
        id.first = 2;
        double percent;
        percent = get_double();
        for (const auto &cs : comp_st) {
            if (cs.second.get_percent() == percent) {
                id.second.push_back(cs.first);
                std::cout << cs.second;
            }
        }
    } else {
        std::cout << "Error! Incorret input, returning to main menu" << std::endl;
        return id;
    }
    return id;
}

std::pair<int, std::vector<int>> enter_id() {
    int id = -1, struct_type, flag = 1;
    std::pair<int, std::vector<int>> ids;
    while (flag) {
        std::cout << "Which struct?" << std::endl
            << "1. Pipe" << std::endl
            << "2. CS" << std::endl;
        struct_type = get_num();
        if (struct_type == 1) {
            ids.first = 1;
            flag = 0;
        } else if (struct_type == 2) {
            ids.first = 2;
            flag = 0;
        } else {
            std::cout << "Error! Incorrect input, try again" << std::endl;
        }
    }
    std::cout << "Enter the IDs, to stop enter 0: " << std::endl;
    while (id != 0) {
        id = get_num();
        if (id > 0) ids.second.push_back(id);
        else if (id != 0) std::cout << "ID must be >= 1, try again" << std::endl;
    }
    return ids;
}

void pipe_edit(std::unordered_map<int, Pipe> &P, std::pair<int, std::vector<int>> &ids) {
    for (int i = 0; i < (int) ids.second.size(); i++) {
        if (P.contains(ids.second[i])) {
            P[ids.second[i]].repair_change();
        }
    }
    return;
}

void pipe_delete(std::unordered_map<int, Pipe> &P, std::pair<int, std::vector<int>> &ids) {
    for (int i = 0; i < (int) ids.second.size(); i++) {
        if (P.contains(ids.second[i])) {
            P.erase(ids.second[i]);
        }
    }
    return;
}

void cs_edit(std::unordered_map<int, CS> &comp_st, std::pair<int, std::vector<int>> &ids, bool mode) {
    for (int i = 0; i < (int) ids.second.size(); i++) {
        if (comp_st.contains(ids.second[i])) {
            comp_st[ids.second[i]].mode_change(mode);
        }
    }
    return;
}

void cs_delete(std::unordered_map<int, CS> &comp_st, std::pair<int, std::vector<int>> &ids) {
    for (int i = 0; i < (int) ids.second.size(); i++) {
        if (comp_st.contains(ids.second[i])) {
            comp_st.erase(ids.second[i]);
        }
    }
    return;
}

void edit(std::unordered_map<int, Pipe> &P, std::unordered_map<int, CS> &comp_st) {
    std::cout << "Enter:" << std::endl
        << "1. Sort by filter" << std::endl
        << "2. Sort by IDs" << std::endl;
    int type = get_num();
    std::pair<int, std::vector<int>> ids;
    if (type == 1) {
        ids = filters(P, comp_st);
    } else if (type == 2) {
        ids = enter_id();
    } else {
        std::cout << "Error! Incorrect input, returning to main menu" << std::endl;
        return;
    }
    std::cout << "Enter:" << std::endl
        << "1. Change" << std::endl
        << "2. Delete" << std::endl;
    int mode = get_num();
    if (mode == 1 && !ids.second.empty() && ids.first == 1) {
        pipe_edit(P, ids);
    } else if (mode == 1 && !ids.second.empty() && ids.first == 2) {
        bool mode;
        std::cout << "Enter:" << std::endl
            << "1. Turn on" << std::endl
            << "2. Turn off" << std::endl;
        mode = get_num();
        if (mode == 1 || mode == 0) cs_edit(comp_st, ids, mode);
        else std::cout << "Error! Incorrect mode, returning to main menu" << std::endl;
    } else if (mode == 2 && !ids.second.empty() && ids.first == 1) {
        pipe_delete(P, ids);
    } else if (mode == 2 && !ids.second.empty() && ids.first == 2) {
        cs_delete(comp_st, ids);
    } else {
        std::cout << "Error! Incorrect input, returning to main menu" << std::endl;
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
        {[&P, &comp_st]() {filters(P, comp_st);}, "Find"},
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