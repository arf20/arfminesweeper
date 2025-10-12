# kernel memory map
Requires system with at least 2MiB installed
```
0x00000000  
    god knows what, int32
0x00001000  kernel entry
    430K kernel + stack
0x00070000  stack top (grows downwards)
    ...
0x000a0000
    VGA graphics buffer
0x000b0000
...
0x000b8000
    VGA text buffer
0x000bffff
...

...
0x00100000  heap bottom
    1MiB of heap
0x001fffff  heap top
...
0xfd000000
    grub framebuffer
...
```
