find_package(Boost 1.67 COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

find_package(OpenCV COMPONENTS videoio core highgui imgproc ml features2d calib3d objdetect flann REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_library(AVUTIL_LIBRARY     avutil)
find_library(AVCODEC_LIBRARY    avcodec)
find_library(AVFORMAT_LIBRARY   avformat)
find_library(AVFILTER_LIBRARY   avfilter)
find_library(SWSCALE_LIBRARY    swscale)
find_library(SWRESAMPLE_LIBRARY swresample)

include_directories(${Sean_KeyframeGridProject_SOURCE_DIR}/LibraryProject/include)

link_directories(${Sean_KeyframeGridProject_SOURCE_DIR}/LibraryProject/source)
add_library(LibraryProject StringUtil.cpp CSVFileWriter.cpp Process.cpp VideoFileFrameInformation.cpp)
target_link_libraries(LibraryProject LINK_PUBLIC ${Boost_LIBRARIES} ${OpenCV_LIBS} ${AVUTIL_LIBRARY} ${AVCODEC_LIBRARY} ${AVFORMAT_LIBRARY} ${AVFILTER_LIBRARY} ${SWSCALE_LIBRARY} ${SWRESAMPLE_LIBRARY})
