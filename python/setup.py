from distutils.core import setup
from Cython.Build import cythonize
from distutils.extension import Extension
import numpy as np

# To compile and install locally run "python setup.py build_ext --inplace"
# To install library to Python site-packages run "python setup.py build_ext install"

ext_modules = [
    Extension(
        'streamingcc._f2',
        sources=[
            '../src/f2.cc',
            '../src/hash.cc',
            '../src/util.cc',
            'streamingcc/_f2.pyx'],
        include_dirs = ['../src/streamingcc_include/'],
        extra_compile_args=['-Wno-unused-function', '-std=c++11'],
        language='c++'
    )
]

setup(
    name='streamingcc',
    packages=['streamingcc'],
    package_dir = {'streamingcc': 'streamingcc'},
    version='1.0',
    ext_modules=cythonize(ext_modules)
)
