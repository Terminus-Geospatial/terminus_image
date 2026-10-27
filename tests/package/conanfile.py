#    File:    conanfile.py
#    Author:  Marvin Smith
#    Date:    8/14/2026
import os

from conan import ConanFile, tools
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout
from conan.tools.build import can_run

class ImagePackageTestConan( ConanFile ):

    name = "terminus_image_test"
    version = "1.0.5"
    description = "Test for Terminus Image Package"

    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        self.requires("terminus_image/1.0.7")

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.configure()
        return cmake

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def test(self):
        if can_run(self):
            self.run( ".%sexample" % os.sep )
