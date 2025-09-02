coverage-build:
	COVERAGE=1 uv run python setup.py build_ext --inplace

coverage-run: coverage-build
	uv run pytest

coverage-report:
	uv run gcovr -r . --print-summary

clean:
	rm -rf build knuth/*.so

coverage: clean coverage-run coverage-report
