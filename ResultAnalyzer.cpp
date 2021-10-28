/* 
 * Code developed for the Team DIANA Recruitment
 * Leonardo Palmucci, October 2021
 */

#include <iostream>
#include <fstream>
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
    cout << "Type a command here" << endl;
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
    file.open(INPUT_FILE);
    if(!file.is_open()){
        return EXIT_FAILURE;
    }
    if(!getline(file, line)){
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
    // loop that manages the menu inquiry, until the terminating command is received
    unordered_map<string, command_type> conversion_table = { {"list", LIST}, {"show", SHOW}, {"best", BEST}, {"end", END} };
    // the command provided by the user
    string command;
    // the corresponding Enum value
    command_type command_value;
    do {
        print_menu();
        cin >> command;
        if(conversion_table.count(command) > 0) {
            command_value = conversion_table[command];
        }
        else command_value = ERROR;
        switch (command_value) {
            case LIST: 
                // TODO
                break;
            case SHOW:
                // TODO
                break;
            case BEST:
                // TODO
                break;
            case END:
                cout << "It's been a pleasure. Bye!" << endl;
                break;
            default:    
                cout << "Command not recognized. Retry." << endl;
                break;
        }

    } while (command_value != END);

    return 0;
}
