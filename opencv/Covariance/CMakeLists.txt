cmake_minimum_required(VERSION 2.8)
project( Covariance )
find_package( OpenCV REQUIRED )

add_executable( Covariance main.cpp)
target_link_libraries( Covariance ${OpenCV_LIBS} )
