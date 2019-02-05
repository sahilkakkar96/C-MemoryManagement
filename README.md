DOCUMENTATION (How to use)
--------
PART 1 : Without Cache
--------

1. To compile write "make part1"
2. (If you delete the fifo_main and fifo_mem that I have left in the files on git) First use the command "./run" to make mkfifo files 
3. To run write "./part1"
4. Open a second terminal. 
5. Use the command "gcc -o mem mem.c" to compile the file that makes emulated memory
6. Run this emulated memory file using "./mem" in the second terminal
-> Now in your first terminal you can run the commands


--------
PART 2 : With Cache
--------

1. If you ran Part 1 before then you have the "./run" part done. Otherwise do : "make part1" and then "./run"
2. Now do "make part2"
3. To run write "./part2" in terminal 1
4. Open a second terminal. 
5. Use the command "gcc -o mem mem.c" to compile the file that makes emulated memory
6. Run this emulated memory file using "./mem" in the second terminal
-> You can now run the commands and results would be using cache

NOTE: > When you use the "exit" command to exit either part1 or part2 it will also exit the running "mem"
      > So when you want to run part1 or part2 again you need to run the "./mem" in the second terminal again
