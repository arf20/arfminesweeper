# minesweeper
You know the game, but in every graphical API I know as well as console

## Build on Linux
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

## TODO frontends
```
                              Linux BSD Mac Win
Console (no graphics)   DONE  X     X   X   X
ncurses                       X     X   X   X 
fbdev                   WIP   X
SVGA buffer                   
X11 Xlib                DONE  X     X
X11 XCB                       X     X
Wayland                       X     X
XForms                  DONE  X     X
Motif                   WIP   X     X
Tcl/Tk                        X     X       X
raylib                        X     X   X   X
SDL2                    DONE  X     X   X   X
SDL1.2                  WIP   X     X   X   X
GLIDE (with Voodoo)           X     X       X
OpenGL 1.x/FreeGLUT     DONE  X     X   X   X
OpenGL 4.6/GLFW/GLAD    WIP   X     X   X   X
ImGui                         X     X   X   X
Vulkan/GLFW             WIP   X     X   X   X
Metal                                       X
Gtk3                    WIP   X     X
Qt5                     DONE  X     X   X   X
wxWidgets                     X     X   X   X
WinAPI                                      X
Win32 GDI+                                  X
Direct2D                                    X
.NET WinForms                               X
.NET WPF                                    X
Java AWT                      X     X   X   X
Java Swing                    X     X   X   X
Unreal Engine 5               X     X   X   X
Webapp adhoc HTTP server      X     X   X   X
```
I apologize.
