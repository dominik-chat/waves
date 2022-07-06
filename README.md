# waves

![LICENSE](https://img.shields.io/github/license/dominik-chat/waves?style=flat-square)
![LOC](https://img.shields.io/tokei/lines/github/dominik-chat/waves?style=flat-square)
[![FOSSA Status](https://app.fossa.com/api/projects/git%2Bgithub.com%2Fdominik-chat%2Fwaves.svg?type=shield)](https://app.fossa.com/projects/git%2Bgithub.com%2Fdominik-chat%2Fwaves?ref=badge_shield)

Waves is a program intended to simulate the motion of ocean waves.

The program uses inverse DFT algorithm for generating waves and [freeglut](http://freeglut.sourceforge.net/) as a graphics library.

## Compilation
The program requires a C compiler, cmake and freeglut library.

The compilation itself can be performed by executing the following commands in the root of the cloned repository:
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

## Documentation
The documentation can be built using [doxygen](https://www.doxygen.nl/) with [graphviz](https://graphviz.org/) using provided `Doxyfile`.


## License
[![FOSSA Status](https://app.fossa.com/api/projects/git%2Bgithub.com%2Fdominik-chat%2Fwaves.svg?type=large)](https://app.fossa.com/projects/git%2Bgithub.com%2Fdominik-chat%2Fwaves?ref=badge_large)