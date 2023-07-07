project(SQLITE_DB_Example)

add_executable(${PROJECT_NAME} "main.cpp")

target_include_directories(${PROJECT_NAME}
	PUBLIC "../include"
)

target_link_libraries(${PROJECT_NAME}
	SQLITE_DB
)