# Lock-free Self-balancing Binary Search Tree
## Team Members: Ziyuan Chen (ziyuanc), Ruogu Du (ruogud)

[# FINAL REPORT](http://htmlpreview.github.io/?https://github.com/sarandia/lockfreebst/blob/master/15-618%20Final%20Project%20Report.html)

## Proposal:
### Summary

For this project, we will implement a lock-free concurrent self-balancing binary search tree, whose performance will be compared against concurrent binary trees with locks. If there is time left, we will use the binary search tree to implement a lock-free C++ STL.

### Background

Self-balancing binary search trees are an important type of data structures used in the underlying implementation of many containers, libraries, and algorithms. Usually, they achieve self-balancing through a certain sequence of rotations when there is a change in the tree while maintaining all BST properties.

If a tree structure is shared among multiple threads, typically in a shared memory address space, the data structure needs to be protected from race conditions. All operations, including insertion, deletion, modification, read, and rotation, need to be atomic. Intuitively, such requirements can be satisfied with locking. However, locks create thread stalls which can hurdle the performance of a parallel program. In this project, we will work on implementing a lock-free version of this data structure.

There are three major variants of self-balancing BSTs that we will consider using. They are AVL trees, red-black trees, and splay trees. They all have their advantages and disadvantages. Because the AVL and the splay tree are relatively easy to implement, we will start from one of them. However, red-black trees are used more frequently in production-level libraries, so we will strive to implement a lock-free version of it as well.

Depending on our schedule, we might work on implementing container classes to achieve a lock-free version of C++ Standard Template Library(STL) using our lock-free self-balancing BSTs. For example, the map<> and set<> classes can benefit from our lock-free trees. Our project, if successful and combined with prior works on lock-free data structures, will result in a lock-free production-level C++ STL.

### Challenges

First of all, it is very difficult to come up with a lock-free algorithm, especially for insertions and rotations, where we need to keep correctness while avoiding race conditions. We will look for existing research papers that could provide us with some insight.

Secondly, due to the complexity of the data structure, we might need to relax the constraint on order. This should be acceptable as long as we avoid a thread trying to read a node that has already been deleted.

Lastly, we need to make sure that our lock-free implementation is sufficiently optimized to outperform the implementation using locks for it to be a meaningful project.

### Goals and Deliverables

Our primary goal is to implement a lock-free self-balancing BST that outperforms implementations with simple locking schemes. We believe that we can achieve this goal because we learned from the lecture that lock-free implementations should outperform simply locking if done correctly, and have already found some valuable research papers in this topic. Our extra goal is to use this BST to implement lock-free STL containers that use self-balancing BSTs, such as the map. If our work goes slower than expected, we would implement a lock-free binary search tree with relaxed capability to self-balance.

We will demonstrate our project using demo programs that we will write. We will run and time some parallel programs that use our lock-free BST, and show its performance compared to the same programs using a BST with locks. If we were to manage to finish lock-free STL, we would demonstrate the performance of our version versus the version with locking. We will run these benchmarks in front of the audience, while also putting diagrams and results on the poster.

Deliverables:

- A simple lock-free self-balancing BST: AVL or splay tree.
- A more complex lock-free BST: red-black tree.
- If we go ahead of the schedule, a lock-free concurrent STL data structure using our BST.


### Platform Choice

We will choose C++ as the programming language. This is because it provides more low-level flexibility such as memory management and pointer operations than other object-oriented languages. Also, it has the pthreads library which can be easily used to write parallel benchmarking programs.

Because this project is not computationally expensive nor relies on any specific hardware, we will mostly be using our own machines as the platform for development. For benchmarking, we might use the Xeon Phi Latedays server because it contains more cores than our own laptops, allowing us to benchmark our project’s performance for a large number of threads.

## Project Milestone Report(11/20/2017):

### Summarize the work that you have completed so far

We started our work by reading relevant research work in lock-free self-adjusting binary trees. We started with Kim et. al’s paper Lock-free Red-black Tree using CAS and attempted to understand the proposed algorithm. However, we later discovered that this algorithm was not practical. Although it was lock-free in a sense that it did not use any locks, it did not support concurrent searching, which is what parallel programmers would expect from a concurrent data structure. After some further research, we found that Natarajan et. al’s research on Concurrent Wait-free Red Black Trees proposed an algorithm that would be lock-free and support concurrent searches.

We then dug into the details of Natarajan’s paper. We found that it was based on previous papers by Tsay and Li and Tarjan. We started implementing a sequential red-black tree that follows Tarjan’s algorithm for insertions.

We have also researched on the required synchronization primitive in the algorithm, namely single-word compare-and-swap. We read C++ 11 documentations and StackOverflow posts and have found the a library that includes this function.

### Describe how you are doing with respect to the goals and deliverables stated in your proposal. Do you still believe you will be able to produce all your deliverables? If not, why? What about the "nice to haves"? In your checkpoint writeup we want a new list of goals that you plan to hit for the poster session

We have decided to skip the AVL/splay tree mentioned in our original proposal. They were planned to be intermediate steps towards a self-balancing red-black tree, and it turns out that we do not need them to help come up with an algorithm for the red-black tree. In fact, implementing lock-free versions of the AVL/splay tree would take comparable effort as the red-black tree. Since the AVL/splay tree was never our final goal, our overall project is not affected by this change.

### What do you plan to show at the poster session? Will it be a demo? Will it be a graph?

Because our project is an abstract data structure, it would be difficult to have a visually pleasing live-demo. Therefore, we will produce graphs of our benchmark results and put them on our poster.

### List the issues that concern you the most. Are there any remaining unknowns (things you simply don't know how to solve, or resource you don't know how to get) or is it just a matter of coding and doing the work? If you do not wish to put this information on a public web site you are welcome to email the staff directly.

In our extra plan, we intended to integrate our lock-free red-black tree into C++ standard template library(STL) to facilitate the measurement of performance. However, we discovered that some of the functionalities of the red-black tree based containers in STL (like iterators of a set or map) can not be implemented by generic lock-free insertion, deletion and search operations of the data structure. As a result, we decided to measure our data structure outside STL as soon as we have finished implementation. We will continue to explore the possibility to fully comply our data structure with the container APIs if we are ahead of schedule.

### Schedule

#### Nov 24

Implement sequential top-down insertion and deletion for red-black tree
(Ruogu Du, Ziyuan Chen)

#### Nov 29

Draft lock-free red-black tree
(Ruogu Du, Ziyuan Chen)

#### Dec 3

Finish lock-free red-black tree
(Ruogu Du, Ziyuan Chen)

#### Dec 6

Research on benchmarks on red-black tree
(Ruogu Du)
Research on red-black tree with locks
(Ziyuan Chen)

#### Dec 10
Finish performance measurement
(Ruogu Du, Ziyuan Chen)

#### Dec 12
Finish poster and report
(Ruogu Du, Ziyuan Chen)

### Resources

https://ppl.stanford.edu/papers/ppopp207-bronson.pdf
https://www.cs.umanitoba.ca/~hacamero/Research/RBTreesKim.pdf
