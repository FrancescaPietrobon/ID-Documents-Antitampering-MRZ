import os

from conans import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake
from conan.tools.layout import cmake_layout
from conans.errors import ConanInvalidConfiguration
from conan.tools.system.package_manager import Apt


def _are_tests_enabled():
    if 'LIBPHYGITALAI_TESTS_ENABLED' in os.environ:
        if os.environ['LIBPHYGITALAI_TESTS_ENABLED']  == 'True':
            return True
    return False

def _are_packages_enabled():
    if 'LIBPHYGITALAI_PACKAGES_ENABLED' in os.environ:
        if os.environ['LIBPHYGITALAI_PACKAGES_ENABLED'] == 'True':
            return True
    return False

class MrzAntitamperingConan(ConanFile):
    name = "lib_antitampering_mrz"

    # Optional metadata
    license = "EULA"
    author = "Pietrobon Francesca"
    url = "https://github.com/FrancescaPietrobon/MRZ_Antitampering"
    description = "Library for MRZ antitampering"
    topics = ("AI", "computer-vision")

    # Dependencies
    requires = [
        ("spdlog/1.10.0@#5881717ce085871b1a2921361eb79986",),
        ("boost/1.69.0@#77454ea00c6bd18ede03c0eb255e7ccb",),
        ("nlohmann_json/3.10.5@#24f8709158b89f1396614ee07e0827d2",),
        ("jsoncpp/1.9.5@#9d91be1604af36ced56ae89ee65d53e0",),
        #("opencv/4.5.5@#18b6d4ea69d84464117246c73e814cbe",),
        ("opencv/4.5.3@#ab0498fa9f6e6ac35c06fa7100f49dcb",),
        ("zlib/1.2.13@#13c96f538b52e1600c40b88994de240f",),
        ("gtest/1.11.0@#d99d2af6a53ab52d3c2dd88cdbc1e0fd")
    ]

    tool_requires = "pkgconf/1.7.4"

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "with_gui": [True, False],
        "with_statx": [True, False],
        "fPIC": [True, False],
    }
    default_options = {
        "with_gui": False,
        "with_statx": False,
        "fPIC": True,

        "opencv:shared": False,
        "opencv:with_gtk": False,
        "opencv:with_ffmpeg": False,
        "opencv:with_v4l": True,
        "opencv:with_cuda": False,
        "opencv:contrib": True,
        "opencv:cpu_baseline": "SSE2",

        "boost:extra_b2_flags": "define=BOOST_FILESYSTEM_DISABLE_STATX",

        "sdplog:header_only": False,
        "spdlog:shared": False,
        
        "gtest:build_gmock": True,
        "gtest:no_main":True,
    }
    
    '''
    def system_requirements(self):
        if self._is_local_env():
            self.output.info(
                "Local environment detected, skipping models installation (assuming they are already installed)")
        else:
            self.output.info("CI environment detected, installing models")
            Apt(self).install(["phygitalai-models-legacy-all"], update=True, check=True)
    '''

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*"

    def config_options(self):
        if self.options.with_gui:
            self.options["opencv:with_gtk"] = True

        if self.options.with_statx:
            del self.options["boost:extra_b2_flags"]

    def requirements(self):
        if self.options.with_gui:
            self.requires("gtk/system", override=True)

    def validate(self):
        if self.settings.os == "Windows":
            raise ConanInvalidConfiguration("This library is not compatible with Windows")

    def package_info(self):
        self.cpp_info.name = "lib_antitampering_mrz"

        self.cpp_info.components["core"].requires = [
            "spdlog::spdlog",
            "boost::boost",
            "nlohmann_json::nlohmann_json",
            "jsoncpp::jsoncpp",
            "opencv::opencv",
        ]
        self.cpp_info.components["core"].libs = ["antitampering_mrz"]
        self.cpp_info.components["core"].libdirs = ["lib/x86_64-linux-gnu"]
        #self.cpp_info.components["core"].includedirs = ["include", "include/phygitalai"]
        self.cpp_info.components["core"].cmake_target_name = "core"

        self.cpp_info.components["ocr_server"].requires = [
            "spdlog::spdlog",
            "boost::boost",
            "opencv::opencv",
            "nlohmann_json::nlohmann_json"
        ]
        self.cpp_info.components["ocr_server"].libs = ["ocr_server"]
        self.cpp_info.components["ocr_server"].libdirs = ["lib/x86_64-linux-gnu"]

        self.cpp_info.components["antitampering_mrz"].requires = [
            "spdlog::spdlog",
            "boost::boost",
            "opencv::opencv",
            "nlohmann_json::nlohmann_json"
        ]
        self.cpp_info.components["antitampering_mrz"].libs = ["antitampering_mrz"]
        self.cpp_info.components["antitampering_mrz"].libdirs = ["lib/x86_64-linux-gnu"]
        #self.cpp_info.components["antitampering_mrz"].includedirs = ["include", "include/phygitalai"]
        
    def set_version(self):
        self.version = os.environ.get("ANTITAMPERING_MRZ_VERSION", "1.0")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        if _are_packages_enabled():
            cmake.build(target="package")
        else:
            cmake.build()
        if _are_tests_enabled():
            cmake.test()

    def package(self):
        cmake = CMake(self)
        cmake.install()
    
    '''
    @staticmethod
    def _is_local_env():
        return "JENKINS_URL" not in os.environ
    '''
