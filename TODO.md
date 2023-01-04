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


## Custom Desktop notifications

```
aww-notify
```

Ubuntu:

There are a bunch of other cool features with notify-send

We can run a command and make it display in the notification:

```
notify-send <title> <`command`>
notify-send Date "`date`"
notify-send Disk "`df / -H`"
We can use icons with the notifications

notify-send -i <icon> <Message>
notify-send -i face-wink "Hello! January"
Really annoying pop up

notify-send  -t 0 "Bringing down the system"
and

notify-send <title> <message>
notify-send "who am i" "I am January"
````
- 2022-10-29 [How can I send a custom desktop notification? - Ask Ubuntu](https://askubuntu.com/questions/187022/how-can-i-send-a-custom-desktop-notification)

Windows

PowerShell.
Or:

- 2022-10-29 [An executable to popup a balloon tip in notification area on Windows, written in C++.](https://gist.github.com/wontoncc/8912875)
- 2022-10-29 [Notifications and the Notification Area - Win32 apps Microsoft Learn](https://learn.microsoft.com/en-us/windows/win32/shell/notification-area?redirectedfrom=MSDN)

Mac
- 2022-10-29 [macos - How to get a notification when my commands are done - Ask Different](https://apple.stackexchange.com/questions/9412/how-to-get-a-notification-when-my-commands-are-done)
(svn update . && ant clean build start && say done) || say error
- 2022-10-29 [notify-send adapter](https://gist.github.com/blech75/fd6c36c43057607507e2)

```
aww-http
```
- 2022-10-29 [yhirose/cpp-httplib: A C++ header-only HTTP/HTTPS server and client library](https://github.com/yhirose/cpp-httplib)
- 2022-10-29 [Search � cpp-httplib](https://github.com/search?l=CMake&q=cpp-httplib&type=code)


```
aww-guid
```

- 2022-10-30 [Cross-platform Linux/Windows GUID generator in C](https://gist.github.com/peschinskiy/17430373872ebed4252dc0a0fe0ceef8)


```
cat /proc/sys/kernel/random/uuid
```

- 2022-10-30 [CoCreateGuid function combaseapi.h - Win32 apps Microsoft Learn](https://learn.microsoft.com/en-us/windows/win32/api/combaseapi/nf-combaseapi-cocreateguid?redirectedfrom=MSDN)
```
HRESULT CoCreateGuid(
  [out] GUID *pguid
);
```

## Toast!
- 2022-11-07 [Send a local toast notification from a C# app - Windows apps Microsoft Learn](https://learn.microsoft.com/en-us/windows/apps/design/shell/tiles-and-notifications/send-local-toast?tabs=desktop)

## Colors

- 2022-11-06 [Terminal colors in C/C++ - DEV Community](https://dev.to/tenry/terminal-colors-in-c-c-3dgc)


## Thanks
- 2022-11-06 [Execute a command and get both output and exit status in C++ Windows & Linux - Raymii.org](https://raymii.org/s/articles/Execute_a_command_and_get_both_output_and_exit_code.html)
> Good example with _popen / _pclose


- 2022-11-08 [CommandLineToArgvW function shellapi.h - Win32 apps Microsoft Learn](https://learn.microsoft.com/en-us/windows/win32/api/shellapi/nf-shellapi-commandlinetoargvw)
```cpp
#include <windows.h>
#include <stdio.h>
#include <shellapi.h>

int __cdecl main()
{
   LPWSTR *szArglist;
   int nArgs;
   int i;

   szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
   if( NULL == szArglist )
   {
      wprintf(L"CommandLineToArgvW failed\n");
      return 0;
   }
   else for( i=0; i<nArgs; i++) printf("%d: %ws\n", i, szArglist[i]);

// Free memory allocated for CommandLineToArgvW arguments.

   LocalFree(szArglist);

   return(1);
}
```


## Voice notification

Prerecord some notifications, like "Done", "Failed", "Building"
in a wav folder. Allow adding custom notifications just by creating files.


## Notification configuration.

It is time to start working on a configuration file for aww.

## aww compress 

- aww compress folder zip
- aww compress file.txt gz

## - aww my XXX

Programmable templates! 

aww my date
will find date.txt

written is mustache 
and execute template:

{{YYYY-MM-DDDate}} and copy it to clipboard
-  [no1msd/mstch: mstch is a complete implementation of {{mustache}} templates using modern C++](https://github.com/no1msd/mstch)


## aww run!

- [ ] Handle gracefull cancelation Ctrl+C for pipe

- [ ] Consider: 2022-12-10 [Enable tab completion - .NET CLI Microsoft Learn](https://learn.microsoft.com/en-us/dotnet/core/tools/enable-tab-autocomplete)

What to add:
aww run should be interactive!

for instance, it can include some built-in grep or something. 

Execute external scripts with `aww run`, now with aww.

aww run script name ===> :

- aww-run-script-name.sh
- aww-run-script-name.ps1

or from folder aww-run (in the current directory and global):

- script-name.sh
- script-name.ps1

- 2022-12-03 [Warp: The terminal for the 21st century](https://www.warp.dev/)
- 2022-12-02 [Drag and drop from terminal](https://blog.meain.io/2022/terminal-drag-and-drop/)

## Configuration

- 2022-12-03 [toml++ TOML for C++](https://marzer.github.io/tomlplusplus/)
- 2022-12-03 [ToruNiina/toml11: TOML for Modern C++](https://github.com/ToruNiina/toml11)


## Logging 

- [ ] Implement logging support

## aww-clip

copy stuff to clipboard from standard input and output.
What else? Maybe path?

## aww-open

same thing!
just cross-platform opener


