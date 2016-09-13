# LeanCloud C++ SDK

## How to set up a playground on Mac OS X

The instructions below have been tested only on Mavericks.

## Prerequisites

- homebrew, a package management tool for Mac OS X

        ruby <(curl -fsSkL raw.github.com/mxcl/homebrew/go)

- CMake, the cross-platform, open-source build system

	    brew install cmake

- Doxygen, the de facto standard tool for generating documentation from annotated C++ sources

	    brew install doxygen

- Boost, a set of libraries for the C++ programming language that provide support for tasks and structures such as linear algebra, pseudorandom number generation, multithreading, image processing, regular expressions, and unit testing.

	    brew install boost

- asio, cross-platform C++ Library for asynchronous programming

        brew install asio

- cpp-netlib, C++ libraries for high level network programming

        brew install cpp-netlib

- jsoncpp, library for interacting with JSON

        brew install jsoncpp

## Build

We use `cmake` to build our C++ SDK

- build AVOSCloud C++ SDK

		$ cd cpp-sdk
		$ mkdir cpp-sdk-build
        $ cd cpp-sdk-build
        $ cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
        $ make -j4

If you want to build this project for ARM, do as follows:

- Get git submodules
		
		$ git submodule init
		$ git submodule update

- build and install cpp-netlib for ARM

		$ cd lib/cpp-netlib
		$ mkdir cpp-netlib-build
		$ cd cpp-netlib-build
		$ cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../ToolchainARM.cmake
		$ make -j4 && make install		
 
- build and install jsoncpp for ARM

		$ cd lib/jsoncpp
		$ mkdir jsoncpp-build
		$ cd jsoncpp-build
		$ cmake .. -DCMAKE_BUILD_TYPE=Debug -DMAKE_TOOLCHAIN_FILE=../ToolchainARM.cmake
		$ make -j4 && make install

- build AVOSCloud C++ SDK for ARM

		$ cd lib/cpp-sdk
		$ mkdir cpp-sdk-build
		$ cd cpp-sdk-build
		$ cmake .. -DCMAKE_BUILD_TYPE=Debug -DMAKE_TOOLCHAIN_FILE=../ToolchainARM.cmake
		$ make -j4
		
More about `CMake Cross Compiling` is [here](http://www.cmake.org/Wiki/CMake_Cross_Compiling).
        
### Docs

We can generate cpp api docs using doxygen.

		$ cd cpp-sdk
		$ doxygen Doxyfile

Then, a `docs` directory will be generated.

### Code style

Usually, we try to follow the google-cpp-code-style. There is a tool named as [cpplint.py](http://google-styleguide.googlecode.com/svn/trunk/cpplint/cpplint.py) that reads source files and identifies many style errors. We can check the code sytle of our project by the following command:

		scripts/check-cpp-code-style.sh


## High-level file hierarchy

- `include`: header files of AVOSCloud

- `src`: source code of AVOSCloud

- `tests`: unit test cases

- `scripts`: some useful scripts

- `lib`: third-party libraries

- `Doxyfile`: used to generate C++ docs

- `README.md`: brief introduction to this project

- `CMakeLists.txt`: cmake script which will be used to generate Makefile


## Contact

If you have any question about the cpp-sdk for AVOSCloud, please contact

    Chaozhong Yang <cyang@avoscloud.com>
    
## License

Distributed under `GNU Lesser General Public License` .
