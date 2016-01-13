run: cliente.out servidor.out
test: 
	clear
	./cliente.out < in.txt cliente.out >servidor.out
cliente.out: comunicacion.o
	g++ -o cliente.out main.cpp comunicacion.o -lpthread -std=c++11
servidor.out:
	g++ -o servidor.out servidor.cpp -lpthread -std=c++11
comunicacion.o:
	g++ -c comunicacion.cpp -std=c++11
clean:
	rm comunicacion.o
	rm cliente.out
	rm servidor.out	
	clear
