#ifndef __a2__C_____OpCode__
#define __a2__C_____OpCode__


#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <vector>

using namespace std;

class OpCode {
public:
    bool getBit(int input, int position);
    string getOpName(int opCode);
    int getOpFormat(int opCode);
};
#endif /* defined(__a2__C_____OpCode__) */