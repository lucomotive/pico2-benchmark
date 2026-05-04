
#include <benchmarks.h>

#include "pico/stdlib.h"
#include <iostream>
#include <string.h>

#include <nlohmann/json.hpp>

void parse_command(const nlohmann::json &json) {
  std::string name = json["benchmark"];
  std::string precision = json["precision"];

  uint32_t min_dim = json["min-dimension"];
  uint32_t max_dim = json["max-dimension"];

  auto benchmark = [&]<bool Debug, typename P>() {
    if (name == "determinant") {
      determinant<Debug, P>(json);
    }
    if (name == "inverse") {
      inverse<Debug, P>(json);
    }
    if (name == "lu") {
      lu<Debug, P>(json);
    }
    if (name == "qr") {
      qr<Debug, P>(json);
    }
    if (name == "matmul") {
      matmul<Debug, P>(json);
    }
    if (name == "heap-alloc") {
      heap_alloc<Debug, P>(json);
    }
    if (name == "stack-alloc") {
      stack_alloc<Debug, P>(json);
    }
    if (name == "copy") {
      copy<Debug, P>(json);
    }
  };

  printf("SOF %s %s\n", name, precision);
  if (precision == "float") {
    benchmark.operator()<true, float>();
    benchmark.operator()<false, float>();
  }
  if (precision == "double") {
    benchmark.operator()<true, double>();
    benchmark.operator()<false, double>();
  }
  printf("EOF\n");
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
