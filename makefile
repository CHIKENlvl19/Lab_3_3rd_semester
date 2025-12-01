CXX=g++
CXXFLAGS= -std=c++20 -Wall -Wextra -O0 --coverage
LDFLAGS= --coverage -lgtest -lgtest_main -pthread

SRC_DIR=src
INC_DIR=include
TEST_DIR=tests
BUILD_DIR=build
COVERAGE_DIR=coverage

SOURCES=$(wildcard $(SRC_DIR)/*.cpp)
TESTS=$(wildcard $(TEST_DIR)/*.cpp)

TARGET=$(BUILD_DIR)/test_exec

all: $(TARGET)

$(TARGET): $(SOURCES) $(TESTS)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) main.cpp $(SOURCES) $(TESTS) -o $(TARGET) $(LDFLAGS)

run:
	$(TARGET)

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

.PHONY: all run coverage clean