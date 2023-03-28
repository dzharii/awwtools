# aww-tools changelog

## 2023-03-27
- fixed bug in `aww create` : `CPP_HEADER_FILE_NAME` to generate valid identifiers.


the commands:
```sh
aww create .\include\internal\aww-xyz.hpp
aww create .\src\internal\aww-xyz.cpp
```
were very helpful during migration to lib.


## 2021-03-26

Migrated to library:
- `aww-create.cpp`
- `aww-git-open.cpp`
- `aww-guid.cpp`
- ... and all others

## 2023-03-24

Changelog created!
`aww create` now creates folders when the file name does not have an extension. This makes it impossible to create extension-less files, but for the sake of simplicity -- it is what it is.

`aww date` -- converted to use `main` implementation from lib.








