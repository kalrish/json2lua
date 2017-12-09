# json2lua

This program converts JSON to Lua efficiently. Instead of parsing and loading the entire JSON data at once, which would cost quite some memory, it reads and processes the input in chunks.

Currently, the project comprises two components:

 -  `libjson2lua`, a C++ library implementing a flexible, pull-style JSON-to-Lua parser and converter. It is kept in tree because it is developed for the program.
 -  `json2lua`, the C++ program which exposes the required functionality to the command line.

In the future, the program might be expanded to support more encodings and, perhaps, to write bytecode directly.


##  Build

The Tup build system manages the process, which comprises three steps: configuration, build and testing.

###  Configuration
Parameters are specified in the usual way, in a `tup.config` file. You might be interested in using variants.

The steps are as follows:

 1.  Choose an appropriate toolchain (`CONFIG_TOOLCHAIN`). Toolchains are defined in [src/toolchains.tup](src/toolchains.tup).
 2.  Fulfill the config items required or supported by the chosen toolchain (e.g. `CONFIG_CXXFLAGS` for the GNU toolchain).
 3.  Choose appropriate values for toolchain-independent variables (like `CONFIG_BUFFER_SIZE`).

[The provided tup.config](tup.config) contains the explanation of toolchain-independent variables.

Example configurations which may or may not be suitable for your build environment are provided in [configs.tup](configs.tup). You could try to proceed to the build step by using one of them after adding your settings for the toolchain-independent parameters.

###  Build
The build step follows the standard procedure:

    $  tup

####  Requirements

 -  the [Tup build system](http://gittup.org/tup/)
 -  a C++ toolchain
 -  [guardcheader](https://github.com/kalrish/guardcheader)
 -  the standard Lua interpreter

###  Testing
Tests are part of the build as well, and thus managed by Tup.

Tests have been implemented for the command-line program. As they involve a compiled executable, they can be carried out only if the target architecture for which the program was built matches that on which it was built. They will be run automatically if that's the case, and, if any of them fails, Tup will complain.


##  Usage

The accepted command-line depends on the version.

All versions admit the GNU standard to find it out:

    $  json2lua --version

This prints `json2lua `_`something VERSION`_, where _VERSION_ is the last token and represents the version.

###  Current version

The current version handles UTF-8-encoded JSON and outputs ASCII-encoded Lua 5.

The program shall be invoked as follows:

    $  json2lua INPUT OUTPUT

_INPUT_ and _OUTPUT_ are the names of the files that shall be read and written, respectively.
