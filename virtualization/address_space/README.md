# Summary

We have seen the introduction of a major OS subsystem: **virtual memory**. The VM system is responsible for providing the illusion of a large, sparse, private address space to programs, which hold all of their instructions and data therein. The OS, with some serious hardware help, will take each of these virtual memory references, and turn them into physical addresses, which can be presented to the physical memory in order to fetch the desired information. The OS will do this for many processes at once, making sure to protect programs from one another, as well as protect the OS. The entire approach requires a great deal of mechanism (lots of low-level machinery) as well as some critical policies to work; we’ll start from the bottom up, describing the critical mechanisms first. And thus we proceed!

> ASIDE: EVERY ADDRESS YOU SEE IS VIRTUAL

> Ever write a C program that prints out a pointer? The value you see (some large number, often printed in hexadecimal), is a virtual address. Ever wonder where the code of your program is found? You can print that out too, and yes, if you can print it, it also is a virtual address. In fact, any address you can see as a programmer of a user-level program is a virtual address. It’s only the OS, through its tricky techniques of virtualizing memory, that knows where in the physical memory of the machine these instructions and data values lie. So never forget: if you print out an address in a program, it’s a virtual one, an illusion of how things are laid out in memory; only the OS (and the hardware) knows the real truth.

> Here’s a little program (va.c) that prints out the locations of the main() routine (where code lives), the value of a heap-allocated value returned from malloc(), and the location of an integer on the stack:
```
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
  printf("location of code : %p\n", main);
  printf("location of heap : %p\n", malloc(100e6));
  int x = 3;
  printf("location of stack: %p\n", &x);
  return x;

}
```
When run on a 64-bit Mac, we get the output similar to the following:
```
location of code : 0x1095afe50
location of heap : 0x1096008c0
location of stack: 0x7fff691aea64
```
> From this, you can see that code comes first in the address space, then the heap, and the stack is all the way at the other end of this large virtual space. All of these addresses are virtual, and will be translated by the OS and hardware in order to fetch values from their true physical locations.

## Exercise

#### 1. Create a little program that uses a certain amount of memory using the file called `memory-user.c`. This program should take one command-line argument: the number of megabytes of memory it will use. When run, it should allocate an array, and constantly stream through the array, touching each entry. The program should do this indefinitely, or, perhaps, for a certain amount of time also specified at the command line.
```
$ ./memory-user 10 2
Address of array[7055]: 0x7f0562302e4c
Address of array[7056]: 0x7f0562302e50
Address of array[7057]: 0x7f0562302e54
Address of array[7058]: 0x7f0562302e58
Address of array[7059]: 0x7f0562302e5c
Address of array[7060]: 0x7f0562302e60
Address of array[7061]: 0x7f0562302e64
Timer elpased!
Performing cleanup...
```
##### 2. To use `pmap`, you have to know the processID of the process you’re interested in. Thus, first run `ps auxw` to see a list of all processes; then, pick an interesting one, such as a browser. You can also use your memory-user program in this case (indeed, you can even have that program call getpid() and print out its PID for your convenience).
Now run `pmap` on some of these processes, using various flags (like -X) to reveal many details about the process. What do you see? How many different entities make up a modern address space, as opposed to our simple conception of `code/stack/heap?`
```
USER         PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
root           1  0.0  0.0   4500   692 ?        Ss   17:38   0:00 sh -c cd /.educative/internal_daemon/src && ID_PORT=8890 ED_HOST=
root           6  0.1  1.2 606040 45380 ?        Sl   17:38   0:00 ./server
root          19  0.0  0.0  18232  3196 pts/0    Ss   17:38   0:00 bash
root          40  0.0  0.0  34420  2840 pts/0    R+   17:47   0:00 ps auxw

root@educative:/usr/local/educative# ps auxw
USER         PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
root           1  0.0  0.0   4500   676 ?        Ss   17:54   0:00 sh -c cd /.educative/internal_daemon/src && ID_PORT=8890 ED_HOST=
root           7  1.2  1.1 605208 45088 ?        Sl   17:54   0:00 ./server
root          20  0.0  0.0  18232  3264 pts/0    Ss   17:54   0:00 bash
root          38  0.0  0.0  34420  2832 pts/0    R+   17:54   0:00 ps auxw

 pmap -X 7
7:   ./server
         Address Perm   Offset Device   Inode   Size   Rss   Pss Referenced Anonymous LazyFree ShmemPmdMapped FilePmdMapped Shared_Hugetlb Private_Hugetlb Swap SwapPss Locked THPeligible Mapping
        00400000 r--p 00000000  08:01 1565106   4164  3892  3892       3892         0        0              0             0              0               0    0       0      0           0 server
        00811000 r-xp 00411000  08:01 1565106  19312 15512 15512      15512         0        0              0             0              0               0    0       0      0           0 server
        01c00000 r-xp 016ed000  08:01 1565106      4     0     0          0         0        0              0             0              0               0    0       0      0           0 server
        01c01000 r--p 016ee000  08:01 1565106  13028  6288  6288       6288         0        0              0             0              0               0    0       0      0           0 server
        028bb000 r--p 023a7000  08:01 1565106      8     8     8          8         8        0              0             0              0               0    0       0      0           0 server
        028bd000 rw-p 023a9000  08:01 1565106     72    72    72         72        48        0              0             0              0               0    0       0      0           0 server
        028cf000 rw-p 00000000  00:00       0    140   124   124        124       124        0              0             0              0               0    0       0      0           0 
        0378e000 rw-p 00000000  00:00       0   3588  3496  3496       3496      3496        0              0             0              0               0    0       0      0           0 [heap]
      c53ff80000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
      daae640000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
     1ddd85c0000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
     4711a200000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
     4fca7dc0000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
     55e0ae00000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
     73d68f80000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
     77406200000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
     7f894ac0000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
     7fcb4640000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
     840d9d00000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
     84b5e740000 rw-p 00000000  00:00       0    256   132   132        132       132        0              0             0              0               0    0       0      0           0 
     852720c0000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
     8f4f4840000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
     97e68980000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
     a9264d40000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
     bacc4f40000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
     c7438680000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
     dcb64600000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
     e3946940000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
     ea3b37c0000 rw-p 00000000  00:00       0    260   260   260        260       260        0              0             0              0               0    0       0      0           0 
     f1a52280000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
     fb295c40000 rw-p 00000000  00:00       0    256   240   240        240       240        0              0             0              0               0    0       0      0           0 
     fc859140000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    1091d7b40000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
    10bc9b100000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
    115d55e40000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
    131f5d500000 rw-p 00000000  00:00       0    256   248   248        248       248        0              0             0              0               0    0       0      0           0 
    13766140f000 ---p 00000000  00:00       0    196     0     0          0         0        0              0             0              0               0    0       0      0           0 
    137661440000 rw-p 00000000  00:00       0      4     4     4          4         4        0              0             0              0               0    0       0      0           0 
    137661441000 ---p 00000000  00:00       0      4     0     0          0         0        0              0             0              0               0    0       0      0           0 
    137661442000 r-xp 00000000  00:00       0     84    84    84         84        84        0              0             0              0               0    0       0      0           0 
    137661457000 ---p 00000000  00:00       0    160     0     0          0         0        0              0             0              0               0    0       0      0           0 
    13766147f000 ---p 00000000  00:00       0      4     0     0          0         0        0              0             0              0               0    0       0      0           0 
    137661480000 rw-p 00000000  00:00       0      4     4     4          4         4        0              0             0              0               0    0       0      0           0 
    137661481000 ---p 00000000  00:00       0      4     0     0          0         0        0              0             0              0               0    0       0      0           0 
    137661482000 r-xp 00000000  00:00       0    244    80    80         80        80        0              0             0              0               0    0       0      0           0 
    1376614bf000 ---p 00000000  00:00       0      4     0     0          0         0        0              0             0              0               0    0       0      0           0 
    1376614c0000 rw-p 00000000  00:00       0      4     4     4          4         4        0              0             0              0               0    0       0      0           0 
    1376614c1000 ---p 00000000  00:00       0      4     0     0          0         0        0              0             0              0               0    0       0      0           0 
    1376614c2000 r-xp 00000000  00:00       0      4     4     4          4         4        0              0             0              0               0    0       0      0           0 
    1376614c3000 ---p 00000000  00:00       0    244     0     0          0         0        0              0             0              0               0    0       0      0           0 
    137661500000 rw-p 00000000  00:00       0      4     4     4          4         4        0              0             0              0               0    0       0      0           0 
    137661501000 ---p 00000000  00:00       0      4     0     0          0         0        0              0             0              0               0    0       0      0           0 
    137661502000 r-xp 00000000  00:00       0      4     4     4          4         4        0              0             0              0               0    0       0      0           0 
    137661503000 ---p 00000000  00:00       0    244     0     0          0         0        0              0             0              0               0    0       0      0           0 
    137661540000 rw-p 00000000  00:00       0      4     4     4          4         4        0              0             0              0               0    0       0      0           0 
    137661541000 ---p 00000000  00:00       0      4     0     0          0         0        0              0             0              0               0    0       0      0           0 
    137661542000 r-xp 00000000  00:00       0      4     4     4          4         4        0              0             0              0               0    0       0      0           0 
    137661543000 ---p 00000000  00:00       0 129840     0     0          0         0        0              0             0              0               0    0       0      0           0 
    1459dbc40000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
    14678f2c0000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    149469540000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    151c3d480000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    1587a0340000 rw-p 00000000  00:00       0    144   144   144        144       144        0              0             0              0               0    0       0      0           0 
    15ff5ed00000 rw-p 00000000  00:00       0    256    56    56         56        56        0              0             0              0               0    0       0      0           0 
    16566edc0000 rw-p 00000000  00:00       0    256   212   212        212       212        0              0             0              0               0    0       0      0           0 
    16b205e80000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
    178f13e80000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
    17f64b300000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    18d91c240000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    1957f9480000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
    1a55a2400000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
    1ab1c2740000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    1ac8e9100000 rw-p 00000000  00:00       0    260   260   260        260       260        0              0             0              0               0    0       0      0           0 
    1b1b3c800000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    1b324ddc0000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    1b7381b00000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    1bd839c40000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
    1bf2e33c0000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    1c17f7040000 rw-p 00000000  00:00       0    168   168   168        168       168        0              0             0              0               0    0       0      0           0 
    1d43fe7c0000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
    1e24c6c00000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    1fa30cc00000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
    20ff70840000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    210c29a80000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
    21a77b540000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    21eda1bc0000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    227b4df80000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    239e22f80000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
    23a13e580000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    2437829c0000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    243a997c0000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
    259f87800000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    2660efd40000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    26d56ab80000 rw-p 00000000  00:00       0    256    16    16         16        16        0              0             0              0               0    0       0      0           0 
    2760d4f00000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    27f6f56c0000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
    2853f27c0000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    29575bfc0000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    29c0c9200000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
    2a82dbd80000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
    2c8c36ec0000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    2d7514700000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    2ddf76940000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
    2de6f8e00000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    2e1a97540000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    2e36c6b40000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    2e47f7c00000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    2e64152c0000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    2eaf4ba40000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    2f5d56400000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    309ec3380000 r--p 00000000  00:00       0    148   148   148        148       148        0              0             0              0               0    0       0      0           0 
    30adf7940000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
    31ae51400000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
    31c624d00000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    322d79fc0000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    32ba5cac0000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    33f4f1000000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
    3435b2ec0000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    34b1ffb80000 rw-p 00000000  00:00       0      4     4     4          4         4        0              0             0              0               0    0       0      0           0 
    34c06de40000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
    35b9f1080000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    360a30c80000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    366e879c0000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    380a22dc0000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    3853289c0000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    38aabfb80000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    398852f00000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    39ee3c280000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
    3a039f380000 rw-p 00000000  00:00       0    256     4     4          4         4        0              0             0              0               0    0       0      0           0 
    3a08ea780000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    3afc5f400000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    3c20f5200000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
    3d3677500000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    3d7cce900000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
    3da1e0540000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    3e0ca2480000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    3e7b00580000 ---p 00000000  00:00       0    256     0     0          0         0        0              0             0              0               0    0       0      0           0 
    3f5293ec0000 rw-p 00000000  00:00       0    256   256   256        256       256        0              0             0              0               0    0       0      0           0 
    7f7c82dbc000 ---p 00000000  00:00       0      4     0     0          0         0        0              0             0              0               0    0       0      0           0 
    7f7c82dbd000 rw-p 00000000  00:00       0   8192     8     8          8         8        0              0             0              0               0    0       0      0           0 
    7f7c835bd000 r-xp 00000000  07:04   30524     44    44    22         44         0        0              0             0              0               0    0       0      0           0 libnss_files-2.23.so
    7f7c835c8000 ---p 0000b000  07:04   30524   2044     0     0          0         0        0              0             0              0               0    0       0      0           0 libnss_files-2.23.so
    7f7c837c7000 r--p 0000a000  07:04   30524      4     4     4          4         4        0              0             0              0               0    0       0      0           0 libnss_files-2.23.so
    7f7c837c8000 rw-p 0000b000  07:04   30524      4     4     4          4         4        0              0             0              0               0    0       0      0           0 libnss_files-2.23.so
    7f7c837c9000 rw-p 00000000  00:00       0     24     0     0          0         0        0              0             0              0               0    0       0      0           0 
    7f7c837cf000 r-xp 00000000  07:04   30528     44    44    22         44         0        0              0             0              0               0    0       0      0           0 libnss_nis-2.23.so
    7f7c837da000 ---p 0000b000  07:04   30528   2044     0     0          0         0        0              0             0              0               0    0       0      0           0 libnss_nis-2.23.so
    7f7c839d9000 r--p 0000a000  07:04   30528      4     4     4          4         4        0              0             0              0               0    0       0      0           0 libnss_nis-2.23.so
    7f7c839da000 rw-p 0000b000  07:04   30528      4     4     4          4         4        0              0             0              0               0    0       0      0           0 libnss_nis-2.23.so
    7f7c839db000 r-xp 00000000  07:04   30518     88    64    32         64         0        0              0             0              0               0    0       0      0           0 libnsl-2.23.so
    7f7c839f1000 ---p 00016000  07:04   30518   2044     0     0          0         0        0              0             0              0               0    0       0      0           0 libnsl-2.23.so
    7f7c83bf0000 r--p 00015000  07:04   30518      4     4     4          4         4        0              0             0              0               0    0       0      0           0 libnsl-2.23.so
    7f7c83bf1000 rw-p 00016000  07:04   30518      4     4     4          4         4        0              0             0              0               0    0       0      0           0 libnsl-2.23.so
    7f7c83bf2000 rw-p 00000000  00:00       0      8     0     0          0         0        0              0             0              0               0    0       0      0           0 
    7f7c83bf4000 r-xp 00000000  07:04   30520     32    32    16         32         0        0              0             0              0               0    0       0      0           0 libnss_compat-2.23.so
    7f7c83bfc000 ---p 00008000  07:04   30520   2044     0     0          0         0        0              0             0              0               0    0       0      0           0 libnss_compat-2.23.so
    7f7c83dfb000 r--p 00007000  07:04   30520      4     4     4          4         4        0              0             0              0               0    0       0      0           0 libnss_compat-2.23.so
    7f7c83dfc000 rw-p 00008000  07:04   30520      4     4     4          4         4        0              0             0              0               0    0       0      0           0 libnss_compat-2.23.so
    7f7c83dfd000 r-xp 00000000  07:04   30541      8     8     8          8         0        0              0             0              0               0    0       0      0           0 libutil-2.23.so
    7f7c83dff000 ---p 00002000  07:04   30541   2044     0     0          0         0        0              0             0              0               0    0       0      0           0 libutil-2.23.so
    7f7c83ffe000 r--p 00001000  07:04   30541      4     4     4          4         4        0              0             0              0               0    0       0      0           0 libutil-2.23.so
    7f7c83fff000 rw-p 00002000  07:04   30541      4     4     4          4         4        0              0             0              0               0    0       0      0           0 libutil-2.23.so
    7f7c84000000 rw-p 00000000  00:00       0    648   384   384        384       384        0              0             0              0               0    0       0      0           0 
    7f7c840a2000 ---p 00000000  00:00       0  64888     0     0          0         0        0              0             0              0               0    0       0      0           0 
    7f7c88000000 rw-p 00000000  00:00       0   1004   132   132        132       132        0              0             0              0               0    0       0      0           0 
    7f7c880fb000 ---p 00000000  00:00       0  64532     0     0          0         0        0              0             0              0               0    0       0      0           0 
    7f7c8c000000 rw-p 00000000  00:00       0    884   132   132        132       132        0              0             0              0               0    0       0      0           0 
    7f7c8c0dd000 ---p 00000000  00:00       0  64652     0     0          0         0        0              0             0              0               0    0       0      0           0 
    7f7c90000000 rw-p 00000000  00:00       0    200   192   192        192       192        0              0             0              0               0    0       0      0           0 
    7f7c90032000 ---p 00000000  00:00       0  65336     0     0          0         0        0              0             0              0               0    0       0      0           0 
    7f7c94000000 rw-p 00000000  00:00       0    132     4     4          4         4        0              0             0              0               0    0       0      0           0 
    7f7c94021000 ---p 00000000  00:00       0  65404     0     0          0         0        0              0             0              0               0    0       0      0           0 
    7f7c981af000 r-xp 00000000  08:01 1290509     24    24    24         24         0        0              0             0              0               0    0       0      0           0 node.abi83.node
    7f7c981b5000 ---p 00006000  08:01 1290509   2044     0     0          0         0        0              0             0              0               0    0       0      0           0 node.abi83.node
    7f7c983b4000 r--p 00005000  08:01 1290509      4     4     4          4         4        0              0             0              0               0    0       0      0           0 node.abi83.node
    7f7c983b5000 rw-p 00006000  08:01 1290509      4     4     4          4         4        0              0             0              0               0    0       0      0           0 node.abi83.node
    7f7c983b6000 ---p 00000000  00:00       0      4     0     0          0         0        0              0             0              0               0    0       0      0           0 
    7f7c983b7000 rw-p 00000000  00:00       0   8192    16    16         16        16        0              0             0              0               0    0       0      0           0 
    7f7c98bb7000 ---p 00000000  00:00       0      4     0     0          0         0        0              0             0              0               0    0       0      0           0 
    7f7c98bb8000 rw-p 00000000  00:00       0   8192    16    16         16        16        0              0             0              0               0    0       0      0           0 
    7f7c993b8000 ---p 00000000  00:00       0      4     0     0          0         0        0              0             0              0               0    0       0      0           0 
    7f7c993b9000 rw-p 00000000  00:00       0   8192    16    16         16        16        0              0             0              0               0    0       0      0           0 
    7f7c99bb9000 ---p 00000000  00:00       0      4     0     0          0         0        0              0             0              0               0    0       0      0           0 
    7f7c99bba000 rw-p 00000000  00:00       0   8192    16    16         16        16        0              0             0              0               0    0       0      0           0 
    7f7c9a3ba000 ---p 00000000  00:00       0      4     0     0          0         0        0              0             0              0               0    0       0      0           0 
    7f7c9a3bb000 rw-p 00000000  00:00       0   8192    12    12         12        12        0              0             0              0               0    0       0      0           0 
    7f7c9abbb000 r-xp 00000000  07:04   30503   1792  1400   447       1400         0        0              0             0              0               0    0       0      0           0 libc-2.23.so
    7f7c9ad7b000 ---p 001c0000  07:04   30503   2048     0     0          0         0        0              0             0              0               0    0       0      0           0 libc-2.23.so
    7f7c9af7b000 r--p 001c0000  07:04   30503     16    16    16         16        16        0              0             0              0               0    0       0      0           0 libc-2.23.so
    7f7c9af7f000 rw-p 001c4000  07:04   30503      8     8     8          8         8        0              0             0              0               0    0       0      0           0 libc-2.23.so
    7f7c9af81000 rw-p 00000000  00:00       0     16    16    16         16        16        0              0             0              0               0    0       0      0           0 
    7f7c9af85000 r-xp 00000000  07:04   30533     96    96    48         96         0        0              0             0              0               0    0       0      0           0 libpthread-2.23.so
    7f7c9af9d000 ---p 00018000  07:04   30533   2044     0     0          0         0        0              0             0              0               0    0       0      0           0 libpthread-2.23.so
    7f7c9b19c000 r--p 00017000  07:04   30533      4     4     4          4         4        0              0             0              0               0    0       0      0           0 libpthread-2.23.so
    7f7c9b19d000 rw-p 00018000  07:04   30533      4     4     4          4         4        0              0             0              0               0    0       0      0           0 libpthread-2.23.so
    7f7c9b19e000 rw-p 00000000  00:00       0     16     4     4          4         4        0              0             0              0               0    0       0      0           0 
    7f7c9b1a2000 r-xp 00000000  07:04   41572     88    84    84         84         0        0              0             0              0               0    0       0      0           0 libgcc_s.so.1
    7f7c9b1b8000 ---p 00016000  07:04   41572   2044     0     0          0         0        0              0             0              0               0    0       0      0           0 libgcc_s.so.1
    7f7c9b3b7000 rw-p 00015000  07:04   41572      4     4     4          4         4        0              0             0              0               0    0       0      0           0 libgcc_s.so.1
    7f7c9b3b8000 r-xp 00000000  07:04   30513   1056   364   364        364         0        0              0             0              0               0    0       0      0           0 libm-2.23.so
    7f7c9b4c0000 ---p 00108000  07:04   30513   2044     0     0          0         0        0              0             0              0               0    0       0      0           0 libm-2.23.so
    7f7c9b6bf000 r--p 00107000  07:04   30513      4     4     4          4         4        0              0             0              0               0    0       0      0           0 libm-2.23.so
    7f7c9b6c0000 rw-p 00108000  07:04   30513      4     4     4          4         4        0              0             0              0               0    0       0      0           0 libm-2.23.so
    7f7c9b6c1000 r-xp 00000000  07:04   10031   1480  1256  1256       1256         0        0              0             0              0               0    0       0      0           0 libstdc++.so.6.0.21
    7f7c9b833000 ---p 00172000  07:04   10031   2048     0     0          0         0        0              0             0              0               0    0       0      0           0 libstdc++.so.6.0.21
    7f7c9ba33000 r--p 00172000  07:04   10031     40    40    40         40        40        0              0             0              0               0    0       0      0           0 libstdc++.so.6.0.21
    7f7c9ba3d000 rw-p 0017c000  07:04   10031      8     8     8          8         8        0              0             0              0               0    0       0      0           0 libstdc++.so.6.0.21
    7f7c9ba3f000 rw-p 00000000  00:00       0     16    16    16         16        16        0              0             0              0               0    0       0      0           0 
    7f7c9ba43000 r-xp 00000000  07:04   30509     12    12     3         12         0        0              0             0              0               0    0       0      0           0 libdl-2.23.so
    7f7c9ba46000 ---p 00003000  07:04   30509   2044     0     0          0         0        0              0             0              0               0    0       0      0           0 libdl-2.23.so
    7f7c9bc45000 r--p 00002000  07:04   30509      4     4     4          4         4        0              0             0              0               0    0       0      0           0 libdl-2.23.so
    7f7c9bc46000 rw-p 00003000  07:04   30509      4     4     4          4         4        0              0             0              0               0    0       0      0           0 libdl-2.23.so
    7f7c9bc47000 r-xp 00000000  07:04   30496    152   152    40        152         0        0              0             0              0               0    0       0      0           0 ld-2.23.so
    7f7c9be60000 rw-p 00000000  00:00       0     28    28    28         28        28        0              0             0              0               0    0       0      0           0 
    7f7c9be6c000 r--p 00025000  07:04   30496      4     4     4          4         4        0              0             0              0               0    0       0      0           0 ld-2.23.so
    7f7c9be6d000 rw-p 00026000  07:04   30496      4     4     4          4         4        0              0             0              0               0    0       0      0           0 ld-2.23.so
    7f7c9be6e000 rw-p 00000000  00:00       0      4     4     4          4         4        0              0             0              0               0    0       0      0           0 
    7ffd5e5dc000 rw-p 00000000  00:00       0    132    44    44         44        44        0              0             0              0               0    0       0      0           0 [stack]
    7ffd5e747000 r--p 00000000  00:00       0     12     0     0          0         0        0              0             0              0               0    0       0      0           0 [vvar]
    7ffd5e74a000 r-xp 00000000  00:00       0      4     4     0          4         0        0              0             0              0               0    0       0      0           0 [vdso]
ffffffffff600000 r-xp 00000000  00:00       0      4     0     0          0         0        0              0             0              0               0    0       0      0           0 [vsyscall]
                                              ====== ===== ===== ========== ========= ======== ============== ============= ============== =============== ==== ======= ====== =========== 
                                              605212 45264 44046      45264     15964        0              0             0              0               0    0       0      0           0 KB 
```

#### 3. Finally, let’s run `pmap` on your `memory-user` program, with different amounts of used memory. What do you see here? Does the output from pmap match your expectations?
```
$ pmap -X 23877
23877:   ./memory-user 100
         Address Perm   Offset Device   Inode   Size  Rss Pss Referenced Anonymous LazyFree ShmemPmdMapped FilePmdMapped Shared_Hugetlb Private_Hugetlb Swap SwapPss Locked THPeligible Mapping
    561472fbd000 r--p 00000000  08:04 2111467      4    4   4          4         0        0              0             0              0               0    0       0      0           0 memory-user
    561472fbe000 r-xp 00001000  08:04 2111467      4    4   4          4         0        0              0             0              0               0    0       0      0           0 memory-user
    561472fbf000 r--p 00002000  08:04 2111467      4    4   4          4         0        0              0             0              0               0    0       0      0           0 memory-user
    561472fc0000 r--p 00002000  08:04 2111467      4    4   4          4         4        0              0             0              0               0    0       0      0           0 memory-user
    561472fc1000 rw-p 00003000  08:04 2111467      4    4   4          4         4        0              0             0              0               0    0       0      0           0 memory-user
    5614745d7000 rw-p 00000000  00:00       0    132    4   4          4         4        0              0             0              0               0    0       0      0           0 [heap]
    7fdcb2bff000 rw-p 00000000  00:00       0 102404    4   4          4         4        0              0             0              0               0    0       0      0           0 
    7fdcb9000000 r--p 00000000  08:04 3152118    160  160   4        160         0        0              0             0              0               0    0       0      0           0 libc.so.6
    7fdcb9028000 r-xp 00028000  08:04 3152118   1620  868  46        868         0        0              0             0              0               0    0       0      0           0 libc.so.6
    7fdcb91bd000 r--p 001bd000  08:04 3152118    352  124   0        124         0        0              0             0              0               0    0       0      0           0 libc.so.6
    7fdcb9215000 r--p 00214000  08:04 3152118     16   16  16         16        16        0              0             0              0               0    0       0      0           0 libc.so.6
    7fdcb9219000 rw-p 00218000  08:04 3152118      8    8   8          8         8        0              0             0              0               0    0       0      0           0 libc.so.6
    7fdcb921b000 rw-p 00000000  00:00       0     52   20  20         20        20        0              0             0              0               0    0       0      0           0 
    7fdcb93a5000 rw-p 00000000  00:00       0     12    8   8          8         8        0              0             0              0               0    0       0      0           0 
    7fdcb93ba000 rw-p 00000000  00:00       0      8    4   4          4         4        0              0             0              0               0    0       0      0           0 
    7fdcb93bc000 r--p 00000000  08:04 3151781      8    8   0          8         0        0              0             0              0               0    0       0      0           0 ld-linux-x86-64.so.2
    7fdcb93be000 r-xp 00002000  08:04 3151781    168  168  11        168         0        0              0             0              0               0    0       0      0           0 ld-linux-x86-64.so.2
    7fdcb93e8000 r--p 0002c000  08:04 3151781     44   40   0         40         0        0              0             0              0               0    0       0      0           0 ld-linux-x86-64.so.2
    7fdcb93f4000 r--p 00037000  08:04 3151781      8    8   8          8         8        0              0             0              0               0    0       0      0           0 ld-linux-x86-64.so.2
    7fdcb93f6000 rw-p 00039000  08:04 3151781      8    8   8          8         8        0              0             0              0               0    0       0      0           0 ld-linux-x86-64.so.2
    7ffe1fdee000 rw-p 00000000  00:00       0    136   20  20         20        20        0              0             0              0               0    0       0      0           0 [stack]
    7ffe1ff25000 r--p 00000000  00:00       0     16    0   0          0         0        0              0             0              0               0    0       0      0           0 [vvar]
    7ffe1ff29000 r-xp 00000000  00:00       0      8    4   0          4         0        0              0             0              0               0    0       0      0           0 [vdso]
ffffffffff600000 --xp 00000000  00:00       0      4    0   0          0         0        0              0             0              0               0    0       0      0           0 [vsyscall]
                                              ====== ==== === ========== ========= ======== ============== ============= ============== =============== ==== ======= ====== =========== 
                                              105184 1492 181       1492       108        0              0             0              0               0    0       0      0           0 KB 
```