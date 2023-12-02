
CXX = icpx

SYCL_CXXFLAGS = -std=c++17 -fsycl -O3 -o
SYCL_LDFLAGS = 
SYCL_EXE_NAME = matrix_mul
SYCL_EXE_NAME_CPU = matrix_mul_cpu
SYCL_EXE_NAME_BEFORE_OPT = matrix_mul_before_opt
SYCL_SOURCES = src/matrix_mul.cpp
SYCL_CPU_SOURCES = src/matrix_mul_cpu.cpp
SYCL_BEFORE_OPT_SOURCES = src/matrix_mul_before_opt.cpp

all:
	$(CXX) $(SYCL_CXXFLAGS) $(SYCL_EXE_NAME) $(SYCL_SOURCES) $(SYCL_LDFLAGS)
	$(CXX) $(SYCL_CXXFLAGS) $(SYCL_EXE_NAME_CPU) $(SYCL_CPU_SOURCES) $(SYCL_LDFLAGS)
	$(CXX) $(SYCL_CXXFLAGS) $(SYCL_EXE_NAME_BEFORE_OPT) $(SYCL_BEFORE_OPT_SOURCES) $(SYCL_LDFLAGS)

build_sycl:
	$(CXX) $(SYCL_CXXFLAGS) $(SYCL_EXE_NAME) $(SYCL_SOURCES) $(SYCL_LDFLAGS)

build_sycl_cpu:
	$(CXX) $(SYCL_CXXFLAGS) $(SYCL_EXE_NAME_CPU) $(SYCL_CPU_SOURCES) $(SYCL_LDFLAGS)

run:
	./$(SYCL_EXE_NAME) > /dev/null

run_cpu:
	./$(SYCL_EXE_NAME_CPU) > /dev/null

run_before_opt:
	./$(SYCL_EXE_NAME_BEFORE_OPT) > /dev/null

clean: 
	rm -rf $(SYCL_EXE_NAME)
	rm -rf $(SYCL_EXE_NAME_CPU)
	rm -rf $(SYCL_EXE_NAME_BEFORE_OPT)
