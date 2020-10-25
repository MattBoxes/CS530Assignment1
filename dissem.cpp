// CS 570 Fall 2020 
// Assignment 1
// Mathew Punsalan 

#include "dissem.h"
#include "OpCode.h"
using namespace std;

// need opcode class to store data structure provided
// store opcodes into data structure
int main(int argc, char const *argv[])
{
    if(argc != 2){
        cout << "Need 2 file arguments" << endl;
        exit(1);
    }
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