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