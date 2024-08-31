# midcxxap
`midcxxap` is a mid, and very small, implementation of an argument parser in C++, mostly inspired by python's `argparse`.

## About the header
After a `make release`, a new file will be created in the root directory called `midcxxap.hpp`, that can easily\* be included in a project.

## Features
The header only has support for *positional* and *standard* arguments, i.e. things like:
```shell
$ prog positional_argument
$ prog -a a_parameter
$ prog -a=a_parameter
```
Work as expected. However, "standard" arguments all expect a paremeter. Support for *switches* may be added, but that's not really a priority.

## Using it in a program
Using the header is quite simple, one just has to add the common `#include` line:
```cpp
#include "midcxxap.hpp"
```
To have access to the main class. An instance of a parser has to be created:
```cpp
MidCxxAP::ArgumentParser ap = MidCxxAP::ArgumentParser("ProgramName", "ProgramDescription");
// OR
auto ap = MidCxxAP::ArgumentParser("ProgramName", "ProgramDescription");
```
The constructor takes two arguments, the program's name and its description. Both are optional, however a weird usage string will then be printed when an error is encountered.

Once the new object has been constructed, one can add arguments to it:
```cpp
ap.addArgument("positional");
ap.addArgument("-s", "--short", false);
ap.addArgument("-l", "--long", true);
```

`addArgument` expects either:
- `const std::string &`, and assumes it's a positional argument.
- `const std::string &, const std::string &, [const bool]`, the first string being a "short" version of an argument, and the second one being the "long" version. The last parameter defines whether it is required or not. That's it, now the arguments are added to the parser's "database".

__Note__: "short" arguments *have* to use a single dash `-`, while "long" arguments *have* to use double dashes `--`. Positional arguments however *cannot* start with a dash `-`.

Now that the expected arguments are specified, one can create an instance of arguments, i.e. an unordered map:
```cpp
std::unordered_map<std::string, std::string> args = ap.parseArgs(argc - 1, argv + 1);
// OR
auto args = ap.parseArgs(argc - 1, argv + 1);
```

`argc - 1` and `argv + 1` exclude the program's name which is passed to every `int main(int argc, char **argv)` program. Obviously, if you intend to use this header in your `main` function, the prototype has to be the one mentioned previously, and not `int main(void)`. Now, the arguments' values can be accessed using the `std::unordered_map`'s `[]` operator. In our previous example:
```cpp
args["positional"]; // Gets the "positional" argument added earlier's value.
args["short"]; // Gets the "-s/--short" argument added earlier's value, might be empty.
args["long"]; // Gets the "-l/--long" argument added earlier's value, cannot be empty since it is required.
```

## Making a release
To make a release, i.e. output the usable header, the following requirements have to be met:
- `python` version that supports `argparse`, likely >= `3.7`.
- GNU `make` (optional, see below).

If you have `make` installed, you can just run:
```shell
$ make release
```
And it will output `midcxxap.hpp` in the root directory, which can be included a project.
Otherwise, you can run the python script manually:
```shell
$ python release.py -s=midcxxap/midcxxap.cpp -i=midcxxap/midcxxap.hpp -o=midcxxap.hpp
```

## Building the test program
To build the test program, the following requirements have to be met:
- A C++17 compliant compiler, by default it's `g++`.
- GNU `make`.

To make the software, you can just run:
```shell
$ make -j$(nproc)
```
And it will output `testmidcxxap`.

To clean the directory from built files, just run:
```shell
$ make clean -j$(nproc)
```