# lai

`lai`, prounced as *line*, add line numbers and, optionally, width rulers to source code in the form of its comment. Commented source code are mainly used as demonstrative code snippets.

## Warning

`lai` is still experimental. Back up your code before using `lai`.

## System Requirements

* A C compiler that supports ANSI C

We tested `lai` on Ubuntu 18.04 LTS. It should work on any platform that owns a standard C compiler.

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
* Objective-C
* Swift
* Golang
* Rust
* Bash

For unsupported language, you may set custom character for single line comment. It should work as well.

## Usage

Just feed your sample source file:

```console
$ lai path/to/file.c
```

Add optional width rulers:

```console
$ lai --ruler path/to/Klass.java
```

There are three options to add ruler(s):

* `--ruler` to add a ruler at the top of target source 
* `--ruler-footer` to add a ruler at the bottom of target source
* `--ruler-both` to add rulers at both the top and the buttom of target source

For unknown source, you may apply start text and, optionally, end text.

```console
$ lai --start "/*" --end "*/"  path/to/unknown.ext
```

## Options

Pending.

## Copyright

Copyright 2019 Michael Chen; the software is licensed under MIT.
