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


//helper function for converting and Opcode and value to its mneomonic
string Dissem::op_Name(OpCode code, int opCode)
{
    string operand_Name = code.getOpName(opCode);
    return operand_Name;
}

// helper function to check for symbol table values when checking formats 1-4 and pushing to output stream
void Dissem::isSymbol(string op_Name)
{
    for (int i = 0; i < symbol_Value.size() - 1; i++)
    {
        // if current address is in the symbol table push symbol to output stream
        if (current_Address == symbol_Value[i])
        {
            sic_Stream << setw(8) << left << symbol_Name[i];
            lis_Stream << setw(8) << left << symbol_Name[i];
            break;
        }
        // else fill with space when finished looking through table
        else if (i + 1 >= symbol_Value.size() - 1)
        {
            sic_Stream << " " << op_Name << "        ";
            lis_Stream << " " << op_Name << "        ";
        }
    }
}

bool Dissem::isLiteral()
{
    for (int i = 0; i < literal_Name.size(); i++)
    {
        if (current_Address == literal_Addresses[i])
        {
            //put LTORG if found, set widths of stream to fit literal names
            sic_Stream << literal_Name[i] << "          " << endl;
            sic_Stream << "              "
                       << "LTORG" << endl;
            lis_Stream << literal_Name[i] << "          " << endl;
            lis_Stream << "              "
                       << "LTORG" << endl;
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
void Dissem::check_Format_1(string op_Name, int row, int currIndex)
{

    // check for symbol, check for literal
    isSymbol(op_Name);
    if (isLiteral())
    {
        return;
    }
}

void Dissem::check_Format_2(string op_Name, int row, int currIndex)
{

    isSymbol(op_Name);
    if (isLiteral())
    {
        return;
    }

    // create registers based on stored objectcode
    int register_One = (int)strtol(object_Storage[row].substr(currIndex + 2, 1).c_str(), NULL, 16);
    int register_Two = (int)strtol(object_Storage[row].substr(currIndex + 3, 1).c_str(), NULL, 16);

    // write out register 1 name
    switch (register_One)
    {
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
    // write out register 2 name
    switch (register_Two)
    {
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

int Dissem::check_Format_3_4(string op_Name, OpCode code, int row, int currIndex)
{
    // create 6 register flags
    bool nixbpe[6];
    int flag_Section = (int)strtol(object_Storage[row].substr(currIndex + 1, 2).c_str(), NULL, 16);

    //assign flags based on opcode
    for (int i = 0; i < 6; i++)
    {
        nixbpe[i] = code.getBit(flag_Section, 5 - i);
    }
    // get obj code, convert using nixbpe flags and turn into a string
    unsigned int instruction = (unsigned int)strtol(object_Storage[row].substr(currIndex, 2 * (3 + nixbpe[5])).c_str(), NULL, 16);

    // special case for checking symbols with format 3, need more spaces
    for (int i = 0; i < symbol_Value.size() - 1; i++)
    {
        if (current_Address == symbol_Value[i])
        {
            sic_Stream << symbol_Name[i] << "        ";
            lis_Stream << symbol_Name[i] << "        ";
            break;
        }
        else if (i + 1 >= symbol_Value.size() - 1)
        {
            sic_Stream << "        ";
            lis_Stream << "        ";
        }
    }

    for (int i = 0; i < literal_Name.size(); i++)
    {
        //check for literals and put correct address with flags
        if (current_Address == literal_Addresses[i])
        {
            int literal = (int)strtol(object_Storage[row].substr(currIndex + (2 * (3 + nixbpe[5])), literal_Length[i]).c_str(), NULL, 16);
            //if extended format add + else a space
            sic_Stream << (nixbpe[5] ? " + " : " ");
            sic_Stream << setw(7) << op_Name << setw(10) << left << literal_Name[i] << endl;
            sic_Stream << setw(14) << right << "LTORG" << endl;
            //if extended format add + else a space
            lis_Stream << (nixbpe[5] ? " + " : " "); //insert a "+" if extended format
            lis_Stream << setw(7) << left << op_Name << setw(10) << left << literal_Name[i];
            lis_Stream << setfill('0') << right << setw(2 * (3 + nixbpe[5])) << instruction << setfill(' ') << endl;
            lis_Stream << setfill('0') << setw(4) << right << current_Address << setfill(' ') << "  ";
            lis_Stream << setw(14) << right << "LTORG"
                       << "            " << literal << endl;
            return (3 + (literal_Length[i] / 2));
        }
    }

    // if extended format add + else a space
    sic_Stream << (nixbpe[5] ? "+" : " ");
    lis_Stream << (nixbpe[5] ? "+" : " ");

    unsigned int target_Address = 0;
    // using 0x000FFFF for displcement to put if extended format
    if (nixbpe[5]) // if e == 1
    {
        target_Address = (unsigned int)strtol(object_Storage[row].substr(currIndex, 8).c_str(), NULL, 16);
        target_Address &= 0x000FFFFF;
    }
    else
    {
        target_Address = (unsigned int)strtol(object_Storage[row].substr(currIndex, 6).c_str(), NULL, 16);
        target_Address &= 0x000FFF;
    }

    if (nixbpe[3]) //if b == 1 Base relative
    {
        target_Address += base_Address;
    }
    else if (nixbpe[4]) //if p == 1 PC relative
    {
        target_Address += (current_Address + 3);
    }
    // check for jump commands
    if (op_Name == "J" || op_Name == "JEQ" || op_Name == "JGT" || op_Name == "JLT" || op_Name == "JSUB")
    {
        target_Address &= (nixbpe[5] ? 0x000FFFFF : 0x000FFF);
    }
    // if both n,i == 0 or 1 we have simple addressing
    if (nixbpe[0] == nixbpe[1])
    {
        sic_Stream << op_Name << "         ";
        lis_Stream << op_Name << "         ";
        // insert X name when using Indexed mode
        for (int i = 0; i < symbol_Value.size(); i++)
        {
            if (target_Address == symbol_Value[i] && op_Name != "RSUB")
            {
                sic_Stream << symbol_Name[i] + (nixbpe[2] ? ",X" : "") << "         " << endl;
                lis_Stream << symbol_Name[i] + (nixbpe[2] ? ",X" : "") << "         ";
                break;
            }
        }

        if (op_Name == "RSUB")
        {
            sic_Stream << endl;
            lis_Stream << "         ";
        }
    }
    //if n == 0 indirect address mode so add @
    else if (nixbpe[0])
    {
        sic_Stream << setw(7) << left << op_Name << "@";
        lis_Stream << setw(7) << left << op_Name << "@";
        for (int i = 0; i < symbol_Value.size(); i++)
        {
            if (target_Address == symbol_Value[i] && op_Name != "RSUB")
            {
                sic_Stream << symbol_Name[i] + (nixbpe[2] ? ",X" : "") << "         " << endl;
                lis_Stream << symbol_Name[i] + (nixbpe[2] ? ",X" : "") << "         ";
                break;
            }
        }
    }
    // else use #
    else
    {
        sic_Stream << setw(7) << left << op_Name << "#";
        lis_Stream << setw(7) << left << op_Name << "#";
        if (!nixbpe[5])
        {
            sic_Stream << target_Address << endl;
            lis_Stream << target_Address << "         ";
        }
    }

    if (op_Name == "LDB")
    {
        //loading symbols with base register
        base_Address = target_Address;
        for (int i = 0; i < symbol_Value.size(); i++)
        {
            if (target_Address == symbol_Value[i])
            {
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
    return (3 + nixbpe[5]);
}

//check for instruction type
int Dissem::instruction_Check(int row, int currIndex)
{
    OpCode code = *new OpCode;
    int opCode = (int)strtol(object_Storage[row].substr(currIndex, 2).c_str(), NULL, 16);
    int instruction_Length = code.getOpFormat(opCode);

    // use current address line in output stream
    lis_Stream << setfill('0') << setw(4) << right << current_Address << setfill(' ') << "  ";
    string operand_Name = op_Name(code, opCode);
    //based on format code use correct format handler
    switch (instruction_Length)
    {
    case 1:
        check_Format_1(operand_Name, row, currIndex);
        break;
    case 2:
        check_Format_2(operand_Name, row, currIndex);
        break;
    case 3:
        instruction_Length = check_Format_3_4(operand_Name, code, row, currIndex);
        break;
    default:
        break;
    }
    //return twice length for object size
    return (instruction_Length * 2);
}
void Dissem::header_Check(int row)
{
    string final_Line;
    string prog_Name = object_Storage[row].substr(1, 6);
    stringstream ss;
    unsigned int progStartAddress = current_Address = (unsigned int)strtol(object_Storage[row].substr(7, 6).c_str(), NULL, 16);
    prog_Length = (unsigned int)strtol(object_Storage[row].substr(13, 6).c_str(), NULL, 16);
    ss << progStartAddress;
    string address = ss.str();
    sic_Stream << setw(9) << left << prog_Name << "START   " << address << endl;
    lis_Stream << setbase(16) << uppercase << setw(4) << setfill('0') << current_Address << setfill(' ') << "  ";
    lis_Stream << setw(9) << left << prog_Name << "START   " << address << endl;
}

//function to read text record
void Dissem::text_Check(int row)
{
    int textLength = (int)strtol(object_Storage[row].substr(7, 2).c_str(), NULL, 16);
    int currIndex = 9;

    //loop through obj code and handle formats/instructions
    while (currIndex < (2 * textLength + 9))
    {
        int object_Size = instruction_Check(row, currIndex);
        current_Address += (object_Size / 2);
        currIndex += object_Size;
    }
    // look for special directives for assembler and see if used RESB or RESW
    for (int i = 0; i < symbol_Value.size(); i++)
    { 
        // current symbol not used so resw or resb used to allocate memory
        if (current_Address <= symbol_Value[i])
        { 
            lis_Stream << setfill('0') << setw(4) << right << current_Address << setfill(' ') << "  ";
            if ((current_Address % 3) == 0)
            {
                sic_Stream << setw(8) << left << symbol_Name[i] << " RESW    ";
                lis_Stream << setw(8) << left << symbol_Name[i] << " RESW    ";
                if (i + 1 < symbol_Value.size())
                {
                    sic_Stream << setw(8) << left << (symbol_Value[i + 1] - symbol_Value[i]) / 3 << endl;
                    lis_Stream << setbase(10) << setw(8) << left << (symbol_Value[i + 1] - symbol_Value[i]) / 3 << setbase(16) << endl;
                    // add total length to program length
                    current_Address += (symbol_Value[i + 1] - symbol_Value[i]);
                }
                else
                {
                    sic_Stream << setw(8) << left << (prog_Length - symbol_Value[i]) / 3 << endl;
                    lis_Stream << setbase(10) << setw(8) << left << (prog_Length - symbol_Value[i]) / 3 << setbase(16) << endl;
                    // add total length to program length
                    current_Address += (prog_Length - symbol_Value[i]);
                }
            }
            else
            {
                sic_Stream << setw(8) << left << symbol_Name[i] << " RESB    ";
                lis_Stream << setw(8) << left << symbol_Name[i] << " RESB    ";
                if (i + 1 < symbol_Value.size())
                {
                    sic_Stream << setw(8) << left << (symbol_Value[i + 1] - symbol_Value[i]) << endl;
                    lis_Stream << setbase(10) << setw(8) << left << (symbol_Value[i + 1] - symbol_Value[i]) << setbase(16) << endl;
                    // add total length to program length
                    current_Address += (symbol_Value[i + 1] - symbol_Value[i]) / 3;
                }
                else
                {
                    sic_Stream << setw(8) << left << (prog_Length - symbol_Value[i]) << endl;
                    lis_Stream << setbase(10) << setw(8) << left << (prog_Length - symbol_Value[i]) << setbase(16) << endl;
                    // add total length to program length
                    current_Address += (prog_Length - symbol_Value[i]) / 3;
                }
            }
        }
    }
}
void Dissem::modify_Check(int row)
{
    //create modify address and modify length based on stored values of text record
    unsigned int modify_Address = (unsigned int)strtol(object_Storage[row].substr(1, 6).c_str(), NULL, 16);
    int modLength = (int)strtol(object_Storage[row].substr(7, 2).c_str(), NULL, 16);
    int i = 0;
    unsigned int address = 0;
    // look for text record first and then proceed past it
    while (object_Storage[i][0] != 'T')
    { 
        i++;
    }
    //text start address
    address = (unsigned int)strtol(object_Storage[i].substr(1, 6).c_str(), NULL, 16);

    // use address to find text record to modify
    while (address < modLength)
    { 
        address += (unsigned int)strtol(object_Storage[i].substr(7, 2).c_str(), NULL, 16);
        i++;
    }
    i--;
    // remove the last text added
    address -= (unsigned int)strtol(object_Storage[i].substr(7, 2).c_str(), NULL, 16); 
    int position = 2 * (modify_Address - address) + 10;
    object_Storage[i][position] += prog_Length;
    // add total length to program length
}

// function to check end record
void Dissem::end_Check(int row)
{
    unsigned int endAddress = (unsigned int)strtol(object_Storage[row].substr(1, 6).c_str(), NULL, 16);
    // look for first instruction address
    for (int i = 0; i < symbol_Value.size(); i++) 
        if (endAddress == symbol_Value[i])
        {
            // store instruction at end
            sic_Stream << "         "
                       << "END" << symbol_Name[i] << "        " << endl;
            lis_Stream << "               "
                       << "END" << symbol_Name[i] << "        " << endl;
            break;
        }
}

// main function to dissassemble input files called in main
void Dissem::disassemble()
{
    // loop through object storage by eows for each record type
    for (int i = 0; i < object_Storage.size(); i++)
    {
        switch (object_Storage[i][0])
        {
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
