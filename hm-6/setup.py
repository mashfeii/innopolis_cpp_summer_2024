#!/usr/bin/env python3

from setuptools import setup, Extension


def main():
    setup(name="cjson",
          version="0.0.1",
          author="T.M. Mashenkov",
          ext_modules=[Extension("cjson", ['cjson.cpp'])])


if __name__ == "__main__":
    main()
