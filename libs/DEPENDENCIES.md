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
2e848d4bf77743968972cc8ec0de7abc0eb022934a5f25ddefed2ecee55cf44a  libs/alembic/lib/osx/libAlembic.a
769f240b3e2be027e0ea37c0e2f0b53648420fb041544a9ce73451434bd141bb  libs/alembic/lib/osx/libImath-3_2.a
edf21e7cbdd8fd632e34a5f78d55bc279cfd16239a023ad5d5b5ed3fbdc955cc  libs/alembic/lib/vs/x64/Release/Alembic.lib
ebf75fb1e8c5e71537ae0a349c900b1687c89c3b157817415ed2da7288b5a9d6  libs/alembic/lib/vs/x64/Release/Alembic.dll
cd75309f2c217ca2aa7a236b3070b80b009780e54a3b238e0e81a1c224f4aa05  libs/alembic/lib/vs/x64/Release/Imath-3_2.lib
e20d0e2556b44d9865295dc15c4c2ee36b8af12e3ca10cc686c64b3a61d60c76  libs/alembic/lib/vs/x64/Release/Imath-3_2.dll
```
