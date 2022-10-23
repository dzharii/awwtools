# The Plan

## User configuration

- [ ] Should definitely support  dot files, like `.awwtools` but also would be great to support a separate git repository for the configuration, like `my-awwtools`. The  awwtools can provide commands to manage configuration, like `aww config push` `aww config pull`

## The Tools

### Git

- `aww git push`
- `aww git open`
- `aww git help`

## Cheat sheets

- would be great to have cheat sheets for the commonly used tools

## aww launcher

`aww` is launcher for aww tools, to make it more useful, first of all it allow starting aww-tool without using dashes `-`

because the dash is located on top of the QWERTY keyboard and it is difficult to reach it with the pinky finger.

So `aww git open` will run `aww-git-open.exe`

So `aww git open my-file.txt` will run `aww-git-open.exe my-file.txt`

This is great, but would be nice to also run scripts in case the aww-tool is being prototyped as a powershell script or bash file.

There should be a way to run shell script if its name starts with aww-

also in this way we can support extensions, for instance, `aww`  can look at multiple folders, based on configuration or convention to locate user defined executables. This will give a centralized launcher, maybe a centralized help system for aww-tools.

- Should aww launcher also show a help page for the aww-tool
- Should aww launcher be aware about command history?

## MACOS Emulation:

- [darlinghq](https://docs.darlinghq.org/build-instructions.html)

## Repo

- [X] `main` branch requires pull request flow to protect it from broken builds

## Security
- [ ] Before running aww-tool -- check it's checksum. The checksum / hash must be generated at compile time
