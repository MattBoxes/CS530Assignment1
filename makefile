# makefile for SIC/XE dissassembler CS570
# Fall 2020 
# Mathew Punsalan

PROGRAM = dissem
CC = gcc
CFLAGS = -g

${PROGRAM}:	${PROGRAM}.o 
		${CC} ${PROGRAM}.o -o ${PROGRAM}


${PROGRAM}.o:	${PROGRAM}.h 

clean:
		rm -f *.o ${PROGRAM} your.output*

splint:
		splint -warnposix +trytorecover -weak ${PROGRAM}.c

