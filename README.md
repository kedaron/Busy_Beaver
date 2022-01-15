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
