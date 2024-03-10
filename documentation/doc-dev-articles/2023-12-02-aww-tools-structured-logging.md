# aww structured logging
Date: 2023-12-02; Dmytro Zharii

I will not rush to a conclusion, but take the time to evaluate all options.

​													*- aww tools*



## Problem

The idea is to slowly replace `cout <<` logs with this beautiful [gabime/spdlog: Fast C++ logging library.](https://github.com/gabime/spdlog), but what I really logs to enable:

**A)** Pinning the error message to log line, so when I run `aww-open` and it fails, I should know from where this error comes from:

```cpp
std::cout << "Failed to launch file " << res.error() << "\n";
```

`Failed to launch file` is quite generic message, I want to jump to the code line as fast as possible

**B)** Structured logging, but not JSON. JSON, 

I want to add `fileToOpen = ${fileToOpen}`somewhere, but feels like logging in  JSON is not the best idea. I don't want to see something like this in console: 

```cpp
{
    "message": "Failed to launch file",
    "error": "res.error()",
    "fileToOpen": "${fileToOpen}"
}
```

or this?

```json
{"message":"Failed to launch file","error":"res.error()","fileToOpen":"${fileToOpen}"}
```

JSON is horrible to read for humans, perfect for robots. 



## Aww logs!

New structured logging format will be as the following: 

```
std::cout << "#nrm6bo5v3rv Failed to launch file #res.error=" << res.error() << "\n";
```

Where:

- `#nrm6bo5v3rv` 11 digit random id + `#` sign. This serves as an unique tag,  I am not planning to tag only some important log lines. Tag generated with `aww tag`
- structured parameters will be in a form `"#bhr65n1a9xt Operation #name='File processor' processed #files=10 took #timeInSec=13"`
  - Strings: `#variable name='string value'`
  - Numbers: `#variable name=99999.99`



So, instead of this:

```json
{"message":"Failed to launch file","error":"File does not exist","fileToOpen":"C:\\hello\\world.txt","tag":"vkmnmvq0sis"}
```



I can have this:

```cpp
#vkmnmvq0sis Failed to launch #fileToOpen='C:\hello\world.txt'; #error='File does not exist'
```

