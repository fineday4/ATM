###########################
#
#Makefile for ATM
#
###########################

vpath %.cc ./Src
vpath %.h  ./Include

ATM: main.o Atm.o Bank_machine.o Interface_machine.o
	g++ -o ATM main.o Atm.o Bank_machine.o Interface_machine.o -std=c++11 -lpthread

main.o: main.cc Bank_machine.cc Interface_machine.cc Atm.cc Bank_machine.h Interface_machine.h Atm.h
	g++ -c ./Src/main.cc ./Src/Bank_machine.cc ./Src/Interface_machine.cc ./Src/Atm.cc -std=c++11 -lpthread

Atm.o: Atm.cc Msg_queue.cc Atm.h
	g++ -c ./Src/Atm.cc ./Src/Msg_queue.cc -std=c++11 -lpthread

Bank_machine.o: Bank_machine.cc Msg_queue.cc Bank_machine.h
	g++ -c ./Src/Bank_machine.cc ./Src/Msg_queue.cc -std=c++11 -lpthread

Interface_machine.o: Interface_machine.cc Msg_queue.cc Interface_machine.h
	g++ -c ./Src/Interface_machine.cc ./Src/Msg_queue.cc -std=c++11 -lpthread

.PHONY: clean
clean:
	-rm ATM main.o Atm.o Bank_machine.o Interface_machine.o