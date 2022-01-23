# Busy_Beaver

## Idea
### Setup
The tape is initialzied  with a size of 2048 and every Touring Machine uses the same tape.

### Crossover
The general idea is to generate 10bit long instruction sets.<br>
These instruction sets can be easily crossovered by swapping segments between two parents <br>

### Mutation
After the crossover there is a chance that the child mutates one random bit in the instruction set.
If no improvement is seen in the result the chance is increased up to 100% and it resets to the default value if the average of the elite improves.

### Validation
Because we have 13 States a validator is needed. The following things have to be checked:

* are the last 4 bits <= 1100 (12)
* is the first bit a 1
* is the 3 bit a 0
* atleast one instruction in the set has to contain 1100 (12) as the next step 

If one of the results is negative we can skip the touring path and let the child die.<br>
On the other hand, if the validator checks are positive the touring path beginns.

#### Example

    // crossover
    parent_1 = 0b0111000100
    parent_2 = 0b0101110110
    
    child_1 = 0b0111110110
    child_2 = 0b0101000100
    
    //Mutation
    child_2 = 0b0111000100


### The Touring Path

### Found Solutions:
#### N = 2
{0b1100110001, 0b1000011010} <br>
{0b1100110001, 0b1000011111} <br>
{0b1000111001, 0b1100010111} <br>
{0b1100100100, 0b1110101100} <br>

#### N = 3
{0b1000111010, 0b1001010111, 0b1100001001} <br>
{0b1000110000, 0b1101011011, 0b1000011001} <br>
{0b1100111000, 0b1001011011, 0b1100010001} <br>
{0b1001010000, 0b0000011010, 0b1100101001} <br>

#### N = 4
{0b1001100110, 0b1101000010, 0b1001101001, 0b0101010010} <br>
{0b1101100011, 0b0000001000, 0b0100000010, 0b1001011000} <br>
{0b1000101101, 0b0001101001, 0b1100110011, 0b1001010001} <br>
{0b1001001001, 0b0000111000, 0b1100110010, 0b0001110000} <br>
