###########################
#
#Makefile for ATM
#
###########################

vpath %.cc ./Src
vpath %.h  ./Include

ATM: main.o Atm.o Bank_machine.o Interface_machine.o Dispatcher.o Msg_queue.o 
	g++ -o ATM main.o Atm.o Bank_machine.o Interface_machine.o Dispatcher.o  Msg_queue.o -std=c++11 -lpthread

Msg_queue.o: Msg_queue.cc
	g++ -c ./Src/Msg_queue.cc

Dispatcher.o: Dispatcher.cc Dispatcher.h TemplateDispatcher.h
	g++ -c ./Src/Dispatcher.cc -std=c++11

main.o: main.cc Bank_machine.cc Interface_machine.cc Atm.cc Msg_queue.cc Bank_machine.h Interface_machine.h Atm.h
	g++ -c ./Src/main.cc ./Src/Msg_queue.cc ./Src/Bank_machine.cc ./Src/Interface_machine.cc ./Src/Atm.cc -std=c++11 -lpthread

Atm.o: Atm.cc Msg_queue.cc Atm.h withdraw.h Receiver.h
	g++ -c ./Src/Atm.cc -std=c++11 -lpthread

Bank_machine.o: Bank_machine.cc Msg_queue.cc Bank_machine.h withdraw.h
	g++ -c ./Src/Bank_machine.cc ./Src/Dispatcher.cc ./Src/Msg_queue.cc -std=c++11 -lpthread

Interface_machine.o: Interface_machine.cc Msg_queue.cc Interface_machine.h withdraw.h
	g++ -c ./Src/Interface_machine.cc ./Src/Dispatcher.cc ./Src/Msg_queue.cc -std=c++11 -lpthread

.PHONY: clean
clean:
	-rm ATM *.o