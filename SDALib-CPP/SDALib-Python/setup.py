from distutils.core import setup, Extension
import os

print(os.getcwd())
module = Extension('SpeedDreamsPython', sources=['SpeedDreamsPython.cpp'])
setup(ext_modules=[module])
