objs = main.o qsufsort.o tools.o
tag = O3 

qsufsolrt : $(objs) 
	gcc $(objs)  -Wall -$(tag) -o qsufsort

main.o : qsufsort.h
	gcc -c -$(tag) -Wall main.c -o main.o

qsufsort.o : qsufsort.h
	gcc -c -$(tag) -Wall qsufsort.c -o qsufsort.o

tools.o : qsufsort.h
	gcc -c -$(tag) -Wall tools.c -o tools.o


.PHONY : ca
ca :
	rm qsufsort $(objs) 

.PHONY : clean_objs
clean_objs:
	rm $(objs)
