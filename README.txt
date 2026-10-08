# MyLittleMalloc
AUTHORS: Anagha Ajesh(aa3611) and Manusri Yarramsetty(my607)

test plans and programs: 

test.c (used to test mymalloc())
- Basic malloc functionality
- Usable memory

testfree.c (used to test myfree())
- Basic free functionality
- tests deallocation and reuse of freed chunks

testcoa.c (used to test the helper function in mymalloc.c for coalescing)
- tests if adjacent chunks are coalesced

testmem.c (used to test memgrind)
- stress correcting
- checks if it frees workloads

MORE SPECIFIC TESTS FOR FREE:

testinvalidfree.c 
-calls free with a pointer  that was not returned by malloc
-should return with an error!

testleak.c
-tests the leak separately, though I believe it is tested in testfree.c 

testmiddlefree.c
-calls free with a pointer into the middle of an allocation
-should return an error

testdoublefree.c
-tests free with a pointer that has already been freed.


design notes: 

- the heap is represented by a 4096-byte static union in mymalloc.c. 
- each chunk contains a header storing the total chunk size and whether the chunk is allocated
- the remainder of the chunk is the payload 

- when the heap is first initialized, it is essentially one big chunk
- mymalloc() rounds the client requested chunk up to an 8-byte boundary, searches the heap for a large enough free chunk, and splits the chunk when there is enough space for another chunk. 
- then, mymalloc() marks the selected chunk as allocated and returns a pointer to its payload.

-myfree() searches the heap for the chunk whose payload matches the pointer. 
-if the pointer is invalid or the chunk has already been freed, an error is reported 
-otherwise, the chunk is marked free and coalesce() combines adjacent free chunks into larger free chunks.

-the leak detector scans the heap at program termination and reports the number of allocated objects and the total payload bytes that remain allocated.