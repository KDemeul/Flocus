cmake_minimum_required(VERSION 2.8)
project( GenerateStat )
find_package( OpenCV REQUIRED )

file(GLOB generate_SRC_SRC
    "*.h"
    "*.cpp"
    "*.hpp"
)

add_executable(GenerateStat ${generate_SRC_SRC})
target_link_libraries( GenerateStat ${OpenCV_LIBS} )
