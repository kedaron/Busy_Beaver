#include <iostream>
#include <vector>
#include <random>
#include<bitset>

std::mt19937 mt(time(nullptr)); 
std::uniform_int_distribution<int> card_selector(0, 3);
std::uniform_int_distribution<int> state_selector(0, 12);

//All possible instructions for a card
uint8_t head_zero[4] =  {0b000, 0b001, 0b010, 0b011};
uint8_t head_one[4] =  {0b100, 0b101, 0b110, 0b111};

//Tape that we reuse
std::vector<int> tape(2048, 0);

/*class TouringMachine{
    private:
        //The first card is always the start state
        //The last card is always the end state
        uint16_t _cards[13];
        vector<int> &_stripe;
        
    public:
        TouringMachine(*stripe, uint16_t cards[13]){
            _stripe = stripe;
            _cards = cards;
        }
    
}*/


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

    
    return cards;
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

    /*for(auto it = std::begin(test); it != std::end(test); ++it) {
        std::cout << *it << "\n";
    }*/

    std::cout << "Blub" << std::endl;

    return 0;
}