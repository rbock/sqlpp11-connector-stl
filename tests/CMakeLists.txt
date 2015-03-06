macro (build_and_run arg)
	# Add headers to sources to enable file browsing in IDEs
	add_executable(SqlppStl${arg} ${arg}.cpp ${sqlpp_headers})
	add_test(${arg} SqlppStl${arg})
endmacro ()

build_and_run(SampleTest)
