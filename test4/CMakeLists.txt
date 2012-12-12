project( how_to_scan_images )
find_package( OpenCV REQUIRED )
add_executable( how_to_scan_images how_to_scan_images.cpp )
target_link_libraries( how_to_scan_images ${OpenCV_LIBS} )
