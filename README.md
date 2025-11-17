# arfminesweeper

Minesweeper, but in every way possible

## Build on Debian GNU/Linux

[![Build](https://github.com/arf20/arfminesweeper/actions/workflows/build.yml/badge.svg)](https://github.com/arf20/arfminesweeper/actions/workflows/build.yml)

Global dependencies: `cmake build-essential`

Dependencies for main frontends:

```
curses:     libncurses-dev
drmfb:      libdrm-dev
fltk:       libfltk1.4-dev
gl11:       libgl-dev freeglut3-dev
gl33:       libgl-dev libglfw3-dev
glx:        libgl-dev libx11-dev
gtk3:       libgtk-3-dev
java:       openjdk-17-jdk-headless
motif:      libmotif-dev
qt5:        qtbase5-dev qttools5-dev qttools5-dev-tools
sdl1:       libsdl1.2-dev libsdl-image1.2-dev libsdl-ttf2.0-dev
sdl2:       libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
vulkan:     libvulkan-dev
wayland:    libwayland-dev
wxwidgets:  libwxgtk3.2-dev
xaw:        libxaw7-dev
xcb:        libxcb1-dev
xforms:     libforms-dev
xlib:       libx11-dev
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

Use -DNO\_MODULE=TRUE, -DNO\_KERNEL=TRUE or -DNO\_EFIAPP=TRUE to
disable building the linux kernel module target, kernel target or EFI app target

Use -DMODULE\_ALL=TRUE to build all modules, -DMODULE\_\<frontend> to build specific modules

Use -DNO\_STATIC to disable all static frontends, -DSTATIC\_\<frontend> to build specific static frontends

## TODO frontends
```
MAIN TARGET                 NAME        STATE Linux BSD Mac Win
    console                 console     DONE  X     X   X   X
    vt100 escapes           vt100       DONE  X     X   X   X
    ANSI color escapes      ansi        DONE  X     X   X   X
    curses/ncurses          curses      DONE  X     X   X   
    fbdev                   fbdev       DONE  X
    X11 Xlib                xlib        DONE  X     X   X   X
    X11 XCB                 xcb         WIP   X     X   X   X
    Wayland                 wayland     WIP   X     X
    XForms                  xforms      DONE  X     X
    FLTK                    fltk        WIP   X     X   X   X
    Motif (Xm/Xt)           motif       WIP   X     X
    XView (OpenLook/Xt)                       X     X
    Athena (Xaw/Xt)         xaw         WIP   X     X
    Xaw3d                                     X     X
    X Toolkit (Xt)                            X     X
    Tcl/Tk                                    X     X       X
    raylib                              NO    X     X   X   X  incompatible with gl33
    SDL3                                      X     X   X   X
    SDL2                    sdl2        DONE  X     X   X   X
    SDL1.2                  sdl1        WIP   X     X   X   X
    DRI DRM fb              drmfb       WIP   X
    GLIDE (with Voodoo)                       X     X       X
    OpenGL GLX              glx               X     X
    OpenGL 2.2 FFP/FreeGLUT gl11        DONE  X     X   X   X
    OpenGL 3.3/GLFW/GLAD    gl33        WIP   X     X   X   X
    ImGui                                     X     X   X   X
    Vulkan/GLFW             vulkan      WIP   X     X   X   X
    Metal                                                   X
    Gtk2                                      X     X   X   X
    Gtk3                    gtk3        DONE  X     X   X   X
    Gtk4 + libadwaita                         X     X   X   X
    Qt4                                       X     X   X   X
    Qt5                     qt5         DONE  X     X   X   X
    Qt6                                       X     X   X   X
    wxWidgets               wxwidgets   WIP   X     X   X   X
    WinAPI                  win32       WIP                 X
    Win32 GDI               gdi         WIP                 X
    Direct2D                d2d         WIP                 X
    .NET WinForms                                           X
    .NET WPF                                                X
    Win UWP                                                 X
    WinUI 3                                                 X
    .NET MAUI                                               X
    Java JNI AWT            java        WIP   X     X   X   X  Architectural limitation
    Java JNI Swing                            X     X   X   X
    Webapp                  httpd       DONE  X     X   X   X

LINUX MODULE TARGET
    Kernel module           DONE

ANDROID TARGET
    Android NDK

BIOS KERNEL TARGET          STATE BIOS UEFI
    VGA text mode console   DONE  X    X
    VGA text mode           DONE  X    X
    VGA text w/graphic char DONE  X    X
    VGA graphic mode        WIP   X    X
    VESA graphic mode                  X
UEFI APPLICATION TARGET
    UEFI console                       X

HARDWARE TARGET
    SystemVerilog
    VHDL
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

