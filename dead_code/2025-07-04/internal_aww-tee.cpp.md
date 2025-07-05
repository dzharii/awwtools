#include <atomic>
#include <chrono>
#include <concepts>
#include <filesystem>
#include <functional>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "aww-html/aww-html.hpp"
#include "aww-os/aww-os.hpp"
#include "aww-spdlog-configuration.hpp"
#include "aww-spsc-queue/aww-spsc-queue.hpp"
#include "fmt/core.h"
#include "internal/aww-tee.hpp"
#include "spdlog/spdlog.h"
#include "webview/webview.h"

namespace aww::internal::aww_tee {
namespace fs = std::filesystem;

int aww_tee_main([[maybe_unused]] const std::vector<std::string>& cmd_args,
                 [[maybe_unused]] aww_tee_io_dependencies_interface& deps) {
  try {
    init_default_spdlog_configuration("aww-tee");

    spdlog::info("aww tee");
    spdlog::info("Application starting.");

    // Create a SPSC queue to buffer input lines (capacity of 1024).
    aww::single_producer_single_consumer_queue<std::string, 10240> input_queue;

    // Create the webview with an initial HTML that holds an empty <ul>
    // (debug shows console true)
    webview::webview w(true, nullptr);
    w.set_title("Basic Example");
    w.set_size(480, 320, WEBVIEW_HINT_NONE);
    std::string html = R"HTML(
<html>
<head>
    <meta charset="utf-8">
</head>
<body>
    Hello
    <ul id="log"></ul>
    <script>
        window.log = document.getElementById("log");
        window.metric_logsPolled = 0;

        // Function to poll for new logs from the backend.
        async function pollLogs() {
            window.metric_logsPolled += 1;
            while (typeof pollNewLogs !== "function") {
                await new Promise(resolve => setTimeout(resolve, 1));
            }
            try {
                const data = await pollNewLogs();
                if (data?.logs?.length) {
                    console.log('data = ', data);
                    data.logs.forEach(msg => {
                        const li = document.createElement("li");
                        li.textContent = msg;
                        window.log.appendChild(li);
                    });
                } else {
                  console.log(`#ukc2ni2z44a empty data = `, data);
                }
            } catch (e) {
                console.error("Error polling logs:", e);
            }
            setTimeout(pollLogs, 10);
        }

        // Ensure notifyReady() is called unconditionally when the document is ready.
        setTimeout(async () => {
            console.log('#hdnhj891gs0 waiting for notifyReady binding');
            while (typeof notifyReady !== "function") {
                await new Promise(resolve => setTimeout(resolve, 1));
            }
            console.log('#sfbtfx0o5iu calling notifyReady');
            notifyReady();

            console.log('#k2tgfs49h42 waiting for pollNewLogs binding');
            while (typeof pollNewLogs !== "function") {
                await new Promise(resolve => setTimeout(resolve, 1));
            }
            console.log('#wxwvpri34v8 calling pollLogs()');
            // setTimeout(() => pollLogs(), 1000);
        }, 0);
    </script>
</body>
</html>
)HTML";
    w.set_html(html);

    // Flag indicating that the webview is now initialized and ready.
    std::atomic_bool webview_ready{false};

    // Bind the notifyReady function to set webview_ready to true.
    w.bind(
        "notifyReady",
        [&w, &webview_ready](const std::string& id, const std::string&, void* /*arg*/) {
          spdlog::info("notifyReady callback invoked; setting webview_ready to true.");
          webview_ready.store(true, std::memory_order_release);
          w.resolve(id, 0, "");
        },
        nullptr);

    // Bind the asynchronous pollNewLogs function that polls up to { poll_max_messages } messages.
    constexpr size_t poll_max_messages = 50;
    std::atomic<bool> poll_thread_running{false};
    w.bind(
        "pollNewLogs",
        [&](const std::string& id, const std::string& /*unused*/, void* /*arg*/) {
          // Only spawn a new thread if none is running.
          bool expected = false;
          if (!poll_thread_running.compare_exchange_strong(expected, true)) {
            spdlog::info("#x5cd38goy08 pollNewLogs already running, skipping spawning a new thread.");
            w.resolve(id, 0, "{\"logs\": []}");
            return;
          }

          // Spawn a new thread.
          std::thread([&, id]() {
            try {
              spdlog::info("#rcvqzrkyni7 pollNewLogs thread started.");
              std::vector<std::string> logs;
              // Poll messages until the queue is empty or we've collected more than 50 messages.
              while (auto opt = input_queue.pop()) {
                logs.push_back(*opt);
                if (logs.size() > poll_max_messages) {
                  break;
                }
              }
              spdlog::info("#pni42fmt3ey pollNewLogs log size = {}", logs.size());

              // Construct a JSON string for the logs.
              std::string json = "{\"logs\": [";
              bool first = true;
              for (const auto& msg : logs) {
                if (!first) {
                  json += ",";
                }
                json += aww::escape_string_as_json_string(msg);
                first = false;
              }
              json += "]}";
              spdlog::info("#rpz21stz36n resolved pollNewLogs with json size: '{}' bytes", json.size());
              w.resolve(id, 0, json.c_str());
            } catch (const std::exception& ex) {
              spdlog::error("#iu1gyxyb1px Exception in pollNewLogs thread: {}", ex.what());
              w.resolve(id, 1, "{\"logs\": []}");
            } catch (...) {
              spdlog::error("#jz4drmxj0wf Unknown exception in pollNewLogs thread.");
              w.resolve(id, 1, "{\"logs\": []}");
            }
            // Ensure the flag is cleared no matter what.
            poll_thread_running.store(false, std::memory_order_release);
          }).detach();
        },
        nullptr);

    // Check if standard input is redirected (i.e. not a terminal)
    bool has_redirected_input = aww::has_redirected_standard_input();
    spdlog::info("#has_redirected_input = {}", has_redirected_input);

    // Thread that reads from redirected standard input and pushes lines into the SPSC queue.

    std::thread input_thread;
    if (has_redirected_input) {
      input_thread = std::thread([&input_queue, &webview_ready]() {
        spdlog::info("#4a7oi5ocha4 waiting for webview_ready");

        while (!webview_ready.load(std::memory_order_acquire)) {
          webview_ready.wait(false, std::memory_order_relaxed);
        }

        spdlog::info("#bde8bz7nnc2 begin input_thread");
        std::string line;
        while (std::getline(std::cin, line)) {
          input_queue.push(line);
          spdlog::info("#4t2hvm4vuqp front={}; back={}", input_queue.front(), input_queue.back());
          std::cout << line << "\n";
        }
      });
    }

    // Run the webview loop (this call blocks until the window is closed)
    w.run();
    spdlog::info("Application exiting normally.");

    if (input_thread.joinable()) {
      input_thread.join();
    }
  } catch (const webview::exception& e) {
    spdlog::critical("Unhandled exception in main: {}", e.what());
    return 1;
  }

  return 0;
}

} // namespace aww::internal::aww_tee
