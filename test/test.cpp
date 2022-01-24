#include <iostream>
#include <vector>
#include <bitset>
#include "TouringMachine.h"

std::vector<std::vector<int>> solutions;


void setup(std::vector<std::vector<int>> &solutions){
    
    solutions.push_back(std::vector<int> {0b1101110001});
    solutions.push_back(std::vector<int> {0b1100100011});
    solutions.push_back(std::vector<int> {0b1101100011});
    solutions.push_back(std::vector<int> {0b1101010000});

    solutions.push_back(std::vector<int> {0b1100110001, 0b1000011010});
    solutions.push_back(std::vector<int> {0b1100110001, 0b1000011111});
    solutions.push_back(std::vector<int> {0b1000111001, 0b1100010111});
    solutions.push_back(std::vector<int> {0b1100100100, 0b1110101100});

    solutions.push_back(std::vector<int> {0b1000111010, 0b1001010111, 0b1100001001});
    solutions.push_back(std::vector<int> {0b1000110000, 0b1101011011, 0b1000011001});
    solutions.push_back(std::vector<int> {0b1100111000, 0b1001011011, 0b1100010001});
    solutions.push_back(std::vector<int> {0b1001010000, 0b0000011010, 0b1100101001});
 
    solutions.push_back(std::vector<int> {0b1001100110, 0b1101000010, 0b1001101001, 0b0101010010});
    solutions.push_back(std::vector<int> {0b1101100011, 0b0000001000, 0b0100000010, 0b1001011000});
    solutions.push_back(std::vector<int> {0b1000101101, 0b0001101001, 0b1100110011, 0b1001010001});
    solutions.push_back(std::vector<int> {0b1001001001, 0b0000111000, 0b1100110010, 0b0001110000});

}

int getEndCondition(int states){

    switch (states)
    {
    case 1:
        return 1;

    case 2:
        return 4;

    case 3:
        return 6;

    case 4:
        return 13;

    default:
        std::cerr << "States bigger than 4 are not implemented" << std::endl;
        return -1;
    }

}
    

int main(){
    setup(solutions);

    TouringMachine touring;
    int size, endConditon;

    for(auto &it: solutions){
        size = it.size();
        endConditon = getEndCondition(size);

        if(it.size() < 0) continue;

        touring = TouringMachine(it, endConditon, size);
        touring.run();

        if(touring.getFitness() == endConditon){
            std::cout << "Solution found " << touring.getFitness()<< " 1s were written" << std::endl;
            for(auto &r: touring.getInstructionSet()){
                std::cout << std::bitset<10>(r) << std::endl;
            }
            std::cout <<"\n\n";
        }
    }
}