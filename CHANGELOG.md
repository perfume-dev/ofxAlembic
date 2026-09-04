# Changelog

All notable changes to `ofxAlembic` will be documented in this file.

## Unreleased

### Added

- Automated macOS and Windows example builds.
- Reproducible dependency-build workflow.
- Installation, usage, contribution, and security documentation.
- Reader and Writer open-state queries.

### Changed

- Updated Alembic from 1.8.9 to 1.8.12.
- Updated Imath from 3.1.12 to 3.2.3.
- Rebuilt macOS Universal libraries for a macOS 11.5 deployment target.
- Improved error handling in the examples.

### Fixed

- Build failure with current Apple Clang caused by a missing pthread declaration.
- Dangling full-path pointers after closing or reopening a Reader.
- Out-of-bounds object access by index.
- Uninitialized Reader and Writer state.
- Process termination on invalid Writer object paths.
- Unsafe reuse of one Writer path with different Alembic schema types.
- Deprecated camera orbit call and unchecked example lookups.
