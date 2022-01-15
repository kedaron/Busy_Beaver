#include <iostream>
#include <vector>
#include <random>
#include<bitset>
#include <algorithm>

std::mt19937 mt(time(nullptr)); 
std::uniform_int_distribution<int> card_selector(0, 3);
std::uniform_int_distribution<int> state_selector(0, 12);

//All possible instructions for a card
uint8_t head_zero[4] =  {0b000, 0b001, 0b010, 0b011};
uint8_t head_one[4] =  {0b100, 0b101, 0b110, 0b111};

//Tape that we reuse
std::vector<int> tape(2048, 0);

class TouringMachine{
    private:
        //The first card is always the start state
        //The last card is always the end state
        std::vector<int> _instructionset;
        int fitness; 

        
    public:
        TouringMachine(std::vector<int>& instructionset){
            _instructionset = instructionset;
        }

        void run(std::vector<int> &tape){
            fitness = 0;
            int tape_value, direction, new_value, instruction, state = 0;
            int head = tape.size()/2;
            tape.assign(tape.size(), 0);

            while(fitness<=20){
                if(head == 0 || head == (int)tape.size()-1)
                    break;

                tape_value = tape[head];
                instruction = _instructionset[state];

                if(tape_value == 1){
                    new_value = (instruction>>8) & 0x1;
                    direction = (instruction>>7) & 0x1;
                }else{
                    new_value = (instruction>>5) & 0x1;
                    direction = (instruction>>4) & 0x1;
                }
                
                tape[head] = new_value;
                fitness += new_value;

                if(direction == 1){
                     head++;
                }else{
                    head--;
                }

                if(state == 12){
                    break;
                }
                
                state = instruction & 0xC;
            }
        }

        int getFitness(){
            return fitness;
        }
    
};


std::vector<int> genarateInstructionSet(int amount_of_cards){
    std::vector<int> cards(amount_of_cards);
    int state;
    bool has_endstep = false;

    for(auto i = 0; i<amount_of_cards; i++){
        state = state_selector(mt);

        if((i+1) == amount_of_cards && !has_endstep){
            state = 12; 
        }

        cards[i] = (head_one[card_selector(mt)]<<7) | (head_zero[card_selector(mt)]<<4) | state;
    }

    std::shuffle(std::begin(cards), std::end(cards), mt);
    return cards;
}
void crossover(std::vector<int>& p1, std::vector<int>&p2){

}
bool validateInstruktionSet(std::vector<int>& vec){
    bool has_endstep = false;

    for(size_t i = 0; i < vec.size(); i++ ) {
        if((((vec[i]>>9)) & 0x1) == 0)
            return false;

        if((((vec[i]>>6)) & 0x1) == 1)
            return false;  

        if(((vec[i] & 0xF) > 0xC))
            return false;

        if((vec[i] & 0xF) == 0xC)
            has_endstep = true;
    }

    return true && has_endstep;
}

int main(){

    std::vector<int> test = genarateInstructionSet(13);
    std::cout << validateInstruktionSet(test) << std::endl;
    TouringMachine t1(test);
    t1.run(tape);

    std::cout << t1.getFitness() << std::endl;

    return 0;
}