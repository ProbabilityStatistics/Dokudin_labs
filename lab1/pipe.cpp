#include "pipe.h"

#include <iostream>
#include <fstream>

std::atomic<int> Pipe::next_id{1};

Pipe::Pipe() {
    id = Pipe::next_id.fetch_add(1, std::memory_order_relaxed);
    name = "Empty pipe";
    len = 0;
    diameter = 0;
    repair = 0;
}

Pipe::~Pipe() {}

void Pipe::print_pipe_data() const{
    std::cout 
    << "ID: " << id
    << ", name: " << name 
    << ", len: " << len 
    << ", diameter: " << diameter 
    << ", repair status: "<< (repair ? "under repair" : "repaired")
    << std::endl;
    return;
}

void Pipe::repair_change() {
    repair = !repair;
}

bool Pipe::isEmpty () const{
    if (len == 0 && diameter == 0) return 1;
    else return 0;
}

void Pipe::pipe_save_into_file(std::ofstream &fout) const {
    fout << "Pipe" << std::endl;
    fout << id << std::endl 
        << name << std::endl 
        << len << std::endl 
        << diameter << std::endl 
        << repair << std::endl;
}

void Pipe::pipe_read_from_file(std::ifstream &fin) {
    std::getline(fin, name);
    fin >> len >> diameter >> repair;
    return;
}

void Pipe::set(std::string n, int l, int d, bool r) {
    name = n;
    len = l;
    diameter = d;
    repair = r;
}

int Pipe::get_id() const {
    return id;
}

std::string Pipe::get_name() const {
    return name;
}

int Pipe::get_repair() const {
    return repair;
}

std::ostream& operator<<(std::ostream& os, const Pipe& P) {
    return os << "ID: " << P.id
        << ", name: " << P.name 
        << ", len: " << P.len 
        << ", diameter: " << P.diameter 
        << ", repair status: "<< (P.repair ? "under repair" : "repaired")
        << std::endl;
}