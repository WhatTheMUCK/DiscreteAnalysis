check: prog
	bash test_gen.sh
	python3 test_gen.py
	bash checker.sh
	bash fullOut.sh

SOURCES=main.cpp preprocess.cpp search.cpp

prog: main.cpp search.cpp preprocess.cpp
	g++ $(SOURCES) -o prog -w -O2

preprocess_europe: prog
	./prog preprocess --nodes europe.nodes --edges europe.edges --output europe.graph

search_europe: prog
	./prog search --graph europe.graph --input input.txt --output output.txt