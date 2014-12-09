cmake_minimum_required(VERSION 2.8)
project( Kalman )
find_package( OpenCV REQUIRED )
file(GLOB generate_SRC
	"src/*.h"
	"src/*.cpp"
	"src/*.hpp"
)

add_executable( Kalman ${generate_SRC})
target_link_libraries( Kalman ${OpenCV_LIBS} )
