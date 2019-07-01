CS 421 – Operating System
Programming Assignment #4

Use each of the concurrency techniques: 
1.    semaphores 
2.    message passing 

To solve the following concurrency problem:
A parking lot with a capacity of N cars is on an island. A ferry with a capacity of M cars shuttles cars between the mainland and 
the island. 

The ferry may cross when:
1) there are no cars waiting to get on the ferry (even if it is not full) provided that there are cars waiting on the other side. 
2) the ferry is full or (in the case of crossing to the island) when the number of cars in the ferry would fill the parking lot. 

You are to avoid deadlock and starvation. You may assume that the system contains more than N cars and that N is greater than M. 
Your program must be demonstrably correct. I will not read code for the purposes of checking correctness. Your description of the
algorithm should make its correctness clear and demonstrate the techniques of semaphores and message passing.
