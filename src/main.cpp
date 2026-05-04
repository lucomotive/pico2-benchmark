#include "pico/stdlib.h"

#include <Benchmark.h>
#include <benchmarks/copy.h>
#include <benchmarks/determinant.h>
#include <benchmarks/fragmentation.h>
#include <benchmarks/heap_alloc.h>
#include <benchmarks/inverse.h>
#include <benchmarks/lu.h>
#include <benchmarks/matmul.h>
#include <benchmarks/qr.h>
#include <benchmarks/stack_alloc.h>

#include <iostream>
#include <string.h>

#include <nlohmann/json.hpp>

Benchmark *parse_command(const nlohmann::json &json) {
  std::string name = json["benchmark"];
  std::string precision = json["precision"];

  uint32_t min_dim = json["min-dimension"];
  uint32_t max_dim = json["max-dimension"];

  auto benchmark = [&]<typename P>() -> Benchmark * {
    if (name == "qr")
      return new QR<P>(min_dim, max_dim);
    if (name == "lu")
      return new LU<P>(min_dim, max_dim);
    if (name == "matmul")
      return new MatMul<P>(min_dim, max_dim);
    if (name == "inverse")
      return new Inverse<P>(min_dim, max_dim);
    if (name == "copy")
      return new Copy<P>(min_dim, max_dim);
    if (name == "determinant")
      return new Determinant<P>(min_dim, max_dim);
    if (name == "stack-alloc")
      return new StackAlloc<P>(min_dim, max_dim);
    if (name == "heap-alloc")
      return new HeapAlloc<P>(min_dim, max_dim);
    if (name == "heap-frag")
      return new HeapFrag<P>(min_dim, max_dim);

    return nullptr;
  };

  if (precision == "float") {
    return benchmark.operator()<float>();
  }
  if (precision == "double") {
    return benchmark.operator()<double>();
  }
  return nullptr;
}

int main() {
  stdio_init_all();

  // wait for PC to open console
  while (!stdio_usb_connected()) {
    sleep_ms(100);
  }

  while (true) {
    // wait for command
    char command[128];
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

    Benchmark *benchmark = parse_command(json);
    if (benchmark == nullptr) {
      printf("ERROR: Invalid benchmark\n");
      continue;
    }

    uint32_t iterations = json["iterations"];

    // debug print once
    benchmark->run(true);

    benchmark->sof();
    for (int i = 0; i < iterations; ++i) {
      benchmark->run(false);
    }
    benchmark->eof();

    fflush(stdout);
  }
}
