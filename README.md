# coru

`coru`, aka code ruler, add line numbers and, optionally, width rulers to source code in the form of its comment. Commented source code are mainly used as demonstrative code snippets.

`uncoru`, the sister program of `coru`, removes line numbers and width rulers in target source commented by `coru`.

## Warning

`coru` and `uncoru` are still experimental. Back up your code before using either.

## System Requirements

* A C compiler that supports ANSI C
* GNU Make

We tested `coru` against several Unix or Unix-like systems:

* Ubuntu 18.04 LTS
* CentOS 8
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

For unsupported language, you may set custom character for single line comment. It should work as well.

## Usage

Just feed your sample source file:

```console
$ coru path/to/file.c
```

Add an optional width ruler:

```console
$ coru --ruler path/to/Klass.java
```

For unsupported target source, you may apply start text and, optionally, end text.

```console
$ coru --start "/*" --end "*/"  path/to/unknown.ext
```

## Options

### General Options

* `-v` or `--version` to show version info and exit
* `--license` to show license info and exit
* `-h` or `--help` to show help info and exit

### Options for its Output

Pending.

### Options related to Line Numbers

* `--all` to add line numbers to all lines of source (default)
* `--non-empty` to add line numbers to only non-empty lines of source

`coru` will skip blocked comments in either parameter.

### Options related to Rulers

* `--ruler` to add a ruler at the top of target source
* `--ruler-footer` to add a ruler at the bottom of target source
* `--ruler-both` to add rulers at both the top and the buttom of target source

The width of a ruler is set by the maximal width of target source code. Nevertheless, its maximal width doesn't exceed 100, which is a sensible number for typical source codes.

### Options related to Comments for Unknown Languages

* `--start` to set start word of comment text
* `--end` to set end word of comment text, default to an empty string
* `--block-start` to set start word of comment block text, default to an empty string
* `--block-end` to set end word of comment block text, default to an empty string

## Note

We copy the source code of `snprintf(3)` [here](https://www.ijs.si/software/snprintf/).

## Copyright

Copyright (c) 2019 Michael Chen. Licensed under MIT.
