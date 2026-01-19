#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // Required for automatic type conversion of STL containers (like std::vector, std::map)
#include <pybind11/operators.h>

// Include necessary Mapnik headers
#include <mapnik/map.hpp>
#include <mapnik/layer.hpp>
#include <mapnik/color.hpp>
#include <mapnik/image.hpp>
#include <mapnik/agg_renderer.hpp>
#include <mapnik/agg_helpers.hpp>
#include <mapnik/datasource.hpp>
#include <mapnik/datasource_cache.hpp>
#include <mapnik/image_util.hpp>
#include <mapnik/agg/agg_renderer_base.h>

namespace py = pybind11;

// A helper function to render the map to a file (as an example of binding a free function)
void render_to_file_wrapper(mapnik::Map& m, const std::string& filename) {
  mapnik::image_32 buf(m.width(), m.height());
  mapnik::agg::renderer<mapnik::image_32> ren(buf, m.scale(), m.srs());
  ren.apply(m);
  mapnik::save_to_file(buf, filename);
}

PYBIND11_MODULE(_mapnik_bindings, m) {
  m.doc() = "pybind11 bindings for the Mapnik C++ library"; // Optional module docstring

  // Bind the mapnik::Color class
  py::class_<mapnik::color>(m, "Color")
    .def(py::init<unsigned char, unsigned char, unsigned char, unsigned char>(),
      py::arg("r"), py::arg("g"), py::arg("b"), py::arg("a") = 255)
    .def("get_rgba", &mapnik::color::rgba)
    .def("to_string", &mapnik::color::to_string);

  // Bind the mapnik::Map class
  py::class_<mapnik::Map>(m, "Map")
    .def(py::init<unsigned int, unsigned int>(),
      py::arg("width"), py::arg("height"))
    .def("load_map", [](mapnik::Map& m, const std::string& stylesheet) {
    mapnik::load_map(m, stylesheet); // Bind load_map functionality
      }, py::arg("stylesheet"), "Loads a Mapnik XML stylesheet")
    .def("zoom_all", &mapnik::Map::zoom_all, "Zooms the map to fit all layers' extents")
    .def("set_background", &mapnik::Map::set_background, py::arg("color"), "Sets the background color of the map")
    .def("add_layer", &mapnik::Map::add_layer, py::arg("layer"), "Adds a layer to the map");

  // Bind the mapnik::Layer class
  py::class_<mapnik::layer>(m, "Layer")
    .def(py::init<const std::string&, const std::string&>(),
      py::arg("name"), py::arg("srs") = "")
    .def("set_datasource", &mapnik::layer::set_datasource, py::arg("datasource"))
    .def("add_style", &mapnik::layer::add_style, py::arg("style_name"));

  // Bind a function to the module
  m.def("render_to_file", &render_to_file_wrapper, "Renders a map object to an image file");

  // Example of using the datasource cache (using a factory method pattern)
  m.def("create_datasource", [](py::dict params) -> mapnik::datasource_ptr {
    mapnik::parameters p;
    for (auto item : params) {
      p[item.first.cast<std::string>()] = item.second.cast<std::string>();
    }
    return mapnik::datasource_cache::instance()->create(p);
    }, py::arg("parameters"), "Creates a Mapnik datasource from a dictionary of parameters");
}
