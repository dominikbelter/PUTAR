# Need to find both Qt4 and QGLViewer if the QQL support is to be built
#FIND_PACKAGE(PUTSLAM)

set(PUTSLAM_INCLUDE_DIR "/usr/local/include/putslam")


FIND_PATH(PUTSLAM_INCLUDE_DIR PUTSLAM.h
    /usr/include/putslam
    /opt/local/include/putslam
    /usr/local/include/putslam
    /sw/include/putslam

    cd ..
    #ENV PUTSLAMROOT
  )

find_library(PUTSLAM_LIBRARY_RELEASE
  NAMES PUTSLAM
  PATHS /usr/lib
        /usr/local/lib
        /opt/local/lib
        /sw/lib
        ENV PUTSLAMROOT
        ENV LD_LIBRARY_PATH
        ENV LIBRARY_PATH
  PATH_SUFFIXES PUTSLAM PUTSLAM/release
)
find_library(PUTSLAM_LIBRARY_DEBUG
  NAMES PUTSLAM
  PATHS /usr/lib
        /usr/local/lib
        /opt/local/lib
        /sw/lib
        ENV PUTSLAMROOT
        ENV LD_LIBRARY_PATH
        ENV LIBRARY_PATH
  PATH_SUFFIXES PUTSLAM PUTSLAM/release
)

if(PUTSLAM_LIBRARY_RELEASE)
  if(PUTSLAM_LIBRARY_DEBUG)
    set(PUTSLAM_LIBRARY optimized ${PUTSLAM_LIBRARY_RELEASE} debug ${PUTSLAM_LIBRARY_DEBUG})
  else()
    set(PUTSLAM_LIBRARY ${PUTSLAM_LIBRARY_RELEASE})
  endif()
endif()

