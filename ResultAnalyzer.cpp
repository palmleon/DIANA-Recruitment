/*
 * The following program implements the second part of the Probe Tool.
 * In particular, it reads the results stored in "results.txt" and makes 
 * it possible to read them via a command line interface
 * The command line interface allows to:
 * - list all information about the rovers
 * - show data about a particular rover
 * - display data about the most profitable rover
 * - terminate the program (explicit termination)
 * When the program is launched, a user-friendly interface illustrates
 * what the Tool is capable of doing, making it more usable
 * 
 * Code developed for the Team DIANA Recruitment
 * Leonardo Palmucci, October 2021
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#define INPUT_FILE "./results.txt"
using namespace std;

enum command_type {
    LIST, SHOW, BEST, END, ERROR
};

class rover{
    public:
    string name;
    int id, n_stones_collected;
    vector<double> stones;
    rover(){} // never used
    rover(string name, int id, int n_stones_collected){
        this->name = name;
        this->id = id;
        this->n_stones_collected = n_stones_collected;
        this->stones = vector<double>(n_stones_collected);
    }
};

// function that prints a user-friendly menu, displaying all possible options
void print_menu(){
    cout << "--------------------------------------------" << endl;
    cout << "TYPE ONE OF THE FOLLOWING COMMANDS:" << endl;
    cout << "list: list number of rovers, their names, ids and number of stones collected " << endl;
    cout << "show <rover_name>: show name, id and stone data of a given rover" << endl;
    cout << "best: show name, id, stone data and avg stone quality of the most proficient rover" << endl;
    cout << "end: close the program" << endl;
    cout << "--------------------------------------------" << endl;
}

int main(){
    ifstream file;
    // data structure holding all rovers
    vector<rover> rovers;

    // the rover with the highest avg quality is evaluated immediately
    // as the input file is never reloaded
    rover best_rover; 
    string line, name;
    int id, n_stones_collected;
    double quality, best_avg_quality = 0;
    unsigned long n_rovers; // stoul returns an unsigned long  (see below)

    // read input file
    file.open(INPUT_FILE);
    if(!file.is_open()){
        cout << "ERROR: File " << INPUT_FILE << " not opened correctly!" << endl;
        return EXIT_FAILURE;
    }
    if(!getline(file, line)){
        cout << "ERROR: Could not read lines from file properly" << endl;
        file.close();
        return EXIT_FAILURE;
    }
    n_rovers = stoul(line);
    rovers = vector<rover>(n_rovers);

    // build the vector of rovers; for each of them:
    // - retrieve the name, the id, the number of stones collected
    // - store the quality of all the collected rocks
    for (unsigned long i = 0; i < n_rovers; i++){
        file >> name >> id >> n_stones_collected;
        rovers[i] = rover(name, id, n_stones_collected);
        double avg_quality, sum_quality = 0;
        for (int j = 0; j < n_stones_collected; j++){
            file >> quality;
            rovers[i].stones[j] = quality;
            sum_quality += quality;
        }
        avg_quality = sum_quality / n_stones_collected;
        if(avg_quality >= best_avg_quality){
            best_rover = rovers[i];
            best_avg_quality = avg_quality;
        }
    }
    file.close();
    unordered_map<string, command_type> conversion_table = { {"list", LIST}, {"show", SHOW}, {"best", BEST}, {"end", END} };
    // the command provided by the user
    string command_string;
    // stringstream containing the command (used to read command line args)
    stringstream command_stream;
    // the actual command (the first command line argument, extracted from command_stream)
    string command;
    // the corresponding Enum value
    command_type command_value;
    // string used to read the other command line arguments (especially for SHOW)
    string command_line_arg;
    // flag used by the SHOW command to recognize if the rover to show exists
    bool rover_found = false;
    // loop that manages the menu inquiry, until the terminating command is received
    print_menu();
    do {
        getline(cin, command_string);
        command_stream = stringstream(command_string);
        command_stream >> command;
        // check if the command is among the supported ones
        if(conversion_table.count(command) > 0) {
            command_value = conversion_table[command];
        }
        else command_value = ERROR;
        switch (command_value) {
            case LIST: 
                if (command_stream >> command_line_arg) 
                    cout << "ERROR: Too many arguments" << endl;
                else {
                    cout << "Number of Rovers: " << n_rovers << endl;
                    for (int i = 0; i < n_rovers; i++){
                        cout << "Name: " << rovers[i].name << "\t";
                        cout << "Id: " << rovers[i].id << "\t";
                        cout << "Stones: " << rovers[i].n_stones_collected << endl;
                    }
                }
                break;
            case SHOW:
                rover_found = false;
                if (! (command_stream >> command_line_arg)) //no args
                    cout << "ERROR: Missing argument" << endl;
                else if (command_stream >> command_line_arg) // n_args > 1
                    cout << "ERROR: Too many arguments" << endl;
                else {
                    for(int i = 0; i < n_rovers && !rover_found; i++) {
                        // check if there exist a rover with such a name
                        if(rovers[i].name.compare(command_line_arg) == 0) {
                            cout << "You have selected: " << endl;
                            cout << "Name: " << rovers[i].name << "\t";
                            cout << "Id: " << rovers[i].id << endl;
                            cout << "Stone Qualities: ";
                            for(int j = 0; j < rovers[i].n_stones_collected; j++)
                                cout << rovers[i].stones[j] << " ";
                            cout << endl;
                            rover_found = true; 
                        }
                    }
                    if (!rover_found) {
                        cout << "ERROR: This Rover does not exist!" << endl;
                    }
                }
                break;
            case BEST:
                if (command_stream >> command_line_arg)
                    cout << "ERROR: Too many arguments" << endl;
                else {
                    cout << "Name: " << best_rover.name << "\t";
                    cout << "Id: " << best_rover.id << endl;
                    cout << "Stone Qualities: ";
                    for(int i = 0; i < best_rover.n_stones_collected; i++)
                        cout << best_rover.stones[i] << " ";
                    cout << endl;
                    cout << "Average Stone Quality: " << best_avg_quality << endl;
                }
                break;
            case END:
                cout << "It's been a pleasure. Bye!" << endl;
                break;
            default:    
                cout << "Command not recognized. Retry." << endl;
                break;
        }
        // print newlines to make output more readable
        if (command_value != END)
            cout << "--------------------------------------------" << endl;
            
    } while (command_value != END);

    return EXIT_SUCCESS;
}
