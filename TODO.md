# TODO

## 🧱 Build System / Toolchain

### ⚠️ Cross Compilation Not Supported (New)

* Current Makefile assumes:

  * build machine == target machine
  * Uses host OS (`detected_OS`) to determine:

    * executable extension
    * library format
    * object extension
    * toolchain behavior

* Problems:

  * Fails with `zig cc -target ...`
  * Compiler name is used to infer object format
  * Object format should be determined by target ABI instead

* Observed issue:

  * `zig ar rcs ... cstring.o` → object files not found
  * Compile stage not triggered correctly

* Root cause:

  * Native-build-only design

* Future direction:

  * Introduce:

    * `HOST_OS`
    * `TARGET_OS`
    * `TARGET_ABI`
  * Derive:

    * `OBJ_EXT` from ABI (gnu → `.o`, msvc → `.obj`)

### 🔧 Existing Build Issues

* (bug) Fix unusable *Makefile*

## ✨ Features

* (feature) Write to file
* (feature) Modify in place
* (feature) Add width ruler
* (feature) Detect heredoc
* (feature) Add `corucb`, aka `coru` for clipboard
* (feature) Add `uncorucb`, aka `uncoru` for clipboard
* (feature) Enable the detection of *Makefile* for `uncoru`
* (feature) Enable the detection of *CMakeLists.txt* for `uncoru`

## 🐛 Bugs

* (bug) Fix the multi-line string issue in some programming language
* (bug) Fix CJK text issue

## 🧪 Testing

* (test) Test whether modified code are still valid

## ♻️ Refactoring

* Refactor some BOOL into either CORU_STATUS or UNCORU_STATUS

## 🧠 Notes / Observations

### Compiler vs Toolchain

* Compiler is not just a frontend (`cc`, `clang`, etc.)
* Modern toolchains (e.g. Zig) bundle:

  * compiler
  * linker
  * archiver
  * libc
  * cross-compilation support

### Build System Abstraction

* Current implicit model:

  ```
  object_format = f(compiler)
  ```

* Correct model:

  ```
  object_format = f(target_abi)
  ```

* Key shift:

  * implementation-driven → artifact-driven

## 🪶 Future (Low Priority)

* [ ] Add cross compilation support
* [ ] Evaluate Zig build system
* [ ] Refactor Makefile (separate host / target / toolchain)
