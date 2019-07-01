Write a program to simulate the Least-Recently-Used (LRU) Page Replacement Algorithm that appears on page 440
of the textbook. In particular, the simulator should accept the following input from standard input (stdin):

Number of Virtual Pages
Number of Page Frames Available (not used by page table)
Number of References
Page Referenced   Read or Write (0 or 1, respectively)   duration
Page Referenced   Read or Write (0 or 1, respectively)   duration
             :		:
For example:

          5
          3
          2
          0 0 2
          1 1 1
          3 0 1

is valid input. There are 5 virtual pages (numbered 0, 1, ... 4), 3 page frames (numbered 0, 1, 2) in memory,
 3 references listed: page 0 is read from for 2 clock ticks, page 1 is written to for 1 clock tick, and page 
 3 is read from for 1 clock tick.

Maintain a page table as an array. Use page number as an index to the array. The array should include an R-bit,
 an M-bit, and a Present/Absent bit, as well as the frame currently mapped.

R-bit = 1 if page has been referenced recently (read/write)
M-bit = 1 if page has been modified (needs to be written to disk)

Corresponding to the previous example, the page table could look like this way:

       1 0 1 0
       1 1 1 1
       0 0 0 0
       1 0 1 2
       0 0 0 0
 
The first row indicates the virtual page 0 is mapped to the physical page 0. The page has been referenced and
 is present in the physical memory.

Assume that the R-bit is reset to 0 after a set of 6 pages have been referenced. This is to simulate a reset 
after each clock interrupt.

The output from the simulator should indicate whenever a fault occurs, and which page has been evicted (if 
one is evicted). Always select the smallest numbered page in a class to be evicted. In addition, the output 
should include the total number of faults that occurred. 

The possible output would look something like:

                    : : 
            Fault: no page evicted, page 0 brought in to memory.
            Fault: page 2 evicted, page 3 brought in to memory.
                    : :
            Total number of faults is 23.

********** Book Definition ************
Counters:
   In the simplest case, we associate with each page-table entry a
time-of-use field and add to the CPU a logical clock or counter. The clock is
incremented for every memory reference. Whenever a reference to a page
is made, the contents of the clock register are copied to the time-of-use
field in the page-table entry for that page. In this way, we always have
the “time” of the last reference to each page.We replace the page with the
smallest time value. This scheme requires a search of the page table to find
the LRU page and a write to memory (to the time-of-use field in the page
table) for each memory access. The times must also be maintained when
page tables are changed (due to CPU scheduling). Overflow of the clock
must be considered.



*************** HW6 ********************
1) Logically addressing segments of physical memory into main memory with fixed-sized blocks called frames. 
2) Logically addressing main memory into blocks of the same size called pages. 
3) Each address is separated into two parts: 
   a) page-number (page table index)
   b) page-offset
4) The page table contains the base address in physical memory.
5) The physical memory addresses are calculated by combining the page-number with the page-offset.

**************** Links *******************
https://web.archive.org/web/20160601022921/http://people.csail.mit.edu/rinard/osnotes/h9.html
https://en.wikipedia.org/wiki/Inode
https://en.cppreference.com/w/cpp/chrono/c/time
