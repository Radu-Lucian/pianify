add_subdirectory(Converter)
if(BUILD_GUI)
	add_subdirectory(GUI)
endif()
add_subdirectory(Tests)
if(USE_BOOST)
	add_subdirectory(Client)
endif()
add_subdirectory(DatasetHelper)
add_subdirectory(InstrumentRecognizer)
if(BUILD_CLI)
	add_subdirectory(CLI)
endif()