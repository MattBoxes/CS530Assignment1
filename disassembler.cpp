// CS 570 Fall 2020 
// Assignment 1
// Mathew Punsalan 

#include "disassembler.h"
#include "OpCode.h"
using namespace std;

// need opcode class to store data structure provided
// store opcodes into data structure
int main(int argc, char const *argv[])
{
    if(argc != 2 ){
        cout << "Incorrect Usage: Need two file " << endl;
        cout << "Correct Usage: dissem <filename>.obj <filename>.sys" << endl;
        exit(1);
    }
    cout << "two files successfully loaded in!" << endl;
    string inFile = argv[1];
    int extPosition = inFile.find_last_of(".");
    if(inFile.substr(extPosition+1) != "obj") {
        cout << "Wrong file extension!" << endl;
        exit(1);
    }
    ifstream objFile(argv[1], ios::in);
    if(!objFile) {
        cout << ".obj File not found" << endl;
        exit(1);
    }
    if(objFile.peek() == istream::traits_type::eof()) {
        cout <<"File "<< inFile <<" is empty" << endl;
        exit(1);
    }
    string fileName = inFile.substr(0,extPosition);
    ifstream symFile((fileName + ".sym").c_str(), ios::in);
    if(!symFile) {
        cout << "Sym File not found" << endl;
        exit(1);
    }
    if(symFile.peek() == istream::traits_type::eof()) {
        cout << "File is empty!" << endl;
        exit(1);
    }
    ofstream outFile((fileName + ".sic").c_str());
   
   
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