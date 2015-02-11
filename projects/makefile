obj=main.o make_patset.o AC.o libcommon.o queue.o make_data.o
obj1=O3

run:$(obj)
	gcc -g $(obj) -lm -Wall -$(obj1) -o run

main.o: ./c/main.c ./h/libcommon.h ./h/AC.h ./h/make_patset.h ./h/make_data.h
	gcc -g -c -I ./h/ -$(obj1) -Wall ./c/main.c

make_patset.o: ./c/make_patset.c ./h/libcommon.h  ./h/make_patset.h 
	gcc -g -c -I ./h/ -$(obj1) -Wall ./c/make_patset.c

make_data.o: ./c/make_data.c ./h/libcommon.h  ./h/make_data.h 
	gcc -g -c -I ./h/ -$(obj1) -Wall ./c/make_data.c

queue.o: ../lib/c/queue.c ./h/libcommon.h  ../lib/h/queue.h 
	gcc -g -c -I ./h/ -I ../lib/h -$(obj1) -Wall ../lib/c/queue.c

AC.o: ./c/AC.c ./h/AC.h ./h/make_patset.h ../lib/h/queue.h
	gcc -g -c -I ./h/ -I ../lib/h -$(obj1) -Wall ./c/AC.c

libcommon.o: ./c/libcommon.c ./h/libcommon.h 
	gcc -g -c -I ./h/ -$(obj1) -Wall ./c/libcommon.c

ca:
	rm run $(obj) results patset_info

co:
	rm $(obj)
