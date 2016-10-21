CC:=gcc
LD:=ld

CCFLAGS:=-Wall -Wextra 
LDFLAGS:=-macosx_version_min 10.12 -lSystem

OBJS:=main.o

parser:=parser.tab.o
parser_h:=parser.tab.h

scanner:=lex.yy.o

all: solver

solver: ${OBJS} ${parser} ${scanner}
	${LD} -o solver ${LDFLAGS} ${OBJS} ${parser} ${scanner}

${parser}: parser.y
	bison -d parser.y
	${CC} -o parser.tab.o ${CCFLAGS} -c parser.tab.c 

${scanner}: scanner.l
	flex scanner.l
	${CC} -o lex.yy.o ${CCFLAGS} -c lex.yy.c

%.o: %.c ${parser_h}
	${CC} ${CCFLAGS} -c $<

${parser_h}: parser.y
	bison -d parser.y

clean:
	rm -f parser.tab.c parser.tab.h lex.yy.c ${scanner} ${parser} ${OBJS} solver
