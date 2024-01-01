cmake_minimum_required(VERSION 3.5)

project(gpr_reader VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 COMPONENTS Widgets PrintSupport REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets PrintSupport REQUIRED)
find_package(Threads REQUIRED)
find_package(Boost COMPONENTS program_options filesystem regex system REQUIRED)
find_package(Eigen3)
find_package(fmt)

set(Boost_USE_STATIC_LIBS ON)
include_directories(${Boost_INCLUDE_DIR})

set(PROJECT_SOURCES
        src/main.cpp
        src/widgets/mainwindow.cpp
        src/widgets/mainwindow.h
        src/widgets/mainwindow.ui
        src/widgets/qcustomplot.cpp
        src/widgets/qcustomplot.h

        src/domain/radargram.hpp
        src/domain/attribute_analysis.hpp
        src/domain/depth_section.hpp
        src/domain/layer.hpp

        src/calculators/attribute_analysis_calculator/energy_attribute_analysis_calculator.cpp
        src/calculators/attribute_analysis_calculator/hilbert_attribute_analysis_calculator.cpp
        src/calculators/depth_section_calculator/simple_depth_section_calculator.cpp

        src/importers/geoscan_project_importer.cpp

        src/utils/helpers.hpp

        src/project/project.cpp
)

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(gpr_reader
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
    )
else ()
    add_executable(gpr_reader
            ${PROJECT_SOURCES}
    )
endif ()

target_link_libraries(gpr_reader PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${Boost_LIBRARIES} Threads::Threads Eigen3::Eigen Qt6::PrintSupport fmt::fmt)
#target_compile_options(gpr_reader PRIVATE -Wall -Wextra -pedantic -Werror)
target_compile_options(gpr_reader PRIVATE -Wall -Wextra -pedantic)


set_target_properties(gpr_reader PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(gpr_reader)
endif ()


include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.13.0
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main gmock_main)


set(TEST_SOURCES
        tests/test_example.cpp
)

add_executable(gpr_reader_test ${TEST_SOURCES})
target_link_libraries(gpr_reader_test gtest gtest_main)
add_test(gpr_reader_test gpr_reader_test)


install(TARGETS gpr_reader RUNTIME DESTINATION bin)
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT khvyus@gmail.com)
include(CPack)
enable_testing()