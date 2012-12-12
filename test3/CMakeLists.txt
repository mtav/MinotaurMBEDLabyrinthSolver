project( test3 )
find_package( OpenCV REQUIRED )
add_executable( test3 test3 )
target_link_libraries( test3 ${OpenCV_LIBS} )
