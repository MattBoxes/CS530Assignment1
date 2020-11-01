#include "OpCode.h"
#include <iomanip>

class Dissem{
public:
    void open_File(char *object_File, char *sym_File);
    void disassemble();

private:
    void load_Data();
    vector<string> object_Storage;
    vector<string> sym_Storage;

    vector<string> sym_Name;
    vector<unsigned int> sym_Value;
    vector<char> sym_Flag;

    vector<string> literal_Name;
    vector<int> literal_Length;
    vector<unsigned int> literal_Address;
string op_Name(OpCode code, int opCode);
void analyze_Format_1(string op_Name, int row, int current);
void analyze_Format_2(string op_Name, int row, int current);
int analyze_Format_3(string op_Name, OpCode code, int row, int current);
    unsigned int current_Address;
    unsigned int base_Address;
    vector<unsigned int> literal_Addresses;
    int instruction_Analyzer(int row, int current);
    void header_Record_Analyzer(int row);
    void text_Record_Analyzer(int row);
    void modify_Record_Analyzer(int row);
    void end_Record_Analyzer(int row);
    unsigned int prog_Length;
};