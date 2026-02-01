include(FetchContent)
FetchContent_Declare(
	argparse
	GIT_REPOSITORY https://github.com/morrisfranken/argparse.git
	# GIT_TAG        main
)
FetchContent_MakeAvailable(argparse)

ADD_EXECUTABLE(prepy_exe app.cpp)
TARGET_LINK_LIBRARIES(prepy_exe PUBLIC prepy argparse)
set_target_properties(prepy_exe PROPERTIES OUTPUT_NAME prepy)
