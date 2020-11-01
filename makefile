# makefile for SIC/XE dissassembler CS570
# Fall 2020 
# Mathew Punsalan

PROGRAM = dissem
CXX = g++
CXXFLAGS=-std=c++11 -g

${PROGRAM}:	disassembler.o OpCode.o 
		${CXX} disassembler.o OpCode.o -o ${PROGRAM}

disassembler.o:	disassembler.cpp disassembler.h OpCode.h 

OpCode.o: OpCode.cpp OpCode.h

clean:
		rm -f *.o ${PROGRAM}
splint:
		splint -warnposix +trytorecover -weak disassembler.cpp
