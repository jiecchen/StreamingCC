from distutils.core import setup
from Cython.Build import cythonize
from distutils.extension import Extension
import glob

# To compile and install locally run "python setup.py build_ext --inplace"
# To install library to Python site-packages run "python setup.py build_ext install"

all_cc_files = glob.glob('../src/*.cc')

ext_modules = [
    Extension(
        'streamingcc._f2',
        sources=['streamingcc/_f2.pyx'] + all_cc_files,
        include_dirs = ['../src/streamingcc_include/'],
        extra_compile_args=['-std=c++11'],
        language='c++'
    ),
    Extension(
        'streamingcc._count_min',
        sources=['streamingcc/_count_min.pyx'] + all_cc_files,
        include_dirs = ['../src/streamingcc_include/'],
        extra_compile_args=['-std=c++11'],
        language='c++'
    ),
    Extension(
        'streamingcc._bloom_filter',
        sources=['streamingcc/_bloom_filter.pyx'] + all_cc_files,
        include_dirs = ['../src/streamingcc_include/'],
        extra_compile_args=['-std=c++11'],
        language='c++'
    ),    
    Extension(
        'streamingcc._hyper_loglog',
        sources=['streamingcc/_hyper_loglog.pyx'] + all_cc_files,
        include_dirs = ['../src/streamingcc_include/'],
        extra_compile_args=['-std=c++11'],
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
