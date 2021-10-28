/* 
 * Code developed for the Team DIANA Recruitment
 * Leonardo Palmucci, October 2021
 */

#include <iostream>
#include <fstream>
#include <vector>
#define FIELD_FILE "./field.txt"
#define ROVERS_FILE "./rovers.txt"
#define OUTPUT_FILE "./results.txt"
using namespace std;

class Stone {
    // for the sake of simplicity, all attributes are public 
    // to avoid making the code unnecessarily cumbersome
public:
    string code;
    double quality;
    unsigned int rocks_found;
    Stone(){} //never used
    Stone(string code, double quality){
        this->code = code; 
        this->quality = quality;
        this->rocks_found = 0;
    }
};

class Rover {
    // for the sake of simplicity, all attributes are public 
    // to avoid making the code unnecessarily cumbersome
public:
    string name;
    int id;
    unsigned int total_rocks_found;
    vector<Stone> stone_vector; // stores all the stones for a rover
    Rover(){} // never used
    Rover(string name, int id, int n_stonecodes){
        this->name = name;
        this->id = id;
        this->total_rocks_found = 0;
        this->stone_vector = vector<Stone>(n_stonecodes);
    }
};

int main(){
    unsigned long n_rovers; // stoul returns an unsigned long (see below)
    int rover_id, n_stonecodes;
    double stone_quality;
    string line, rover_name, stone_code;
    vector<Rover> rover_vector;
    // read the rovers.txt file
    fstream file;
    file.open(ROVERS_FILE, ios::in);
    if(!file.is_open()) {
        return EXIT_FAILURE;
    } 
    // read the first line of rovers.txt: extract n_rovers
    if(!getline(file, line)){
        file.close();
        return EXIT_FAILURE;
    }
    // a bit safer than using stream operators, in this case
    n_rovers = stoul(line);
    // we assume that, from now on, there are no syntax errors
    // in the rovers.txt file
    rover_vector = vector<Rover>(n_rovers);
    for (unsigned long i = 0; i < n_rovers; i++) {
        file >> rover_name >> rover_id >> n_stonecodes;
        rover_vector[i] = Rover(rover_name, rover_id, n_stonecodes);
        for (int j = 0; j < n_stonecodes; j++) {
            file >> stone_code >> stone_quality;
            rover_vector[i].stone_vector[j] = Stone(stone_code, stone_quality);
        }
    }
    file.close();
    // read the field.txt file
    file.open(FIELD_FILE, ios::in);
    if(!file.is_open()) {
        return EXIT_FAILURE;
    } 
    /* these variables are used to traverse lines, keeping track
     * of where we are:
     * - pos is the index where we should read from
     * - match_pos is the point where a rock has been found
     */
    int match_pos, pos, linenum = 0;
    /* for each line, we try to look for all the rocks, one after the other
     * Assumption: it is not possible to find substrings like
     * FR-MA-GU, i.e. rocks sharing some letters
     * If this assumption is false, the file is ambiguous, hence ill-formatted
     * */
    while(getline(file,line)){
        for (unsigned long i = 0; i < n_rovers; i++){
            for (int j = 0; j < rover_vector[i].stone_vector.size(); j++){
                pos = 0;
                stone_code = rover_vector[i].stone_vector[j].code;
                // loop until no more rocks of that type are found
                match_pos = line.find(stone_code, pos);
                while(match_pos != -1){
                    // the rover has found one rock
                    rover_vector[i].total_rocks_found++;
                    // the rover has found one rock of this type
                    rover_vector[i].stone_vector[j].rocks_found++;
                    // from now on, we should look for the rock only in the rest of the string
                    pos = match_pos + stone_code.length();
                    match_pos = line.find(stone_code, pos);
                }
            }
        }
        linenum++;
    }
    file.close();
    // create the output file
    file.open(OUTPUT_FILE, ios::out);
    if(!file.is_open()) {
        return EXIT_FAILURE;
    } 
    file << n_rovers << endl;
    for(unsigned long i = 0; i < n_rovers; i++) {
        file << rover_vector[i].name << " ";
        file << rover_vector[i].id << " ";
        file << rover_vector[i].total_rocks_found << endl;
        // for each rock, print its quality as many times as the number of rocks
        // that we have found of that particular type
        for(int j = 0; j < rover_vector[i].stone_vector.size(); j++) {
            for(int k = 0; k < rover_vector[i].stone_vector[j].rocks_found; k++){
                file << rover_vector[i].stone_vector[j].quality << " ";
            }
        }
        file << endl;
    }
    file.close();
    return 0;
}