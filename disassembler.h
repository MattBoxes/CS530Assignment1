#include "OpCode.h"
#include <iomanip>

class Dissem{
public:
    void open_File(char *object_File, char *sym_File);

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

    
};