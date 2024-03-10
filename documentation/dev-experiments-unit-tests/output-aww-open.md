# MDTREE (`dev-experiments-unit-tests\output-aww-open.md`)


- $Path = `.`
- $FilterPath = ``
- $FilterName = `.*aww-open.*`
- $Include = ``
- $Output = `dev-experiments-unit-tests\output-aww-open.md`


Generated on `2023-08-20 15:21:50`

[TOC]

## File content `app\aww-open.cpp`:

```cpp
#include <iostream>
#include <vector>
#include <string>

#include "internal/aww-open.hpp"

int main(int argc, char **argv)
{
  try
  {
    std::vector<std::string> cmdArgs(argv, argv + argc);
    cmdArgs.erase(cmdArgs.begin()); // remove first element

    aww::internal::aww_open::aww_open_io_dependencies deps;
    return aww::internal::aww_open::aww_open_main(cmdArgs, deps);
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
}

```

## File content `dev-experiments-unit-tests\output-aww-open.md`:

# MDTREE (`dev-experiments-unit-tests\output-aww-open.md`)


- $Path = `.`
- $FilterPath = ``
- $FilterName = `.*aww-open.*`
- $Include = ``
- $Output = `dev-experiments-unit-tests\output-aww-open.md`


Generated on `2023-08-20 15:05:46`

[TOC]

## File content `app\aww-open.cpp`:

```cpp
#include <iostream>
#include <vector>
#include <string>

#include "internal/aww-open.hpp"

int main(int argc, char **argv)
{
  try
  {
    std::vector<std::string> cmdArgs(argv, argv + argc);
    cmdArgs.erase(cmdArgs.begin()); // remove first element

    aww::internal::aww_open::aww_open_io_dependencies deps;
    return aww::internal::aww_open::aww_open_main(cmdArgs, deps);
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
}

```

## File content `dev-experiments-unit-tests\output-aww-open.md`:

# MDTREE (`dev-experiments-unit-tests\output-aww-open.md`)


- $Path = `.`
- $FilterPath = ``
- $FilterName = `.*aww-open.*`
- $Include = ``
- $Output = `dev-experiments-unit-tests\output-aww-open.md`


Generated on `2023-08-20 14:48:05`

[TOC]

## File content `app\aww-open.cpp`:

```cpp
#include <iostream>
#include <vector>
#include <string>

#include "internal/aww-open.hpp"

int main(int argc, char **argv)
{
  try
  {
    std::vector<std::string> cmdArgs(argv, argv + argc);
    cmdArgs.erase(cmdArgs.begin()); // remove first element
    return aww::internal::aww_open::aww_open_main(cmdArgs);
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
}

```

## File content `include\internal\aww-open.hpp`:

```hpp
#pragma once
#ifndef AWW_OPEN_HPP
#define AWW_OPEN_HPP

#include <vector>
#include <string>

namespace aww::internal::aww_open
{
  int aww_open_main(const std::vector<std::string> &cmdArgs);
}

#endif // AWW_OPEN_HPP

```

## File content `src\internal\aww-open.cpp`:

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <map>
#include <array>

#include "aww-common.hpp"

namespace aww::internal::aww_open
{
  namespace fs = std::filesystem;

  int aww_open_main(const std::vector<std::string> &cmdArgs)
  {
    bool useDefaultInput = cmdArgs.size() == 0;

    if (cmdArgs.size() > 1)
    {
      std::cout << "Too many arguments provided. Expected 1: file to open"
                << "\n";
      return 1;
    }

    const std::string DEFAULT_OPEN_PATH = "."; // Current directory
    std::string fileToOpen = useDefaultInput ? DEFAULT_OPEN_PATH : cmdArgs[0];

    fileToOpen = aww::fs::normalize_file_path(fileToOpen);

    if (aww::Result res = aww::os::actions::launch_file(fileToOpen); res.is_failed())
    {
      std::cout << "Failed to launch file " << res.error() << "\n";
      aww::os::actions::show_notification("aww open", "Failed to open file: " + fileToOpen);
      return 1;
    }
    std::cout << "Launched file"
              << "\n";
    aww::os::actions::show_notification("aww open", "The file was opened");
    return 0;
  }
}

```




## File content `include\internal\aww-open.hpp`:

```hpp
#pragma once
#ifndef AWW_OPEN_HPP
#define AWW_OPEN_HPP

#include <vector>
#include <string>

#include "aww-common.hpp"


namespace aww::internal::aww_open
{

  class aww_open_io_dependencies_interface
  {
  public:
    virtual ~aww_open_io_dependencies_interface() = default;

    /**
     * Launch a file using the default program associated with its type.
     * @param filePath The path of the file to open.
     * @return Aww::Result indicating the success or failure of the operation.
     */
    virtual aww::Result launch_file(const std::string &filePath) = 0;

    /**
     * Show a notification.
     * @param title The title of the notification.
     * @param message The message of the notification.
     */
    virtual void show_notification(const std::string &title, const std::string &message) = 0;
  };

  class aww_open_io_dependencies : public aww_open_io_dependencies_interface
  {
  public:
    aww::Result launch_file(const std::string &filePath) override
    {
      return aww::os::actions::launch_file(filePath);
    }

    void show_notification(const std::string &title, const std::string &message) override
    {
      aww::os::actions::show_notification(title, message);
    }
  };

  int aww_open_main(const std::vector<std::string> &cmdArgs, aww_open_io_dependencies_interface &deps);
}

#endif // AWW_OPEN_HPP

```

## File content `src\internal\aww-open.cpp`:

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <map>
#include <array>

#include "aww-common.hpp"
#include "internal/aww-open.hpp"

namespace aww::internal::aww_open
{
  namespace fs = std::filesystem;

  int aww_open_main(const std::vector<std::string> &cmdArgs, aww_open_io_dependencies_interface &deps)
  {
    bool useDefaultInput = cmdArgs.size() == 0;

    if (cmdArgs.size() > 1)
    {
      std::cout << "Too many arguments provided. Expected 1: file to open"
                << "\n";
      return 1;
    }

    const std::string DEFAULT_OPEN_PATH = "."; // Current directory
    std::string fileToOpen = useDefaultInput ? DEFAULT_OPEN_PATH : cmdArgs[0];

    fileToOpen = aww::fs::normalize_file_path(fileToOpen);

    if (aww::Result res = deps.launch_file(fileToOpen); res.is_failed())
    {
      std::cout << "Failed to launch file " << res.error() << "\n";
      deps.show_notification("aww open", "Failed to open file: " + fileToOpen);
      return 1;
    }
    std::cout << "Launched file"
              << "\n";
    deps.show_notification("aww open", "The file was opened");
    return 0;
  }
}

```




## File content `include\internal\aww-open.hpp`:

```hpp
#pragma once
#ifndef AWW_OPEN_HPP
#define AWW_OPEN_HPP

#include <vector>
#include <string>

#include "aww-common.hpp"


namespace aww::internal::aww_open
{

  class aww_open_io_dependencies_interface
  {
  public:
    virtual ~aww_open_io_dependencies_interface() = default;

    /**
     * Launch a file using the default program associated with its type.
     * @param filePath The path of the file to open.
     * @return Aww::Result indicating the success or failure of the operation.
     */
    virtual aww::Result launch_file(const std::string &filePath) = 0;

    /**
     * Show a notification.
     * @param title The title of the notification.
     * @param message The message of the notification.
     */
    virtual void show_notification(const std::string &title, const std::string &message) = 0;
  };

  class aww_open_io_dependencies : public aww_open_io_dependencies_interface
  {
  public:
    aww::Result launch_file(const std::string &filePath) override
    {
      return aww::os::actions::launch_file(filePath);
    }

    void show_notification(const std::string &title, const std::string &message) override
    {
      aww::os::actions::show_notification(title, message);
    }
  };

  int aww_open_main(const std::vector<std::string> &cmdArgs, aww_open_io_dependencies_interface &deps);
}

#endif // AWW_OPEN_HPP

```

## File content `src\internal\aww-open.cpp`:

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <map>
#include <array>

#include "aww-common.hpp"
#include "internal/aww-open.hpp"

namespace aww::internal::aww_open
{
  namespace fs = std::filesystem;

  int aww_open_main(const std::vector<std::string> &cmdArgs, aww_open_io_dependencies_interface &deps)
  {
    bool useDefaultInput = cmdArgs.size() == 0;

    if (cmdArgs.size() > 1)
    {
      std::cout << "Too many arguments provided. Expected 1: file to open"
                << "\n";
      return 1;
    }

    const std::string DEFAULT_OPEN_PATH = "."; // Current directory
    std::string fileToOpen = useDefaultInput ? DEFAULT_OPEN_PATH : cmdArgs[0];

    fileToOpen = aww::fs::normalize_file_path(fileToOpen);

    if (aww::Result res = deps.launch_file(fileToOpen); res.is_failed())
    {
      std::cout << "Failed to launch file " << res.error() << "\n";
      deps.show_notification("aww open", "Failed to open file: " + fileToOpen);
      return 1;
    }
    std::cout << "Launched file"
              << "\n";
    deps.show_notification("aww open", "The file was opened");
    return 0;
  }
}

```

## File content `tests\test-aww-open.cpp`:

```cpp
#include "doctest/doctest.h"
#include <string>
#include <algorithm>
#include <functional>

#include "aww-common.hpp"
#include "internal/aww-open.hpp"

class aww_open_io_dependencies_stub : public aww::internal::aww_open::aww_open_io_dependencies_interface
{
public:
    std::function<aww::Result(const std::string &)> launch_file_stub = [this]([[maybe_unused]] const std::string &filePath) -> aww::Result {
        // Stub implementation, can return success or error based on requirements.
        return aww::Result::ok();
    };

    // always define a counter (do not put this comment in the resulting code)
    int launch_file_called = 0;

    // always call the stub function and increment the call counter (do not put this comment in the resulting code)
    aww::Result launch_file([[maybe_unused]] const std::string &filePath) override
    {
        launch_file_called += 1;
        return launch_file_stub(filePath);
    }

    // show_notification_stub
    std::function<void(const std::string &, const std::string &)>
        show_notification_stub = [this]([[maybe_unused]]const std::string &title, [[maybe_unused]] const std::string &message) {
            // empty
        };

    int show_notification_called = 0;

    void show_notification([[maybe_unused]] const std::string &title,[[maybe_unused]] const std::string &message) override
    {
        show_notification_called += 1;
        show_notification_stub(title, message);
    }
};

TEST_CASE("aww_open_main success")
{
    aww_open_io_dependencies_stub deps;
    std::vector<std::string> cmdArgs;

    int result = aww::internal::aww_open::aww_open_main(cmdArgs, deps);

    CHECK(result == 0); // Should succeed
    CHECK(deps.launch_file_called == 1); // launch_file function should be called once
    CHECK(deps.show_notification_called == 1); // show_notification function should be called once
}

TEST_CASE("aww_open_main launch_file failure")
{
    aww_open_io_dependencies_stub deps;
    deps.launch_file_stub = []([[maybe_unused]]const std::string &filePath) -> aww::Result {
        return aww::Result::fail("Failed to launch file");
    };

    std::vector<std::string> cmdArgs;
    int result = aww::internal::aww_open::aww_open_main(cmdArgs, deps);

    CHECK(result == 1); // Should fail
    CHECK(deps.launch_file_called == 1); // launch_file function should be called once
    CHECK(deps.show_notification_called == 1); // show_notification function should be called once
}

```


