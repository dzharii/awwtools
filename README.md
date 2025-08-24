# aww tools

[o:ʊ to͞olz] 

For developers who work across Windows, Linux, and potentially other environments, finding and learning a new set of tools for each platform can be time-consuming and complex. 
Each system often requires a unique workflow, which adds friction to what should be simple tasks.

[aww tools](https://github.com/dzharii/awwtools) goal is to offer a  cross-platform toolkit built in C++ to simplify productivity workflows across different systems. 
By prioritizing simplicity and early releases, these tools are designed to provide lightweight, accessible functionality without the need to re-learn tools on each OS. 
While these tools are experimental and iteratively improved, they are fully functional for daily use, helping developers (me!) explore, test, and even contribute to new ideas as the toolkit evolves.

[aww tools](https://github.com/dzharii/awwtools) is my exploration into productivity, built through iterative, early-stage releases. 
Code here isn’t perfect; it’s a space for testing, refining, and sharing ideas.

Project moto:

---



<center><i>Embrace the AWW!</i></center>



---

**Current project state**

I started awwtools several years ago to learn modern C++ and move my PowerShell tools to a faster, cross-platform stack. I chose C++ **for its complexity** and to test myself against it. Curiosity drove the work: how is C++ today different from ten years ago. The answer surprised me. C++ feels modern. The goal is met. I understand the toolchain, CMake, and cross-platform builds. I can write portable code and ship small utilities. awwtools remains a place to refine that skill and improve the tools I use every day.



**Everyday use and scripting**

Developer block is like writer's block. Shipping small tools broke it for me. The tools now solve **my** real problems. They make **my** routine work faster and more pleasant.

I used `aww date` for years to prefix filenames with an ISO date. Today I lean on [Espanso](https://espanso.org/) for quick expansion. I still use `aww date` when I want the date joined with a safe filename. It does the sanitation right and saves time.

`aww run` is my main entry point for scripts. I keep the same script name across platforms and swap the extension: `.ps1` or `.cmd` on Windows, `.sh` on Linux. I call it as `aww run scriptname` without an extension. The command stays the same everywhere. The interface is consistent. That is a core goal of awwtools.

I add [new extension scripts](https://github.com/dzharii/dot-awwtools/tree/main/aww-scripts) to `aww run` often. It keeps the toolkit alive and tailored to my workflow.

`aww run` also has basic Lua support. Lua scripts run the same on Windows and Linux through an embedded interpreter. One script. No shell differences. Portable by default.



**Direction and next steps**

My pace on awwtools slowed. I fixed the big inconsistencies. I ran the most interesting experiments in this C++ version. I learned enough C++ to be effective. I know the toolchain and the libraries. I use awwtools every day.

So, what is next. Do I add more C++ tools, or change layers. I am almost happy with the current architecture. But I want a new challenge and a deeper look under the hood. I really need to reconsider some of my previous decisions and make the things easier. 

I will not switch to another modern high level language. .NET, Go, and Rust would teach me less right now. C++ is already modern.

I am moving to C99. I am rewriting and refactoring from scratch in C. It is fun. It pushes me closer to the metal. It keeps the cross platform goal, while forcing clear, simple designs.



**Developer Retrospectives: C vs C++**

- **"Why should I have written ZeroMQ in C, not C++ (part I)"** (May 2012) – Martin Sustrik (ZeroMQ co-author) admits that choosing C++ was a poor choice in hindsight.  
  [250bpm.com](https://250bpm.com/blog:4/)  
  ZeroMQ required strict error handling with no undefined behavior, and C++ exceptions made that very hard. Sustrik shows that C’s explicit error returns simplify reasoning about failures. He ultimately switched to "C++ minus exceptions" (effectively a C-style API) in ZeroMQ to avoid exception-related pitfalls.
- **"NNG Rationale: Why I’m rewriting nanomsg in C"** (2020) – Garrett D’Amore explains why the new nanomsg (NNG) and its Go bindings were designed in C instead of C++. 
  [nng.nanomsg.org](https://nng.nanomsg.org/RATIONALE.html)  
  He writes that he was dissatisfied with the ZeroMQ licensing model and the C++ baggage that came with it. In other words, avoiding C++ features (and sticking to plain C) eliminated unwanted complexity and made the library easier to embed and maintain.

**Thank you:**

- 💖 [bsamseth/cpp-project: Boiler plate template for C++ projects, with CMake, Doctest, Travis CI, Appveyor, Github Actions and coverage reports.](https://github.com/bsamseth/cpp-project)
- [Lua](https://www.lua.org/) is a free and open-source project by Roberto Ierusalimschy and the rest of the [Lua team](https://www.lua.org/authors.html).
- [Clip Library](https://github.com/dacap/clip) by David Capello is a library to copy/retrieve content to/from the clipboard/pasteboard.
- [{fmt}](https://github.com/fmtlib/fmt) is an open-source formatting library providing a fast and safe alternative to C stdio and C++ iostreams.
- [nlohmann/json](https://github.com/nlohmann/json) library is a C++ library that simplifies working with JSON, allowing seamless parsing, serialization, and manipulation of JSON data with an intuitive, modern interface.
- [spdlog](https://github.com/gabime/spdlog) library is a fast, header-only C++ logging library that supports both synchronous and asynchronous logging, offering a simple, extensible API for high-performance logging with various formatting options and output targets.
- [WinToast](https://github.com/mohabouje/WinToast) is a lightweight C++ library for displaying native Windows toast notifications with customizable templates and event handling.
- [cpr](https://github.com/libcpr/cpr) is a C++ library that provides a simple wrapper around libcurl, enabling developers to perform HTTP requests with an intuitive and modern interface.
