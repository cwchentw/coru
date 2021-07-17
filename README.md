# coru

`coru`, aka code ruler, adds line numbers to source code in the form of its comments. Commented source code are mainly used as demonstrative code snippets.

## Warning

`coru` and her sister program `uncoru` are still *experimental*. Back up your code before using either.

## `coru` and her Sister Programs

* `coru` adds line numbers to source
* `uncoru` removes line numbers from source modified by `coru`

## Why `coru`?

When we write programming tutorials, demonstrative code snippets are essential. Nevertheless, it is uneasy for beginners to trace the code if no helpful information is provided. Line numbers represent conventional means to guide learners to trace code.

Some online tutorials add line numbers in the form of HTML elements on web pages, which is common practices among programming teaching sites. However, these metadata are limited to web pages only. If we want to reuse the demo code in another material, the metadata of the code on web pages are lost.

In contrary, `coru` adds line numbers to source code as its comments. The commented code still works as that without any line number. Furthermore, the commented code can be utilized multiple times, in any format of teaching materials.

By the way, `uncoru`, the sister program of `coru`, removes the line numbers in source modified by `coru` in case that you no longer require these metadata on code.

## System Requirements

* A C compiler that supports ANSI C
* GNU Make

To use GNU Make on non-Linux Unix or Unix-like systems, use `gmake`. To use GNU Make on Windows, use `mingw32-make`.

We compile and run `coru` with GCC, Clang, Visual C++ and Intel C++ Compiler.

We test `coru` against several Unix or Unix-like systems:

* Ubuntu 18.04 LTS
* CentOS 8
* openSUSE Leap 15.1
* TrueOS, which is FreeBSD-compatible

Besides, we check `coru` manually on Windows 10 as well.

It should work on any platform that provides a decent C compiler.

## Supported Languages

Here are currently supported programming languages:

* C
* C++
* Object Pascal and Delphi
* Modern Fortran
* Common Lisp
* Java
* C#
* Perl
* Python
* Ruby
* PHP
* JavaScript
* Objective-C and Objective-C++
* Swift
* Golang
* Rust
* C shell or its derivatives
* Bourne shell or its derivatives
* CMake config file (default to *CMakeLists.txt*)
* Make config file (default to *Makefile*)

## Usage of this Project

Our default build task will generate

* The executable `coru`
* The static library and the dynamic library of `coru`
* The executable `uncoru`
* The static library and the dynamic library of `uncoru`

## Use `coru` as a Console Program

Just feed your sample source file:

```console
$ coru path/to/file.c
```

Treat target source as C language:

```console
$ coru -c path/to/source
```

## Use `coru` as a Library

See [example](/example).

## Options for `coru`

### General Options

* `-v` or `--version` to show version info and exit
* `--license` to show license info and exit
* `-h` or `--help` to show help info and exit

### Options for Target Language

Run `coru -h` for more information.

### Options related to Line Numbers

* `--non-empty` to add line numbers to only non-empty lines of target source (default)
* `--all` to add line numbers to all lines of target source

`coru` will always skip block comments because some language, like C, cannot parse nested block comments.

## Note

`coru` and her sister program `uncoru` are never language linters. They assume their inputs are always correct, performing code conversion accordingly.

We copy some utility code from [cwchentw/clibs](https://github.com/cwchentw/clibs).

## Known Issues or Bugs

* We only tested primitively whether the code converted by `coru` are still valid or not
* `coru` fails to handle the multiline strings in some languages
* `coru` cannot detect Heredoc
* `coru` modifled *Makefile*s are unusable
* `coru` fails to handle source with CJK characters

## Copyright

Copyright (c) 2019-2020 Michelle Chen. Licensed under MIT.
