# waves
Waves is a program intended to simulate the motion of ocean waves.

The program uses inverse FFT algorithm for generating waves and [freeglut](http://freeglut.sourceforge.net/) as a graphics library.

## Compilation
The program requires a C compiler, cmake and freeglut library.

The compilation itself can be performed by executing the following commands in the cloned repository:
```
cmake -S. -Bbuild
cd build
make
```

The program can be installed using this command:
```
make install
```
**Note that you may be required to execute this command as a superuser**
