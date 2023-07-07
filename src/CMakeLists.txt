project(SQLITE_DB)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "../include/*.hpp")
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp" "*.hpp")
set(SQLITE_FILES "../external/sqlite/sqlite3.c" "../external/sqlite/sqlite3.h")

# add_library(${PROJECT_NAME} ${SOURCES} "Source.cpp")

add_library(${PROJECT_NAME} ${SQLITE_FILES} ${HEADERS} ${SOURCES})

target_include_directories(${PROJECT_NAME} 
                PRIVATE "../include/SQLiteDB"
                PRIVATE "../external/sqlite")

#source_group(TREE "internal" FILES ${SOURCES})