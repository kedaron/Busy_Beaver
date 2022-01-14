#include <iostream>
#include <vector>

std::random_device r;
std::default_random_engine rand_eng(r());
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


std::vector<int< genarateInstructionSet(int amount_of_cards){
    std::vector<int> cards(amount_of_cards);
    int state;

    for(auto i = 0; i<amount_of_cards; i++){
        state = state_selector(rand_eng);
        cards[i] = (head_zero[card_selector(rand_eng)]<<7) | (head_one[card_selector(rand_eng)]<<4) | state;
    }

    
    return cards;
}

int main(){
 
    std::cout << "Blub" << std::endl;
    return 0;
}