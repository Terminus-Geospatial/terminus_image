# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.3] - 2026-08-14

### Fixed

* Marked the `terminus_core`, `terminus_log`, `terminus_math`, and `terminus_outcome` requirements in `requirements()` with `transitive_headers=True, transitive_libs=True`. Without these, Conan did not propagate those packages' headers/libs to consumers of `terminus_image`, even though `terminus_image`'s own public headers directly expose their types (e.g. `terminus/math/rectangle.hpp` via `Image_Format`). This caused `file not found` errors for any package depending on `terminus_image` without also directly requiring those dependencies itself.

## [1.0.2] - 2026-08-14

### Changed

* Updated `terminus_math` dependency from `1.0.3` to `1.0.4`.

## [1.0.1] - 2026-08-14

### Added

* Added a Conan package test at `tests/package/` (`conanfile.py`, `CMakeLists.txt`, `example.cpp`) that requires the packaged `terminus_image` library, builds an `Image_Format`, and prints the library's build information.
* Added `templates/exports.hpp.in`, generating an installed `terminus/image/exports.hpp` header with version macros, build date, git commit hash, and a `tmns::image::get_build_info()` helper, matching the pattern used by `terminus_core`/`terminus_math`/`terminus_outcome`.

### Fixed

* Fixed `export_sources()` in `conanfile.py`, which referenced a stale `test/*` path; it now correctly exports `tests/*` so the package test folder is included when the recipe is packaged.

## [1.0.0] - 2026-08-03

### Changed

* Bumped package version to `1.0.0`.
* Updated `terminus_log` dependency from `0.0.14` to `1.0.2`.
* Updated `terminus_math` dependency from `0.0.15` to `1.0.2`.
* Updated `terminus_outcome` dependency from `0.0.12` to `1.0.3`.
* Updated `terminus_cmake` build requirement from `1.0.8` to `1.0.10`.
* `terminus_core` remains at `1.0.3` (current latest).
* `boost`, `nlohmann_json`, and `tomlplusplus` versions remain at their current latest pins.
