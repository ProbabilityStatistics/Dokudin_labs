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

int check_num() {
    std::string str;
    int n;
    try {
        std::cin >> str;
        n = std::stoi(str);
    } catch (...) {
        return -1;
    }
    return n;
}

void tmp_repair_change(std::unordered_map<int, Pipe> &P) {
    int id;
    std::cout << "Enter pipe ID" << std::endl;
    id = check_num();
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
    id = check_num();
    if (auto search = comp_st.find(id); search != comp_st.end()) {
        std::cout << "Enter" << std::endl << "1. Turn on" << std::endl << "2. Turn off" << std::endl;
        mode = check_num();
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
    len = check_num();
    std::cout << "Print pipe diameter" << std::endl;
    diameter = check_num();
    std::cout << "Print pipe repair status" << std::endl;
    repair = check_num();
    if (repair == 1 || repair == 0 && len > -1 && diameter > -1) {
        p.set(name, len, diameter, repair);
        P.emplace(p.get_id(), std::move(p));
    } else std::cout << "Error! Incorrect input, returning in main menu" << std::endl;
    return;
}

void cs_add(std::unordered_map<int, CS> &comp_st) {
    CS cs;
    std::string name;
    int workshop_count, working_workshop, station_class;
    std::cout << "Print CS name" << std::endl;
    std::getline(std::cin>>std::ws, name);
    std::cout << "Print CS workshops count" << std::endl;
    workshop_count = check_num();
    std::cout << "Print CS number of working workshops" << std::endl;
    working_workshop = check_num();
    std::cout << "Print CS station class" << std::endl;
    station_class = check_num();
    if (working_workshop > -1 && workshop_count > -1 && station_class > -1) {
        cs.set(name, workshop_count, working_workshop, station_class);
        comp_st.emplace(cs.get_id(), std::move(cs));
    } else {
        std::cout << "Error! Incorrect input, returning in main menu" << std::endl;
    }
    return;
}

void pipe_info(std::unordered_map<int, Pipe> &P) {
    for (const auto &p : P)
        std::cout << p.second;
    return;
}

void cs_info(std::unordered_map<int, CS> &comp_st) {
    for (const auto &cs : comp_st)
        std::cout << cs.second;
    return;
}

int parse_filter() {
    std::string str;
    int struct_type, filter, ans = 0;
    std::cout << "Enter: " << std::endl
        << "1. Pipe" << std::endl
        << "2. CS" << std::endl;
    struct_type = check_num();
    if (struct_type < 1 || struct_type > 2) return -1;
    std::cout << "Enter: " << std::endl
        << "1. Filter by name" << std::endl
        << "2. Filter by parameter" << std::endl;
    filter = check_num();
    if (filter < 1 || filter > 2) return -1;
    if (struct_type == 1) ans = filter;
    else if (struct_type == 2) ans = filter + 2;
    return ans;
}

std::vector<int> filters(std::unordered_map<int, Pipe> &P, std::unordered_map<int, CS> &comp_st) {
    int filter;
    std::vector<int> id;
    filter = parse_filter();    
    if (filter == 1) {
        id.push_back(-1);
        std::string name;
        std::cout << "Enter the pipe name: " << std::endl;
        std::cin >> name;
        for (const auto &p : P) {
            if (p.second.get_name() == name) {
                id.push_back(p.first);
                std::cout << p.second;
            }
        }
    } else if (filter == 2) {
        id.push_back(-1);
        int repair;        
        repair = check_num();
        for (const auto &p : P) {
            if (p.second.get_repair() == repair) {
                id.push_back(p.first);
                std::cout << p.second;
            }
        }
    } else if (filter == 3) {
        id.push_back(-2);
        std::string name;
        std::cout << "Enter the cs name: " << std::endl;
        std::cin >> name;
        for (const auto &cs : comp_st) {
            if (cs.second.get_name() == name) {
                id.push_back(cs.first);
                std::cout << cs.second;
            }
        }
    } else if (filter == 4) {
        id.push_back(-2);
        double percent;
        std::cin >> percent; //!!!!
        for (const auto &cs : comp_st) {
            if (cs.second.get_percent() == percent) {
                id.push_back(cs.first);
                std::cout << cs.second;
            }
        }
    } else {
        std::cout << "Error! Incorret input, returning to main menu" << std::endl;
        return;
    }
    return id;
}

std::vector<int> enter_id() {
    int id = 0, struct_type, flag = 1;
    std::vector<int> ids;
    while (flag) {
        std::cout << "Which struct?" << std::endl
            << "1. Pipe" << std::endl
            << "2. CS" << std::endl;
        struct_type = check_num();
        if (struct_type == 1) {
            ids.push_back(-1);
            flag = 0;
        } else if (struct_type == 2) {
            ids.push_back(-2);
            flag = 0;
        } else {
            std::cout << "Error! Incorrect input, try again" << std::endl;
        }
    }
    std::cout << "Enter the IDs, to stop enter -1: " << std::endl;
    while (id != -1) {
        id = check_num();
        if (id > -1) ids.push_back(id);
        else std::cout << "ID must be >= 1, try again" << std::endl;
    }
    return ids;
}

void pipe_edit(std::unordered_map<int, Pipe> &P, std::vector<int> &ids) {
    for (int i = 1; i < ids.size(); i++) {
        if (auto search = P.find(ids[i]); search != P.end()) {
            P[ids[i]].repair_change();
        }
    }
    return;
}

void pipe_delete(std::unordered_map<int, Pipe> &P, std::vector<int> &ids) {
    for (int i = 1; i < ids.size(); i++) {
        if (auto search = P.find(ids[i]); search != P.end()) {
            P.erase(search);
        }
    }
    return;
}

void cs_edit(std::unordered_map<int, CS> &comp_st, std::vector<int> &ids, bool mode) {
    for (int i = 1; i < ids.size(); i++) {
        if (auto search = comp_st.find(ids[i]); search != comp_st.end()) {
            comp_st[ids[i]].mode_change(mode);
        }
    }
    return;
}

void cs_delete(std::unordered_map<int, CS> &comp_st, std::vector<int> &ids) {
    for (int i = 1; i < ids.size(); i++) {
        if (auto search = comp_st.find(ids[i]); search != comp_st.end()) {
            comp_st.erase(search);
        }
    }
    return;
}

void edit(std::unordered_map<int, Pipe> &P, std::unordered_map<int, CS> &comp_st) {
    std::cout << "Enter:" << std::endl
        << "1. Sort by filter" << std::endl
        << "2. Sort by IDs" << std::endl;
    int type = check_num();
    std::vector<int> ids;
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
    int mode = check_num();
    if (mode == 1 && ids[0] == -1) {
        pipe_edit(P, ids);
    } else if (mode == 1 && ids[0] == -2) {
        bool mode;
        std::cout << "Enter:" << std::endl
            << "1. Turn on" << std::endl
            << "2. Turn off" << std::endl;
        mode = check_num();
        if (mode == 1 || mode == 0) cs_edit(comp_st, ids, mode);
        else std::cout << "Error! Incorrect mode, returning to main menu" << std::endl;
    } else if (mode == 2 && ids[0] == -1) {
        pipe_delete(P, ids);
    } else if (mode == 2 && ids[0] == -2) {
        cs_delete(comp_st, ids);
    } else {
        std::cout << "Error! Incorrect input, returning to main menu" << std::endl;
    }
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
    return ;
}