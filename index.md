# Lock-free Self-balancing Binary Search Tree
## Team Members: Ziyuan Chen (ziyuanc), Ruogu Du (ruogud)

### Summary

For this project, we will implement a lock-free concurrent self-balancing binary search tree, whose performance will be compared against concurrent binary trees with locks. If there is time left, we will use the binary search tree to implement a lock-free C++ STL.

### Background

Self-balancing binary search trees are an important type of data structures used in the underlying implementation of many containers, libraries, and algorithms. Usually, they achieve self-balancing through a certain sequence of rotations when there is a change in the tree while maintaining all BST properties.

If a tree structure is shared among multiple threads, typically in a shared memory address space, the data structure needs to be protected from race conditions. All operations, including insertion, deletion, modification, read, and rotation, need to be atomic. Intuitively, such requirements can be satisfied with locking. However, locks create thread stalls which can hurdle the performance of a parallel program. In this project, we will work on implementing a lock-free version of this data structure.

There are three major variants of self-balancing BSTs that we will consider using. They are AVL trees, red-black trees, and splay trees. They all have their advantages and disadvantages. Because the AVL and the splay tree are relatively easy to implement, we will start from one of them. However, red-black trees are used more frequently in production-level libraries, so we will strive to implement a lock-free version of it as well.

Depending on our schedule, we might work on implementing container classes in a lock-free C++ STL using our lock-free self-balancing BSTs. For example, the unordered_map<> and unoredered_set<> classes can benefit from our lock-free trees. Our project, if successful and combined with prior works on lock-free data structures, will result in a lock-free production-level C++ STL.

### Challenges

This project is challenging in several ways. First of all, it is very difficult to come up with a lock-free algorithm, especially for insertions and rotations. We will look for existing research papers that could provide us with some insight. Secondly, due to the complexity of the data structure, we might need to relax the constraint on order. This should be okay as long as we avoid a thread trying to read a node that has already been deleted. Lastly, we need to make sure that our lock-free implementation is sufficiently optimized to outperform the implementation using locks for it to be a meaningful project.

### Goals and Deliverables

Our primary goal is to implement a lock-free self-balancing BST that outperforms implementations with locks. We believe that we can achieve this goal because we have six weeks of time, and have already found some valuable research papers in this topic. Our extra goal is to use this BST to implement lock-free STL containers that use self-balancing BSTs, such as the unordered_map. If our work goes slower than expected, we would implement a lock-free binary search tree without the capability to self-balance.

We will demonstrate our project using demo programs that we will write. We will run and time some parallel programs that use our lock-free BST, and show its performance compared to the same program using a BST with locks. We will run these benchmarks in front of the audience, while also putting diagrams and results on the poster.

Deliverables:

- A lock-free self-balancing BST

- If we go ahead of the schedule, a lock-free concurrent STL data structure using our BST. 

### Platform Choice

We will choose C++ as the programming language. This is because it provides more low-level flexibility such as memory management and pointer operations than other object-oriented languages. Also, it has the pthreads library which can be easily used to write parallel benchmarking programs.

Because this project is not computationally expensive and does not rely on any specific hardware, we will mostly be using our own machines as the platform for development. For benchmarking, we might use the Xeon Phi Latedays server because it contains more cores than our own laptops, allowing us to benchmark our project’s performance for a large number of threads.

### Schedule

#### Week 1 (10/30 - 11/5)
- Preliminary Research
- Project Website

#### Week 2 (11/6 - 11/12)
= Implement non-concurrent AVL tree
- Implement non-concurrent Red-black Tree

#### Week 3 (11/13 - 11/19)
- Implement lock-free AVL tree

#### Week 4 (11/20 - 11/26)
- Implement lock-free Red-black Tree
- Implement lock version of AVL and Red-black tree

#### Week 5 (11/27 - 12/3)
- Implement benchmark programs
- Run benchmarks
- Fix bugs and further optimize based on benchmark results

#### Week 6 (12/4 - 12/10)
- Write project report

#### Extra Time
- Implement concurrent map/set STL containers using lock-free BSTs

  
### Resources

https://ppl.stanford.edu/papers/ppopp207-bronson.pdf
https://www.cs.umanitoba.ca/~hacamero/Research/RBTreesKim.pdf
