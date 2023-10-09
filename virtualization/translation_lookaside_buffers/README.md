# Summary

To speed up address translation, the OS get help from **TLB(translation lookaside buffer)**.


A TLB is part of the chip’s **memory-management unit (MMU)**, and is simply a hardware cache of popular virtual-to-physical address translations; thus, a better name would be an address-translation cache. Upon each virtual memory reference, the hardware first checks the TLB to see if the desired translation is held therein; if so, the translation is performed (quickly) without having to consult the page table (which has all translations). Because of their tremendous performance impact, TLBs in a real sense make virtual memory possible.

> The TLB, like all caches, is built on the premise that in the common case, translations are found in the cache (i.e., are hits). If so, little overhead is added, as the TLB is found near the processing core and is designed to be quite fast. When a miss occurs, the high cost of paging is incurred; the page table must be accessed to find the translation and an extra memory reference (or more, with more complex page tables) results. If this happens often, the program will likely run noticeably more slowly; memory accesses, relative to most CPU instructions, are quite costly, and TLB misses lead to more memory accesses.

- TLB misses can be handled by both hardware managed TLBs and software managed TLBs

### Two types of locality: Temporal locality and Spatial locality
The idea behind hardware caches is to take advantage of locality in instruction and data references. 
With temporal locality, the idea is that an instruction or data item that has been recently accessed will likely be re-accessed soon in the future. Think of loop variables or instructions in a loop; they are accessed repeatedly over time. With spatial locality, the idea is that if a program accesses memory at address x, it will likely soon access memory near x. Imagine here streaming through an array of some kind, accessing one element, and then the next. Of course, these properties depend on the exact nature of the program and thus are not hard-and-fast laws but more like rules of thumb.

Hardware caches, whether for instructions, data, or address translations (as in our TLB) take advantage of the locality by keeping copies of memory in small, fast on-chip memory. Instead of having to go to a (slow) memory to satisfy a request, the processor can first check if a nearby copy exists in a cache; if it does, the processor can access it quickly (i.e., in a few CPU cycles) and avoid spending the costly time it takes to access memory (many nanoseconds).

You might be wondering: if caches (like the TLB) are so great, why don’t we just make bigger caches and keep all of our data in them? Unfortunately, this is where we run into more fundamental laws like those of physics. If you want a fast cache, it has to be small, as issues like the speed-of-light and other physical constraints become relevant. Any large cache by definition is slow, and thus defeats the purpose. Thus, we are stuck with small, fast caches; the question that remains is how to best use them to improve performance.

In particular, if the number of pages a program accesses in a short period of time exceeds the number of pages that fit into the TLB, the program will generate a large number of TLB misses, and thus run quite a bit more slowly. We refer to this phenomenon as **exceeding the TLB coverage**, and it can be quite a problem for certain programs.

### Viable solution 
Is to include support for larger page sizes; by mapping key data structures into regions of the program’s address space that are mapped by larger pages, the effective coverage of the TLB can be increased. Support for large pages is often exploited by programs such as a database management system (a DBMS), which have certain data structures that are both large and randomly-accessed.


### FUN FACT: RAM ISN’T ALWAYS RAM (CULLER’S LAW)
> The term random-access memory, or RAM, implies that you can access any part of RAM just as quickly as another. While it is generally good to think of RAM in this way, because of hardware/OS features such as the TLB, accessing a particular page of memory may be costly, particularly if that page isn’t currently mapped by your TLB. Thus, it is always good to remember the implementation tip: RAM isn’t always RAM. Sometimes randomly accessing your address space, particularly if the number of pages accessed exceeds the TLB coverage, can lead to severe performance penalties.


## Exercise
- `TODO:` Write the program, called tlb.c, that can roughly measure the cost of accessing each page. Inputs to the program should be the number of pages to touch and the number of trials.
