# coru

`coru`, short for **code ruler**, adds line numbers to source code in the form of comments.  
The resulting code remains valid source code and can still be compiled or executed normally.

Such commented source files are primarily intended for use in **programming tutorials and demonstrative code snippets**, where line numbers help readers follow explanations more easily.

## coru and its Companion Program

The `coru` toolset consists of two small utilities:

- **coru** – adds line numbers to source code
- **uncoru** – removes line numbers previously added by `coru`

These tools allow source code to move freely between **teaching materials** and **normal development environments**.

## Rationale

When writing programming tutorials, demonstrative code snippets are essential. However, beginners often find it difficult to follow explanations when no clear reference points are provided.

Line numbers are a traditional way to guide readers through code:

> “See line 12”, “modify line 27”, etc.

Many online tutorials add line numbers using HTML elements on web pages. While convenient, those line numbers exist **only in the rendered page**.

Once the code is reused in other materials — such as:

- another article
- a slide deck
- a PDF
- a book

the line number metadata are lost.

`coru` takes a different approach. It embeds line numbers directly into the source code as **comments**.

This provides several advantages:

- the code remains valid and runnable
- the numbered code can be reused in **any format of teaching material**
- the same snippet works across **web pages, books, slides, and documents**

If line numbers are no longer needed, the companion program `uncoru` can remove them and restore the original clean source.

## Project Status

`coru` is usable and has been tested on several platforms.

The current implementation contains some known architectural limitations. These issues generally **do not affect the basic functionality of the tool**, but they may make the internal codebase harder to extend or maintain collaboratively without future refactoring.

At the moment, the project can be described as:

- usable for practical purposes
- primarily maintained by the original author
- open to bug reports and small improvements
- potentially subject to internal refactoring in future versions

If you plan to build upon the codebase or contribute large changes, please note that some internal components may change in later revisions.

## System Requirements

- A C compiler that supports ANSI C
- GNU Make

To use GNU Make on non-Linux Unix or Unix-like systems, use `gmake`.  
To use GNU Make on Windows, use `mingw32-make`.

## Supported Languages

Currently supported programming languages include:

- C
- C++
- Object Pascal / Delphi
- Modern Fortran
- Common Lisp
- Java
- C#
- Perl
- Python
- Ruby
- PHP
- JavaScript
- Objective-C / Objective-C++
- Swift
- Go
- Rust
- OCaml
- C shell and derivatives
- Bourne shell and derivatives
- PowerShell
- CMake configuration files (default: *CMakeLists.txt*)
- ~~Make configuration files (default: *Makefile*)~~

## Building

To build the executables of `coru` and `uncoru`:

```shell
$ make
```

To build static libraries:

```shell
$ make static
```

To build dynamic libraries:

```shell
$ make dynamic
```

## Using coru as a Console Program

Simply pass a source file:

```shell
$ coru path/to/file.c
```

Force the target source to be treated as C:

```shell
$ coru -c path/to/source
```

## Using coru as a Library

See the `examples/` directory for usage examples.

## Options for coru

### General Options

- `-v` or `--version` — show version information and exit
- `--license` — show license information and exit
- `-h` or `--help` — show help information and exit

### Line Number Options

- `--non-empty` — add line numbers only to non-empty lines (default)
- `--all` — add line numbers to all lines

`coru` always skips block comments because some languages, such as C, cannot parse nested block comments.

### Target Language Options

Run `coru -h` for more details.

## Notes

`coru` and `uncoru` are **not language linters**. They assume the input source code is syntactically valid and perform text-based transformations accordingly.

Some utility code is derived from:
https://github.com/cwchentw/clibs

## Known Issues

- Only limited testing has been performed to ensure that code modified by `coru` remains valid
- Multiline strings in some languages may not be handled correctly
- Heredoc syntax is not detected
- `Makefile`s modified by `coru` may become unusable

## Copyright

Copyright (c) 2019–2026 ByteBard. Licensed under the MIT License.
