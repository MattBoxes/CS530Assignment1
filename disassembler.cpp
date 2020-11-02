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


// function for reading files and storing code into a symbol table and object code table
void Dissem::load_Data()
{

    string file_Line;
    // get obj file line by line and store into vector
    while (object_Stream.good())
    {
        getline(object_Stream, file_Line);
        object_Storage.push_back(file_Line);
    }
    // get sym file line by line and store into vector
    while (sym_Stream.good())
    {
        getline(sym_Stream, file_Line);
        symbol_Storage.push_back(file_Line);
    }

    //loop through stored symbols and store each value,name, and flag for format into a vector
    int i = 2;
    for (i = 2; i < symbol_Storage.size() - 1; i++)
    {   
        // while symbols stored is not null push into vector
        if (symbol_Storage[i][0] != (char)NULL)
        {
            symbol_Name.push_back(symbol_Storage[i].substr(0, 6));
            symbol_Value.push_back((unsigned int)strtol(symbol_Storage[i].substr(8, 6).c_str(), NULL, 16));
            symbol_Flag.push_back((unsigned int)strtol(symbol_Storage[i].substr(16, 1).c_str(), NULL, 16));
        }

        // else move ahead 3 indexes since symbols are stored in vector as 3 strings for name,value, and format flag
        else
        {
            i += 3;
            break;
        }

    }
    // loop through stored symbols and check for literals and push into stored literals vector
    for (int j = i; j < symbol_Storage.size() - 1; j++)
    {
        literal_Name.push_back(symbol_Storage[j].substr(8, 6));
        literal_Length.push_back((int)strtol(symbol_Storage[i].substr(19, 1).c_str(), NULL, 16));
        literal_Address.push_back((unsigned int)strtol(symbol_Storage[i].substr(24, 1).c_str(), NULL, 16));
    }
}


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

//helper function for converting and Opcode and value to its mneomonic 
string Dissem::op_Name(OpCode code, int opCode){
    string operand_Name = code.getOpName(opCode);
    return operand_Name;
}

// helper function to check for symbol table values when checking formats 1-4 and pushing to output stream
void Dissem::isSymbol(string op_Name){
    for(int i = 0; i < symbol_Value.size() - 1; i++){
        // if current address is in the symbol table push symbol to output stream
        if(current_Address == symbol_Value[i]){
            sic_Stream << setw(8) << left << symbol_Name[i];
            lis_Stream << setw(8) << left << symbol_Name[i];
            break;
        }
        // else fill with space
        else if(i + 1 >= symbol_Value.size() - 1){
            sic_Stream << " " << setw(7) << left << op_Name;
            lis_Stream << " " << setw(7) << left << op_Name;
        }
    }
}

bool Dissem::isLiteral(){
    for(int i = 0; i < literal_Name.size(); i++){
        if(current_Address == literal_Addresses[i]){
            sic_Stream << setw(10) << left << literal_Name[i] << endl;
            sic_Stream << setw(14) << right << "LTORG" << endl;
            lis_Stream << setw(10) << left << literal_Name[i] << endl;
            lis_Stream << setw(14) << right << "LTORG" << endl;
            return 1;
        }
    }
    return 0;
}
//Open files from cmd line arguments, store data, create output files
void Dissem::open_File(char *object_File, char *sym_File)
{
    // open files into a stream for easier manipulation
    object_Stream.open(object_File);
    sym_Stream.open(sym_File);

    // create sic and lis files
    string sic_File = object_File;
    sic_File.erase(sic_File.find_last_of("."));
    sic_File.append(".sic");
    sic_Stream.open(sic_File.c_str());

    string lis_File = object_File;
    lis_File.erase(lis_File.find_last_of("."));
    lis_File.append(".lis");
    lis_Stream.open(lis_File.c_str());

    //store file contents into vector for manipulation
    load_Data();

    object_Stream.close();
    sym_Stream.close();
}

// analyze an opcode for format 1 and store contents 
void Dissem::check_Format_1(string op_Name, int row, int current){

    isSymbol(op_Name);
    if(isLiteral()){return;}

}


void Dissem::check_Format_2(string op_Name, int row, int current){

    isSymbol(op_Name);
    if(isLiteral()){return;}

    int register_One = (int)strtol(object_Storage[row].substr(current + 2, 1).c_str(), NULL, 16);
    int register_Two = (int)strtol(object_Storage[row].substr(current + 3, 1).c_str(), NULL, 16);

    switch (register_One) {           //output register name for first register operand
        case 0:
            sic_Stream << "A,";
            lis_Stream << "A,";
            break;
        case 1:
            sic_Stream << "X,";
            lis_Stream << "X,";
            break;
        case 2:
            sic_Stream << "L,";
            lis_Stream << "L,";
            break;
        case 3:
            sic_Stream << "B,";
            lis_Stream << "B,";
            break;
        case 4:
            sic_Stream << "S,";
            lis_Stream << "S,";
            break;
        case 5:
            sic_Stream << "T,";
            lis_Stream << "T,";
            break;
        case 6:
            sic_Stream << "F,";
            lis_Stream << "F,";
            break;
        default:
            break;
    }

    switch (register_Two) {           //output register name for 2nd operand
        case 0:
            sic_Stream << "A" << endl;
            lis_Stream << "A" << endl;
            break;
        case 1:
            sic_Stream << "X" << endl;
            lis_Stream << "X" << endl;
            break;
        case 2:
            sic_Stream << "L" << endl;
            lis_Stream << "L" << endl;
            break;
        case 3:
            sic_Stream << "B" << endl;
            lis_Stream << "B" << endl;
            break;
        case 4:
            sic_Stream << "S" << endl;
            lis_Stream << "S" << endl;
            break;
        case 5:
            sic_Stream << "T" << endl;
            lis_Stream << "T" << endl;
            break;
        case 6:
            sic_Stream << "F" << endl;
            lis_Stream << "F" << endl;
            break;
        default:
            break;
    }
}

int Dissem::check_Format_3_4(string op_Name, OpCode code, int row, int current){
    //string op_Name = code.getOpName(opCode);
    bool nixbpe[6];
    int flag_Section = (int)strtol(object_Storage[row].substr(current + 1, 2).c_str(), NULL, 16);

    for(int i = 0; i < 6; i++){
        nixbpe[i] = code.getBit(flag_Section, 5 - i);
    }

    unsigned int instruction = (unsigned int) strtol(object_Storage[row].substr(current, 2 * (3 + nixbpe[5])).c_str(), NULL, 16);
    for(int i = 0; i < symbol_Value.size() - 1; i++){
        if(current_Address == symbol_Value[i]){
            sic_Stream << setw(8) << left << symbol_Name[i];
            lis_Stream << setw(8) << left << symbol_Name[i];
            break;
        }
        else if(i + 1 >= symbol_Value.size() - 1){
            sic_Stream << "        ";
            lis_Stream << "        ";
        }
    }

    for(int i = 0; i < literal_Name.size(); i++){
        if(current_Address == literal_Addresses[i]){
            int literal = (int)strtol(object_Storage[row].substr(current+(2 * (3 + nixbpe[5])), literal_Length[i]).c_str(), NULL, 16);
            sic_Stream << (nixbpe[5] ? " + ":" "); //insert a "+" if extended format
            sic_Stream << setw(7) << op_Name << setw(10) << left << literal_Name[i] << endl;
            sic_Stream << setw(14) << right << "LTORG" << endl;
            lis_Stream << (nixbpe[5] ? " + ":" "); //insert a "+" if extended format
            lis_Stream << setw(7) << left << op_Name << setw(10) << left << literal_Name[i];
            lis_Stream << setfill('0') << right << setw(2 * (3 + nixbpe[5])) << instruction << setfill(' ') << endl;
            lis_Stream << setfill('0') << setw(4) << right << current_Address << setfill(' ') << "  ";
            lis_Stream << setw(14) << right << "LTORG" << "            " << literal << endl;
            return (3 + (literal_Length[i]/2));
        }
    }

    sic_Stream << (nixbpe[5] ? "+":" "); //insert a "+" if extended format
    lis_Stream << (nixbpe[5] ? "+":" ");
    
    unsigned int target_Address = 0;
    if (nixbpe[5]) {                    //check for extended format and put displacement field into targetAddress
        target_Address = (unsigned int)strtol(object_Storage[row].substr(current, 8).c_str(), NULL, 16);
        target_Address &= 0x000FFFFF;
    }
    else {
        target_Address = (unsigned int)strtol(object_Storage[row].substr(current, 6).c_str(), NULL, 16);
        target_Address &= 0x000FFF;
    }
    
    if (nixbpe[3]) {        //base relative
        target_Address += base_Address;
    }
    else if (nixbpe[4]) {   //PC relative
        target_Address += (current_Address + 3);
    }
    
    if (op_Name == "J" || op_Name == "JEQ" || op_Name == "JGT" || op_Name == "JLT" || op_Name == "JSUB") {
        target_Address &= (nixbpe[5] ? 0x000FFFFF:0x000FFF);
    }
    
    if (nixbpe[0] == nixbpe[1]) {   // simple addressing
        sic_Stream << setw(8) << left << op_Name;
        lis_Stream << setw(8) << left << op_Name;
        
        for (int i = 0; i < symbol_Value.size(); i++) { //insert symbol name
            if (target_Address == symbol_Value[i] && op_Name != "RSUB") {
                sic_Stream << setw(9) << left << symbol_Name[i] + (nixbpe[2] ? ",X":"") << endl;
                lis_Stream << setw(9) << left << symbol_Name[i] + (nixbpe[2] ? ",X":"");
                break;
            }
        }
        if (op_Name == "RSUB") {
            sic_Stream << endl;
            lis_Stream << "         ";
        }
    }
    else if (nixbpe[0]) {   //indirect addressing
        sic_Stream << setw(7) << left << op_Name << "@";
        lis_Stream << setw(7) << left << op_Name << "@";
        for (int i = 0; i < symbol_Value.size(); i++) { //insert symbol name
            if (target_Address == symbol_Value[i] && op_Name != "RSUB") {
                sic_Stream << setw(9) << left << symbol_Name[i] + (nixbpe[2] ? ",X":"") << endl;
                lis_Stream << setw(9) << left << symbol_Name[i] + (nixbpe[2] ? ",X":"");
                break;
            }
        }
    }
    else {                  //immediate addressing`
        sic_Stream << setw(7) << left << op_Name << "#";
        lis_Stream << setw(7) << left << op_Name << "#";
        if (!nixbpe[5]) {
            sic_Stream << target_Address << endl;
            lis_Stream << setw(9) << target_Address;
        }
    }
    
    if (op_Name == "LDB") {
        base_Address = target_Address;
        for (int i = 0; i < symbol_Value.size(); i++) { //check if symbol name should be inserted
            if (target_Address == symbol_Value[i]) {
                sic_Stream << setw(10) << left << symbol_Name[i] << endl;
                sic_Stream << setw(17) << right << "BASE    " << symbol_Name[i] << endl;
                lis_Stream << setw(9) << left << symbol_Name[i];
                lis_Stream << setw(2 * (3 + nixbpe[5])) << setfill('0') << instruction << setfill(' ') << endl;
                lis_Stream << setfill('0') << setw(4) << right << current_Address << setfill(' ') << "  ";
                lis_Stream << setw(17) << right << "BASE    " << symbol_Name[i] << endl;
                return (3 + nixbpe[5]);
            }
        }
    }


    lis_Stream << right << setfill('0') << setw(2 * (3 + nixbpe[5])) << instruction << setfill(' ') << endl;
    return (3+nixbpe[5]);
}
int Dissem::instruction_Check(int row, int current){
    OpCode code = *new OpCode;
    int opCode = (int)strtol(object_Storage[row].substr(current, 2).c_str(), NULL, 16);
    int instruction_Length = code.getOpFormat(opCode);
    lis_Stream << setfill('0') << setw(4) << right << current_Address << setfill(' ') << "  ";     //print current address in listing file
    string operand_Name = op_Name(code, opCode);
    switch (instruction_Length) {
        case 1:
            check_Format_1(operand_Name, row, current);
            break;
        case 2:
            check_Format_2(operand_Name, row, current); 
            break;
        case 3:
            instruction_Length = check_Format_3_4(operand_Name, code, row, current);
            break;
        default:
            break;
    }
    return (instruction_Length * 2);
}
void Dissem::header_Check(int row) {
    string final_Line;
    string prog_Name = object_Storage[row].substr(1, 6);
    stringstream ss;
    unsigned int progStartAddress = current_Address = (unsigned int)strtol(object_Storage[row].substr(7, 6).c_str(), NULL, 16);
    prog_Length = (unsigned int)strtol(object_Storage[row].substr(13, 6).c_str(), NULL, 16);
    ss << progStartAddress;
    string address = ss.str();
    sic_Stream << setw(9) << left << prog_Name << "START   " << address <<endl;
    lis_Stream << setbase(16) << uppercase << setw(4) << setfill('0') << current_Address << setfill(' ') << "  ";
    lis_Stream << setw(9) << left << prog_Name << "START   " << address <<endl;
}
void Dissem::text_Check(int row) {
    int textLength = (int)strtol(object_Storage[row].substr(7, 2).c_str(), NULL, 16);
    int current = 9;
    while (current < (2 * textLength + 9)) {
        int object_Size = instruction_Check(row, current);
        current_Address += (object_Size/2);
        current += object_Size;
    }
    for (int i = 0; i < symbol_Value.size(); i++) { //check for assembler directives
        if (current_Address <= symbol_Value[i]) {    //symbol was not used in program, use "RESW" or "RESB" to allocate memory
            lis_Stream << setfill('0') << setw(4) << right << current_Address << setfill(' ') << "  ";
            if ((current_Address % 3) == 0) {  // if divisible by 3, reserve word
                sic_Stream << setw(8) << left << symbol_Name[i] << " RESW    ";
                lis_Stream << setw(8) << left << symbol_Name[i] << " RESW    ";
                if (i+1 < symbol_Value.size()) {
                    sic_Stream << setw(8) << left << (symbol_Value[i+1]-symbol_Value[i])/3 << endl;
                    lis_Stream << setbase(10) << setw(8) << left << (symbol_Value[i + 1] - symbol_Value[i])/3 << setbase(16) << endl;
                    current_Address +=(symbol_Value[i + 1] - symbol_Value[i]);
                }
                else {
                    sic_Stream << setw(8) << left << (prog_Length-symbol_Value[i])/3 << endl;
                    lis_Stream << setbase(10) << setw(8) << left << (prog_Length - symbol_Value[i])/3 << setbase(16) << endl;
                    current_Address += (prog_Length-symbol_Value[i]);
                }
            }
            else {
                sic_Stream << setw(8) << left << symbol_Name[i] << " RESB    ";
                lis_Stream << setw(8) << left << symbol_Name[i] << " RESB    ";
                if (i+1 < symbol_Value.size()) {
                    sic_Stream << setw(8) << left << (symbol_Value[i + 1] - symbol_Value[i]) << endl;
                    lis_Stream << setbase(10) << setw(8) << left << (symbol_Value[i + 1] - symbol_Value[i]) << setbase(16) << endl;
                    current_Address += (symbol_Value[i + 1] - symbol_Value[i])/3;
                }
                else {
                    sic_Stream << setw(8) << left << (prog_Length - symbol_Value[i]) << endl;
                    lis_Stream << setbase(10) << setw(8) << left << (prog_Length - symbol_Value[i]) << setbase(16) << endl;
                    current_Address += (prog_Length-symbol_Value[i])/3;
                }
            }
        }
    }
}
void Dissem::modify_Check(int row) {
    unsigned int modify_Address = (unsigned int)strtol(object_Storage[row].substr(1, 6).c_str(), NULL, 16);
    int modLength = (int)strtol(object_Storage[row].substr(7, 2).c_str(), NULL, 16);
    int i = 0;
    unsigned int address = 0;
    while (object_Storage[i][0] != 'T') {   //find the first text record
        i++;
    }
    address = (unsigned int)strtol(object_Storage[i].substr(1, 6).c_str(), NULL, 16);  //store text start address
    
    while (address < modLength) {       //find text record that contains the address to modify
        address += (unsigned int)strtol(object_Storage[i].substr(7, 2).c_str(), NULL, 16);
        i++;
    }
    i--;
    address -= (unsigned int)strtol(object_Storage[i].substr(7, 2).c_str(), NULL, 16);  //subtract the last text length that was added
    int position = 2 * (modify_Address - address) + 10;
    object_Storage[i][position] += prog_Length;
}
void Dissem::end_Check(int row) {
    unsigned int endAddress = (unsigned int)strtol(object_Storage[row].substr(1, 6).c_str(), NULL, 16);
    for (int i = 0; i < symbol_Value.size(); i++) //check the symbol table for address of first instruction
        if (endAddress == symbol_Value[i]) {
            sic_Stream << "         " << setw(8) << left << "END" << symbol_Name[i] << endl;
            lis_Stream << "               " << setw(8) << left << "END" << symbol_Name[i] << endl;
            break;
        }
}
void Dissem::disassemble() {
    for (int i = 0; i < object_Storage.size(); i++) {
        switch (object_Storage[i][0]) {
            case 'H':
                header_Check(i);
                break;
            case 'T':
                text_Check(i);
                break;
            case 'M':
                modify_Check(i);
                break;
            case 'E':
                end_Check(i);
                break;
            default:
                break;
        }
    }
}


//opcode class
// public:
//getOpName for parsing given data structure
//getOpFormat for returning format for assembler to interpret using different functions for format 1,2,3,4