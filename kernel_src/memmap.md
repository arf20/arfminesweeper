# kernel memory map
```
0x00000000  god knows what
0x00001000  kernel entry
    589K kernel + stack
0x00090000  stack top (grows downwards)
0x000a0000  heap bottom (grows upwards)
    393K    heap
0x000fffff  1MB mark
```
