#include <string>
//#include "GeneticAlgorithm.h"
#include "BusyBeaver.h"
#include <omp.h>
#include <algorithm>
#include <vector>
#include <iostream>
#include <random>
#include <bitset>

std::mt19937 mt(time(nullptr)); 
std::uniform_int_distribution<int> card_selector(0, 3);
std::uniform_int_distribution<int> state_selector(0, 5);
std::uniform_int_distribution<int> bit_selector(0, 9);
std::uniform_int_distribution<int> mutationSelector(0,100);




template <typename T>
T tournamentSelect(std::vector<T> population, int n_select){
    std::uniform_int_distribution<int32_t> selector(0,population.size()-1);
    std::vector<T> select_parents;

    for(auto i = n_select-1; i>=0; i--){
        select_parents.push_back(population[selector(mt)]);
    }

    std::sort(select_parents.begin(), select_parents.end(),[](T & one, T & two){return one.getFitness() > two.getFitness();});

    return select_parents[0];
}

template <typename T>
T randomSelect(std::vector<T> population){
    std::uniform_int_distribution<int32_t> selector(0,population.size()-1);

    return population[selector(mt)];
}

//All possible instructions for a card
int head[4] =  {0x0, 0x1, 0x2, 0x3};

std::vector<int> generateInstructionSet(int amount_of_cards){
    std::vector<int> cards(amount_of_cards);

    for(auto i = 0; i<amount_of_cards; i++){
        cards[i] = (head[card_selector(mt)]<<8) | state_selector(mt) <<6 | (head[card_selector(mt)]<<4) | state_selector(mt);
    }

    return cards;
}

class TouringMachine{
    private:
        //The first card is always the start state
        //The last card is always the end state
        std::vector<int> _instructionset;
        int _max_age;
        int fitness; 
        int age;
        int _endcondition;
        int _states;
        std::vector<int> tape;
       
        
    public:
        TouringMachine(std::vector<int>& instructionset, int endcondition, int states){
            _instructionset = instructionset;
            _states = states;
            fitness = 0;
            age = 0; 
            tape.resize(10000);
            _endcondition = endcondition;
        }


        TouringMachine(int endcondition, int states){
            _instructionset = generateInstructionSet(states);
            _states = states;
            fitness = 0;
            age = 0;  
            tape.resize(10000);
           _endcondition = endcondition;
        }

        TouringMachine(){
        }

        TouringMachine& operator= (const TouringMachine& other){
            if(this != &other){
                _instructionset = other._instructionset;
                fitness = other.fitness;
                age = other.age;
                tape = other.tape;
                _states = other._states;
                _endcondition = other._endcondition;
            }
            return *this;
        }
        

        TouringMachine(const TouringMachine &t1) {
            _instructionset = t1._instructionset; 
            fitness = t1.fitness;
            age = t1.age; 
            tape = t1.tape;
            _states = t1._states;
            _endcondition = t1._endcondition;
        }

        bool operator > (const TouringMachine &other) const {
            return fitness > other.fitness;
        }

        void run(){
            fitness = 0;
            int timeout = 0;
            int tape_value, direction, new_value, instruction, state = 0;
            int head = tape.size()/2;
            tape.assign(tape.size(), 0);

            while(fitness <= _endcondition+10 && timeout++ < 1000){

                tape_value = tape[head];
                instruction = _instructionset[state];

                if(tape_value == 1){
                    instruction &= 0x1F;
                }else{
                    instruction >>= 5;                   
                }

                new_value = (instruction>>4) & 0x1;
                direction = (instruction>>3) & 0x1;
                state = instruction & 0x7;
                
                 
                tape[head] = new_value;
                fitness = std::count (tape.begin(), tape.end(), 1);

                if((tape[head]^new_value) == 1){
                    timeout = 0;
                }

                if(direction == 1){
                    head++;
                }else{
                    head--;
                }
               // std::cout << (instruction & 0xC) << std::endl;
                if(state >= _states){
                    break;
                }
                
                age++;
            }

            if(fitness > _endcondition){
                fitness = 0;
            }
        }

        void mutate(int chance){
    
            if(mutationSelector(mt) <= chance){
                for(auto &inst : _instructionset){
                    inst^= (1<<bit_selector(mt));
                }
            }
        }

        std::vector<int> getInstructionSet(){
            return _instructionset;
        }

        void setInstructionSet(std::vector<int> instructionset){
            _instructionset = instructionset;
        }

        int getFitness(){
            return fitness;
        }

        int getEndcondition(){
            return _endcondition;
        }

        int getStates(){
            return _states;
        }
    
};

void insertElement(std::vector<TouringMachine> &pop, std::vector<TouringMachine> &next_gen, int elite){

    for(auto i = elite; i < (int) pop.size(); i++){
        pop[i] = next_gen[i-elite];
    }

    std::sort(pop.begin(), pop.end(),[](TouringMachine & one, TouringMachine & two){return one.getFitness() > two.getFitness();});
}

void crossover(TouringMachine& p1, TouringMachine& p2, TouringMachine& c1, TouringMachine& c2, int chance){
    std::vector<int>child_1 = p1.getInstructionSet();
    std::vector<int>child_2 = p2.getInstructionSet();

    int child_1_value, child_2_value;
    int crosspoint = (int) pow(2, bit_selector(mt));
    int inverse_crosspoint = (1024 - crosspoint);

    for(auto i = 0; i< (int) child_1.size() && chance<100; i++){
        
        child_1_value =  child_1[i] & crosspoint;
        child_2_value =  child_2[i] & crosspoint;
     

        child_1[i] = (child_1[i] &  inverse_crosspoint) | child_1_value;
        child_2[i] = (child_2[i] &  inverse_crosspoint) | child_2_value;
    }

    c1 = TouringMachine(child_1, p1.getEndcondition(), p1.getStates());
    c2 = TouringMachine(child_2, p2.getEndcondition(), p2.getStates());

    c1.mutate(chance); 
    c2.mutate(chance); 

    c1.run();
    c2.run();
}


//Busy Beaver
std::vector<TouringMachine> population;
std::vector<TouringMachine> next_gen;

BusyBeaver::BusyBeaver(int _states, int _population_int, int _end_condition){
    states = _states;
    population_int = _population_int;
    end_condition = _end_condition;
}

void BusyBeaver::setup(){
    population.resize(population_int);
    next_gen.resize(population_int);

    for(auto &pop : population){
        pop = TouringMachine(end_condition, states);
        pop.run();
    }

    std::sort(population.begin(), population.end(),[](TouringMachine & one, TouringMachine & two){return one.getFitness() > two.getFitness();});
    std::cout << "Setup ended! Start Computation:" << std::endl;
}  


void BusyBeaver::compute(){
    TouringMachine ch1, ch2;
    int bestFitness = 0, elite = 10;

    while(population[0].getFitness() != end_condition){
        for(auto i = 0; i< (int) population.size(); i+=2){

            ch1 = tournamentSelect(population, 2);
            ch2 = tournamentSelect(population, 2);

            crossover(ch1, ch2, next_gen[i], next_gen[i+1], 100);
        }

        std::sort(next_gen.begin(), next_gen.end(),[](TouringMachine & one, TouringMachine & two){return one.getFitness() > two.getFitness();});
       
        insertElement(population, next_gen, elite);
     
        std::sort(population.begin(), population.end(),[](TouringMachine & one, TouringMachine & two){return one.getFitness() > two.getFitness();});
        
        if(bestFitness < population[0].getFitness()){
            bestFitness = population[0].getFitness();
            std::cout << "New Fitness "<< bestFitness << std::endl;
        }
    }

}

void BusyBeaver::finalize(){
    std::cout << std::endl;
    std::cout << "Solution found: " << population[0].getFitness() << std::endl;

    for(auto &r: population[0].getInstructionSet()){
        std::cout << std::bitset<10>(r) << std::endl;
    }
}