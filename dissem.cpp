// CS 570 Fall 2020
// Assignment 1
// Mathew Punsalan
// James Nguyen
// Jonothan Giang

#include "dissem.h"
#include "OpCode.h"
using namespace std;


int main(int argc, char *argv[])
{
    string inFile1;
    string inFile2;
    int inFile1Length;
    int inFile2Length;

    // check for 3 needed arguments (1)dissem (2)<filename>.obj (3)<filename>.sym
    if (argc != 3)
    {
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
    if (inFile1.substr(inFile1Length + 1) != "obj")
    {
        cout << "Incorrect Usage: First file must be of type 'obj'" << endl;
        cout << "Correct Usage: dissem <filename>.obj <filename>.sym" << endl;
        exit(1);
    }
    if (inFile2.substr(inFile2Length + 1) != "sym")
    {
        cout << "Incorrect Usage: Second file must be of type 'sym'" << endl;
        cout << "Correct Usage: dissem <filename>.obj <filename>.sym" << endl;
        exit(1);
    }

    // load files and check if empty or nonexistent
    ifstream objFile(argv[1], ios::in);
    ifstream symFile(argv[2], ios::in);

    // check for .obj file
    if (!objFile)
    {
        cout << "Incorrect Usage: obj file not found" << endl;
        cout << "Correct Usage: dissem <filename>.obj <filename>.sym" << endl;
        exit(1);
    }
//check if .obj file is empty
    if (objFile.peek() == istream::traits_type::eof())
    {
        cout << "File " << inFile1 << " is empty" << endl
             << "Exiting" << endl;
        exit(1);
    }

    // check for .sym file
    if (!symFile)
    {
        cout << "Incorrect Usage: sym file not found" << endl;
        cout << "Correct Usage: dissem <filename>.obj <filename>.sym" << endl;
        exit(1);
    }
    // check if .sym file is empty
    if (symFile.peek() == istream::traits_type::eof())
    {
        cout << "File " << inFile2 << " is empty" << endl
             << "Exiting" << endl;
        ;
        exit(1);
    }

    // .obj and .sym file found, disassemble
    cout << inFile1 << " and " << inFile2 << " succesfully loaded" << endl;
    Dissem *dissassembler = new Dissem;
    dissassembler->open_File(argv[1], argv[2]);
    dissassembler->disassemble();
    cout << ".sic and .lis created" << endl;
}