objs = main.o AC.o make_patset.o queue.o textools.o
tag=O3

AC : $(objs)
	gcc $(objs) -Wall -o AC -$(tag)

main.o : make_data.h textools.h AC.h main.c
	gcc -Wall -c main.c -$(tag)

AC.o : textools.h AC.h queue.h AC.c
	gcc -Wall -c AC.c -$(tag)

make_patset.o : make_patset.h textools.h make_patset.c
	gcc -Wall -$(tag) -lm -c make_patset.c

queue.o : textools.h queue.h queue.c
	gcc -Wall -c queue.c -$(tag)

textools.o : textools.h textools.c
	gcc -Wall -c textools.c -$(tag)

.PHONY : clean

clean :
	rm $(objs) AC

