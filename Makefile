compfs: main.o Scanner.o Parser.o Tree.o CodeGenerator.o
	g++ main.o Scanner.o Parser.o Tree.o CodeGenerator.o -o compfs

main.o: main.cpp main.h
	g++ -c main.cpp

Scanner.o: Scanner.cpp Scanner.h
	g++ -c Scanner.cpp

Parser.o: Parser.cpp Parser.h
	g++ -c Parser.cpp

Tree.o: Tree.cpp Tree.h
	g++ -c Tree.cpp

CodeGenerator.o: CodeGenerator.cpp CodeGenerator.h
	g++ -c CodeGenerator.cpp

clean:
	rm *.o compfs
