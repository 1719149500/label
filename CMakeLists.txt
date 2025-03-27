cmake_minimum_required(VERSION 3.16)

# 指定编译器
set(CMAKE_C_COMPILER "F:/AsOneFile/Qt/Tools/mingw1120_64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "F:/AsOneFile/Qt/Tools/mingw1120_64/bin/g++.exe")

project(label2 VERSION 0.1 LANGUAGES CXX)

# 设置Qt路径 - 确保这个路径正确
set(CMAKE_PREFIX_PATH "F:/AsOneFile/Qt/6.6.3/mingw_64")

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找Qt包
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6  COMPONENTS Core Gui Widgets REQUIRED)

# 指定OpenCV路径（如果需要）
# set(OpenCV_DIR "C:/opencv")  # 如果OpenCV安装在非默认路径，取消注释并修改路径
find_package(OpenCV REQUIRED)

# Qt设置
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

)

set(PROJECT_HEADERS
    mainwindow.h
    processors/MyImage.h
    dialogs/ScaleDialog.h
    dialogs/BrightnessContrastDialog.h
    dialogs/ThresholdDialog.h
    dialogs/BinaryOptionsDialog.h
    dialogs/ConvolveDialog.h
    dialogs/FilterDialog.h
    dialogs/GaussianBlurDialog.h
    dialogs/UnsharpMaskDialog.h
    dialogs/TopHatDialog.h
    processors/BinaryProcessor.h
    processors/FilterProcessor.h

)


# 创建可执行文件
qt_add_executable(label2
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    resources.qrc
)

# 包含OpenCV的头文件路径
include_directories(${OpenCV_INCLUDE_DIRS})

# 链接Qt库和OpenCV库
target_link_libraries(label2 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${OpenCV_LIBS})

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

# 最后再调用finalize
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(label2)
endif()
