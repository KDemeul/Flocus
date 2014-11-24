cmake_minimum_required(VERSION 2.8)
project( Annotate )
find_package( OpenCV REQUIRED )

file(GLOB annotate_SRC
    "*.h"
    "*.cpp"
    "*.hpp"
)

add_executable(Annotate ${annotate_SRC})
target_link_libraries( Annotate ${OpenCV_LIBS} )
