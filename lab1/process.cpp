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
#include <variant>
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

bool check_by_repair(const Pipe &s, const bool param) {
	return s.get_repair() == param;
}

bool check_by_occupancy(const CS &s, const double param) {
	return s.get_percent() == param;
}

int choose_filter(int struct_type) {
    std::string str;
    int filter, ans = 0;
    std::cout << "Enter: " << std::endl
        << "1. Filter by name" << std::endl
        << "2. Filter by parameter" << std::endl;
    filter = get_correct_number<int>(1, 2);
    if (struct_type == 1) ans = filter;
    else if (struct_type == 2) ans = filter + 2;
    return ans;
}

void filtration(std::unordered_map<int, Pipe> &p, std::unordered_map<int, CS> &cs) {
    std::cout << "Enter struct: " << std::endl
        << "1. Pipe" << std::endl
        << "2. CS" << std::endl;
    int struct_type = get_correct_number<int>(1, 2);
    int filter_num = choose_filter(struct_type);
    std::variant<std::string, bool, double> param;
    if (filter_num == 1 || filter_num == 3) {
        std::cout << "Enter struct name: " << std::endl;
        param = get_str();
    } else if (filter_num == 2) {
        std::cout << "Enter repair status (0. under repair, 1. repaired)" << std::endl;
        param = get_correct_number<bool>(0, 1);
    } else {
        std::cout << "Enter percent" << std::endl;
        param = get_correct_number<double>(0.0, 100.0);
    }
    if (struct_type == 1)
        std::visit([&p, &filter_num](auto&& arg) {
            if (filter_num == 2) find_by_filter(p, check_by_repair, arg);
            else find_by_filter(p, check_by_name, arg);
        }, param);
    else 
        std::visit([&cs, &filter_num](auto&& arg) {
            if (filter_num == 4) find_by_filter(cs, check_by_occupancy, arg);
            else find_by_filter(cs, check_by_name, arg);
        }, param);
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

void pipe_edit(std::unordered_map<int, Pipe> &P, std::vector<int> &ids) {
    for (int i = 0; i < (int) ids.size(); i++) {
        if (P.contains(ids[i])) {
            P[ids[i]].repair_change();
        }
    }
    return;
}

void pipe_delete(std::unordered_map<int, Pipe> &P, std::vector<int> &ids) {
    for (int i = 0; i < (int) ids.size(); i++) {
        if (P.contains(ids[i])) {
            P.erase(ids[i]);
        }
    }
    return;
}

void cs_edit(std::unordered_map<int, CS> &comp_st, std::vector<int> &ids, bool mode) {
    for (int i = 0; i < (int) ids.size(); i++) {
        if (comp_st.contains(ids[i])) {
            comp_st[ids[i]].mode_change(mode);
        }
    }
    return;
}

void cs_delete(std::unordered_map<int, CS> &comp_st, std::vector<int> &ids) {
    for (int i = 0; i < (int) ids.size(); i++) {
        if (comp_st.contains(ids[i])) {
            comp_st.erase(ids[i]);
        }
    }
    return;
}

void edit(std::unordered_map<int, Pipe> &P, std::unordered_map<int, CS> &comp_st) {
    int struct_type;
    std::cout << "Enter:" << std::endl
        << "1. Pipe" << std::endl
        << "2. CS" << std::endl;
    struct_type = get_correct_number<int>(1, 2);
    std::cout << "Enter:" << std::endl
        << "1. Sort by filter" << std::endl
        << "2. Sort by IDs" << std::endl;
    int type = get_correct_number<int>(1, 2);
    std::vector<int> ids;
    if (type == 1) {
        int filter_num = choose_filter(struct_type);
        std::variant<std::string, bool, double> param;
        if (filter_num == 1 || filter_num == 3) {
            std::cout << "Enter struct name: " << std::endl;
            param = get_str();
        } else if (filter_num == 2) {
            std::cout << "Enter repair status (0. under repair, 1. repaired)" << std::endl;
            param = get_correct_number<bool>(0, 1);
        } else {
            std::cout << "Enter percent" << std::endl;
            param = get_correct_number<double>(0.0, 100.0);
        }
        if (struct_type == 1) 
            std::visit([&P, &filter_num, &ids](auto&& arg) {
                if (filter_num == 2) ids = find_by_filter(P, check_by_repair, arg);
                else ids = find_by_filter(P, check_by_name, arg);
            }, param);
        else 
            std::visit([&cs, &filter_num, &ids](auto&& arg) {
                if (filter_num == 4) ids = find_by_filter(comp_st, check_by_occupancy, arg);
                else ids = find_by_filter(comp_st, check_by_name, arg);
            }, param);
    } else{
        ids = enter_id();
    }
    std::cout << "Enter:" << std::endl
        << "1. Change" << std::endl
        << "2. Delete" << std::endl;
    int mode = get_correct_number<int>(1, 2);
    if (mode == 1 && !ids.empty() && struct_type == 1) {
        pipe_edit(P, ids);
    } else if (mode == 1 && !ids.empty() && struct_type == 2) {
        bool mode;
        std::cout << "Enter:" << std::endl
            << "0. Turn off" << std::endl
            << "1. Turn on" << std::endl;
        mode = get_correct_number<bool>(0, 1);
        if (mode == 1 || mode == 0) cs_edit(comp_st, ids, mode);
        else std::cout << "Error! Incorrect mode, returning to main menu" << std::endl;
    } else if (mode == 2 && !ids.empty() && struct_type == 1) {
        pipe_delete(P, ids);
    } else if (mode == 2 && !ids.empty() && struct_type == 2) {
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
        {[&P, &comp_st]() {filtration(P, comp_st);}, "Find"},
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