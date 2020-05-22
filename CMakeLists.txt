cmake_minimum_required(VERSION 3.16)
project(ImageProcessing)

# Gets CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# C++ standard.
set(CMAKE_CXX_STANDARD 14)
# Path to Qt MinGw dir
set(CMAKE_PREFIX_PATH "C:\\Qt\\5.14.1\\mingw73_64\\")

include_directories(.)
# Qt .dll's that are used
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
add_executable(ImageProcessing
        constants.h
        convolutiontool.cpp
        convolutiontool.h
        dataretriver.cpp
        dataretriver.h
        descriptor.cpp
        descriptor.h
        descriptorbuilder.cpp
        descriptorbuilder.h
        helper.cpp
        helper.h
        imagehandler.cpp
        imagehandler.h
        imagetoprocess.cpp
        imagetoprocess.h
        ipworker.cpp
        ipworker.h
        main.cpp
        mainform.cpp
        mainform.h
        octave.h
        parallelconvolutiontool.cpp
        parallelconvolutiontool.h
        pointofinterest.cpp
        pointofinterest.h
        poishandler.cpp
        poishandler.h
        pyramid.h
        sandbox.cpp
        sandbox.h
        sequentialconvolutiontool.cpp
        sequentialconvolutiontool.h
        ui_mainform.h CppDemo.cpp CppDemo.h Distortion.cpp Distortion.h Rotate.cpp Rotate.h Identity.cpp Identity.h KernelsHandler.cpp KernelsHandler.h Contrast.cpp Contrast.h Shift.cpp Shift.h)

# Link Qt libraries to the exe's
target_link_libraries(ImageProcessing Qt5::Core Qt5::Widgets Qt5::Gui)
