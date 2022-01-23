//#include "TouringMachine.h";
#include "Timing.h"
#include "BusyBeaver.h"
#include <iostream>

int states = 2;
int population_int = 100;
bool measure = false;
int end_condition;

void show_usage(const std::string& name);

int main(int argc,  char* argv[]){
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if ((arg == "-h") || (arg == "--help")) {
            show_usage(argv[0]);
            return 1;
        } else if ((arg == "-st") || (arg == "--states")) {
            if (i + 1 < argc) {
                states = atoi(argv[i+1]);
            } else { 
                std::cerr << "--states option requires one argument." << std::endl;
                return 1;
            }
        }else if ((arg == "-p") || (arg == "--population")) {
            if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                population_int =  atoi(argv[i+1]);
            } else { // Uh-oh, there was no argument to the destination option.
                std::cerr << "--population option requires one argument." << std::endl;
                return 1;
            }
        }else if ((arg == "-m") || (arg == "--measure")) {
            if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                measure = true;
            } 
        }
    }

    switch (states)
    {
    case 1:
        end_condition = 1;
        break;
    case 2:
        end_condition = 4;
        break;
    case 3:
        end_condition = 6;
        break;
    case 4:
        end_condition = 13;
        break;


    default:
        std::cerr << "States bigger than 4 are not implemented" << std::endl;
        return 1;
    }

    //Init Timing and GameOfLife Instance
    Timing* timing = Timing::getInstance();

   //Start setup time measurement if the --measure flag is set
    if(measure)
       timing->startSetup();

    // int _population_int, int _threads, int _end_condition)
   //Load the file into a vector<vector<char>> and store width and height of it
    BusyBeaver beaver(states, population_int, end_condition);
    beaver.setup();

    //Stop setup time measurement if the --measure flag is set
    if(measure)
        timing->stopSetup();

    //Start computation time measurement if the --measure flag is set
    if(measure)
        timing->startComputation();

    beaver.compute();


    //Stop computation time measurement if the --measure flag is set
    if(measure)
        timing->stopComputation();

    //Start finalization time measurement if the --measure flag is set
    if(measure)
        timing->startFinalization();

    beaver.finalize();

    //Stop finalization time measurement if the --measure flag is set
    if(measure)
        timing->stopFinalization();

    //Output the timing results if the --measure flag is set
    if(measure) {
        std::cout << timing->getResults() << std::endl;
    }

   return 0;
}


void show_usage(const std::string& name)
{
    std::cerr << "\nUsage: " << name
              << "\nOptions:\n"
              << "\t-h,  --help\t\tShow this help message\n"
              << "\t-p,  --population\tSpecify the population Size (Optional, default=100)\n"
              << "\t-st, --states\t\tHow many states should be solved (Optional, default=2, n>=2 && n<5) \n"
              << "\t-m,  --measure\t\tSpecify if execution should be timed (Optional, default=false)\n"
              << std::endl;
}