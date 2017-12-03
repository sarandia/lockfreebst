all: print_tree.cpp tree.h
	g++ -O3 -std=c++11 print_tree.cpp -o rbtree
debug: print_tree.cpp tree.h
	g++ -O0 -ggdb -std=c++11 treeprinter.cpp print_tree.cpp -o rbtree
test: test_delete.cpp tree.h treeprinter.cpp treeprinter.h
	g++ -O0 -ggdb -std=c++11 treeprinter.cpp test_delete.cpp -o test
ptest: ptest.cpp tree.h treeprinter.cpp treeprinter.h
	g++ -O0 -ggdb -std=c++11 treeprinter.cpp ptest.cpp -lpthread -o ptest
pdelete: pdelete.cpp tree.h treeprinter.cpp treeprinter.h
	g++ -O0 -ggdb -std=c++11 treeprinter.cpp pdelete.cpp -lpthread -o pdelete
clean:
	rm rbtree
	rm test
	rm ptest
	rm pdelete
