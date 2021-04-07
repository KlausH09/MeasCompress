build_tests:
	cmake -B ./build -S . -DCMAKE_BUILD_TYPE=Release
	cmake --build ./build --parallel --config Release
