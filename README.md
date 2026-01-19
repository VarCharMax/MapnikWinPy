# MapnikWinPy

Mapnik Python bindings for Windows.

The Mapnik library currently does not have bindings for Python on Windows. So I thought I'd set myself the challenge of creating them. There isn't a pressing need to this, as there are bindings for Ubuntu. And there's also a complete native dotnet port. And a Node.js library. Still, the bindings can be added using Pybind11, drawing on the VCPKG Mapnik header files. It's mostly a matter of declaring them in a local cpp file, wrapped in the declarations provided by pybind11. It doesn't involve very much actual coding on my part.

So it's really more of a challenge than anything.

The `CMAKE` command I used to create the project (using bash syntax) is:

`cmake -DPYTHON_EXECUTABLE=$(python3 -c "import sys; print(sys.executable)") -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake -DPYBIND11_PYTHON_VERSION=3.12`

I specified Python 3.12 because that seems to the highest version compatible with mapnik (looking at the vcpkg build output - pythonnet supports 3.13). However, the CMAKE command reported that it ignored the `-DPYBIND11_PYTHON_VERSION` flag.
