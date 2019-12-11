# coru

`coru`, aka code ruler, add line numbers and, optionally, width rulers to source code in the form of its comment. Commented source code are mainly used as demonstrative code snippets.

## Sister Programs of `coru`

* `coru` adds line numbers and, optionally, width numbers for source code
* `corucb` does the same code conversion as `coru` but for clipboard data
* `uncoru` removes line numbers and width numbers left by `coru`
* `uncorucb` does the same code conversion as `uncoru` but for clipboard data

## Warning

`coru` and `uncoru` are still experimental. Back up your code before using either.

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

For unsupported language, you may set custom character for single line comment. It should work as well.

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

* `--all` to add line numbers to all lines of source (default)
* `--non-empty` to add line numbers to only non-empty lines of source

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

We copy some utility code from [cwchentw/clibs](https://github.com/cwchentw/clibs).

## Copyright

Copyright (c) 2019 Michael Chen. Licensed under MIT.
