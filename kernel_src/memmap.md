# kernel memory map
Requires system with at least 2MiB installed
```
0x00000000  
    god knows what
0x00001000  kernel entry
    430K kernel + stack
0x00070000  stack top (grows downwards)
    a bit of unused free space,
    EBDA, and reserved stuff
0x00100000  heap bottom
    1MiB of heap
0x001fffff  heap top
```
