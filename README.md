# coru

`coru`, aka code ruler, add line numbers and, optionally, width numbers to source code in the form of its comments. Commented source code are mainly used as demonstrative code snippets.

## Warning

`coru` and her sister programs are still experimental. Back up your code before using either.

## Why `coru`?

When we write programming tutorials, demonstrative code snippets are essential. Nevertheless, it is uneasy for beginners to trace the code if no helpful information is provided. Line numbers represent conventional means to guide learners to trace code.

Some online tutorials add line numbers in the form of HTML elements on web pages, which is common practices among programming teaching sites. However, these metadata are limited to web pages only. If we want to reuse the demo code in another material, the metadata of the code on web pages are lost.

In contrary, `coru` adds line numbers and, optionally, width numbers, to source code as it comments. The commented code still works as that without any line number. Furthermore, the commented code can be utilized multiple times, in any format of teaching materials.

By the way, `uncoru`, the sister program of `coru`, removes the line numbers and the width numbers left by `coru` in case that you no longer require such metadata on code.

## `coru` and her Sister Programs

* `coru` adds line numbers and, optionally, width numbers to source code
* `corucb` does the same code conversion as `coru` but for clipboard data
* `uncoru` removes line numbers and width numbers left by `coru`
* `uncorucb` does the same code conversion as `uncoru` but for clipboard data

## System Requirements

* A C compiler that supports ANSI C
* GNU Make

We tested `coru` against several Unix or Unix-like systems:

* Ubuntu 18.04 LTS
* CentOS 8
* openSUSE Leap 15.1
* TrueOS, FreeBSD compatible
* Solaris 11

It should work on any platform that owns a standard C compiler.

Currently, our Makefile works with GNU Make. On some Unix or Unix-like systems, use `gmake` instead of `make`.

## Supported Languages

Most major tier languages are supported. Here are currently supported ones:

* C
* C++
* Java
* C#
* Perl
* Python
* Ruby
* PHP
* JavaScript
* Objective-C (and Objective-C++)
* Swift
* Golang
* Rust
* C Shell
* Bourne Shell
* CMake config file (default to *CMakeLists.txt*)

For unsupported language, you may set custom character for single line comment. It should work as well.

## Usage of this Project

Our default build task will generate

* The executables of `coru` and `corucb`
* The static library and the dynamic library of `coru`
* The executables of `uncoru` and `uncorucb`
* The static library and the dynamic library of `uncoru`

If you only want the executables, use the build task `release` instead.

## Usage of `coru`

Just feed your sample source file:

```console
$ coru path/to/file.c
```

Add an optional width ruler:

```console
$ coru --ruler path/to/Klass.java
```

Treat target source as C language:

```console
$ coru -c path/to/source
```

For unsupported target source, you may apply start text and, optionally, end text.

```console
$ coru --start "/*" --end "*/"  path/to/unknown.ext
```

## Options for `coru`

### General Options

* `-v` or `--version` to show version info and exit
* `--license` to show license info and exit
* `-h` or `--help` to show help info and exit

### Options for Target Language

Pending.

### Options related to Line Numbers

* `--non-empty` to add line numbers to only non-empty lines of target source (default)
* `--all` to add line numbers to all lines of target source

`coru` will always skip block comments because some language, like C, cannot parse nested block comments.

### Options related to Width Ruler(s)

By default, no width ruler is added to target source. Apply either if you want to add width ruler(s) to the source.

* `--ruler` to add a ruler at the top of target source
* `--ruler-footer` to add a ruler at the bottom of target source
* `--ruler-both` to add rulers at both the top and the buttom of target source

The width of a ruler is set by the maximal width of target source code. Nevertheless, its maximal width doesn't exceed 100, which is a sensible default for typical source codes.

### Options related to Comments of Unknown Languages

* `--start` to set start word of single line comment text
* `--end` to set end word of single line comment text
* `--block-start` to set start word of block comment text
* `--block-end` to set end word of block comment block text

Either option default to an empty string `""`.

## Note

`coru` and her sister programs are never language linters. They assume their inputs are always correct, performing code conversion accordingly.

We copy some utility code from [cwchentw/clibs](https://github.com/cwchentw/clibs).

## Known Issues or Bugs

* We only tested primitively whether the code converted by `coru` are still valid or not
* `coru` cannot detect heredoc
* Though usable, Valgrind still reports errors on the codebase of `coru`, related to string concatenation issues

## Copyright

Copyright (c) 2019 Michael Chen. Licensed under MIT.
