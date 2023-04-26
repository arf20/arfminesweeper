# minesweeper
You know the game, but in every graphical API I know as well as console

## Build
Dependencies required: `build-essential cmake`
Dependencies for frontends:
```
Console: none
ncurses: libncurses-dev
fbdev: none
Xlib: libx11-dev
XCB: libxcb1-dev
XForms: libforms-dev
Motif: libmotif-dev
Tcl/Tk: 
SDL2: libsdl2-dev
SDL1.2: libsdl1.2-dev
OpenGL*: Included with your GPU vendor's driver or libgl-dev with llvmpipe
ImGui: none
Vulkan: libvulkan-dev
Gtk3: libgtk-3-dev
Qt5: qtbase5-dev qttools5-dev qttools5-dev-tools
WinAPI:
```
Build
```
mkdir build && cd build
cmake ..
make
```

## TODO APIs and Toolkits
```
Console (no graphics)   DONE
ncurses
fbdev                   ON DEVELOPMENT
X11 Xlib                DONE
X11 XCB
XForms (nice)           DONE
Motif                   ON DEVELOPMENT
Tcl/Tk
SDL2
SDL1.2 (lolz)
OpenGL 1.x
OpenGL 4.6 (yikes)
ImGui
Vulkan (yes.)
Gtk3 (pain)
Qt5
WinAPI (oh no)
Win32 GDI+
```
