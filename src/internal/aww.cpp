#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <unordered_set>

#include "aww-common.hpp"

#include "internal/aww-create.hpp"
#include "internal/aww-date.hpp"
#include "internal/aww-git-open.hpp"
#include "internal/aww-guid.hpp"
#include "internal/aww-open.hpp"
#include "internal/aww-run.hpp"
#include "internal/aww-tag.hpp"
#include "internal/aww-term.hpp"
#include "internal/aww-toast.hpp"

enum class AwwTool
{
  None,
  Create,
  Date,
  GitOpen,
  Guid,
  Open,
  Run,
  Tag,
  Term,
  Toast,
};

AwwTool getAwwTool(const std::string &awwTool)
{
  const std::string awwToolLower = aww::string::to_lower(awwTool);

  if (awwToolLower == "aww-create")
  {
    return AwwTool::Create;
  }
  else if (awwToolLower == "aww-date")
  {
    return AwwTool::Date;
  }
  else if (awwToolLower == "aww-git-open")
  {
    return AwwTool::GitOpen;
  }
  else if (awwToolLower == "aww-guid")
  {
    return AwwTool::Guid;
  }
  else if (awwToolLower == "aww-open")
  {
    return AwwTool::Open;
  }
  else if (awwToolLower == "aww-run")
  {
    return AwwTool::Run;
  }
  else if (awwToolLower == "aww-tag")
  {
    return AwwTool::Tag;
  }
  else if (awwToolLower == "aww-term")
  {
    return AwwTool::Term;
  }
  else if (awwToolLower == "aww-toast")
  {
    return AwwTool::Toast;
  }
  return AwwTool::None;
}

int aww_main(const std::vector<std::string> &cmdArgs)
{
  if (cmdArgs.size() == 0)
  {
    std::cout << "No arguments provided\n";
    return 1;
  }

  std::string awwToolName = "aww";
  AwwTool awwTool = AwwTool::None;

  auto itCmdArg = cmdArgs.begin();
  for (; itCmdArg != cmdArgs.end(); ++itCmdArg)
  {
    awwToolName += "-" + aww::string::to_lower(*itCmdArg);
    awwTool = getAwwTool(awwToolName);

    if (awwTool != AwwTool::None)
    {
      break;
    }
  }

  if (awwTool == AwwTool::None)
  {
    std::cout << "No aww executable found\n";
    return 1;
  }

  // slice itCmdArg to end
  ++itCmdArg;
  std::vector<std::string> awwExecutableArgs(itCmdArg, cmdArgs.end());

  try
  {
    switch (awwTool)
    {
    case AwwTool::Create:
    {
      aww::internal::aww_create::aww_create_io_dependencies deps;
      return aww::internal::aww_create::aww_create_main(awwExecutableArgs, deps);
    }
    case AwwTool::Date:
    {
      aww::internal::aww_date::aww_date_io_dependencies deps;
      return aww::internal::aww_date::aww_date_main(awwExecutableArgs, deps);
    }
    case AwwTool::GitOpen:
    {
      aww::internal::aww_git_open::aww_git_open_io_dependencies deps;
      return aww::internal::aww_git_open::aww_git_open_main(awwExecutableArgs, deps);
    }
    case AwwTool::Guid:
    {
      aww::internal::aww_guid::aww_guid_io_dependencies deps;
      return aww::internal::aww_guid::aww_guid_main(awwExecutableArgs, deps);
    }
    case AwwTool::Open:
    {
      aww::internal::aww_open::aww_open_io_dependencies deps;
      return aww::internal::aww_open::aww_open_main(awwExecutableArgs, deps);
    }
    case AwwTool::Run:
    {
      return aww::internal::aww_run::aww_run_main(awwExecutableArgs);
    }
    case AwwTool::Tag:
    {
      return aww::internal::aww_tag::aww_tag_main(awwExecutableArgs);
    }
    case AwwTool::Term:
    {
      return aww::internal::aww_term::aww_term_main(awwExecutableArgs);
    }
    case AwwTool::Toast:
    {
      return aww::internal::aww_toast::aww_toast_main(awwExecutableArgs);
    }
    default:
    {
      std::cerr << "No aww executable found\n";
      return 1;
    }}
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
