# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.7] - 2026-10-05

### Fixed

- Fixed GDAL footprint coordinate axis order: set `OAMS_TRADITIONAL_GIS_ORDER` on both source and destination SRS in `reproject_to_wgs84()`, preventing GDAL 3.x from silently swapping longitude/latitude when reprojecting from projected CRSes (e.g. UTM) to WGS84.
- Case-insensitive file extension matching in `Image_Resource_Disk_GDAL_Factory` for both read and write paths, allowing uppercase/mixed-case suffixes like `.TIF`.
- Fixed `Metadata_Container_Base::get()` to use `get_optional()` for nested path lookup instead of `count()`, which only checks immediate children.
- Added regression tests for flat, nested, and missing metadata paths.
- Fixed `num_channels()` returning 3 instead of 4 for RGBA/GENERIC_4_CHANNEL pixel formats.
- Fixed `convert()` alpha detection using `dstf` instead of `srcf` for RGBA check.
- Fixed ORB descriptor extraction using `float` instead of `uchar` for descriptor matrix access.
- Fixed dangling-reference risk in `compute_line_points` by materializing expression-template temporaries into concrete `Point2d` values.
- Null-safety improvements in TOML collection file parser.

### Changed

- Removed `TEST_collection_resource_file.cpp` (test fixture no longer available).
- Updated `terminus_core` to `1.0.6`, `terminus_math` to `1.0.8`.

## [1.0.6] - 2026-09-27

### Added
- GDAL loader now extracts geospatial metadata (CRS, geo-transform, WGS84 footprint, GSD) and stores it in the image resource's `Metadata_Container`, accessible via `Image_Resource_Disk_GDAL::create(path)->metadata()` or through `Image_Disk`/`Image_Base`.
- Added metadata keys: `gdal.projection`, `gdal.geo_transform.0..5`, `gdal.footprint.{min_lon,min_lat,max_lon,max_lat}`, `gdal.gsd_meters`.

## [1.0.5] - 2026-08-17

### Changed
- Bumped version to force a new upload to the Conan remote; no functional changes.
- Updated `terminus_core` dependency to `1.0.5`, `terminus_log` dependency to `1.0.4`, `terminus_math` dependency to `1.0.6`, and `terminus_outcome` dependency to `1.0.5`.

## [1.0.4] - 2026-08-15

### Added

- Added `docker/Dockerfile.rhel10.build` for building, testing, and uploading the package on RHEL 10.
- Added `.dockerignore` to reduce the Docker build context size.

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
