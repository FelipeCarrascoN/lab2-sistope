FLAGS = -Wall -lm

all: lab2 broker worker

lab2: lab2.c
	gcc -o lab2 lab2.c

broker: broker.c fbroker.o
	gcc -o broker broker.c fbroker.o $(FLAGS)

fbroker.o: fbroker.c
	gcc -c fbroker.c $(FLAGS)
	
worker: worker.c fworker.o
	gcc -o worker worker.c fworker.o $(FLAGS)

fworker.o: fworker.c
	gcc -c fworker.c $(FLAGS)

clean:
	rm -f *.o lab2 broker worker