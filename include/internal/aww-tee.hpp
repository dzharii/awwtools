#ifndef AWW_TEE_HPP
#define AWW_TEE_HPP

#include <string>
#include <vector>

#include "aww-common.hpp"
#include "clip.h"

namespace aww::internal::aww_tee {

class aww_tee_io_dependencies_interface {
public:
  virtual ~aww_tee_io_dependencies_interface() = default;

  virtual void show_notification(const std::string& title, const std::string& message,
                                 [[maybe_unused]] aww::call_tag_t tag) = 0;
};

class aww_tee_io_dependencies : public aww_tee_io_dependencies_interface {
public:
  void show_notification(const std::string& title, const std::string& message,
                         [[maybe_unused]] aww::call_tag_t tag) override {
    aww::os::actions::show_notification(title, message);
  }
};

int aww_tee_main(const std::vector<std::string>& cmdArgs, aww_tee_io_dependencies_interface& deps);

} // namespace aww::internal::aww_tee

#endif
