#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <functional>
#include <iostream>
#include "pipe.h"
#include "compressor_station.h"

class redirect_output_wrapper // https://github.com/papilinatm/cpp_lessons_2020/blob/master/cpp_lessons/utils.h
{
private:
    std::ostream &stream;
    std::streambuf *const old_buf;
public:
    redirect_output_wrapper(std::ostream &src)
        : stream(src), old_buf(src.rdbuf()) {
    }
    ~redirect_output_wrapper() {
        stream.rdbuf(old_buf);
    }
    void redirect(const std::ostream &dest) const {
        stream.rdbuf(dest.rdbuf());
    }
};

void process();
int main_menu();
void execute_repair_change(std::unordered_map<int, Pipe> &P);
void execute_mode_change(std::unordered_map<int, CS> &comp_st);
void save_into_file(const std::unordered_map<int, Pipe> &P, const std::unordered_map<int, CS> &comp_st);
void read_from_file(Pipe &P, CS &comp_st);
void pipe_add(std::unordered_map<int, Pipe> &P);
void cs_add(std::unordered_map<int, CS> &comp_st);
void pipe_info(const std::unordered_map<int, Pipe> &P);
void cs_info(const std::unordered_map<int, CS> &comp_st);
std::vector<int> filtration(std::unordered_map<int, Pipe> &p, std::unordered_map<int, CS> &cs, int struct_type = 0);
void filtration_void_wrapper(std::unordered_map<int, Pipe> &p, std::unordered_map<int, CS> &cs);
std::string get_str();
std::vector<int> enter_id();
void pipe_edit(std::unordered_map<int, Pipe> &P, std::pair<int, std::vector<int>> &ids);
void pipe_delete(std::unordered_map<int, Pipe> &P, std::pair<int, std::vector<int>> &ids);
void cs_edit(std::unordered_map<int, CS> &comp_st, std::pair<int, std::vector<int>> &ids, bool mode);
void cs_delete(std::unordered_map<int, CS> &comp_st, std::pair<int, std::vector<int>> &ids);
void edit(std::unordered_map<int, Pipe> &P, std::unordered_map<int, CS> &comp_st);
bool check_by_repair(const Pipe &s, const bool param);
bool check_by_occupancy(const CS &s, const double param);

//template<typename T, typename S>
//using Filter = bool(*)(const S &s, const T &param); // https://github.com/papilinatm/cpp_lessons_2020/blob/master/cpp_lessons/main.cpp

template <typename T>
T get_correct_number(T min, T max)  // https://github.com/papilinatm/cpp_lessons_2020/blob/master/cpp_lessons/utils.h
{
	T x;
	while ((std::cin >> x).fail()	// check type
		|| std::cin.peek() != '\n'	// is buffer empty (int/float check)
		|| x < min || x > max)		// check range
	{
		std::cin.clear();
		std::cin.ignore(10000, '\n');
		std::cout << "Type number (" << min << "-" << max << "):";
	}
	std::cerr << x << std::endl;
	return x;
}

template<typename T>
bool check_by_name(const T &s, const std::string &param) {
	return s.get_name().find(param) != std::string::npos;
}

template<typename T, typename S, typename F>
std::vector<int> find_by_filter(const std::unordered_map<int, S>& st, F func, T param) {
	std::vector<int> res;
	for (auto& s : st) {
		if (func(s.second, param))
			res.push_back(s.first);
	}
	return res;
}

template<typename T>
void delete_by_ids(std::unordered_map<int, T>& collection, const std::vector<int>& ids) {
    for (int id : ids) {
        collection.erase(id);
    }
}

#endif