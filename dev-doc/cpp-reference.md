# CPP 17 Reference
Date: 2023-06-22



## filesystem functions that are noexcept



```
In C++17, the following functions in the std::filesystem library are specified to not throw exceptions:

std::filesystem::exists()
std::filesystem::equivalent()
std::filesystem::is_block_file()
std::filesystem::is_character_file()
std::filesystem::is_directory()
std::filesystem::is_empty()
std::filesystem::is_fifo()
std::filesystem::is_other()
std::filesystem::is_regular_file()
std::filesystem::is_socket()
std::filesystem::is_symlink()
```



## Global error handlers

```cpp
int main(int argc, char **argv)
{
  try
  {
    std::vector<std::string> cmdArgs(argv, argv + argc);
    cmdArgs.erase(cmdArgs.begin()); // remove first element

    aww::internal::aww_create::aww_create_io_dependencies deps;
    return aww::internal::aww_create::aww_create_main(cmdArgs, deps);
  }
  catch (std::exception &ex)
  {
    std::cerr << ex.what() << "\n";
    return 1;
  }
  catch (...)
  {
    std::cerr << "Caught unknown exception.\n";
    return 1;
  }
```



