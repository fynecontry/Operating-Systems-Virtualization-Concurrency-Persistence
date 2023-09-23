# Simulator
The program, [malloc.py](https://github.com/fynecontry/Operating-Systems-Virtualization-Concurrency-Persistence/blob/main/virtualization/free_space_management/malloc.py), lets you explore the behavior of a simple free-space allocator as described in the chapter. See the README-malloc for details of its basic operation.

## Questions
#### 1. First, run with the flags `-n 10 -H 0 -p BEST -s 0` to generate a few random allocations and frees. Can you predict what alloc()/free() will return? Can you guess the state of the free list after each request? What do you notice about the free list over time?
```
# ./malloc.py -n 10 -H 0 -p BEST -s 0 -c

ptr[0] = Alloc(3)  returned 1000 (searched 1 elements)
Free List [ Size 1 ]:  [ addr:1003 sz:97 ] 

Free(ptr[0]) returned 0
Free List [ Size 2 ]:  [ addr:1000 sz:3 ] [ addr:1003 sz:97 ] 

ptr[1] = Alloc(5)  returned 1003 (searched 2 elements)
Free List [ Size 2 ]:  [ addr:1000 sz:3 ] [ addr:1008 sz:92 ] 

Free(ptr[1]) returned 0
Free List [ Size 3 ]:  [ addr:1000 sz:3 ] [ addr:1003 sz:5 ] [ addr:1008 sz:92 ] 

ptr[2] = Alloc(8)  returned 1008 (searched 3 elements)
Free List [ Size 3 ]:  [ addr:1000 sz:3 ] [ addr:1003 sz:5 ] [ addr:1016 sz:84 ] 

Free(ptr[2]) returned 0
Free List [ Size 4 ]:  [ addr:1000 sz:3 ] [ addr:1003 sz:5 ] [ addr:1008 sz:8 ] [ addr:1016 sz:84 ] 

ptr[3] = Alloc(8)  returned 1008 (searched 4 elements)
Free List [ Size 3 ]:  [ addr:1000 sz:3 ] [ addr:1003 sz:5 ] [ addr:1016 sz:84 ] 

Free(ptr[3]) returned 0
Free List [ Size 4 ]:  [ addr:1000 sz:3 ] [ addr:1003 sz:5 ] [ addr:1008 sz:8 ] [ addr:1016 sz:84 ] 

ptr[4] = Alloc(2)  returned 1000 (searched 4 elements)
Free List [ Size 4 ]:  [ addr:1002 sz:1 ] [ addr:1003 sz:5 ] [ addr:1008 sz:8 ] [ addr:1016 sz:84 ] 

ptr[5] = Alloc(7)  returned 1008 (searched 4 elements)
Free List [ Size 4 ]:  [ addr:1002 sz:1 ] [ addr:1003 sz:5 ] [ addr:1015 sz:1 ] [ addr:1016 sz:84 ]
```

#### 2. How are the results different when using a WORST fit policy to search the free list (-p WORST)? What changes?
> Lot's of fragmentation
```
# ./malloc.py -n 10 -H 0 -p WORST -s 0 -c

ptr[0] = Alloc(3)  returned 1000 (searched 1 elements)
Free List [ Size 1 ]:  [ addr:1003 sz:97 ] 

Free(ptr[0]) returned 0
Free List [ Size 2 ]:  [ addr:1000 sz:3 ] [ addr:1003 sz:97 ] 

ptr[1] = Alloc(5)  returned 1003 (searched 2 elements)
Free List [ Size 2 ]:  [ addr:1000 sz:3 ] [ addr:1008 sz:92 ] 

Free(ptr[1]) returned 0
Free List [ Size 3 ]:  [ addr:1000 sz:3 ] [ addr:1003 sz:5 ] [ addr:1008 sz:92 ] 

ptr[2] = Alloc(8)  returned 1008 (searched 3 elements)
Free List [ Size 3 ]:  [ addr:1000 sz:3 ] [ addr:1003 sz:5 ] [ addr:1016 sz:84 ] 

Free(ptr[2]) returned 0
Free List [ Size 4 ]:  [ addr:1000 sz:3 ] [ addr:1003 sz:5 ] [ addr:1008 sz:8 ] [ addr:1016 sz:84 ] 

ptr[3] = Alloc(8)  returned 1016 (searched 4 elements)
Free List [ Size 4 ]:  [ addr:1000 sz:3 ] [ addr:1003 sz:5 ] [ addr:1008 sz:8 ] [ addr:1024 sz:76 ] 

Free(ptr[3]) returned 0
Free List [ Size 5 ]:  [ addr:1000 sz:3 ] [ addr:1003 sz:5 ] [ addr:1008 sz:8 ] [ addr:1016 sz:8 ] [ addr:1024 sz:76 ] 

ptr[4] = Alloc(2)  returned 1024 (searched 5 elements)
Free List [ Size 5 ]:  [ addr:1000 sz:3 ] [ addr:1003 sz:5 ] [ addr:1008 sz:8 ] [ addr:1016 sz:8 ] [ addr:1026 sz:74 ] 

ptr[5] = Alloc(7)  returned 1026 (searched 5 elements)
Free List [ Size 5 ]:  [ addr:1000 sz:3 ] [ addr:1003 sz:5 ] [ addr:1008 sz:8 ] [ addr:1016 sz:8 ] [ addr:1033 sz:67 ] 
```

#### 3. What about when using FIRST fit (-p FIRST)? What speeds up when you use first fit?
> In this case with the parameters **-n 10 -H 0 -p FIRST -s 0** - results same as -p BEST fit

#### 4. For the above questions, how the list is kept ordered can affect the time it takes to find a free location for some of the policies. Use the different free list orderings (`-l ADDRSORT`, `-l SIZESORT+`, `-l SIZESORT-`) to see how the policies and the list orderings interact.
**SIZESORT-**
```
# ./malloc.py -n 10 -H 0 -p BEST -s 0 -l SIZESORT- -c

ptr[0] = Alloc(3)  returned 1000 (searched 1 elements)
Free List [ Size 1 ]:  [ addr:1003 sz:97 ] 

Free(ptr[0]) returned 0
Free List [ Size 2 ]:  [ addr:1003 sz:97 ] [ addr:1000 sz:3 ] 

ptr[1] = Alloc(5)  returned 1003 (searched 2 elements)
Free List [ Size 2 ]:  [ addr:1008 sz:92 ] [ addr:1000 sz:3 ] 

Free(ptr[1]) returned 0
Free List [ Size 3 ]:  [ addr:1008 sz:92 ] [ addr:1003 sz:5 ] [ addr:1000 sz:3 ] 

ptr[2] = Alloc(8)  returned 1008 (searched 3 elements)
Free List [ Size 3 ]:  [ addr:1016 sz:84 ] [ addr:1003 sz:5 ] [ addr:1000 sz:3 ] 

Free(ptr[2]) returned 0
Free List [ Size 4 ]:  [ addr:1016 sz:84 ] [ addr:1008 sz:8 ] [ addr:1003 sz:5 ] [ addr:1000 sz:3 ] 

ptr[3] = Alloc(8)  returned 1008 (searched 4 elements)
Free List [ Size 3 ]:  [ addr:1016 sz:84 ] [ addr:1003 sz:5 ] [ addr:1000 sz:3 ] 

Free(ptr[3]) returned 0
Free List [ Size 4 ]:  [ addr:1016 sz:84 ] [ addr:1008 sz:8 ] [ addr:1003 sz:5 ] [ addr:1000 sz:3 ] 

ptr[4] = Alloc(2)  returned 1000 (searched 4 elements)
Free List [ Size 4 ]:  [ addr:1016 sz:84 ] [ addr:1008 sz:8 ] [ addr:1003 sz:5 ] [ addr:1002 sz:1 ] 

ptr[5] = Alloc(7)  returned 1008 (searched 4 elements)
Free List [ Size 4 ]:  [ addr:1016 sz:84 ] [ addr:1015 sz:1 ] [ addr:1003 sz:5 ] [ addr:1002 sz:1 ]
```
**INSERT-FRONT with FIRST-FIT**
```
# ./malloc.py -n 10 -H 0 -p FIRST -s 0 -l INSERT-FRONT -c

ptr[0] = Alloc(3)  returned 1000 (searched 1 elements)
Free List [ Size 1 ]:  [ addr:1003 sz:97 ] 

Free(ptr[0]) returned 0
Free List [ Size 2 ]:  [ addr:1000 sz:3 ] [ addr:1003 sz:97 ] 

ptr[1] = Alloc(5)  returned 1003 (searched 2 elements)
Free List [ Size 2 ]:  [ addr:1000 sz:3 ] [ addr:1008 sz:92 ] 

Free(ptr[1]) returned 0
Free List [ Size 3 ]:  [ addr:1003 sz:5 ] [ addr:1000 sz:3 ] [ addr:1008 sz:92 ] 

ptr[2] = Alloc(8)  returned 1008 (searched 3 elements)
Free List [ Size 3 ]:  [ addr:1003 sz:5 ] [ addr:1000 sz:3 ] [ addr:1016 sz:84 ] 

Free(ptr[2]) returned 0
Free List [ Size 4 ]:  [ addr:1008 sz:8 ] [ addr:1003 sz:5 ] [ addr:1000 sz:3 ] [ addr:1016 sz:84 ] 

ptr[3] = Alloc(8)  returned 1008 (searched 1 elements)
Free List [ Size 3 ]:  [ addr:1003 sz:5 ] [ addr:1000 sz:3 ] [ addr:1016 sz:84 ] 

Free(ptr[3]) returned 0
Free List [ Size 4 ]:  [ addr:1008 sz:8 ] [ addr:1003 sz:5 ] [ addr:1000 sz:3 ] [ addr:1016 sz:84 ] 

ptr[4] = Alloc(2)  returned 1008 (searched 1 elements)
Free List [ Size 4 ]:  [ addr:1010 sz:6 ] [ addr:1003 sz:5 ] [ addr:1000 sz:3 ] [ addr:1016 sz:84 ] 

ptr[5] = Alloc(7)  returned 1016 (searched 4 elements)
Free List [ Size 4 ]:  [ addr:1010 sz:6 ] [ addr:1003 sz:5 ] [ addr:1000 sz:3 ] [ addr:1023 sz:77 ]
```

#### 5. Coalescing of a free list can be quite important. Increase the number of random allocations (say to -n 1000). What happens to larger allocation requests over time? Run with and without coalescing (i.e., without and with the -C flag). What differences in outcome do you see? How big is the free list over time in each case? Does the ordering of the list matter in this case?

**After ops**
```
ptr[590] = Alloc(9)  returned -1 (searched 31 elements)
Free List [ Size 31 ]:  [ addr:1000 sz:2 ] [ addr:1002 sz:1 ] [ addr:1006 sz:1 ] [ addr:1007 sz:1 ] [ addr:1008 sz:5 ] [ addr:1013 sz:1 ] [ addr:1014 sz:1 ] [ addr:1015 sz:1 ] [ addr:1016 sz:5 ] [ addr:1021 sz:1 ] [ addr:1022 sz:3 ] [ addr:1031 sz:1 ] [ addr:1032 sz:2 ] [ addr:1034 sz:3 ] [ addr:1037 sz:4 ] [ addr:1041 sz:1 ] [ addr:1042 sz:2 ] [ addr:1052 sz:1 ] [ addr:1053 sz:6 ] [ addr:1059 sz:2 ] [ addr:1061 sz:1 ] [ addr:1068 sz:1 ] [ addr:1069 sz:3 ] [ addr:1072 sz:5 ] [ addr:1077 sz:3 ] [ addr:1080 sz:1 ] [ addr:1081 sz:5 ] [ addr:1086 sz:3 ] [ addr:1089 sz:5 ] [ addr:1094 sz:2 ] [ addr:1096 sz:4 ]
```

> With coalescing,
```
Free(ptr[511]) returned 0
Free List [ Size 1 ]:  [ addr:1000 sz:100 ] 

ptr[514] = Alloc(2)  returned 1000 (searched 1 elements)
Free List [ Size 1 ]:  [ addr:1002 sz:98 ]
```


 