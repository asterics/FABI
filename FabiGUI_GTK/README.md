 
# Getting started

This folder contains the second version of the FABI configuration GUI. Instead of using C# (version 1), version 2 is based on plain C with GTK+v3.
GTK enables a cross-platform compiling for the GUI. Currently, the build system is based on Code:Blocks, which is easy to install and use on Windows and Linux.

# First steps - Windows

- Install Code:BLocks from http://codeblocks.org/
- Extract the gtk+-bundle file (from this folder) to C:\. The resulting path should be: C:\GTK\... If this is not the case, please adjust the project build options. IMPORTANT: do not use a path with spaces!
- Add the path C:\GTK\bin to the PATH environment variable.
- Open the FabiGUI_GTK_WIN.cbp Code:Blocks project file
- Press F9 (Build and run)
- If everything is fine, you should see the GUI


# First steps - Linux

- Install Code:Blocks and the gtk-dev files via your packet manager (e.g. Debian: apt-get install codeblocks libgtk-3-dev)
- Open the FabiGUI_GTK_LINUX.cbp Code:Blocks project file
- Press F9 (Build and run)
- If everything is fine, you should see the GUI

