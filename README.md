# Unix-Type-Shell
***
The bash developed is a Unix type shell developed in C language. It performs all the tasks that can be done by a normal shell.
***
### Instructions
To use this shell run the `Makefile`, which genereates the executable `shell`.
```
make
```
followed by 
```
./shell
```
to enter into this shell. Once in the shell, you can run all the commands that you ran run on a bash. Apart from that this shell also supports a host of custom user commands to interact with the shell. The user commands are : 
* <strong>HISTN</strong> : view the last N commands executed using this shell.
* <strong>!HISTN</strong> : execute the Nth command executed on this shell.
* <strong>pid</strong> : get the process id of this shell.
* <strong>pid all</strong> : get the process ids of all the processes spawned from this shell.
* <strong>pid current</strong> : get the process ids of all the processes spawned from this shell and currently active.
* <strong>HELP</strong> : get the instructions for this shell.
* <strong>STOP</strong> : exit from this shell.

This shell also supports I/O redirection via piping.

***

### License

This project is distributed under [MIT license](https://opensource.org/licenses/MIT). Any feedback, suggestions are higly appreciated.


