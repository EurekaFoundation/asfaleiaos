# Asfaleia OS(Operating System)
 This is a simple OS created in C and ASM. This OS is completely offline, it doesn't have access on internet. The OS doesn't recognize the internet drivers of all PC. We invite all people who would try the OS to try this, in a Virtual Machine or in a old PC. Because for the moment we don't know if this OS can conflict with operating systems or other files present on hard drives connected to the PC.


 # How the code work
   This code is very simple. The programmes and commands in this OS are located in the application folder. If you want to create a programme, just create a .c file in this folder. If you then want to launch these using a command, you need to create a file in the include folder in which you declare all the functions present in the new programme file you have created. Next, you need to go to the shell. c (located in the application folder) and include the .h file created in the include folder, then declare the programme function, then insert a call to the main function of the programme you have created (the function that starts and runs this programme) within this function, then insert the command in the commands array (static const struct command commands[] ={...}). By doing so, the programme can be started from the shell. However, we invite you to view the written code and the code of the existing programmes if you want to contribute to finding solutions to any bugs and/or to the creation of new applications and features that could be useful. In addition to this, reviewing the existing code helps those seeing it for the first time to understand how it all works, as standard libraries are not used, but similar ones have been recreated, although they do not always have the same keywords or the same features, and sometimes the syntax is slightly different.

# How to compile and run the OS
 Compile the code   

```
make
``` 

 Run the OS   
  ```
make qemu
```

 If you have modified the code, run this command before recompiling  
```
make clean
```

 To create ISO file use this command
```
make iso 
```

# The E language

 This OS have a language created by us, the name is Elang or E language. This language is very simple, it has the basic features of a programming language, it is not very complex. For use this type "elang" in the shell of the OS.
   
# Contact Us
  If you wish to contact us, you can do so by email at asfaleia@eurekafoundation.it or directly to the developer at vincenzomaria.franchino@eurekafoundation.it .

# Social
 Telegram: https://t.me/asfaleiaos

 Discord: https://discord.gg/Q5PYqEapW5