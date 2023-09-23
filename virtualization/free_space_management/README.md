# Summary

### Types of fragmentation
- **External fragmentation:** Is a general problem that arises when a physical memory quickly becomes full of little holes of free space, making it difficult to allocate new segments, or to grow existing ones.
- **Internal fragmentation:** If an allocator hands out chunks of memory bigger than that requested, any unasked for (and thus unused) space in such a chunk is considered internal fragmentation (because the waste occurs inside the allocated unit) and is another example of space waste.

### Splitting and Coalescing
Splitting and coalescing are memory management techniques used to optimize the allocation and deallocation of memory in a dynamic memory allocation system. These techniques are commonly employed by memory managers, such as those used by malloc and free functions in C and other programming languages, to efficiently manage the heap memory.

**Splitting**:
- Splitting is the process of dividing a large free memory block in the heap into smaller blocks to fulfill the memory allocation request from a program.
- When a program requests a certain amount of memory to be allocated (e.g., through the malloc function), the memory manager searches for a suitable free block of memory that is large enough to accommodate the request.
- If the found free block is larger than the requested size, the memory manager splits the free block into two parts: one part is allocated to the program for its use, and the other part remains as a smaller free block in the heap.
- The idea behind splitting is to avoid wasting memory by using the larger free block to fulfill multiple allocation requests instead of just one.

**Coalescing**:
- Coalescing is the process of merging adjacent free memory blocks in the heap to form larger free blocks.
- When a program deallocates or frees a previously allocated memory block (e.g., through the free function), the memory manager marks the corresponding memory block as free and adds it to the list of free blocks.
- To prevent external fragmentation and improve memory utilization, the memory manager may check whether there are any adjacent free blocks in the heap. If it finds any, it merges them into a larger contiguous free block.
- Coalescing helps to create larger free blocks that can be utilized for future allocation requests, thus reducing the likelihood of smaller free blocks that might lead to external fragmentation.

By employing splitting and coalescing, memory managers can optimize the heap memory management and improve overall memory utilization.

### Tracking the size of allocated regions

When a user requests N bytes of memory, the `malloc()` library does not search for just a free chunk of size N. The library does not search for a free chunk of size N; rather, it searches for a free chunk of size N plus the size of the header.
This has to do with how the memory allocators track the size of allocated regions
To accomplish this task, most allocators store a little bit of extra information in a header block which is kept in memory, usually just before the handed-out chunk of memory


In this example, we are examining an allocated block of size 20 bytes, pointed to by ptr; imagine the user called `malloc()` and stored the results in ptr, e.g., `ptr = malloc(20);`.
The header minimally contains the size of the allocated region (in this case, 20); it may also contain additional pointers to speed up deallocation, a magic number to provide additional integrity checking, and other information. Let’s assume a simple header which contains the size of the region and a magic number, like this:
![image](https://github.com/fynecontry/Operating-Systems-Virtualization-Concurrency-Persistence/assets/27024731/e634eff2-26bd-4084-ad0e-5c7a6161b00c)


When the user calls `free(ptr)`, the library then uses simple pointer arithmetic to figure out where the header begins:
```
void free(void *ptr) {
    header_t *hptr = (header_t *) ptr - 1;
    ...
```

After obtaining such a pointer to the header, the library can easily determine whether the magic number matches the expected value as a sanity check (`assert(hptr->magic == 1234567)`) and calculate the total size of the newly-freed region via simple math (i.e., adding the size of the header to the size of the region). Note the small but critical detail in the last sentence: the size of the free region is the size of the header plus the size of the space allocated to the user. Thus, when a user requests N bytes of memory, the library does not search for a free chunk of size N; rather, it searches for a free chunk of size N plus the size of the header.

### Strategies for managing free space
1. Best Fit - first, search through the free list and find chunks of free memory that are as big or bigger than the requested size. Then, return the one that is the smallest in that group of candidates; this is the so-called best-fit chunk (it could be called the smallest fit too). One pass through the free list is enough to find the correct block to return.

2. Worst Fit - Find the largest chunk and return the requested amount, and keep the remaining (large) chunk on the free list. Worst fit tries to thus leave big chunks free instead of lots of small chunks that can arise from a best-fit approach. Once again, however, a full search of free space is required, and thus this approach can be costly. Worse, most studies show that it performs badly, leading to excess fragmentation while still having high overheads.

3. First Fit - finds the first block that is big enough and returns the requested amount to the user. As before, the remaining free space is kept free for subsequent requests.
First fit has the advantage of speed — no exhaustive search of all the free spaces are necessary — but sometimes pollutes the beginning of the free list with small objects. Thus, how the allocator manages the free list’s order becomes an issue. One approach is to use address-based ordering; by keeping the list ordered by the address of the free space, coalescing becomes easier, and fragmentation tends to be reduced.

4. Next Fit - Instead of always beginning the first-fit search at the beginning of the list, the next fit algorithm keeps an extra pointer to the location within the list where one was looking last. The idea is to spread the searches for free space throughout the list more uniformly, thus avoiding splintering of the beginning of the list. The performance of such an approach is quite similar to the first fit, as an exhaustive search is once again avoided.

**Examples**

Here are a few examples of the above strategies. Envision a free list with three elements on it, of sizes 10, 30, and 20 (we’ll ignore headers and other details here, instead just focusing on how strategies operate):
![image](https://github.com/fynecontry/Operating-Systems-Virtualization-Concurrency-Persistence/assets/27024731/2f17fead-864a-4b0d-a3b8-6230bd990214)

Assume an allocation request of size 15. A best-fit approach would search the entire list and find that 20 was the best fit, as it is the smallest free space that can accommodate the request. The resulting free list:
![image](https://github.com/fynecontry/Operating-Systems-Virtualization-Concurrency-Persistence/assets/27024731/d94df8e5-6a4a-46ab-a237-9fa7a8707070)

As happens in this example, and often happens with a best-fit approach, a small free chunk is now leftover​. A worst-fit approach is similar but instead finds the largest chunk, in this example 30. The resulting list:
![image](https://github.com/fynecontry/Operating-Systems-Virtualization-Concurrency-Persistence/assets/27024731/745c9484-0f8c-4306-9a64-30cb154e80aa)

The first-fit strategy, in this example, does the same thing as worst-fit, also finding the first free block that can satisfy the request. The difference is in the search cost; both best-fit and worst-fit look through the entire list; first-fit only examines free chunks until it finds one that fits, thus reducing search cost.


### Segregated lists
- One interesting approach that has been around for some time is the use of segregated lists. The basic idea is simple: if a particular application has one (or a few) popular-sized request that it makes, keep a separate list just to manage objects of that size; all other requests are forwarded to a more general memory allocator.
- The benefits of such an approach are obvious. By having a chunk of memory dedicated to one particular size of requests, fragmentation is much less of a concern; moreover, allocation and free requests can be served quite quickly when they are of the right size, as no complicated search of a list is required.
- Specifically, when the kernel boots up, it allocates a number of object caches for kernel objects that are likely to be requested frequently (such as locks, file-system inodes, etc.); the object caches thus are each segregated free lists of a given size and serve memory allocation and free requests quickly. When a given cache is running low on free space, it requests some slabs of memory from a more general memory allocator (the total amount requested being a multiple of the page size and the object in question). Conversely, when the reference counts of the objects within a given slab all go to zero, the general allocator can reclaim them from the specialized allocator, which is often done when the VM system needs more memory.

### Buddy allocation
In such a system, free memory is first conceptually thought of as one big space of size 2^N . When a request for memory is made, the search for free space recursively divides free space by two until a block that is big enough to accommodate the request is found (and a further split into two would result in a space that is too small). At this point, the requested block is returned to the user. Here is an example of a 64KB free space getting divided in the search for a 7KB block (see figure below).
![image](https://github.com/fynecontry/Operating-Systems-Virtualization-Concurrency-Persistence/assets/27024731/20deff53-9835-44f9-9e6b-cfefd139795b)

- In the example, the leftmost 8KB block is allocated (as indicated by the darker shade of gray) and returned to the user; note that this scheme can suffer from internal fragmentation, as you are only allowed to give out power-of-two-sized blocks.
- The beauty of buddy allocation is found in what happens when that block is freed. When returning the 8KB block to the free list, the allocator checks whether the “buddy” 8KB is free; if so, it coalesces the two blocks into a 16KB block. The allocator then checks if the buddy of the 16KB block is still free; if so, it coalesces those two blocks. This recursive coalescing process continues up the tree, either restoring the entire free space or stopping when a buddy is found to be in use.
- The reason buddy allocation works so well is that it is simple to determine the buddy of a particular block. How, you ask? Think about the addresses of the blocks in the free space above. If you think carefully enough, you’ll see that the address of each buddy pair only differs by a single bit; which bit is determined by the level in the buddy tree. And thus you have a basic idea of how binary buddy allocation schemes work.
