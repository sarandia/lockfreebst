#include "tree.h"

#define Allocator allocator
#include <defalloc.h>

int main() {
	rb_tree<int, int> t = new rb_tree<int, int>();
}

