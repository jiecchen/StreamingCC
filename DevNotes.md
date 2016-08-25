Development Notes for `StreamingCC`
----------------------------------------

## The Framework
- TODO(jiecchen): add the design docs



## Workflow of adding new code
To add a new streaming algorithm, please follow the steps:

-  create `foo.cc` file in `src/` and a corresponding `foo.h` file in `src/streamingcc_include/`.
-  add `foo_test.cc` in `tests/`
-  add `_foo.pyx` and `foo.py` in `python/streamingcc`, and edit `python/setup.py`
- before submitting your code, please run [cpplint.py](https://raw.githubusercontent.com/google/styleguide/gh-pages/cpplint/cpplint.py) on all `*.cc` and  `*.h` files to check your coding style. Make sure no error is reported.
