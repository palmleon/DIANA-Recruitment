#include <iostream>
#include <fstream>
#include <vector>
#define FIELD_FILE "./field.txt"
#define ROVERS_FILE "./rovers.txt"
using namespace std;

class Stone {
public:
    string code;
    float quality;
    Stone(){}
    Stone(string code, float quality){
        this->code = code; 
        this->quality = quality;
    }
};

class Rover {
public:
    string name;
    int id;
    vector<Stone> stone_vector; // stores all the stones for a rover
    Rover(){}
    Rover(string name, int id, int n_stonecodes){
        this->name = name;
        this->id = id;
        this->stone_vector = vector<Stone>(n_stonecodes);
    }
};

int main(){
    int n_rovers, rover_id, n_stonecodes;
    float stone_quality;
    string line, rover_name, stone_code;
    vector<Rover> rover_vector;
    // read the rovers.txt file
    fstream file_in;
    file_in.open(ROVERS_FILE, ios::in);
    if(!file_in.is_open()) {
        file_in.close();
        return EXIT_FAILURE;
    } 
    // read the first line of rovers.txt: extract n_rovers
    if(!getline(file_in, line)){
        file_in.close();
        return EXIT_FAILURE;
    }
    n_rovers = stoul(line);
    // we assume that, from now on, there are no syntax errors
    // in the rovers.txt file
    rover_vector = vector<Rover>(n_rovers);
    for (int i = 0; i < n_rovers; i++) {
        file_in >> rover_name >> rover_id >> n_stonecodes;
        Rover rover = Rover(rover_name, rover_id, n_stonecodes);
        rover_vector[i] = rover;
        for (int j = 0; j < n_stonecodes; j++) {
            file_in >> stone_code >> stone_quality;
        }
        
    }
    


    file_in.close();
    // read the field.txt file
    return 0;
}