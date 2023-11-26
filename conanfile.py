import os

from conans import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake
from conan.tools.layout import cmake_layout
from conans.errors import ConanInvalidConfiguration
from conan.tools.system.package_manager import Apt


def _are_tests_enabled():
    if 'LIBDOCUMENTVALIDATOR_TESTS_ENABLED' in os.environ:
        if os.environ['LIBDOCUMENTVALIDATOR_TESTS_ENABLED']  == 'True':
            return True
    return False

def _are_packages_enabled():
    if 'LIBDOCUMENTVALIDATOR_PACKAGES_ENABLED' in os.environ:
        if os.environ['LIBDOCUMENTVALIDATOR_PACKAGES_ENABLED'] == 'True':
            return True
    return False

def _is_coverage_enabled():
    if 'LIBDOCUMENTVALIDATOR_COVERAGE_ENABLED' in os.environ:
        if os.environ['LIBDOCUMENTVALIDATOR_COVERAGE_ENABLED'] == 'True':
            return True
    return False


class MrzAntitamperingConan(ConanFile):
    name = "lib_document_validator"

    # Optional metadata
    license = "EULA"
    author = "Pietrobon Francesca"
    url = "https://github.com/FrancescaPietrobon/MRZ_Antitampering"
    description = "Library for document validation"
    topics = ("AI", "computer-vision")

    # Dependencies
    requires = [
        ("spdlog/1.10.0@#5881717ce085871b1a2921361eb79986",),
        ("boost/1.81.0@#9bd7ae86a881631b6cc76590621e470b",),
        ("nlohmann_json/3.10.5@#24f8709158b89f1396614ee07e0827d2",),
        ("jsoncpp/1.9.5@#2b8f5f13685f844b4b74beac3a796e8d",),
        ("opencv/4.5.3@#ab0498fa9f6e6ac35c06fa7100f49dcb",),
        ("zlib/1.2.13@#13c96f538b52e1600c40b88994de240f", "override"),
        ("gtest/1.11.0@#d99d2af6a53ab52d3c2dd88cdbc1e0fd"),
        ("protobuf/3.17.1@#e64fa06824a80ebce90de3e3e2460033"),
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

        "opencv:shared": True,
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

        "protobuf:shared": True,
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
            raise ConanInvalidConfiguration(
                "This library is not compatible with Windows")

    def package_info(self):
        self.cpp_info.name = "lib_document_validator"

        self.cpp_info.components["ocr_server"].requires = [
            "spdlog::spdlog",
            "opencv::opencv",
        ]
        self.cpp_info.components["ocr_server"].libs = ["document_validator_ocr_server"]
        self.cpp_info.components["ocr_server"].libdirs = ["lib/x86_64-linux-gnu"]
        self.cpp_info.components["ocr_server"].includedirs = ["include", "include/document_validator"]

        '''
        self.cpp_info.components["characters_clustering"].requires = [
            "spdlog::spdlog",
            "boost::boost",
            "opencv::opencv",
            "jsoncpp::jsoncpp",
            "nlohmann_json::nlohmann_json"
        ]
        self.cpp_info.components["characters_clustering"].libs = ["document_validator_characters_clustering"]
        self.cpp_info.components["characters_clustering"].libdirs = ["lib/x86_64-linux-gnu"]
        self.cpp_info.components["characters_clustering"].includedirs = ["include", "include/document_validator"]
        '''

        self.cpp_info.components["antitampering"].requires = [
            "spdlog::spdlog",
            "opencv::opencv",
        ]
        self.cpp_info.components["antitampering"].libs = ["document_validator_antitampering"]
        self.cpp_info.components["antitampering"].libdirs = ["lib/x86_64-linux-gnu"]
        self.cpp_info.components["antitampering"].includedirs = ["include", "include/document_validator"]
        

    def set_version(self):
        self.version = os.environ.get("DOCUMENT_VALIDATOR_VERSION", "1.0")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        if _is_coverage_enabled():
            tc.variables["COVERAGE_ENABLED"] = True
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
