.PHONY: build assync static test coverage linter

build:
	sudo apt install libgtest-dev
	sudo apt install lcov
	mkdir build
	mkdir log

assync:
	@cd build && cmake .. -DASSYNC=ON > cmakelog.txt && make > makelog.txt && ./hw2
	@rm -r build
	
static:
	@cd build && cmake .. -DASSYNC=OFF > cmakelog.txt && make > makelog.txt && ./hw2
	@rm -r build

test:
	@echo "STATIC LIBRARY"
	@cd build && cmake .. -DASSYNC=OFF -DTEST=ON > ../log/cmakelog_static.txt && make > ../log/makelog_static.txt && ./test/hw2_test > ../log/test_log_static.txt 
	@echo "ASSYNC LIBRARY"
	@cd build && cmake .. -DASSYNC=ON -DTEST=ON > ../log/cmakelog_assync.txt && make > ../log/makelog_assync.txt && ./test/hw2_test > ../log/test_log_assync.txt 
	@rm -r build

coverage:
	@echo "ASYNC LIBRARY"
	@cd build && cmake .. -DASSYNC=ON -DTEST=ON > ../log/cmakelog_static.txt && make > ../log/makelog_static.txt && ./test/hw2_test
	@cd build && lcov -t "test/hw2_test" -o coverage.info -c -d k_means_multiprocess/ && genhtml -o report coverage.info > ../log/async_cov.txt
	

linter:
	@cppcheck main.c
	@cppcheck k_means_classic/k_means.c
	@cppcheck k_means_multiprocess/k_means.c