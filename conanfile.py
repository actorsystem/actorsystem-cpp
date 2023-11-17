from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout

class CppS3UploaderConan(ConanFile):
    name = "cpp_rabbi"
    version = "0.1"
    settings = "os", "compiler", "build_type", "arch"

    requires = "boost/1.83.0", "rabbitmq-c/0.13.0"
    generators = "CMakeDeps", "CMakeToolchain"

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
