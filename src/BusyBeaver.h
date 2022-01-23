#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <fstream>

class BusyBeaver{
    private: 
    //std::string save;
    int states;
    int population_int;
    int threads;
    int end_condition;


    public:
    BusyBeaver(int _states, int _population_int, int _end_condition);
    void setup();
    void compute();
    void finalize();
};


