# ofxAlembic

[![Build examples](https://github.com/perfume-dev/ofxAlembic/actions/workflows/build.yml/badge.svg)](https://github.com/perfume-dev/ofxAlembic/actions/workflows/build.yml)

`ofxAlembic` is an [openFrameworks](https://openframeworks.cc/) addon for reading, writing, and drawing [Alembic](https://www.alembic.io/) scene data. It supports points, curves, polygon meshes, transforms, and cameras using the Alembic Ogawa backend.

## Requirements

- openFrameworks 0.12.1
- macOS 11.0 or later with Xcode 15 or later
- Windows 11 with Visual Studio 2022, x64 Release builds

The addon bundles:

- Alembic 1.8.12
- Imath 3.2.3
- Universal macOS static libraries for Intel and Apple Silicon
- Windows x64 libraries and runtime DLLs

HDF5 support is not included. Use Alembic files written with the Ogawa backend.
Build settings, source tags, and binary checksums are recorded in [`libs/DEPENDENCIES.md`](libs/DEPENDENCIES.md).

## Installation

Clone the addon into the `addons` directory of an openFrameworks 0.12.1 installation:

```sh
cd /path/to/openFrameworks/addons
git clone https://github.com/perfume-dev/ofxAlembic.git
```

Add `ofxAlembic` to a project with the openFrameworks Project Generator. No additional system libraries are required.

## Reading an Alembic file

```cpp
#include "ofxAlembic.h"

ofxAlembic::Reader reader;

if (!reader.open("scene.abc")) {
    ofLogError() << "Could not open scene.abc";
    return;
}

reader.setTime(1.0);

ofMesh mesh;
if (reader.get("/model/modelShape", mesh)) {
    mesh.draw();
}
```

Object paths can be either local names or full Alembic paths. Check the return value from `open()` and typed `get()` calls before using their output.

## Writing an Alembic file

```cpp
#include "ofxAlembic.h"

ofxAlembic::Writer writer;

if (!writer.open("output.abc", 30.0f)) {
    ofLogError() << "Could not create output.abc";
    return;
}

std::vector<glm::vec3> points = {
    {0.0f, 0.0f, 0.0f},
    {100.0f, 0.0f, 0.0f}
};

writer.addPoints("/points", points);
writer.close();
```

Writer paths must start with `/`. Create a parent object before adding child objects below it.

## Examples

- `example-read`: reads and draws meshes, points, and curves
- `example-write`: writes animated geometry and a camera, then reads it back
- `example-shader`: draws Alembic meshes with a shader

Generate each example project with the Project Generator before opening it in Xcode or Visual Studio.

## Supported data

| Schema | Read | Write |
| --- | :---: | :---: |
| Points | Yes | Yes |
| Curves | Yes | Yes |
| Polygon mesh | Yes | Yes |
| Transform | Yes | Yes |
| Camera | Yes | Yes |

Polygon meshes are triangulated when read. Mesh writing currently expects triangle topology.

## Previous openFrameworks releases

- macOS, OF 0.10.0–0.11.2: use [commit `b29dbd5`](https://github.com/perfume-dev/ofxAlembic/tree/b29dbd5e5ccc823f585599d6a41734c111bbf88b)
- macOS, OF 0.9.8: use [commit `9f6ad89`](https://github.com/perfume-dev/ofxAlembic/tree/9f6ad898491f3b5ab10982c44b5ec32fce4f9f77)
- Windows, OF 0.10.0–0.11.2: use the [P-A-N fork](https://github.com/P-A-N/ofxAlembic)
- Windows, OF 0.9.8: use the [hanasaan fork](https://github.com/hanasaan/ofxAlembic/tree/vs_of098)

Older snapshots are not covered by the current build checks or security updates.

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for the development and pull-request workflow. Please report security problems according to [SECURITY.md](SECURITY.md).

## License

`ofxAlembic` is released under the MIT License. See [LICENSE.txt](LICENSE.txt). Alembic and Imath retain their respective licenses in `libs/`.
