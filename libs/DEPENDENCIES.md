# Vendored dependencies

`ofxAlembic` bundles matching headers and binaries so that openFrameworks projects do not need a separate Alembic installation.

| Dependency | Version | Source |
| --- | --- | --- |
| Alembic | 1.8.12 | https://github.com/alembic/alembic/releases/tag/1.8.12 |
| Imath | 3.2.3 | https://github.com/AcademySoftwareFoundation/Imath/releases/tag/v3.2.3 |

The `Build vendored dependencies` GitHub Actions workflow produces the distributable artifacts from these tags.

## Build configuration

- macOS: static Universal libraries, `arm64` and `x86_64`, deployment target macOS 11.0
- Windows: x64 shared libraries, Visual Studio 2022, Release configuration
- Alembic HDF5, Python bindings, binaries, and tests: disabled

The installed Alembic `Foundation.h` includes an explicit POSIX pthread declaration required by current Apple Clang releases. Imath's global debug-only `printBits` helpers are given an addon-specific name to avoid a symbol collision with the Imath copy embedded in openFrameworks' FreeImage library. Third-party license texts are stored alongside this file.

## SHA-256

```text
a517881bc1999456f5559ebd09021d7be0376b94815b5af4320ec021eccc08f9  libs/alembic/lib/osx/libAlembic.a
e726b2a90ec0be857d43f69340c5f0a695ece2afbe51126b92aca77977ab2907  libs/alembic/lib/osx/libImath-3_2.a
edf21e7cbdd8fd632e34a5f78d55bc279cfd16239a023ad5d5b5ed3fbdc955cc  libs/alembic/lib/vs/x64/Release/Alembic.lib
4715bfdc7d75c855944849402abd002c42dc50985526127c3624f19d3728bd71  libs/alembic/lib/vs/x64/Release/Alembic.dll
46aa77fc00d5c5c6f72177074ca6e0ab3df12ce26320e3b986d6a8a4275dc697  libs/alembic/lib/vs/x64/Release/Imath-3_2.lib
a71ebac1b89d190bb7187762cd3a61fcc477a87d3682968e4ffb2ea2a3e08ef6  libs/alembic/lib/vs/x64/Release/Imath-3_2.dll
```
