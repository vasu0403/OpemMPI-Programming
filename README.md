# Distributed Systems Assignment 1
#### P1: Estimating the value of (pi * pi) / 6
- **Description**
    - The task is to estimate the value of (pi * pi) / 6. We do this by using the numerical identity that the sum of the reciprocals of the squares of integers converges to (pi * pi) /6.
    - One process (rank = 0) reads the input (value of N) from the input file given and broadcasts this value to each process.
    - Based on the number of processes with which the program is run (from 1 to 11), each process decides on which segment of numbers from 1 to N should the current process work on.
    - Each process works on its own segments (which are all disjoint) parallely and all the process send their result to process with rank 0. The process with rank 0 accumulates all the results (sum) and outputs the estimated value.
- **Analysis**
    - With the increasing value of N, the run-time of the program increases (which is as expected).
    - On increasing the number of processes, the run-time increases almost monotonically. This is a little counter intuitive but given that the task is computationally very simple, the major overhead is the message passing. Increasing the number of processes, increases the amount of message passsing, and message passing being the major bottleneck, therefore the run time increases on increasing the number of processes.
#### P2: Quick sort
- **Description**
    - One process (rank = 0) reads the input (value of N and the unsorted array) and broadcasts the input to every other process.
    - Each process works on disjoint segments of the array (each process can itself calculate the starting and ending index of its segment from the value of N and the number of processes with the program was run).
    - Each process sorts its own segment (using quick sort). After sorting, it sends its sorted segment to the process with rank 0.
    - The process with rank 0 ends up with a number of sorted segments in the end. It combines the result to produce the final sorted array by merging all the sorted segments (using K-way merge, where K is the number of processes with which the program was run)
- **Analysis**
    - On increasing the value of N, the run-time of the program increases
    - On changing the number of processes, the program finishes the fastest for 2 processes, while it takes the longest to complete for 11 processes.
        - [N = 1000000, p = 1]: runtime = 0.523826 
        - [N = 1000000, p = 2]: runtime = 0.495579
        - [N = 1000000, p = 11]: runtime = 1.135955
    - It seems that after increasing the number of processes from 2, message passing becomes the major bottlenock and therefore runtime of the program gradually increases from there on. p = 2 is therefore the sweet spot, at which the program is able to take benefit of parallelism and message passing is also not that big of an overhead.

#### P3: Edge coloring
- **Description**
    - The process with rank 0 reads the input. It converts the original graph into a line graph. So now solving the problem of vertex coloring on the line graph is equivalent to solving the problem of vertex coloring on the original graph. This process distrubtes the line graph to every other process.
    - Each process selects a few number of nodes on which it will work.
    - Coloring happens in rounds. In each round, each process selects all those nodes which it can color in this particular round. In each round, no two vertices sharing an edge are colored at the same time. This is ensured by numbering all the nodes of the graph. Each process selects a node for coloring only if its value is greater than the value of all the other adjacent uncolored nodes.
    - For each node, aditionally a list of available colors is also maintained. After selecting a particular node to color, the process colors it with the minimum color with which it can be colored.
    - After coloring, each process broadcasts the information about all the nodes it colored in that round to all the other process. In this way, each process can know all the nodes which have been colored till previous round.
    - The algorithm stops when all the nodes have been colored.
- **Analysis**
    - On increasing the value of N or M, the runtime increase almost monotically.
    - On increasing the number of processes, the runtime sometimes increases, and sometimes decreases. The minimum run time usually comes for either 2 processes, or for 3 processes (depending on the input). While the maximum runtime always comes for 11 processes. The runtime for a particular test case are shown below.
        - [N = 100, M = 500, p = 1]: runtime = 0.007590
        - [N = 100, M = 500, p = 2]: runtime = 0.007501
        - [N = 100, M = 500, p = 3]: runtime = 0.006534
        - [N = 100, M = 500, p = 11]: runtime = 0.015450
    - It seems that after 4 processes, message passing becomes too much of an overhead, and therefore runtime gradually increases on increasing the number of processes further (although not monotonically).

