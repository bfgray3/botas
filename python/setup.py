from setuptools import Extension
from setuptools import setup

setup(
    name="botas",
    version="0.0.1",
    ext_modules=[Extension("botas", ["t.cpp"])],
)
