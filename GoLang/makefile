# GoLang/Makefile

GOCMD=go
GOBUILD=$(GOCMD) build
GOTEST=$(GOCMD) test
GOMOD=$(GOCMD) mod
GOFMT=$(GOCMD) fmt
GOVET=$(GOCMD) vet

.PHONY: all
all: clean fmt vet test bench
	@echo ""
	@echo "All tests and benchmarks completed successfully"

.PHONY: quick
quick: fmt vet test

.PHONY: fmt
fmt:
	@echo "Formatting code..."
	@$(GOFMT) ./...

.PHONY: vet
vet:
	@echo "Running go vet..."
	@$(GOVET) ./...

.PHONY: tidy
tidy:
	@echo "Tidying modules..."
	@$(GOMOD) tidy

.PHONY: test
test:
	@echo ""
	@echo "Running all tests..."
	@echo ""
	@$(GOTEST) -v -count=1 ./...

.PHONY: bench
bench:
	@echo ""
	@echo "Running all benchmarks..."
	@echo ""
	@$(GOTEST) -bench=. -benchmem -benchtime=1s -timeout=5m ./...

.PHONY: coverage
coverage:
	@echo "Running tests with coverage..."
	@$(GOTEST) -coverprofile=coverage.out ./...
	@$(GOCMD) tool cover -html=coverage.out -o coverage.html
	@echo "Coverage report: coverage.html"
	@$(GOCMD) tool cover -func=coverage.out | tail -1

.PHONY: bench-save
bench-save:
	@echo "Running benchmarks and saving results..."
	@mkdir -p bench_results
	@$(GOTEST) -bench=. -benchmem -benchtime=3s ./... | tee bench_results/bench_$$(date +%Y%m%d_%H%M%S).txt

.PHONY: bench-compare
bench-compare:
	@echo "Running benchmarks for comparison..."
	@mkdir -p bench_results
	@$(GOTEST) -bench=. -benchmem -benchtime=3s ./... | tee bench_results/current.txt
	@echo ""
	@echo "To compare: benchstat bench_results/previous.txt bench_results/current.txt"

.PHONY: test-array
test-array:
	@$(GOTEST) -v -run "TestArray" .

.PHONY: test-queue
test-queue:
	@$(GOTEST) -v -run "TestQueue" .

.PHONY: test-stack
test-stack:
	@$(GOTEST) -v -run "TestStack" .

.PHONY: test-slist
test-slist:
	@$(GOTEST) -v -run "TestSList" .

.PHONY: test-dlist
test-dlist:
	@$(GOTEST) -v -run "TestDList" .

.PHONY: test-cbt
test-cbt:
	@$(GOTEST) -v -run "TestCBT" .

.PHONY: test-htoa
test-htoa:
	@$(GOTEST) -v -run "TestHashTableOA" .

.PHONY: test-htchains
test-htchains:
	@$(GOTEST) -v -run "TestHashTableChains" .

.PHONY: bench-array
bench-array:
	@$(GOTEST) -bench=BenchmarkArray -benchmem .

.PHONY: bench-queue
bench-queue:
	@$(GOTEST) -bench=BenchmarkQueue -benchmem .

.PHONY: bench-stack
bench-stack:
	@$(GOTEST) -bench=BenchmarkStack -benchmem .

.PHONY: bench-slist
bench-slist:
	@$(GOTEST) -bench=BenchmarkSList -benchmem .

.PHONY: bench-dlist
bench-dlist:
	@$(GOTEST) -bench=BenchmarkDList -benchmem .

.PHONY: bench-cbt
bench-cbt:
	@$(GOTEST) -bench=BenchmarkCBT -benchmem .

.PHONY: bench-htoa
bench-htoa:
	@$(GOTEST) -bench=BenchmarkHashTableOA -benchmem .

.PHONY: bench-htchains
bench-htchains:
	@$(GOTEST) -bench=BenchmarkHashTableChains -benchmem .

.PHONY: clean
clean:
	@echo "Cleaning up..."
	@rm -f coverage.out coverage.html
	@rm -f *.txt *.bin
	@rm -f test_*.txt test_*.bin
	@rm -f bench_*.txt bench_*.bin
	@$(GOCMD) clean -testcache

.PHONY: clean-all
clean-all: clean
	@rm -rf bench_results

.PHONY: build
build:
	@echo "Building..."
	@$(GOBUILD) -v ./...

.PHONY: check-fmt
check-fmt:
	@echo "Checking code formatting..."
	@test -z "$$(gofmt -l .)" || (echo "Code is not formatted. Run 'make fmt'" && exit 1)

.PHONY: ci
ci: check-fmt vet test coverage
	@echo "CI checks passed"

.PHONY: help
help:
	@echo ""
	@echo "Available targets:"
	@echo "  make all              - Run everything (fmt, vet, test, bench)"
	@echo "  make quick            - Quick check (fmt, vet, test only)"
	@echo "  make test             - Run all tests"
	@echo "  make bench            - Run all benchmarks"
	@echo ""
	@echo "Code quality:"
	@echo "  make fmt              - Format code"
	@echo "  make vet              - Run go vet"
	@echo "  make coverage         - Generate coverage report"
	@echo "  make ci               - Run CI checks"
	@echo ""
	@echo "Individual tests:"
	@echo "  make test-array       - Test MyArray"
	@echo "  make test-queue       - Test MyQueue"
	@echo "  make test-stack       - Test MyStack"
	@echo "  make test-slist       - Test SinglyLinkedList"
	@echo "  make test-dlist       - Test DoublyLinkedList"
	@echo "  make test-cbt         - Test CompleteBinaryTree"
	@echo "  make test-htoa        - Test HashTableOA"
	@echo "  make test-htchains    - Test HashTableChains"
	@echo ""
	@echo "Individual benchmarks:"
	@echo "  make bench-array      - Benchmark MyArray"
	@echo "  make bench-queue      - Benchmark MyQueue"
	@echo "  make bench-stack      - Benchmark MyStack"
	@echo "  make bench-slist      - Benchmark SinglyLinkedList"
	@echo "  make bench-dlist      - Benchmark DoublyLinkedList"
	@echo "  make bench-cbt        - Benchmark CompleteBinaryTree"
	@echo "  make bench-htoa       - Benchmark HashTableOA"
	@echo "  make bench-htchains   - Benchmark HashTableChains"
	@echo ""
	@echo "Utilities:"
	@echo "  make clean            - Clean temporary files"
	@echo "  make clean-all        - Clean everything including results"
	@echo "  make build            - Build project"
	@echo "  make bench-save       - Save benchmark results to file"
	@echo "  make bench-compare    - Prepare for performance comparison"
	@echo ""
