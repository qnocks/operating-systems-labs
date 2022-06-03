# OS #5. Memory management
## Objective of the work
Familiarity with the principles of organizing virtual memory

## Reference material

<!-- ### 0. Virtual memmory -->

### 1. Page tables
The virtual address consists of two parts: the virtual page number and the offset relative to the beginning of this page. Typically, on a 32-bit system, pages are 4KB in size, which means that 12 bits are used for the offset (2<sup>12</sup> = 4096) and 20 bits for the page number. To convert such a virtual address to a physical address, each process needs to have a page table that maps virtual pages to physical page frames. During address translation, the memory manager looks up the page table for the desired virtual page and replaces its number with the number of the corresponding physical page frame, leaving the offset unchanged. <!-- see figure --> The page table entry also contains additional information, such as page protection level (read, write, execute), access, modification, presence bits, etc. The situation when the page being searched is not in physical memory (the presence bit in the page table is zero) is called a page fault. <!-- example of address translation as a table -->

The use of paging memory is not without drawbacks. A process can only allocate memory in chunks that are multiples of the page size. As a result, pages may be formed, most of the memory in which is not used. This situation is called *internal fragmentation*. Smaller pages can be used to solve this problem, but this results in a larger page table. Indeed, consider a 32-bit address space divided into 4KB pages. 20 bits are reserved for the page number, which corresponds to 2<sup>20</sup> = 1M entries in the page table. Each entry in the page table contains the 20-bit number of its corresponding physical page and a few additional bits of auxiliary information, about 32 bits in total. Thus, the size of the page table is 1M * 32 bits = 4MB, and all these 4MB must be located in physical memory, otherwise, when loading the page table from the hard disk, address translation will take too long. Because each process has its own page table, when using paging on a 32-bit system, you need to allocate 4MB of RAM to store the page table for each process. With a large number of running processes, this can lead to a situation where the page tables take up a significant amount of physical memory.

<!-- ### 2. Multilevel paging -->

### 2. Inverted page tables
#### 2.1. Introduction
Consider a system where the virtual address space consists of 64-bit addresses, the page size is 4KB, and the amount of available physical memory is 512MB. How much memory space is required for a simple single-level page table? Such a table contains one entry per virtual page, i.e. 2<sup>64</sup>/2<sup>12</sup> = 2<sup>52</sup> entries. Each entry in the page table will take up about 4 bytes, so the total size of the page table will be 2<sup>54</sup> bytes or 16 petabytes (*peta-* > *tera-* > *giga-*)! And **every** process will have such a page table.

Of course, a process is unlikely to use all 64 bits of the address space, so one might try using multilevel page tables. How many levels of the page table will be required if a separate table at each level must fit on one virtual page of memory, i.e. take no more than 4KB? Let a single entry (row) in the page table be 4 bytes long, then the page table at each level can contain 1024 entries or 10 bits of address space. It turns out that the multilevel page table will have `[52/10] = 6` levels. But this means that each address translation will result in 6 memory access operations, i.e. performance will drop by a factor of 6 compared to a simple single-level page table.

You may notice that the amount of physical memory installed in the system is much less than the size of the virtual address space: 512MB = 2<sup>29</sup>B << 2<sup>64</sup>B. Or, in terms of page frames: 2<sup>29</sup>/2<sup>12</sup> = 2<sup>17</sup> = 128K page frames. Keeping one entry in the page table for each page frame, rather than for each virtual page of each process, will greatly reduce the size of the page table. For example, if the length of one entry in such a page table is 16 bytes, the entire table will take up only 2MB. Because all processes use the same physical memory, there is only one such table needed. This concept is called *inverted page table*.

#### 2.2. Linear inverted page table
The simplest inverted page table contains one entry for each page block of physical memory as a linear array of consecutive entries. Because the table is shared by all processes, each entry must contain the ID of the process that owns the page. And since page frames are now mapped to virtual pages, each entry contains a virtual page number instead of a physical one. The number of the physical page (page block) is not explicitly stored, because it matches the index number of the record (array index). The table also contains standard bits with additional information about the level of protection of the page and accounting for recent accesses to it. If the process ID is 16 bits long, another 52 bits are allocated for the virtual page number, and 12 bits are for additional information, then each entry in the inverted page table will occupy 80 bits or 10 bytes, which gives a total table size of 10 * 128KB = 1.3MB.

To translate a virtual address into a physical address, the virtual page number and the current process ID are compared in turn with each of the entries in the inverted page table. If a match is found, the index of the matched entry replaces the virtual page number and the triplet `<process ID; index in the inverted page table; offset>` forms a physical address. If there are no matches, a page-out error is generated. <!-- This address translation procedure is shown in the figure. -->

Although the inverted page table is relatively small, sequential lookups can be very slow. In the worst case, in order to find a match, you need to look through the entire table, which is equivalent to 128K memory accesses. On average, you can expect 64K memory accesses to translate an address. This is too inefficient, so hashing is used to reduce the lookup time.

#### 2.3. Inverted page table with hashing
This approach adds another layer just before the page table itself, the hash table. This hash table must have at least as many entries as the page table. It maps the value pair `<process ID; virtual page number>` entry in the inverted page table. Since collisions are possible when using a hash function, you must use linked lists for each of the entries in the hash table. <!-- If there are as many rows in the hash table as there are physical pages (page blocks) in the given computer, then on average the list will be one entry long. --> Each element of the list corresponds to a physical page and has a corresponding entry in the inverted page table, and the entire list can be represented as a sequence of entries in the page table, where each entry has a pointer to the next entry in the list. This approach would require an additional 4 bytes for each entry in the page table, after which the page table would be 14 * 128KB = 1.8MB. The hash table takes up an additional 4 * 128KB = 512KB. The total overhead is 2.3MB.

To convert a virtual address to a physical address, the process ID and virtual page number are input to a hash function, the value of which allows you to find the desired entry in the hash table. Then the transition to the page table is carried out according to the pointer from the entry in the hash table. The process ID and virtual page number are compared to the page table entry. If they do not match, the pointer to the next page specified in the found entry in the inverted page table is followed, and the procedure is repeated until a match is found. If the end of the chain is reached (the pointer to the next page is null), a page-out error is generated. <!-- This procedure is shown in the figure -->

Assuming a good hash function is used, the average list length will be on the order of 1.5 elements. This means that, on average, only 2.5 memory accesses are required to resolve the address. This is a pretty good result considering that a two-level page table requires 2 memory accesses. However, this is not the limit, and the time required to translate an address can be further reduced by using caching with the Translation Lookaside Buffer (TLB).

### 3. TLB
See [Translation lookaside buffer](https://en.wikipedia.org/wiki/Translation_lookaside_buffer) and [Translation lookaside buffer](https://en.wikipedia.org/wiki/Translation_lookaside_buffer)

### 4. Page replacement algorithms

#### 4.1. FIFO
This algorithm is one of the easiest to implement. The operating system maintains a queue of pages loaded into memory. When a new page is loaded into memory, it is added to the end of the queue. When a page out error occurs, space for a new page is freed by removing the page from the head of the queue.

#### 4.2. Second chance
Modification of the FIFO algorithm. If a page out-of-memory error occurs for a page at the head of the queue, if the use bit `R` is set (i.e. equal to `1`), then this bit is reset to zero, and the page itself is moved to the end of the queue. After that, the next page in the head of the queue is checked. The procedure is repeated until there is a page in the head of the queue with the usage bit set to zero (i.e., zero). This page is unloaded from memory and a new one is loaded in its place.

#### 4.3. Clock
Further modification of the Second Chance algorithm. Since moving pages from the head of the queue to the back is very inefficient, to increase the speed of the Second Chance algorithm, the queue can be implemented as a circular list. In this case, no pages are actually moved from the head to the back of the queue. Instead, the value of the variable that points to the head of the queue is changed. In this algorithm, the pointer variable ("hand" of the clock) always points to the head of the list, i.e. to the page that loaded first.

#### 4.4. Random
The choice of the replaced page is carried out using a random number generator with a uniform distribution. Thus, when a page missing error occurs, the probability of being paged is the same for all pages.

#### 4.5. NRU, Not Recently Used
According to the values of the use and modification bits (`R` and `M`), each page frame is assigned to one of 4 classes:
- class 0, the page has not been used and has not been modified;
- class 1, the page has not been used, but has been modified;
- class 2, the page has been used but not modified;
- class 3, the page has been used and modified.

Delete the page belonging to the class with the smallest number. If there are several such pages, one of them is chosen randomly.

#### 4.6. LRU, Least Recently Used
All pages are sorted in the order in which they are accessed. In the head of the list is the page that was last accessed (just now). At the end of the list is the page that was accessed before everyone else (long ago). With each memory access, the list is rebuilt. When a page out error occurs, the page is removed from the end of the queue.

#### 4.7. NFU, Not Frequently Used
Modification of the LRU algorithm, designed to improve its performance by eliminating the permutation of the list elements during each memory access operation. Instead, a counter is used for each of the pages, the initial value of which is zero. Each time the timer is interrupted, the operating system adds to the value of each page counter the value of the page's usage (`R`) bit. In case of a page missing error, the page with the minimum counter value is deleted. If there are multiple pages with the same minimum counter value, you must select one of them at random.

#### 4.8. Aging
Further modification of the LRU and NFU algorithms. For each page loaded into memory, a counter is started. On a timer interrupt, the counters of all pages are shifted to the right by 1 bit. Then, the most significant (left) bit of the counter is set to the value of the use bit (`R`) of that page. In case of a page missing error, the page with the minimum counter value is deleted. If there are multiple pages with the same minimum counter value, you must select one of them at random.

#### 4.9. Working set
When a page fault occurs, the OS scans the entire page table (page frame). For each page that has its usage bit set (`R = 1`), a separate field in the page table (special counter) records the current system time. Next, the "age" of each page is calculated as the difference between the current value of the system time and the value in a separate field (counter) of this page. The page with the highest age is removed. If during the last timer interval there were accesses to all pages, and, accordingly, the age of all pages is equal to zero, one of the pages with the usage bit not set (`M = 0`) is randomly selected for deletion. If all pages in memory have the R and M bits set, then an arbitrary page is randomly deleted. For a page that caused a page fault and was loaded in place of the deleted one, the counter value is set to the current system time.

## Task

In this work, it is necessary to implement a fragment of the memory manager and a part of the operating system functionality responsible for replacing pages when page faults occur. To simplify the work, it is assumed to use a linear inverted page table, the work with which must be implemented in the form of a program. Also, for simplicity, it is assumed that there is only one process in the system, so the process ID is not stored in the inverted page table. The input data is a sequence of memory access operations, the output data is the state of the inverted page table after each memory access operation.

1.  Calculate the variant number from the list in the log and save it to the file [`TASKID.txt`](TASKID.txt) in the repository.
2.  Write a C++ program according to the following specification.
    1. Input data:
         1. Command line argument (number): The number of the page replacement algorithm that the program should use. Takes the values `1` or `2` corresponding to the two page replacement algorithms given by the variant.
         2. A list of memory access instructions read by a program from standard input. There is at most one instruction per line. The instruction consists of two numbers separated by a space, for example: `0 1`. The first number indicates the type of memory access operation: `0` - read and `1` - write. The second number is the number of the virtual page being accessed.
    2. Output data:
         1. For each memory access operation, information about which was received at the input of the program, a string containing the contents of the inverted page table as a sequence of virtual page numbers separated by a space must be generated at the output. If any of the entries in the page table is missing (the page table is not completely filled), instead of the virtual page number, you must print the `#` character.

3. Place all code in the `lab4.cpp` file. The code should compile correctly with `g++ lab4.cpp -o lab4 -std=c++11`. It is highly recommended to use the standard STL library. Containers [`list`](https://en.cppreference.com/w/cpp/container/list), [`vector`](https://en.cppreference.com/w/cpp/container /vector), [`bitset`](https://en.cppreference.com/w/cpp/utility/bitset), etc. 
4. If the `R` bit is used in the page replacement algorithm, then it is necessary to implement emulation of a timer interrupt. To do this, every 5 memory access operations, it is necessary to run the handler for this interrupt. The `R` bits are cleared by a timer interrupt.
5. For algorithms that use a counter (NFU, Aging): if several pages have the same counter value, one of them is chosen at random. When a page is reloaded into memory, its counter is reset to zero. In the aging algorithm, the counter has a size of 1 byte. In the NFU algorithm, the counter has a size of at least 4 bytes.
6. In all algorithms that use a random number generator (Random, NRU, NFU, Aging, ...), it is allowed to use **only** the `int uniform_rnd(int a, int b)` function declared in the file [`lab4 .h`](lab4.h). This function generates a random evenly distributed integer from the range `[a, b]`. Using other functions to work with random numbers is prohibited!
7. The number of memory access instructions processed since the program was started should be used as the system time in the working set algorithm.
8. After successfully passing the local tests, you need to upload the code to the github repository.
9. Draw conclusions about the effectiveness of the implemented page replacement algorithms. Compare the number of page faults generated on test data using each algorithm.
<!-- Алгоритмы.
+FIFO
+Random
+Second chance (FIFO extension with R bit)
+Clock (Second chance extension with circular list)
LRU (linked list reordered on each memory access, extremely slow)
+NFU (R bit added to a counter on each timer interrupt)
+Aging (shift right, than add R bit to counter on each timer interrupt)
+NRU (4 класса по битам R и M)

Для алгоритмов, использующих счетчик (NFU, Aging): если несколько страниц имеют одинаковое значение счетчика, одна из них выбирается случайным образом. При повторной загрузке страницы в память ее счетчик обнуляется. 
В Aging счетчик имеет размер 1 байт. В NFU счетчик должен меть размер 8 байт.
Для генерации случайных чисел использовать только функцию `int uniform_rnd(int a, int b)`, объявленную в файле `lab4.h`.

Working Set
WSClock (Working Set Clock)
-->

## Комментарии и примеры

### Структуры данных и алгоритмы
The simplified linear posting page table that you will use in this lab has the following structure:

| PPN | VPN | `R` | `M` | Счетчик |
| --: | --: | --: | --: |   --:   |
|  0 | 21 | 0 | 0 | `0b00001001` | 
|  1 |  7 | 0 | 1 | `0b01000000` | 
|  2 | 12 | 1 | 0 | `0b10000000` | 
|  3 |  0 | 1 | 1 | `0b00000010` | 
|  4 |  1 | 0 | 0 | `0b01000000` | 
|  5 | 34 | 1 | 0 | `0b00000001` | 
|  6 | 33 | 0 | 0 | `0b00000000` | 
|  7 |  - | - | - |            - | 

where 
- PPN, physical page number - physical page number (page block); may not be stored explicitly, since it matches the record number in the table;
- VPN - virtual page number;
- `R` - use (reference) bit;
- `M` - modification bit;
- Counter - an additional numeric field that can be used in NFU, Aging, Working set algorithms; here, as an example, the possible binary values of an 8-bit counter using the aging algorithm are shown.

The software implementation of this table, depending on the page replacement algorithm used, can be implemented as an array, list, queue, as well as a combination of these and other data structures. Working with individual bits can be avoided by using boolean variables, because the simulated page tables are small and there is no need to conserve memory. You can also use the standard library template [`bitset`](https://en.cppreference.com/w/cpp/utility/bitset) for this purpose.


### Sample input and output
Let there be a computer with a certain amount of RAM and an installed operating system that uses the virtual memory mechanism. The physical memory available in RAM is divided into 5 page blocks. The operating system or user starts the process. After the process starts, it sequentially accesses the addresses in memory located on the following virtual pages (second column) to perform read (`0` in the first column) or write (`1` in the first column) operations:
```
0 0
0 1
0 2
0 3
0 4
0 5
0 0
1 0
0 5
1 0
```

The order of filling the page table in this case might look like this:
```
0 # # # #
0 1 # # #
0 1 2 # #
0 1 2 3 #
0 1 2 3 4
5 1 2 3 4
5 0 2 3 4
5 0 2 3 4
5 0 2 3 4
5 0 2 3 4
```
Here, each row represents the contents of the page table after the memory access operation has been performed. The `#` character means an empty page block. Initially, the entire inverted page table is empty (no page frame is used), which can be represented as `# # # # #`. On the sixth memory access (virtual page `5`), a page fault occurs and the page replacement algorithm starts because there are no more free spaces in the inverted page table. This algorithm evicts virtual page `0` from memory and places the requested virtual page `5` in its place. On the next memory access, the just deleted virtual page `0` is requested and it has to be reloaded into memory, after deleting the virtual page `1`. Subsequent memory accesses do not result in page faults and, at first glance, the state of the page table does not change. In fact, this is not the case: unlike the previous memory accesses, which did not change the data, the eighth and tenth requests change the data on the virtual page `0`. The consequence of this is to set the `M` bit for the given page. Also, every few memory accesses, a timer interrupt is emulated and the `R` bits of all pages are reset. As part of the assignment for this lab, you need to display only the virtual page numbers that are in the inverted page table. The values ​​of the `R` and `M` bits are not displayed, but may be taken into account for the correct operation of one or another page replacement algorithm.

The above shows examples of inputs and outputs for a program to be developed in this lab. Let's also consider an example of what the data for debugging a program might look like.
**NB!** Below are sample outputs that can be used to debug algorithms, but may not be the final result of the program, because. do not match the description of the output.
```
0(1,0) #(0,0) #(0,0) #(0,0) #(0,0)
0(1,0) 1(1,0) #(0,0) #(0,0) #(0,0)
0(1,0) 1(1,0) 2(1,0) #(0,0) #(0,0)
0(1,0) 1(1,0) 2(1,0) 3(1,0) #(0,0)
0(0,0) 1(0,0) 2(0,0) 3(0,0) 4(0,0)
5(1,0) 1(0,0) 2(0,0) 3(0,0) 4(0,0)
5(1,0) 0(1,0) 2(0,0) 3(0,0) 4(0,0)
5(1,0) 0(1,1) 2(0,0) 3(0,0) 4(0,0)
5(1,0) 0(1,1) 2(0,0) 3(0,0) 4(0,0)
5(0,0) 0(0,1) 2(0,0) 3(0,0) 4(0,0)
```
In this example, the values ​​of the `R` and `M` bits are indicated in brackets separated by commas after the virtual page number. When loading a new page, as well as when accessing a page already in memory, its `R` bit is set to one. If the page is accessed to write data, then the `M` bit is also set to one. In this example, the timer interrupt emulation occurs after every fifth memory access and causes the `R` bits of all pages loaded into memory to be reset.

In the above example, the page replacement algorithm did not use the counter, so it was not displayed during debugging. Let's now consider an example of how the data for debugging a program may look like when using another page replacement algorithm, namely, the aging algorithm. An 8-bit counter value has been added for each entry in the page table, which can help when debugging the program.
```
0(1,0,0b00000000) #(0,0,0b00000000) #(0,0,0b00000000) #(0,0,0b00000000) #(0,0,0b00000000)
0(1,0,0b00000000) 1(1,0,0b00000000) #(0,0,0b00000000) #(0,0,0b00000000) #(0,0,0b00000000)
0(1,0,0b00000000) 1(1,0,0b00000000) 2(1,0,0b00000000) #(0,0,0b00000000) #(0,0,0b00000000)
0(1,0,0b00000000) 1(1,0,0b00000000) 2(1,0,0b00000000) 3(1,0,0b00000000) #(0,0,0b00000000)
0(0,0,0b10000000) 1(0,0,0b10000000) 2(0,0,0b10000000) 3(0,0,0b10000000) 4(0,0,0b10000000)
0(0,0,0b10000000) 1(0,0,0b10000000) 5(1,0,0b00000000) 3(0,0,0b10000000) 4(0,0,0b10000000)
0(1,0,0b10000000) 1(0,0,0b10000000) 5(1,0,0b00000000) 3(0,0,0b10000000) 4(0,0,0b10000000)
0(1,1,0b10000000) 1(0,0,0b10000000) 5(1,0,0b00000000) 3(0,0,0b10000000) 4(0,0,0b10000000)
0(1,1,0b10000000) 1(0,0,0b10000000) 5(1,0,0b00000000) 3(0,0,0b10000000) 4(0,0,0b10000000)
0(0,1,0b11000000) 1(0,0,0b01000000) 5(0,0,0b10000000) 3(0,0,0b01000000) 4(0,0,0b01000000)
```
It should be noted that this snapshot of the page table, obtained at the end of the program, differs from that shown in the previous example. The use of a different page replacement algorithm affected which pages were unloaded. In this case, on the sixth memory access to virtual page `5`, all pages had the same age, and virtual page `2` was randomly selected for eviction.

## Building and testing
The program must be compiled with `g++ lab4.cpp -o lab4 -std=c++11`. Tests are run with the `./tests.sh` command. For debugging the program, it is recommended to display additional information about the state of the page table at each step: the values of the usage and modification bits, the value of the counter, etc. Before running tests, the output of redundant debugging information should be disabled.

## Variants
| Variant number | Number of page blocks | Algorithm 1 | Algorithm 2 |
| ---:           |           ------:           |           ------:           |         ------:       |
|  1 |  8 | FIFO          | Aging |
|  2 |  6 | Second chance | NFU |
|  3 |  8 | Clock         | NRU |
|  4 |  6 | Random        | LRU |
|  5 |  6 | FIFO          | NFU |
|  6 |  8 | Second chance | NRU |
|  7 |  6 | Clock         | LRU |
|  8 |  8 | Random        | Aging |
|  9 | 10 | FIFO          | NRU |
| 10 | 12 | Second chance | LRU |
| 11 | 10 | Clock         | Aging |
| 12 | 12 | Random        | NFU |
| 13 | 12 | FIFO          | LRU |
| 14 | 10 | Second chance | Aging |
| 15 | 12 | Clock         | NFU |
| 16 |  5 | FIFO          | Working set |
| 17 |  5 | Second chance | Working set |
| 18 | 10 | Random        | NRU |
| 19 |  5 | Clock         | Working set |
| 20 |  5 | Random        | Working set |
| 21 | 14 | FIFO          | Aging |
| 22 | 16 | Second chance | NFU |
| 23 | 14 | Clock         | NRU |
| 24 | 16 | Random        | LRU |
| 25 | 16 | FIFO          | NFU |
| 26 | 14 | Second chance | NRU |
| 27 | 16 | Clock         | LRU |
| 28 | 14 | Random        | Aging |
| 29 | 18 | FIFO          | NRU |
| 30 | 18 | Second chance | LRU |

