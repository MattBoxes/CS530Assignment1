# makefile for SIC/XE dissassembler CS570
# Fall 2020 
# Mathew Punsalan

PROGRAM = dissem
CC = g++
CFLAGS = -c

${PROGRAM}:	${PROGRAM}.o OpCode.o
		${CC} ${PROGRAM}.o OpCode.o -o ${PROGRAM}


${PROGRAM}.o:	${PROGRAM}.h 

OpCode.o: OpCode.cpp
	$(CC) $(CFLAGS) OpCode.cpp

clean:
		rm -f *.o ${PROGRAM}

splint:
		splint -warnposix +trytorecover -weak ${PROGRAM}.cpp

