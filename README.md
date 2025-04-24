# arfminesweeper
Minesweeper, but in every way possible

## Build on Linux
Global dependencies: `build-essential cmake`

Dependencies for main frontends:
```
ncurses: libncurses-dev
Xlib: libx11-dev
XCB: libxcb1-dev
Wayland: libwayland-dev
XForms: libforms-dev
Motif: libmotif-dev
Tcl/Tk: 
SDL2: libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
SDL1.2: libsdl1.2-dev libsdl-image1.2-dev libsdl-ttf2.0-dev
OpenGL 1.2: libgl-dev freeglut3-dev
OpenGL 3.3: libgl-dev libglfw3-dev
Vulkan: libvulkan-dev
Gtk3: libgtk-3-dev
Qt5: qtbase5-dev qttools5-dev qttools5-dev-tools
Java: openjdk-17-jdk-headless
```

Dependencies for kernel module: `linux-headers-amd64`

Dependencies for BIOS kernel: `nasm`

Dependencies for UEFI application: `gnu-efi binutils-mingw-w64 gcc-mingw-w64 xorriso`

Build
```
mkdir build && cd build
cmake ..
make
```

Use -DNO\_MODULE:BOOL=TRUE, -DNO\_KERNEL:BOOL=TRUE or -DNO\_EFIAPP:BOOL=TRUE to
avoid building the kernel module, kernel or EFI app

## TODO frontends
```
MAIN TARGET                       Linux BSD Mac Win
    console                 DONE  X     X   X   X
    vt100 escapes           DONE  X     X   X   X
    ANSI color escapes      DONE  X     X   X   X
    curses/ncurses          DONE  X     X   X   
    fbdev                   DONE  X
    X11 Xlib                DONE  X     X
    X11 XCB                 WIP   X     X
    Wayland                 WIP   X     X
    XForms                  DONE  X     X
    FLTK                    WIP   X     X   X   X
    Motif (Xm/Xt)           WIP   X     X
    XView (OpenLook/Xt)           X     X
    Athena (Xaw/Xt)         WIP   X     X
    Xaw3d                         X     X
    X Toolkit (Xt)                X     X
    Tcl/Tk                        X     X       X
    raylib                  NO    X     X   X   X  Redefines GLAD
    SDL2                    DONE  X     X   X   X
    SDL1.2                  WIP   X     X   X   X
    DRI DRM fb              WIP   X
    GLIDE (with Voodoo)           X     X       X
    OpenGL GLX                    X     X
    OpenGL 1.x FFP/FreeGLUT DONE  X     X   X   X
    OpenGL 3.3/GLFW/GLAD    WIP   X     X   X   X
    ImGui                         X     X   X   X
    Vulkan/GLFW             WIP   X     X   X   X
    Metal                                       X
    Gtk3                    DONE  X     X   X   X
    Gtk4                          X     X   X   X
    Qt5                     DONE  X     X   X   X
    Qt6                           X     X   X   X
    wxWidgets                     X     X   X   X
    WinAPI                  WIP                 X
    Win32 GDI               WIP                 X
    Direct2D                WIP                 X
    .NET WinForms                               X
    .NET WPF                                    X
    Win UWP                                     X
    WinUI 3                                     X
    .NET MAUI                                   X
    Java JNI AWT            WIP   X     X   X   X  Architectural limitation
    Java JNI Swing                X     X   X   X
    Webapp                  DONE  X     X   X   X
LINUX MODULE TARGET
    Kernel module           DONE  X              

BIOS KERNEL TARGET                BIOS UEFI
    VGA text mode console   DONE  X    X
    VGA text mode           DONE  X    X
    VGA text w/graphic char DONE  X    X
    VGA graphic mode        WIP   X    X
    VESA graphic mode                  X
UEFI APPLICATION TARGET
    UEFI console                       X

HARDWARE TARGET
    SystemVerilog FPGA      
```
I apologize.

### Notes:
#### gl33 (OpenGL 3.3 GLSL 330 core)
Does not work on Intel iGPUs apparently, for some god unknown reason, maybe the bit shit?
#### Win32
Currently I think it only builds correctly under MSVC
#### Kernel module
Exposes a character device at /dev/arfminesweeper.
`cat` it to get the board, and send commands like echo 'c 0 0' > /dev/arfminesweer
Usage is c|f x y, c for clear f for flag
