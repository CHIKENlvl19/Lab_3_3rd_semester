CXX=g++
CXXFLAGS= -std=c++20 -Wall -Wextra -O0 --coverage
LDFLAGS= --coverage -lgtest -lgtest_main -pthread -lgmp -lgmpxx

SRC_DIR=src
INC_DIR=include
TEST_DIR=tests
BENCH_DIR = benchmarks
BUILD_DIR=build
COVERAGE_DIR=coverage

SOURCES=$(wildcard $(SRC_DIR)/*.cpp)
TESTS=$(wildcard $(TEST_DIR)/*.cpp)

TARGET=$(BUILD_DIR)/test_exec
BENCH_TARGET=$(BUILD_DIR)/bench_exec

BENCH_SOURCES=$(wildcard $(BENCH_DIR)/*.cpp)
TEST_SOURCES=$(filter-out $(BENCH_SOURCES), $(TESTS))

all: $(TARGET)

# ----------- Обычные тесты (с покрытием) -----------
$(TARGET): $(SOURCES) $(TEST_SOURCES)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) main.cpp $(SOURCES) $(TEST_SOURCES) -o $(TARGET) $(LDFLAGS)

run:
	$(TARGET)

# ----------- Бенчмарки (ВНЕ покрытия) -----------
bench: $(BENCH_TARGET)

$(BENCH_TARGET): $(SOURCES) $(BENCH_SOURCES)
	mkdir -p $(BUILD_DIR)
	$(CXX) -std=c++20 -O2 -I$(INC_DIR) $(SOURCES) $(BENCH_SOURCES) -o $(BENCH_TARGET) -lgtest -lbenchmark -lpthread

run_bench: bench
	$(BENCH_TARGET)

# ----------- Coverage -----------
coverage: clean all run
	mkdir -p $(COVERAGE_DIR)

	# Сбор покрытия только из build/, игнорируем ошибки GCC+GTest
	lcov --capture \
		--directory $(BUILD_DIR) \
		--output-file $(COVERAGE_DIR)/coverage.info \
		--ignore-errors inconsistent \
		--ignore-errors mismatch

	# Удаляем чужие файлы покрытия (стандартная библиотека, gtest)
	lcov --remove coverage/coverage.info \
	"/usr/*" \
	"*/googletest/*" \
	"*/test_*" \
	"myQueue_for_CBT.hpp" \
	-o coverage/coverage_clean.info \
	--ignore-errors inconsistent \
	--ignore-errors mismatch \
	--ignore-errors unused

	# HTML отчёт
	genhtml $(COVERAGE_DIR)/coverage_clean.info \
		--output-directory $(COVERAGE_DIR)

	@echo "HTML report ready: $(COVERAGE_DIR)/index.html"

clean:
	rm -rf $(BUILD_DIR) $(COVERAGE_DIR) *.gcda *.gcno *.txt

.PHONY: all run bench run_bench coverage clean
