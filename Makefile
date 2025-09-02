clean:
	rm -rf build knuth/*.so leaks.trace


coverage-build:
	COVERAGE_BUILD=1 uv run python setup.py build_ext --inplace
coverage-run: coverage-build
	uv run pytest --cov=knuth --cov-report=term-missing
coverage-report:
	uv run gcovr -r . --print-summary
coverage: clean coverage-run coverage-report
