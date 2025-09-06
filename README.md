# knuth
Code from "The Art of Computer Programming"

# usage
## build
```
uv run setup.py build_ext --inplace 
```

## run tests
```
uv run pytest
```

## run tests with code coverage

```
make coverage
```

On some operating systems the compiler may need to be specified to match what gcovr expects, e.g.

```
CC=gcc make coverage
```
