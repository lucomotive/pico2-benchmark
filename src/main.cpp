
#include <benchmarks.h>

#include "pico/stdlib.h"

#include <iostream>
#include <string.h>

#include <nlohmann/json.hpp>

void parse_command(const nlohmann::json &json) {
  std::string name = json["benchmark"];
  std::string precision = json.value("precision", "float");

  auto benchmark = [&]<bool Debug, typename P>() {
    if (name == "determinant" || name = "det")
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

int main() {
  stdio_init_all();

  // wait for PC to open console
  while (!stdio_usb_connected()) {
    sleep_ms(100);
  }

  char command[128];
  while (true) {
    // wait for command
    gets(command);

    if (strlen(command) == 0) {
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
    auto json = nlohmann::json::parse(command);

    parse_command(json);

    fflush(stdout);
  }
}
