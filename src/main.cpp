
#include <benchmarks.h>

#include "pico/multicore.h"
#include "pico/stdlib.h"

#include <cstdio>
#include <pico/time.h>
#include <string>

#include <nlohmann/json.hpp>

// want to run benchmarks on thread?
#define MULITCORE true

void parse_command(const nlohmann::json &json) {
  std::string name = json["benchmark"];
  std::string precision = json.value("precision", precision_default);

  auto benchmark = [&]<bool Debug, typename P>() {
    if (name == "determinant" || name == "det")
      determinant<Debug, P>(json);
    if (name == "inverse")
      inverse<Debug, P>(json);
    if (name == "lu")
      lu<Debug, P>(json);
    if (name == "qr")
      qr<Debug, P>(json);
    if (name == "matmul")
      matmul<Debug, P>(json);
    if (name == "heap-alloc")
      heap_alloc<Debug, P>(json);
    if (name == "stack-alloc")
      stack_alloc<Debug, P>(json);
    if (name == "copy")
      copy<Debug, P>(json);
    if (name == "read-flash")
      read_flash<Debug, P>(json);
    if (name == "eigen-values")
      eigen<Debug, P>(json);
    if (name == "rank")
      rank<Debug, P>(json);
    if (name == "llt")
      llt<Debug, P>(json);
  };

  auto run = [&]<typename P>() {
    printf("SOF test %s %s\n", name.c_str(), precision.c_str());
    benchmark.operator()<true, P>();
    printf("EOF test %s %s\n", name.c_str(), precision.c_str());
    printf("SOF result %s %s\n", name.c_str(), precision.c_str());
    benchmark.operator()<false, P>();
    printf("EOF result %s %s\n", name.c_str(), precision.c_str());
    printf("EOT\n");
  };
  if (precision == "float") {
    run.operator()<float>();
  }
  if (precision == "double") {
    run.operator()<double>();
  }
}

#if MULITCORE
void core1_main() {
  while (true) {
    auto *json = (nlohmann::basic_json<> *)multicore_fifo_pop_blocking();
    parse_command(*json);
    delete json; // delete from memory
    // send nullptr as confirmation
    multicore_fifo_push_blocking(0);
  }
}
#endif

// 153DAE50CB3F2B4D
int main() {
  stdio_init_all();

  // wait for PC to open console
  while (!stdio_usb_connected()) {
    sleep_ms(100);
  }

#if MULITCORE
  multicore_launch_core1(core1_main);
#endif

  const uint32_t BUFFER_SIZE = 256;
  char buffer[BUFFER_SIZE];
  while (true) {
    // wait for command
    fgets(buffer, BUFFER_SIZE, stdin);

    if (strlen(buffer) == 0) {
      sleep_ms(100);
      continue;
    }

    // parse to json
    // expects:
    // {
    //   "benchmark": "<benchmark-name>",
    //   "precision": "<float|double>",
    //
    //   # the minimal possible size for the matrices used in the benchmark
    //   "min-dimension": "<some int>",
    //   "max-dimension": "<some int>",
    //
    //   "iterations": "<some int>"
    // }

#if MULITCORE
    auto *json = new nlohmann::basic_json<>(
        nlohmann::json::parse(buffer)); // move to heap
    // move to thread
    multicore_fifo_push_blocking((uint32_t)json);
    auto response = multicore_fifo_pop_blocking(); // wait for response
#else
    auto json = nlohmann::json::parse(buffer);
    parse_command(json);
#endif
  }
}
