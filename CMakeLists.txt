# cmake requirements
cmake_minimum_required(VERSION 2.8)

# Build options have to be before PROJECT(...)
SET(CMAKE_CONFIGURATION_TYPES "Release" CACHE PATH "Configuration types")
SET(CMAKE_BUILD_TYPE "Release" CACHE PATH "Current build configuration")

# Example Project configuration
PROJECT(PUTAR)
if(APPLE)
    SET(CMAKE_CXX_FLAGS "-Wall -Wextra -Wconversion -std=c++11 -pthread -fopenmp")
else()
    SET(CMAKE_CXX_FLAGS "-Wall -Wextra -Wconversion -std=c++11 -pthread -malign-double -fopenmp")
endif()

#add_definitions(-DDEBUG)

# Folders
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# Settings
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/resources")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/include/putar")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/include/putar/Defs")

# core libraries
option(BUILD_PUTAR "Build PUTAR libraries" YES)
option(BUILD_DEFS "Build PUTAR Defs library" YES)
mark_as_advanced(BUILD_DEFS)
option(BUILD_PUTAR_UTILITIES "Build PUTAR Utilities library" ON)
mark_as_advanced(BUILD_PUTAR_UTILITIES)
option(BUILD_VISUALIZER "Build Visualizer 3D" ON)
mark_as_advanced(BUILD_VISUALIZER)
option(BUILD_IMAGEVISUALIZER "Build ImageVisualizer" ON)
mark_as_advanced(BUILD_IMAGEVISUALIZER)
option(BUILD_OBJLOADER "Build ObjLoader" ON)
mark_as_advanced(BUILD_OBJLOADER)
option(BUILD_HMIControl "Build HMIControl" ON)
mark_as_advanced(BUILD_HMIControl)

# demos
option(BUILD_PUTAR_DEMO "Build PUTAR demonstration programs and libraries" ON)
option(BUILD_PUTAR_DEMO_MAIN "Build main PUTAR test" ON)
mark_as_advanced(BUILD_PUTAR_DEMO_MAIN)
option(BUILD_PUTAR_DEMO_VISUALIZER "Build PUTAR visualizer demo" ON)
mark_as_advanced(BUILD_PUTAR_DEMO_VISUALIZER)
option(BUILD_PUTAR_DEMO_OBJLOADER "Build PUTAR objloader demo" ON)
mark_as_advanced(BUILD_PUTAR_DEMO_OBJLOADER)
option(BUILD_PUTAR_DEMO_3DSLOADER "Build PUTAR 3dsLoader demo" ON)
mark_as_advanced(BUILD_PUTAR_DEMO_3DSLOADER)
option(BUILD_PUTAR_DEMO_PUTSLAM "Build PUTAR PUTSLAM demo" ON)
mark_as_advanced(BUILD_PUTAR_DEMO_PUTSLAM)
option(BUILD_PUTAR_DEMO_HMICONTROL "Build PUTAR HMIControl demo" ON)
mark_as_advanced(BUILD_PUTAR_DEMO_HMICONTROL)
option(BUILD_PUTAR_DEMO_IMAGEVISUALIZER "Build PUTAR imagevisualizer demo" ON)
mark_as_advanced(BUILD_PUTAR_DEMO_IMAGEVISUALIZER)


#additional dependencies
# Boost
FIND_PACKAGE(Boost COMPONENTS system filesystem thread asio)

# OpenCV
FIND_PACKAGE(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

#octomap library
find_package(octomap REQUIRED)
include_directories(${OCTOMAP_INCLUDE_DIRS})
link_libraries(${OCTOMAP_LIBRARIES})

#g2o library
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)
find_package(G2O REQUIRED)
include_directories(${G2O_INCLUDE_DIR})
link_directories(${G2O_LIBRARY_DIRS})

#Eigen
if(NOT APPLE)
    set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/3rdParty/Eigen)
endif()
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})
add_definitions(-Wno-deprecated -DEIGEN_DONT_VECTORIZE -DEIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT)

if (BUILD_PUTAR)
    #QGLViewer library
    set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)
    FIND_PACKAGE(OpenGL)
    include_directories(${OPENGL_INCLUDE_DIR})
    link_directories(${OPENGL_LIBRARY})

    #QGLViewer library
    FIND_PACKAGE(QGLViewer)
    include_directories(${QGLVIEWER_INCLUDE_DIR})
    include_directories(${QT_INCLUDES})
    link_directories(${QGLVIEWER_LIBRARY})

    #CSparse library
    FIND_PACKAGE(CSparse)
    include_directories(${LIBCSPARSE_INCLUDE_DIR})
    link_directories(${LIBCSPARSE_LIBRARY})

    # PUTSLAM
    set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)
    FIND_PACKAGE(PUTSLAM)
    include_directories(${PUTSLAM_INCLUDE_DIR})
    set(PUTSLAM_INCLUDE_DIR "/usr/local/include/putslam")
    MESSAGE(${PUTSLAM_INCLUDE_DIR})
    #include_directories("/usr/local/include/putslam")
    #include_directories("${PUTSLAM_INCLUDE_DIR}/include/putslam")
    link_directories(${PUTSLAM_LIBRARY})
    set(PUTSLAM_LIBRARY_DIR "/usr/local/lib")
    MESSAGE(${PUTSLAM_LIBRARY_DIR})

    # Architecture
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            SET(X86_64 1)
            #MESSAGE("Architecture x86 64 bit")
    else(CMAKE_SIZEOF_VOID_P EQUAL 8)
            SET(X86_64 0)
            #MESSAGE("Architecture x86 32 bit")
    endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

endif(BUILD_PUTAR)

# Executable output directory
SET(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/bin${OUTPUT_DIRECTORY_POSTFIX} CACHE PATH "Executable output directory")
mark_as_advanced(RUNTIME_OUTPUT_DIRECTORY)

# Dynamic library output directory
SET(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/bin${OUTPUT_DIRECTORY_POSTFIX} CACHE PATH "Dynamic library output directory")
mark_as_advanced(LIBRARY_OUTPUT_DIRECTORY)

# Static library output directory
SET(ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/lib${OUTPUT_DIRECTORY_POSTFIX} CACHE PATH "Static library output directory")
mark_as_advanced(ARCHIVE_OUTPUT_DIRECTORY)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_DIRECTORY})
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ARCHIVE_OUTPUT_DIRECTORY})
foreach(CONFIGURATION_TYPE ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${CONFIGURATION_TYPE} CONFIGURATION_TYPE)
        SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIGURATION_TYPE} ${RUNTIME_OUTPUT_DIRECTORY})
        SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIGURATION_TYPE} ${LIBRARY_OUTPUT_DIRECTORY})
        SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIGURATION_TYPE} ${ARCHIVE_OUTPUT_DIRECTORY})
endforeach(CONFIGURATION_TYPE CMAKE_CONFIGURATION_TYPES)

if(MSVC)
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
endif()

###############################################################################
#
# PUTAR tinyXML2 library
#
###############################################################################

#add_library(tinyxml2 SHARED ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/tinyXML/tinyxml2.h ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/tinyXML/tinyxml2.cpp)
add_library(tinyxml2 STATIC ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/tinyXML/tinyxml2.h ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/tinyXML/tinyxml2.cpp)

###############################################################################
#
# PUTAR defs library
#
###############################################################################

if(BUILD_DEFS)
        if(APPLE)
          file(GLOB DEFS_SOURCES
              "${CMAKE_SOURCE_DIR}/src/Defs/*.cpp"
          )
        endif()
        file(GLOB DEFS_HEADERS
            "${CMAKE_SOURCE_DIR}/include/putar/Defs/*.h"
        )
        if(APPLE)
            add_library(PUTARDefs STATIC ${DEFS_SOURCES} ${DEFS_HEADERS})
        else()
            add_library(PUTARDefs STATIC ${DEFS_HEADERS})
            set_target_properties(PUTARDefs PROPERTIES LINKER_LANGUAGE CXX)
        endif()
# 	INSTALL(TARGETS PUTARDefs RUNTIME DESTINATION bin LIBRARY DESTINATION bin ARCHIVE DESTINATION lib)
        INSTALL(FILES ${DEFS_HEADERS} DESTINATION include/putar/Defs/)

endif(BUILD_DEFS)

###############################################################################
#
# PUTAR Utilities library
#
###############################################################################

if(BUILD_PUTAR_UTILITIES)
        file(GLOB UTILITIES_SOURCES
            "${CMAKE_SOURCE_DIR}/src/Utilities/*.cpp"
        )
        file(GLOB UTILITIES_HEADERS
            "${CMAKE_SOURCE_DIR}/include/putar/Utilities/*.h"
        )
        add_library(Utilities STATIC ${UTILITIES_SOURCES} ${UTILITIES_HEADERS})
#         INSTALL(TARGETS Utilities RUNTIME DESTINATION bin LIBRARY DESTINATION bin ARCHIVE DESTINATION lib)
        INSTALL(FILES ${TRACKER_HEADERS} DESTINATION include/putar/Utilities/)
endif(BUILD_PUTAR_UTILITIES)

###############################################################################
#
# PUTAR Visualizer library
#
###############################################################################

if(BUILD_PUTAR AND BUILD_VISUALIZER)
        file(GLOB VISUALIZER_SOURCES
            "${CMAKE_SOURCE_DIR}/src/Visualizer/*.cpp"
        )
        file(GLOB VISUALIZER_HEADERS
            "${CMAKE_SOURCE_DIR}/include/putar/Visualizer/*.h"
        )
        ADD_LIBRARY(Visualizer STATIC ${VISUALIZER_SOURCES} ${VISUALIZER_HEADERS})
        if(APPLE)
            TARGET_LINK_LIBRARIES(Visualizer tinyxml2 ${QGLVIEWER_LIBRARY} ${QT_LIBRARIES} /usr/local/Cellar/qt/4.8.7_2/lib/QtCore.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtXml.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtOpenGL.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtGui.framework ${OPENGL_LIBRARY})
        else()
            TARGET_LINK_LIBRARIES(Visualizer tinyxml2 ${QGLVIEWER_LIBRARY} ${QT_LIBRARIES} QtCore QtXml QtOpenGL QtGui ${OPENGL_LIBRARY})
        endif()

#        INSTALL(TARGETS Utilities RUNTIME DESTINATION bin LIBRARY DESTINATION bin ARCHIVE DESTINATION lib)
#        INSTALL(FILES ${VISUALIZER_HEADERS} DESTINATION include/walkers/Visualizer/)
endif(BUILD_PUTAR AND BUILD_VISUALIZER)

###############################################################################
#
# PUTAR ObjLoader library
#
###############################################################################

if(BUILD_PUTAR AND BUILD_OBJLOADER)
        file(GLOB VISUALIZER_SOURCES
            "${CMAKE_SOURCE_DIR}/src/ObjLoader/*.cpp"
        )
        file(GLOB VISUALIZER_HEADERS
            "${CMAKE_SOURCE_DIR}/include/putar/ObjLoader/*.h"
        )
        ADD_LIBRARY(ObjLoader STATIC ${VISUALIZER_SOURCES} ${VISUALIZER_HEADERS})
        TARGET_LINK_LIBRARIES(ObjLoader Utilities tinyxml2)
#        INSTALL(TARGETS Utilities RUNTIME DESTINATION bin LIBRARY DESTINATION bin ARCHIVE DESTINATION lib)
#        INSTALL(FILES ${VISUALIZER_HEADERS} DESTINATION include/walkers/Visualizer/)
endif(BUILD_PUTAR AND BUILD_OBJLOADER)

###############################################################################
#
# HMIControl library
#
###############################################################################

if(BUILD_PUTAR AND BUILD_HMIControl)
        file(GLOB HMICONTROL_SOURCES
            "${CMAKE_SOURCE_DIR}/src/HMIControl/*.cpp"
        )
        file(GLOB HMICONTROL_HEADERS
            "${CMAKE_SOURCE_DIR}/include/putar/HMIControl/*.h"
        )
        ADD_LIBRARY(HMIControl STATIC ${HMICONTROL_SOURCES} ${HMICONTROL_HEADERS})
        TARGET_LINK_LIBRARIES(HMIControl tinyxml2)
endif(BUILD_PUTAR AND BUILD_HMIControl)

###############################################################################
#
#
# PUTAR demo Visualizer
#
###############################################################################

if(BUILD_PUTAR AND BUILD_PUTAR_DEMO_VISUALIZER)
        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath ./")
        SET(DEMO_SOURCES ./demos/demoVisualizer.cpp)
        ADD_EXECUTABLE(demoVisualizer ${DEMO_SOURCES})
        if(APPLE)
            TARGET_LINK_LIBRARIES(demoVisualizer Visualizer tinyxml2 ${QGLVIEWER_LIBRARY} ${QT_LIBRARIES} ${QT_LIBRARIES} /usr/local/Cellar/qt/4.8.7_2/lib/QtCore.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtXml.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtOpenGL.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtGui.framework ${OPENGL_LIBRARY} /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks/GLUT.framework)
        else()
            TARGET_LINK_LIBRARIES(demoVisualizer Visualizer tinyxml2 ${QGLVIEWER_LIBRARY} ${QT_LIBRARIES} ${QT_LIBRARIES} QtCore QtXml QtOpenGL QtGui ${OPENGL_LIBRARY} glut)
        endif()
        INSTALL(TARGETS demoVisualizer RUNTIME DESTINATION bin)

endif(BUILD_PUTAR AND BUILD_PUTAR_DEMO_VISUALIZER)

###############################################################################
#
#
# PUTAR demo ObjLoader
#
###############################################################################

if(BUILD_PUTAR AND BUILD_PUTAR_DEMO_OBJLOADER)
        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath ./")
        SET(DEMO_SOURCES ./demos/demoObjLoader.cpp)
        ADD_EXECUTABLE(demoObjLoader ${DEMO_SOURCES})
        if(APPLE)
            TARGET_LINK_LIBRARIES(demoObjLoader ObjLoader Visualizer tinyxml2 ${OpenCV_LIBS} ${QGLVIEWER_LIBRARY} ${QT_LIBRARIES} ${QT_LIBRARIES} /usr/local/Cellar/qt/4.8.7_2/lib/QtCore.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtXml.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtOpenGL.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtGui.framework ${OPENGL_LIBRARY} /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks/GLUT.framework)
        else()
            TARGET_LINK_LIBRARIES(demoObjLoader ObjLoader Visualizer tinyxml2 ${OpenCV_LIBS} ${QGLVIEWER_LIBRARY} ${QT_LIBRARIES} ${QT_LIBRARIES} QtCore QtXml QtOpenGL QtGui ${OPENGL_LIBRARY} glut)
        endif()
        INSTALL(TARGETS demoObjLoader RUNTIME DESTINATION bin)

endif(BUILD_PUTAR AND BUILD_PUTAR_DEMO_OBJLOADER)

###############################################################################
#
#
# PUTAR demo PUTSLAM
#
###############################################################################

if(BUILD_PUTAR AND BUILD_PUTAR_DEMO_PUTSLAM)
        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath ./")
        SET(DEMO_SOURCES ./demos/demoPUTSLAM.cpp)
        ADD_EXECUTABLE(demoPUTSLAM ${DEMO_SOURCES})
        if(APPLE)
            TARGET_LINK_LIBRARIES(demoPUTSLAM Visualizer tinyxml2 ${QGLVIEWER_LIBRARY} ${QT_LIBRARIES} ${QT_LIBRARIES} /usr/local/Cellar/qt/4.8.7_2/lib/QtCore.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtXml.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtOpenGL.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtGui.framework ${OPENGL_LIBRARY} /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks/GLUT.framework)
        else()
            TARGET_LINK_LIBRARIES(demoPUTSLAM Visualizer tinyxml2 ${QGLVIEWER_LIBRARY} ${QT_LIBRARIES} ${QT_LIBRARIES} QtCore QtXml QtOpenGL QtGui ${OPENGL_LIBRARY} glut)
        endif()
        INSTALL(TARGETS demoPUTSLAM RUNTIME DESTINATION bin)

endif(BUILD_PUTAR AND BUILD_PUTAR_DEMO_PUTSLAM)

###############################################################################
#
#
# PUTAR demo HMIControl
#
###############################################################################

if(BUILD_PUTAR AND BUILD_PUTAR_DEMO_HMICONTROL)
        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath ./")
        SET(DEMO_SOURCES ./demos/demoHMIControl.cpp)
        ADD_EXECUTABLE(demoHMIControl ${DEMO_SOURCES})
        if(APPLE)
            TARGET_LINK_LIBRARIES(demoHMIControl HMIControl Visualizer tinyxml2 ${OpenCV_LIBS} ${QGLVIEWER_LIBRARY} ${QT_LIBRARIES} ${QT_LIBRARIES} /usr/local/Cellar/qt/4.8.7_2/lib/QtCore.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtXml.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtOpenGL.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtGui.framework ${OPENGL_LIBRARY} /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks/GLUT.framework)
        else()
            TARGET_LINK_LIBRARIES(demoHMIControl HMIControl Visualizer tinyxml2 ${OpenCV_LIBS} ${QGLVIEWER_LIBRARY} ${QT_LIBRARIES} ${QT_LIBRARIES} QtCore QtXml QtOpenGL QtGui ${OPENGL_LIBRARY} glut)
        endif()
        INSTALL(TARGETS demoHMIControl RUNTIME DESTINATION bin)

endif(BUILD_PUTAR AND BUILD_PUTAR_DEMO_HMICONTROL)

###############################################################################
#
#
# PUTAR  demo ImageVisualizer
#
#
###############################################################################

if(BUILD_PUTAR AND BUILD_PUTAR_DEMO_IMAGEVISUALIZER)
        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath ./")
        SET(DEMO_SOURCES ./demos/demoImageVisualizer.cpp)
        ADD_EXECUTABLE(demoImageVisualizer ${DEMO_SOURCES})
        if(APPLE)
            TARGET_LINK_LIBRARIES(demoImageVisualizer  tinyxml2 ${OpenCV_LIBS} )
        else()
            TARGET_LINK_LIBRARIES(demoImageVisualizer tinyxml2 ${OpenCV_LIBS} )
        endif()
        INSTALL(TARGETS demoImageVisualizer RUNTIME DESTINATION bin)

endif(BUILD_PUTAR AND BUILD_PUTAR_DEMO_IMAGEVISUALIZER)


###############################################################################
#
# PUTAR ImageVisualizer
#
###############################################################################

if(BUILD_PUTAR AND BUILD_IMAGEVISUALIZER)

        file(GLOB IMAGEVISUALIZER_SOURCES
            "${CMAKE_SOURCE_DIR}/src/ImageVisualizer/*.cpp"
        )
        file(GLOB IMAGEVISUALIZER_HEADERS
            "${CMAKE_SOURCE_DIR}/include/putar/ImageVisualizer/*.h"
        )
        ADD_LIBRARY(ImageVisualizer STATIC ${IMAGEVISUALIZER_SOURCES} ${IMAGEVISUALIZER_HEADERS})
        if(APPLE)
            TARGET_LINK_LIBRARIES(Visualizer tinyxml2 ${QGLVIEWER_LIBRARY} ${QT_LIBRARIES} /usr/local/Cellar/qt/4.8.7_2/lib/QtCore.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtXml.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtOpenGL.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtGui.framework ${OPENGL_LIBRARY})
        else()
            TARGET_LINK_LIBRARIES(Visualizer tinyxml2 ${OpenCV_LIBS} ${QGLVIEWER_LIBRARY} ${QT_LIBRARIES} QtCore QtXml QtOpenGL QtGui ${OPENGL_LIBRARY})
        endif()

endif(BUILD_PUTAR AND BUILD_IMAGEVISUALIZER)

###############################################################################
#
#
# PUTAR demo 3dsLoader
#
###############################################################################

if(BUILD_PUTAR AND BUILD_PUTAR_DEMO_3DSLOADER)
        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath ./")
        SET(DEMO_SOURCES ./demos/demo3dsLoader.cpp)
        ADD_EXECUTABLE(demo3dsLoader ${DEMO_SOURCES})
        if(APPLE)
            TARGET_LINK_LIBRARIES(demo3dsLoader ObjLoader Visualizer tinyxml2 ${QGLVIEWER_LIBRARY} ${QT_LIBRARIES} ${QT_LIBRARIES} /usr/local/Cellar/qt/4.8.7_2/lib/QtCore.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtXml.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtOpenGL.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtGui.framework ${OPENGL_LIBRARY} /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks/GLUT.framework)
        else()
            TARGET_LINK_LIBRARIES(demo3dsLoader ObjLoader Visualizer tinyxml2 ${QGLVIEWER_LIBRARY} ${QT_LIBRARIES} ${QT_LIBRARIES} QtCore QtXml QtOpenGL QtGui ${OPENGL_LIBRARY} glut)
        endif()
        INSTALL(TARGETS demo3dsLoader RUNTIME DESTINATION bin)

endif(BUILD_PUTAR AND BUILD_PUTAR_DEMO_3DSLOADER)


################################################################################
##
##
## PUTAR demo ObjLoader
##
################################################################################

#if(BUILD_PUTAR AND BUILD_PUTAR_DEMO_OBJLOADER)
#        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath ./")
#        SET(DEMO_SOURCES ./demos/demoObjLoader.cpp)
#        ADD_EXECUTABLE(demoObjLoader ${DEMO_SOURCES})
#        if(APPLE)
#            TARGET_LINK_LIBRARIES(demoObjLoader ObjLoader Visualizer tinyxml2 ${OpenCV_LIBS} ${QGLVIEWER_LIBRARY} ${QT_LIBRARIES} ${QT_LIBRARIES} /usr/local/Cellar/qt/4.8.7_2/lib/QtCore.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtXml.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtOpenGL.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtGui.framework ${OPENGL_LIBRARY} /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks/GLUT.framework)
#        else()
#            TARGET_LINK_LIBRARIES(demoObjLoader ObjLoader Visualizer tinyxml2 ${OpenCV_LIBS} ${QGLVIEWER_LIBRARY} ${QT_LIBRARIES} ${QT_LIBRARIES} QtCore QtXml QtOpenGL QtGui ${OPENGL_LIBRARY} glut)
#        endif()
#        INSTALL(TARGETS demoObjLoader RUNTIME DESTINATION bin)

#endif(BUILD_PUTAR AND BUILD_PUTAR_DEMO_OBJLOADER)

###############################################################################
#
# PUTAR demo
#
###############################################################################

if(BUILD_PUTAR AND BUILD_PUTAR_DEMO AND BUILD_PUTAR_DEMO_MAIN)

        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath ./")
        SET(DEMO_SOURCES ./main.cpp)
        ADD_EXECUTABLE(putar ${DEMO_SOURCES})
        if(APPLE)
            TARGET_LINK_LIBRARIES(putar PUTSLAM PutslamMatcher PutslamTransformEst PutslamRGBD  PutslamGrabber OpenNI2 PutslamUtilities  PutslamMap PutslamLoopClosure PutslamVisualPlaceRecognition PutslamPoseGraph csparse g2o_types_slam2d g2o_types_slam3d g2o_csparse_extension g2o_stuff g2o_core g2o_solver_csparse PUTARDefs ImageVisualizer HMIControl Utilities ObjLoader Visualizer tinyxml2  ${OpenCV_LIBS} ${QGLVIEWER_LIBRARY} ${QT_LIBRARIES} ${QT_LIBRARIES} /usr/local/Cellar/qt/4.8.7_2/lib/QtCore.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtXml.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtOpenGL.framework /usr/local/Cellar/qt/4.8.7_2/lib/QtGui.framework ${OPENGL_LIBRARY} /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks/GLUT.framework)
        else()
            TARGET_LINK_LIBRARIES(putar PUTSLAM PutslamMatcher PutslamTransformEst PutslamRGBD PutslamGrabber OpenNI2 PutslamUtilities  PutslamMap PutslamLoopClosure PutslamVisualPlaceRecognition PutslamPoseGraph csparse g2o_types_slam2d g2o_types_slam3d g2o_csparse_extension g2o_stuff g2o_core g2o_solver_csparse PUTARDefs ImageVisualizer HMIControl Utilities ObjLoader Visualizer tinyxml2 ${QGLVIEWER_LIBRARY} ${QT_LIBRARIES} ${QT_LIBRARIES} QtCore QtXml QtOpenGL QtGui ${OPENGL_LIBRARY} ${OpenCV_LIBS} glut)
        endif()
        INSTALL(TARGETS putar RUNTIME DESTINATION bin)

endif(BUILD_PUTAR AND BUILD_PUTAR_DEMO AND BUILD_PUTAR_DEMO_MAIN)
