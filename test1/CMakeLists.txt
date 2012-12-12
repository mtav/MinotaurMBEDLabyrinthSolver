project( CannyDetector_Demo )
find_package( OpenCV REQUIRED )
add_executable( CannyDetector_Demo CannyDetector_Demo.cpp )
target_link_libraries( CannyDetector_Demo ${OpenCV_LIBS} )
