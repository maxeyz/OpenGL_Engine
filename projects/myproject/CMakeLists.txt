#--------------------------------------------------------------------------
# MyProject project
#--------------------------------------------------------------------------

PROJECT(MyProject)
FILE(GLOB MyProject_headers code/*.h)
FILE(GLOB MyProject_sources code/*.cc)

SET(files_MyProject ${MyProject_headers} ${MyProject_sources})
SOURCE_GROUP("MyProject" FILES ${files_MyProject})

ADD_EXECUTABLE(MyProject ${files_MyProject})
TARGET_LINK_LIBRARIES(MyProject core render)
ADD_DEPENDENCIES(MyProject core render)