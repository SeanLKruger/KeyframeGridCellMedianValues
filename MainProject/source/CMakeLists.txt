set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(Boost 1.67 COMPONENTS program_options filesystem system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

find_package(OpenCV COMPONENTS videoio core highgui REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(${Sean_KeyframeGridProject_SOURCE_DIR}/LibraryProject/include)
include_directories(${Sean_KeyframeGridProject_SOURCE_DIR}/MainProject/include)

link_directories(${Sean_KeyframeGridProject_SOURCE_DIR}/MainProject/source)
add_executable(MainProject ProcessArguments.cpp CalculateMedianValuesOfPixelsOfFrameGridCells.cpp PrintCSVFileLines.cpp main.cpp)
target_link_libraries(MainProject LINK_PUBLIC Threads::Threads ${Boost_LIBRARIES} ${OpenCV_LIBS} LibraryProject)
