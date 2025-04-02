cmake_minimum_required(VERSION 3.16)
project(label2 VERSION 0.1 LANGUAGES CXX)

# 设置 Qt 路径 - 确保这个路径正确
set(CMAKE_PREFIX_PATH "F:/AsOneFile/Qt/6.6.3/msvc2019_64")

# 设置 CMake 工具链文件
#set(CMAKE_TOOLCHAIN_FILE "F:/AsOneFile/src/vcpkg-master/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 指定 OpenCV 的路径
set(OpenCV_DIR "F:/AsOneFile/opencv/build/x64/vc16/lib")

# 包含 nlohmann/json.hpp 的路径

set(nlohmann_json_DIR "F:/AsOneFile/src/vcpkg-master/installed/x64-windows/share/nlohmann_json")
find_package(nlohmann_json REQUIRED)
# 查找 Qt 包
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

# 查找 OpenCV 包
find_package(OpenCV REQUIRED)

# 指定 Torch 的路径
set(Torch_DIR "F:/AsOneFile/libtorch-win-shared-with-deps-debug-2.6.0+cpu/libtorch/share/cmake/Torch")
# 找到 PyTorch
find_package(Torch REQUIRED)

# 假设你的DLL文件位于Qt的bin目录中
set(DLL_DIR "F:/AsOneFile/Qt/6.6.3/msvc2019_64/bin")

# 列出所有需要复制的DLL文件
set(DLL_FILES
    "${DLL_DIR}/Qt6Guid.dll"
    "${DLL_DIR}/Qt6Core.dll"
    "${DLL_DIR}/Qt6Widgets.dll"
    "F:/AsOneFile/opencv/build/x64/vc16/bin/opencv_world470d.dll"
    # 添加其他需要的DLL文件
)



# Qt 设置
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 设置源文件
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    dialogs/ScaleDialog.cpp
    dialogs/BrightnessContrastDialog.cpp
    dialogs/ThresholdDialog.cpp
    dialogs/BinaryOptionsDialog.cpp
    dialogs/ConvolveDialog.cpp
    dialogs/GaussianBlurDialog.cpp
    dialogs/FilterDialog.cpp
    dialogs/UnsharpMaskDialog.cpp
    dialogs/TopHatDialog.cpp
    processors/MyImage.cpp
    processors/BinaryProcessor.cpp
    processors/FilterProcessor.cpp
    processors/MyModel.cpp
    processors/Shape.cpp
    processors/Workspace.cpp
    processors/ModelProcessor.cpp
)

set(PROJECT_HEADERS
    mainwindow.h
    dialogs/ScaleDialog.h
    dialogs/BrightnessContrastDialog.h
    dialogs/ThresholdDialog.h
    dialogs/BinaryOptionsDialog.h
    dialogs/ConvolveDialog.h
    dialogs/FilterDialog.h
    dialogs/GaussianBlurDialog.h
    dialogs/UnsharpMaskDialog.h
    dialogs/TopHatDialog.h
    processors/MyImage.h
    processors/BinaryProcessor.h
    processors/FilterProcessor.h
    processors/MyModel.h
    processors/Shape.h
    processors/Workspace.h
)

# 创建可执行文件
qt_add_executable(label2
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    resources.qrc
)

# 包含 OpenCV 的头文件路径
include_directories(${OpenCV_INCLUDE_DIRS})
# 包含 libtorch 的头文件路径
include_directories(${TORCH_INCLUDE_DIRS})

# 链接 Qt 库和 OpenCV 库
target_link_libraries(label2 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${OpenCV_LIBS} ${TORCH_LIBRARIES} nlohmann_json)

# 添加一个自定义命令来复制DLL文件
add_custom_command(TARGET label2 POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                   ${DLL_FILES}
                   $<TARGET_FILE_DIR:label2>)

# 设置运行时库路径（RPATH）
set_target_properties(${PROJECT_NAME} PROPERTIES
    INSTALL_RPATH_USE_LINK_PATH TRUE
)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.label2)
endif()
set_target_properties(label2 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS label2
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 最后再调用 finalize
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(label2)
endif()