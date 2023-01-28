

## Play audio file for aww remind
2023-01-20 [OpenAL: Cross Platform 3D Audio](https://www.openal.org/)

2023-01-19 💖 [sf::Music Class Reference SFML / Learn / 2.0 Documentation](https://www.sfml-dev.org/documentation/2.0/classsf_1_1Music.php) 

> 2023-01-19 [CMake: Building SFML and Game Projects on Linux by Dane Bulat Medium](https://dane-bulat.medium.com/cmake-building-sfml-and-game-projects-on-linux-3947b3ba6e8)
>
> 2023-01-19 [SFML as a Dependency in a CMake Project by Riley Entertainment Game Dev Medium](https://medium.com/@RileyEntertainmentGameDev/sfml-as-a-dependency-in-a-cmake-project-a37be848243e)
>
> https://github.com/SFML/SFML/tags



> ## Example CMake Project with an SFML Dependency
>
> In a previous blog post, I included a reference to an example Visual Studio project that just opens a window and draws a circle. Here I’ll be referencing an [example CMake project](https://github.com/ryantherileyman/cmake-testing-grounds/tree/main/learning-sfml/sfml-intro/setup-manual/sfml-cmake-setup) with the exact same C++ source code. Instead of a Visual Studio solution and project file, there is a single `CMakeLists.txt` file that acts as the cross-platform build script.
>
> ```cmake
> cmake_minimum_required(VERSION 3.19)
> project(SfmlWithCMake VERSION 1.0)
> include(FetchContent)
> set(BUILD_SHARED_LIBS OFF)
> FetchContent_Declare(
>  SFML
>  GIT_REPOSITORY https://github.com/SFML/SFML.git
>  GIT_TAG 2.5.1
> )
> FetchContent_MakeAvailable(SFML)
> set(CMAKE_CXX_STANDARD 11)
> set(CMAKE_CXX_STANDARD_REQUIRED true)
> add_executable(
>  SfmlWithCMake
>  main.cpp
> )
> target_link_libraries(
>  SfmlWithCMake
>  sfml-graphics
> )
> ```
>
> The `include` command tells CMake to make use of the FetchContent module.
>
> The line `set(BUILD_SHARED_LIBS OFF)` is specific to the way I wanted to pull in SFML. I’m deciding that I want CMake to build SFML as a static library. Looking through SFML’s own `CMakeLists.txt` build file, its default behaviour is to build it as a shared library for every platform but iOS:
>
> ```cmake
> # add an option for choosing the build type (shared or static)
> if(NOT (SFML_OS_IOS OR SFML_OS_ANDROID))
>     sfml_set_option(BUILD_SHARED_LIBS TRUE BOOL "TRUE to build SFML as shared libraries, FALSE to build it as static libraries")
> else()
>     if(SFML_OS_IOS)
>         set(BUILD_SHARED_LIBS FALSE)
>     elseif(SFML_OS_ANDROID)
>         set(BUILD_SHARED_LIBS TRUE)
>     endif()
> endif()
> ```
>
> The `FetchContent_Declare` command is used to tell CMake where it can find your dependency. In this example, CMake will download SFML from the Git tag named `2.5.1` during its generator phase.

## Try SFML



2023-01-19 [audio - How to play or open *.mp3 or *.wav sound file in c++ program? - Stack Overflow](https://stackoverflow.com/questions/22253074/how-to-play-or-open-mp3-or-wav-sound-file-in-c-program)

## Windows

First of all, write the following code:

```cpp
#include <Mmsystem.h>
#include <mciapi.h>
//these two headers are already included in the <Windows.h> header
#pragma comment(lib, "Winmm.lib")
```

To open *.mp3:

```cpp
mciSendString("open \"*.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);
```

To play *.mp3:

```cpp
mciSendString("play mp3", NULL, 0, NULL);
```

To play and wait until the *.mp3 has finished playing:

```cpp
mciSendString("play mp3 wait", NULL, 0, NULL);
```

To replay (play again from start) the *.mp3:

```cpp
mciSendString("play mp3 from 0", NULL, 0, NULL);
```

To replay and wait until the *.mp3 has finished playing:

```cpp
mciSendString("play mp3 from 0 wait", NULL, 0, NULL);
```

To play the *.mp3 and replay it every time it ends like a loop:

```cpp
mciSendString("play mp3 repeat", NULL, 0, NULL);
```

If you want to do something when the *.mp3 has finished playing, then you need to `RegisterClassEx` by the `WNDCLASSEX` structure, `CreateWindowEx` and process it's messages with the **GetMessage**, **TranslateMessage** and **DispatchMessage** functions in a `while` loop and call:

```cpp
mciSendString("play mp3 notify", NULL, 0, hwnd); //hwnd is an handle to the window returned from CreateWindowEx. If this doesn't work, then replace the hwnd with MAKELONG(hwnd, 0).
```

In the window procedure, add the `case MM_MCINOTIFY:` The code in there will be executed when the mp3 has finished playing.

**But** if you program a **Console** Application and you don't deal with **windows**, then you can `CreateThread` in suspend state by specifying the `CREATE_SUSPENDED` flag in the `dwCreationFlags` parameter and keep the return value in a `static` variable and call it whatever you want. For instance, I call it mp3. The type of this `static` variable is `HANDLE` of course.

Here is the `ThreadProc` for the `lpStartAddress` of this thread:

```cpp
DWORD WINAPI MP3Proc(_In_ LPVOID lpParameter) //lpParameter can be a pointer to a structure that store data that you cannot access outside of this function. You can prepare this structure before `CreateThread` and give it's address in the `lpParameter`
{
    Data *data = (Data*)lpParameter; //If you call this structure Data, but you can call it whatever you want.
    while (true)
    {
        mciSendString("play mp3 from 0 wait", NULL, 0, NULL);
        //Do here what you want to do when the mp3 playback is over
        SuspendThread(GetCurrentThread()); //or the handle of this thread that you keep in a static variable instead
    }
}
```

All what you have to do now is to `ResumeThread(mp3);` every time you want to replay your mp3 and something will happen every time it finishes.

You can `#define play_my_mp3 ResumeThread(mp3);` to make your code more readable.

Of course you can remove the `while (true)`, `SuspendThread` and the **from 0** codes, if you want to play your mp3 file only **once** and do whatever you want when it is over.

If you **only** remove the `SuspendThread` call, then the sound will play over and over again and do something whenever it is over. This is equivalent to:

```cpp
mciSendString("play mp3 repeat notify", NULL, 0, hwnd); //or MAKELONG(hwnd, 0) instead
```

in windows.

To pause the *.mp3 in middle:

```cpp
mciSendString("pause mp3", NULL, 0, NULL);
```

and to resume it:

```cpp
mciSendString("resume mp3", NULL, 0, NULL);
```

To stop it in middle:

```cpp
mciSendString("stop mp3", NULL, 0, NULL);
```

Note that you cannot resume a sound that has been stopped, but only paused, but you can replay it by carrying out the **play** command. When you're done playing this *.mp3, don't forget to:

```cpp
mciSendString("close mp3", NULL, 0, NULL);
```

All these actions also apply to (work with) wave files too, but with wave files, you can use "waveaudio" instead of "mpegvideo". Also you can just play them directly without opening them:

```cpp
PlaySound("*.wav", GetModuleHandle(NULL), SND_FILENAME);
```

If you don't want to specify an handle to a module:

```cpp
sndPlaySound("*.wav", SND_FILENAME);
```

If you don't want to wait until the playback is over:

```cpp
PlaySound("*.wav", GetModuleHandle(NULL), SND_FILENAME | SND_ASYNC);
//or
sndPlaySound("*.wav", SND_FILENAME | SND_ASYNC);
```

To play the wave file over and over again:

```cpp
PlaySound("*.wav", GetModuleHandle(NULL), SND_FILENAME | SND_ASYNC | SND_LOOP);
//or
sndPlaySound("*.wav", SND_FILENAME | SND_ASYNC | SND_LOOP);
```

Note that you must specify both the `SND_ASYNC` and `SND_LOOP` flags, because you never going to wait until a sound, that repeats itself countless times, is over!

Also you can `fopen` the wave file and copy all it's bytes to a buffer (an enormous/huge (very big) array of bytes) with the `fread` function and then:

```cpp
PlaySound(buffer, GetModuleHandle(NULL), SND_MEMORY);
//or
PlaySound(buffer, GetModuleHandle(NULL), SND_MEMORY | SND_ASYNC);
//or
PlaySound(buffer, GetModuleHandle(NULL), SND_MEMORY | SND_ASYNC | SND_LOOP);
//or
sndPlaySound(buffer, SND_MEMORY);
//or
sndPlaySound(buffer, SND_MEMORY | SND_ASYNC);
//or
sndPlaySound(buffer, SND_MEMORY | SND_ASYNC | SND_LOOP);
```

Either `OpenFile` or `CreateFile` or `CreateFile2` and either `ReadFile` or `ReadFileEx` functions can be used instead of `fopen` and `fread` functions.

Hope this fully answers perfectly your question.



## ChatGPT Generated

Here's an example of C++ code for playing a sound notification using SFML library in a separate thread and allowing the user to stop the audio by pressing `Ctrl+C`:

```cpp
#include <SFML/Audio.hpp>
#include <iostream>
#include <thread>
#include <csignal>

std::thread soundThread;
bool running = true;

void signalHandler(int signum) {
    running = false;
    soundThread.join();
    std::cout << "Interrupt signal (" << signum << ") received.\n";
    exit(signum);
}

void playSound(const std::string &soundFile) {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(soundFile))
    {
        std::cout << "Error loading sound file" << std::endl;
        return;
    }

    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.play();

    while (sound.getStatus() == sf::Sound::Playing && running)
    {
        // Wait for the sound to finish playing
    }
}

int main()
{
    std::string soundFile = "notification.wav";

    signal(SIGINT, signalHandler);

    soundThread = std::thread(playSound, soundFile);
    soundThread.detach();

    while(running) {
        // Do nothing, waiting for the user to press Ctrl+C
    }

    return 0;
}
```



This code uses SFML library to load the sound file and play it in a separate thread using `std::thread`. The `playSound` function is used to play the sound. The `getStatus` function is used to check if the sound is still playing. The `signal` function is used to handle the `SIGINT` signal (`Ctrl+C`) to stop the audio and exit the program.

When the user press `Ctrl+C`, the `signalHandler` function is called, and it sets the `running` variable to false, which will stop the sound thread and exit the program.

Also, the `detach()` method is used to detach the thread from the main thread, so it can run independently and the program doesn't wait for it to finish.

Please note that this is a general example, you might need to adjust it based on your specific project's structure and requirements.

