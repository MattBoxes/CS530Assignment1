// CS 570 Fall 2020 
// Assignment 1
// Mathew Punsalan 

#include "disassembler.h"
using namespace std;

// need opcode class to store data structure provided
// store opcodes into data structure
int main(int argc, char const *argv[])
{
    if(argc != 2){
        cout << "Incorrect Usage: Need two  file arguments " << endl;
        cout << "Correct Usage: dissem <filename>.obj <filename>.sys" << endl;
        exit(1);
    }
    cout << "two files successfully loaded in!" << endl;
    //TODO:
    // open files and check for correct format using ifstream, exit if not
    // write file contents to vector data structure
    // loop through checking opcodes in structure by format Header
    // add to outstream structure for writing out

}

//opcode class
// public:
    //getOpName for parsing given data structure
    //getOpFormat for returning format for assembler to interpret using different functions for format 1,2,3,4