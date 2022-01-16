#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <bitset>



std::mt19937 mt(time(nullptr)); 
std::uniform_int_distribution<int> card_selector(0, 3);
std::uniform_int_distribution<int> state_selector(0, 12);
std::uniform_int_distribution<int> mutationSelector(0,100);
//All possible instructions for a card
int head_zero[4] =  {0x0, 0x1, 0x2, 0x3};
int head_one[4] =   {0x4, 0x5, 0x6, 0x7};

//Tape that we reuse
std::vector<int> tape(2048);

std::vector<int> generateInstructionSet(int amount_of_cards){
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

class TouringMachine{
    private:
        //The first card is always the start state
        //The last card is always the end state
        std::vector<int> _instructionset;
        int fitness; 
        const int max_tries = 500;
        int tries;
       
        
    public:
        TouringMachine(std::vector<int>& instructionset){
            _instructionset = instructionset;
        }

        TouringMachine(){
            _instructionset = generateInstructionSet(13);
        }
        
        TouringMachine& operator= (TouringMachine && other){
            if(this != &other){
                _instructionset = std::move(other._instructionset);
                fitness = std::move( other.fitness);
                tries = std::move(other.tries);
            }
            return *this;
        }

        TouringMachine(const TouringMachine &t1) {
            _instructionset = t1._instructionset; 
            fitness = t1.fitness;
            tries = t1.tries; 
            }

        bool operator > (const TouringMachine &other) const {
            return fitness > other.fitness;
        }

        void run(std::vector<int> &tape){
            fitness = 0;
            tries = 0;
            int tape_value, direction, new_value, instruction, state = 0;
            int head = tape.size()/2;
            tape.assign(tape.size(), 0);

            while(fitness < 20){
                if(head == 0 || head == (int)tape.size()-1 || tries >= max_tries){
                    fitness = 0;
                    break;
                }

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
                fitness = std::count (tape.begin(), tape.end(), 1);

                if(direction == 1){
                     head++;
                }else{
                    head--;
                }
               // std::cout << (instruction & 0xC) << std::endl;
                if(state == 12){
                    break;
                }
                
                state = instruction & 0xC;
                tries++;
            }

            if(fitness >= 20){
                fitness = 0;
            }
        }

        void mutate(int chance){
            for(auto &inst: _instructionset){
                if(mutationSelector(mt) <= chance){
                    inst ^= state_selector(mt);
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
    
};


TouringMachine tournamentSelect(std::vector<TouringMachine> population, int n_select){
    std::uniform_int_distribution<int32_t> selector(0,population.size()-1);
    std::vector<TouringMachine> select_parents;

    for(auto i = n_select-1; i>=0; i--){
        select_parents.push_back(population[selector(mt)]);
    }

    std::sort(select_parents.begin(), select_parents.end(),[](TouringMachine & one, TouringMachine & two){return one.getFitness() > two.getFitness();});

    return select_parents[0];
}

unsigned int countSetBits(unsigned int n)
{
    unsigned int count = 0;
    while (n) {
        count += n & 1;
        n >>= 1;
    }
    return count;
}

int shiftBit(int bit, int amount){
    int last_elem;
    for(auto i= 0; i < amount; i++){
        last_elem = bit & 0x1;
        if(last_elem==1){
            bit = bit>>1;
            bit |= last_elem<<3;
        }  
    }
    
}

void crossover(TouringMachine& p1, TouringMachine& p2, TouringMachine& c1, TouringMachine& c2){
    std::vector<int>child_1 = p1.getInstructionSet();
    std::vector<int>child_2 = p2.getInstructionSet();

    int child_1_value, child_2_value, shift_1, shift_2;

    for(auto i = 0; i< child_1.size(); i++){
        child_1_value =  child_1[i] & 0xF;
        child_2_value =  child_2[i] & 0xF;

        shift_1 = countSetBits(child_1_value);
        shift_2 = countSetBits(child_2_value);

        child_1_value = shiftBit(child_1_value, shift_1);
        child_2_value = shiftBit(child_2_value, shift_2);

        child_1[i] = (child_1[i] & 0x3F0) | child_1_value;
        child_2[i] = (child_2[i] & 0x3F0) | child_2_value;
    }

    c1 = TouringMachine(child_1);
    c2 = TouringMachine(child_2);
}

bool validateInstruktionSet(std::vector<int>& vec){
    bool has_endstep = false;

    for(size_t i = 0; i < vec.size(); i++ ) {
        if((((vec[i]>>9)) & 0x1) == 0)
            return false;

        if((((vec[i]>>6)) & 0x1) == 1)
            return false;  

        if(((vec[i] & 0xF) > 0xC))
            vec[i]  &= 0x3FB;

        if((vec[i] & 0xF) == 0xC)
            has_endstep = true;
    }

    return true && has_endstep;
}

int main(){
    
    std::vector<TouringMachine> population(100);
    std::vector<TouringMachine> childs(100);
    std::vector<TouringMachine> elite(6);

    for(auto &pop : population){
        pop.run(tape);
        std::cout << pop.getFitness() << std::endl; 
        std::sort(population.begin(), population.end(),[](TouringMachine & one, TouringMachine & two){return one.getFitness() > two.getFitness();});
    }

    std::cout <<"\n" << (population[0].getFitness()) <<"\n"<<std::endl;

    while(true){
        

        for(auto &pop : population){
            pop.mutate(30); 
        }

        for(auto &pop : population){
            pop.run(tape);
            std::cout << pop.getFitness() << std::endl; 
            std::sort(population.begin(), population.end(),[](TouringMachine & one, TouringMachine & two){return one.getFitness() > two.getFitness();});

        }

        std::cout <<"\n" << (population[0].getFitness())<<std::endl;

        break;
    }


    return 0;
}