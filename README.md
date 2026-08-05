

=
Overview
=


This project implements a simple custom memory allocator using a fixed-size heap stored in a global byte array. Instead of relying on the operating system's malloc() and free(), the allocator manages memory manually by storing metadata before every allocated block.

The allocator uses a first-fit allocation strategy, meaning it scans the heap from the beginning and allocates memory in the first free region that is large enough to satisfy the request.
--–-----–-----–-----–-----–-----–-----–-----–-----–-----–-----–-----–-----–-----–-----–-----–-----–-----

=
HEAP LAYOUT
=

The allocator manages a fixed-size heap:

unsigned char heap[HEAP_SIZE];

->Each allocated block begins with 2 bytes of metadata followed by the user data.

->The two metadata bytes store the number of bytes allocated for the user.

->The pointer returned to the user always points to the beginning of the data section, not the metadata. When finding out the size of the Block, Subtract By Two Bytes relative to the Returned Pointer
--–-----–-----–-----–-----–-----–-----–-----–-----–-----–----–-----–----–-----–------
