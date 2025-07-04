#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "aww-common.hpp"
#include "aww.hpp"
#include "internal/aww-api.hpp"
#include "internal/aww-create.hpp"
#include "internal/aww-date.hpp"
#include "internal/aww-git-open.hpp"
#include "internal/aww-guid.hpp"
#include "internal/aww-open.hpp"
#include "internal/aww-run.hpp"
#include "internal/aww-stuff.hpp"
#include "internal/aww-tag.hpp"

#include "internal/aww-term.hpp"
#include "internal/aww-toast.hpp"

enum class aww_tool {
  none,
  create,
  date,
  git_open,
  guid,
  open,
  run,
  tag,
  term,
  toast,
  api,
  stuff,
  
};

aww_tool get_aww_tool_by_name(const std::string& aww_tool_name) {
  const std::string aww_tool_name_lower = aww::string::to_lower(aww_tool_name);

  if (aww_tool_name_lower == "aww-create") {
    return aww_tool::create;
  } else if (aww_tool_name_lower == "aww-date") {
    return aww_tool::date;
  } else if (aww_tool_name_lower == "aww-git-open") {
    return aww_tool::git_open;
  } else if (aww_tool_name_lower == "aww-guid") {
    return aww_tool::guid;
  } else if (aww_tool_name_lower == "aww-open") {
    return aww_tool::open;
  } else if (aww_tool_name_lower == "aww-run") {
    return aww_tool::run;
  } else if (aww_tool_name_lower == "aww-tag") {
    return aww_tool::tag;
  } else if (aww_tool_name_lower == "aww-term") {
    return aww_tool::term;
  } else if (aww_tool_name_lower == "aww-toast") {
    return aww_tool::toast;
  } else if (aww_tool_name_lower == "aww-api") {
    return aww_tool::api;
  } else if (aww_tool_name_lower == "aww-stuff") {
    return aww_tool::stuff;
  
  }
  return aww_tool::none;
}

int aww_main(const std::vector<std::string>& cmd_args) {
  if (cmd_args.size() == 0) {
    std::cout << "No arguments provided\n";
    return 1;
  }

  std::string aww_tool_name = "aww";
  aww_tool aww_tool = aww_tool::none;

  auto cmd_arg = cmd_args.begin();
  for (; cmd_arg != cmd_args.end(); ++cmd_arg) {
    aww_tool_name += "-" + aww::string::to_lower(*cmd_arg);
    aww_tool = get_aww_tool_by_name(aww_tool_name);

    if (aww_tool != aww_tool::none) {
      break;
    }
  }

  if (aww_tool == aww_tool::none) {
    std::cout << "No aww executable found\n";
    return 1;
  }

  // slice itCmdArg to end
  ++cmd_arg;
  std::vector<std::string> commandline_args(cmd_arg, cmd_args.end());

  try {
    switch (aww_tool) {
    case aww_tool::create: {
      aww::internal::aww_create::aww_create_io_dependencies deps;
      return aww::internal::aww_create::aww_create_main(commandline_args, deps);
    }
    case aww_tool::date: {
      aww::internal::aww_date::aww_date_io_dependencies deps;
      return aww::internal::aww_date::aww_date_main(commandline_args, deps);
    }
    case aww_tool::git_open: {
      aww::internal::aww_git_open::aww_git_open_io_dependencies deps;
      return aww::internal::aww_git_open::aww_git_open_main(commandline_args, deps);
    }
    case aww_tool::guid: {
      aww::internal::aww_guid::aww_guid_io_dependencies deps;
      return aww::internal::aww_guid::aww_guid_main(commandline_args, deps);
    }
    case aww_tool::open: {
      aww::internal::aww_open::aww_open_io_dependencies deps;
      return aww::internal::aww_open::aww_open_main(commandline_args, deps);
    }
    case aww_tool::run: {
      return aww::internal::aww_run::aww_run_main(commandline_args);
    }
    case aww_tool::tag: {
      return aww::internal::aww_tag::aww_tag_main(commandline_args);
    }
    case aww_tool::term: {
      return aww::internal::aww_term::aww_term_main(commandline_args);
    }
    case aww_tool::toast: {
      return aww::internal::aww_toast::aww_toast_main(commandline_args);
    }
    case aww_tool::api: {
      aww::internal::aww_api::aww_api_io_dependencies deps;
      return aww::internal::aww_api::aww_api_main(commandline_args, deps);
    }
    case aww_tool::stuff: {
      aww::internal::aww_stuff::aww_stuff_io_dependencies deps;
      return aww::internal::aww_stuff::aww_stuff_main(commandline_args, deps);
    }
    
    default: {
      std::cerr << "No aww executable found\n";
      return 1;
    }
    }
  } catch (std::exception& ex) {
    std::cerr << ex.what() << "\n";
    return 1;
  } catch (...) {
    std::cerr << "Caught unknown exception.\n";
    return 1;
  }
}
