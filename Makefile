all:
	g++ -o run_tests main.cpp
	./run_tests

clean:
	rm -f run_tests
