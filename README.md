# Image Processing
The repository is dedicated to Image Processing SE, term 2.

Can be run in both _Qt Creator_ and _CLion_ IDE's.
`CMakeLists.txt` provides project's config to run it in CLion.

In order to deploy it in CLion IDE you need
* [Download](http://mingw-w64.org/doku.php/download/mingw-builds/) and specify in CLion 
`Settings(Ctrl+Alt+S) | Build, Execution, Deployment | Toolchains` 
MinGW environment. _(e.g C:\Qt\Tools\mingw730_64)_.
* Provide CMAKE_PREFIX_PATH with path to MinGW dll's 
(that's not the same you've specified). (e.g. `set(CMAKE_PREFIX_PATH "C:\\Qt\\5.14.1\\mingw73_64\\")`)
* Instruct CMake to find QT libs by adding `find_package(Qt5 COMPONENTS Core Widgets REQUIRED)` 
* Link it to executable by adding `target_link_libraries(ImageProcessing Qt5::Core Qt5::Widgets Qt5::Gui)`

For more information see [official CLion tutorial](https://www.jetbrains.com/help/clion/qt-tutorial.html).
