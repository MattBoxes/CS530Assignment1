#include "OpCode.h"
#include <iomanip>
#include "OpCode.h"
#include <iomanip>

class Dissem{
public:
    void open_File(char *object_File, char *sym_File);
    void disassemble();

private:
    void load_Data();
    vector<string> object_Storage;
    vector<string> symbol_Storage;

    vector<string> symbol_Name;
    vector<unsigned int> symbol_Value;
    vector<char> symbol_Flag;

    vector<string> literal_Name;
    vector<int> literal_Length;
    vector<unsigned int> literal_Address;
    string op_Name(OpCode code, int opCode);
    
    void isSymbol(string op_Name);
    bool isLiteral();
    
    void check_Format_1(string op_Name, int row, int current);
    void check_Format_2(string op_Name, int row, int current);
    int check_Format_3_4(string op_Name, OpCode code, int row, int current);

    unsigned int current_Address;
    unsigned int base_Address;
    vector<unsigned int> literal_Addresses;
    int instruction_Check(int row, int current);
    void header_Check(int row);
    void text_Check(int row);
    void modify_Check(int row);
    void end_Check(int row);
    unsigned int prog_Length;
};
