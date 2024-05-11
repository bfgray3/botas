from setuptools import Extension
from setuptools import setup

setup(
    name="botas",
    version="0.0.1",
    ext_modules=[
        Extension("botas", sources=["t.cpp"], extra_compile_args=["-std=c++20"])
    ],
)
