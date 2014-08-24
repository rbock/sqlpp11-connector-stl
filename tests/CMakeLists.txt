macro (build_and_run arg)
	# Add headers to sources to enable file browsing in IDEs
	add_executable(Sqlpp11Stl${arg} ${arg}.cpp ${sqlpp_headers})
	target_link_libraries(Sqlpp11Stl${arg} sqlpp11-connector-sqlite3)
	target_link_libraries(Sqlpp11Stl${arg} sqlite3)
	add_test(Sqlpp11Stl${arg} ${arg})
endmacro ()

build_and_run(SampleTest)
