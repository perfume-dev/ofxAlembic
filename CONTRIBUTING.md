# Contributing to ofxAlembic

Bug reports, compatibility results, documentation improvements, and code contributions are welcome.

## Before opening an issue

- Use openFrameworks 0.12.1 unless the issue is specifically about an older release.
- Confirm that the Alembic file uses the Ogawa backend.
- Try one of the included examples with the same toolchain.
- Search existing issues and pull requests for the same problem.

Include the operating system, CPU architecture, openFrameworks version, compiler or IDE version, a minimal reproduction, and the complete build or runtime error. Attach the smallest shareable Alembic file that reproduces a data-specific problem.

Do not attach confidential production assets.

## Pull requests

1. Create a focused branch from `master`.
2. Keep public API changes backward-compatible when practical.
3. Update an example or add a regression test for behavior changes.
4. Generate and build all three example projects with openFrameworks 0.12.1.
5. Run `git diff --check` and remove generated project, build, and IDE files.
6. Describe the tested platforms and any untested platform-specific behavior in the pull request.

The GitHub Actions build must pass on macOS and Windows before merge.

## Vendored dependencies

Alembic and Imath headers and binaries are intentionally kept at matching versions. Do not update only headers or only one platform's libraries. Use the `Build vendored dependencies` workflow and verify its artifacts before replacing files under `libs/alembic`.

Keep upstream copyright and license files unchanged.
