SoF1 Designer Script decompiler by dende. 2019 January.
Usage instructions.

This program can convert .os files back into their original scripted format, .ds files.

I recommend opening up a command prompt.  On windows hit winkey+r then type cmd or cmd.exe and hit enter.
A black terminal/console should appear and it will start in your users home folder.
Navigate to your .os files that you are interested in. eg "cd c:\users\name\Desktop\osfiles"
or relative navigation with "cd .." (parent directory) and repeatedly typing "dir" to list 
the contents of the folder you are in.

Anyway. Once you reach your destination, you are ready for the next step.
extract the sofos.exe to this directory.

Type in your console "sofos.exe" ... read what it says...

Now type "sofos.exe nameoffile.os", replacing nameoffile with the name of your .os file.
By default this will try to create a file named decompiled.ds in the same directory as sofos.exe

A helper file "helper.ds" will also be generated in the same directory as your sofos.exe when you run it for
the first time.  This file is pointed to in the first line of the .ds scripts and allows the
decompiled script to compile with sofds.exe Just make sure that sofos.exe and sofds.exe are in
the same folders/directories, and you'll be fine.

I will supply the sofds.exe which comes from the SoFSDK with this, so you can see if the decompiled.ds
 will compile back into the .os. SoF servers use the .os file to make dynamic things happen within a map.
 Put os files into the ds folder of your server's user directory.

sofds.exe created by Raven compiles .ds files into .os files
sofos.exe created by me decompiles .os files into .ds files

os files are what the computer understands, ds files are what humans understand.

sofds.exe generates compiled file with the same name as the input.ds file with .os appended instead of ds.
If you want to control output file of sofds, use -output option.

To control the output file generated from sofos.exe .. pass it as a second argument.
Eg. ./sofos.exe inputos.os outputds.ds
You can even supply different path
Eg. ./sofos.exe in/inputos.os out/outputds.ds

and sofds example
Eg. ./sofds.exe inputos.os -output outputds.ds
Eg. ./sofds.exe in/inputos.os -output out/outputds.ds

Enjoy. And post faults, I will gladly update it to work for more scenarios.

