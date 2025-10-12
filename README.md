# ofxAlembic
Alembic wrapper addon for OF 0.12.1+.

- Bump Alembic to 1.8.9 + Imath 3.1.12
- Support openFrameworks 0.12.1 on macOS + Windows
  - macOS   : Universal (Intel + Apple Sillicon) support, tested on Xcode 15 & 16 (Sonoma & Sequoia)
  - Windows : Windows11 + Visual Studio 2022 (x64 Release Build Only)

### Note
- Legacy Alembic HDF5 support is completely dropped. Please use Alembic Ogawa file format.


## For Previous OF Releases
- macOS + OF 0.10.0 - OF 0.11.2 : use [this commit](https://github.com/perfume-dev/ofxAlembic/tree/b29dbd5e5ccc823f585599d6a41734c111bbf88b)
- macOS + OF 0.9.8 : use [this commit](https://github.com/perfume-dev/ofxAlembic/tree/9f6ad898491f3b5ab10982c44b5ec32fce4f9f77)
- Windows + OF 0.10.0 - OF 0.11.2 : use [this fork by P-A-N](https://github.com/P-A-N/ofxAlembic)
- Windows + OF 0.9.8 use [this fork by hanasaan](https://github.com/hanasaan/ofxAlembic/tree/vs_of098)

## Issues
Example porting is working in progress.