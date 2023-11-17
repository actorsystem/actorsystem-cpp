from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout

class RabbiConan(ConanFile):
    name = "rabbi"
    version = "0.1"
    settings = "os", "compiler", "build_type", "arch"

    license = "All Rights Reserved"
    author = "Owen Kellogg"
    url = "https://github.com/rabbijs/rabbi-cpp"
    description = "AMQP-based Actor System"
    topics = ("conan", "cpp", "actor-system", "amqp")

    requires = "boost/1.83.0", "rabbitmq-c/0.13.0"
    generators = "CMakeDeps", "CMakeToolchain"

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy("*.h", dst="include", src="include")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
