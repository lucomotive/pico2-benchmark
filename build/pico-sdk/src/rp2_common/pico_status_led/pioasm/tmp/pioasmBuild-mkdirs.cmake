# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/nix/store/prh85xff9skzl4idfajbbc7a27c6179x-pico-sdk-2.2.0/lib/pico-sdk/tools/pioasm")
  file(MAKE_DIRECTORY "/nix/store/prh85xff9skzl4idfajbbc7a27c6179x-pico-sdk-2.2.0/lib/pico-sdk/tools/pioasm")
endif()
file(MAKE_DIRECTORY
  "/home/lucomotive/Bunker/Projects/bachelorarbeit/build/pioasm"
  "/home/lucomotive/Bunker/Projects/bachelorarbeit/build/pioasm-install"
  "/home/lucomotive/Bunker/Projects/bachelorarbeit/build/pico-sdk/src/rp2_common/pico_status_led/pioasm/tmp"
  "/home/lucomotive/Bunker/Projects/bachelorarbeit/build/pico-sdk/src/rp2_common/pico_status_led/pioasm/src/pioasmBuild-stamp"
  "/home/lucomotive/Bunker/Projects/bachelorarbeit/build/pico-sdk/src/rp2_common/pico_status_led/pioasm/src"
  "/home/lucomotive/Bunker/Projects/bachelorarbeit/build/pico-sdk/src/rp2_common/pico_status_led/pioasm/src/pioasmBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/lucomotive/Bunker/Projects/bachelorarbeit/build/pico-sdk/src/rp2_common/pico_status_led/pioasm/src/pioasmBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/lucomotive/Bunker/Projects/bachelorarbeit/build/pico-sdk/src/rp2_common/pico_status_led/pioasm/src/pioasmBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
