// CS 570 Fall 2020 
// Assignment 1
// Mathew Punsalan 
// James Nguyen
// Jonothan Giang

#include "disassembler.h"
#include "OpCode.h"
using namespace std;
ifstream object_Stream;
ifstream sym_Stream;
ofstream sic_Stream;
ofstream lis_Stream;
#include <vector>

void Dissem::load_Data(){
    string file_Line;

    while(object_Stream.good()){
        getline(object_Stream, file_Line);
        object_Storage.push_back(file_Line);
    }

    while (sym_Stream.good()){
        getline(sym_Stream, file_Line);
        sym_Storage.push_back(file_Line);
    }

    int i = 2;
    for(i = 2; i < sym_Storage.size() - 1; i++){
         if(sym_Storage[i][0] != (char)NULL){
            sym_Name.push_back(sym_Storage[i].substr(0,6));
            sym_Value.push_back((unsigned int)strtol(sym_Storage[i].substr(8,6).c_str(), NULL, 16));
            sym_Flag.push_back((unsigned int)strtol(sym_Storage[i].substr(16,1).c_str(), NULL, 16));
         }
         else{
            i += 3;
            break;
         }
    }

    for(int j = i; j < sym_Storage.size() - 1; j++){
        literal_Name.push_back(sym_Storage[j].substr(8,6));
        literal_Length.push_back((int)strtol(sym_Storage[i].substr(19,1).c_str(), NULL, 16));
        literal_Address.push_back((unsigned int)strtol(sym_Storage[i].substr(24,1).c_str(), NULL, 16));
    }
}
// need opcode class to store data structure provided
// store opcodes into data structure
int main(int argc, char *argv[])
{   
    string inFile1;
    string inFile2;
    int inFile1Length;
    int inFile2Length;

    // check for 3 needed arguments (1)dissem (2)<filename>.obj (3)<filename>.sym
    if(argc != 3 ){
        cout << "Incorrect Usage: Need two files" << endl;
        cout << "Correct Usage: dissem <filename>.obj <filename>.sym" << endl;
        exit(1);
    }
    
    // infiles coming from sameline arguments
    // argv stores pointers to strings passed on the commandline argv[0] is this main process
    inFile1 = argv[1];
    inFile2 = argv[2];

    // find . in string name for checking if obj or sym file
    inFile1Length = inFile1.find_last_of(".");
    inFile2Length = inFile2.find_last_of(".");

    // check for correct file types
    if(inFile1.substr(inFile1Length+1) != "obj") {
        cout << "Incorrect Usage: First file must be of type 'obj'" << endl;
        cout << "Correct Usage: dissem <filename>.obj <filename>.sym" << endl;
        exit(1);
    }
    if(inFile2.substr(inFile2Length+1) != "sym") {
        cout << "Incorrect Usage: Second file must be of type 'sym'" << endl;
        cout << "Correct Usage: dissem <filename>.obj <filename>.sym" << endl;
        exit(1);
    }   

    // load files and check if empty or nonexistent
    ifstream objFile(argv[1], ios::in);
    ifstream symFile(argv[2], ios::in);

    // obj check
    if(!objFile) {
        cout << "Incorrect Usage: obj file not found" << endl;
        cout << "Correct Usage: dissem <filename>.obj <filename>.sym" << endl;
        exit(1);
    }
    if(objFile.peek() == istream::traits_type::eof()) {
        cout <<"File "<< inFile1 <<" is empty" << endl << "Exiting" << endl;
        exit(1);
    }

    // sym check
    if(!symFile) {
        cout << "Incorrect Usage: sym file not found" << endl;
        cout << "Correct Usage: dissem <filename>.obj <filename>.sym" << endl;
        exit(1);
    }
    
    if(symFile.peek() == istream::traits_type::eof()) {
        cout << "File "<< inFile2 <<" is empty" << endl << "Exiting" << endl;;
        exit(1);
    }

    cout << inFile1 << " and " << inFile2 << " succesfully loaded" << endl;

    //TODO remove .sym from inFile2 and append .sic correctly
    //at this point all error checking for files is done, can read and write now

    ofstream outFile((inFile2 + ".sic").c_str());
   
   
    //TODO:
    // open files and check for correct format using ifstream, exit if not
    // write file contents to vector data structure
    // loop through checking opcodes in structure by format Header
    // add to outstream structure for writing out

}


void Dissem::open_File(char *object_File){
    object_Stream.open(object_File);
    if(!object_Stream.is_open()){
        cout << " '.obj' file not found" << endl;
        exit(EXIT_FAILURE);
    }

    string sym_File = object_File;
    sym_File.erase(sym_File.find_last_of('.'));
    sym_File.append(".sym");
    sym_Stream.open(sym_File.c_str());
    if(!sym_Stream.is_open()){
        cout << " '.sym' file not found" << endl;
        exit(EXIT_FAILURE);
    }

    string sic_File = object_File;
    sic_File.erase(sym_File.find_last_of("."));
    sic_File.append(".sic");
    sic_Stream.open(sic_File.c_str());

    string lis_File = object_File;
    lis_File.erase(lis_File.find_last_of("."));
    lis_File.append(".lis");
    lis_Stream.open(lis_File.c_str());

    load_Data();

    object_Stream.close();
    sym_Stream.close();
}


//opcode class
// public:
    //getOpName for parsing given data structure
    //getOpFormat for returning format for assembler to interpret using different functions for format 1,2,3,4