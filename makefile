# makefile for SIC/XE dissassembler CS570
# Fall 2020 
# Mathew Punsalan

PROGRAM = dissem
CXX = g++
CXXFLAGS=-std=c++11 -g

${PROGRAM}:	dissem.o disassembler.o OpCode.o 
		${CXX} dissem.o disassembler.o OpCode.o -o ${PROGRAM}

dissem.o: dissem.cpp dissem.h

disassembler.o:	disassembler.cpp disassembler.h OpCode.h 

OpCode.o: OpCode.cpp OpCode.h

clean:
		rm -f *.o ${PROGRAM}
		rm -f *.sic ${PROGRAM}
		rm -f *.lis ${PROGRAM}
splint:
		splint -warnposix +trytorecover -weak disassembler.cpp
