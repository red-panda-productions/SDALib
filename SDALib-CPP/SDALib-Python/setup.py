from distutils.core import setup, Extension
import os

userDir = os.getcwd()
newDir = userDir.split('SDALib-CPP')
os.chdir(newDir[0])

sdaLibDir = os.path.join(os.getcwd(), 'SDALib-CPP')
sdaLibCppDir = os.path.join(sdaLibDir, 'SDALib-CPP')
sdaLibPyDir = os.path.join(sdaLibDir, 'SDALib-Python')
sdaLibTestDir = os.path.join(sdaLibDir, 'SDALibTests')
sdaLibSDDir = os.path.join(sdaLibDir, 'SpeedDreams')
libDir = os.path.join(os.getcwd(), 'libraries')
libincludeDir = os.path.join(libDir, 'include')
libcmake = os.path.join(sdaLibDir, 'cmake-build-debug')
libcmakeSDALib = os.path.join(libcmake, 'SDALib-CPP')

sdTgfDir = os.path.join(sdaLibSDDir, 'tgf')
sdMathDir = os.path.join(sdaLibSDDir, 'math')
sdSimDir = os.path.join(sdaLibSDDir, 'simuv4')
sdPortDir = os.path.join(sdaLibSDDir, 'portability')
sdRobotDir = os.path.join(sdaLibSDDir, 'robottools')

sdMsgPackDir = os.path.join(libincludeDir, 'msgpack')
sdMsgPackPredefDir = os.path.join(sdMsgPackDir, 'predef')
sdMsgPackOtherDir = os.path.join(sdMsgPackPredefDir, 'other')
sdBoostDir = os.path.join(sdMsgPackDir, 'adaptor')

speedDreamsPythonFile = os.path.join(sdaLibPyDir, 'SpeedDreamsPython.cpp')

os.path.join(os.getcwd(), '/SDALib-CPP/SDALib-CPP')
module = Extension('SpeedDreamsPython',
                    include_dirs = [sdaLibPyDir, sdaLibCppDir, sdaLibTestDir, sdaLibSDDir, libcmakeSDALib,
                                    sdTgfDir, sdMathDir, libincludeDir, sdSimDir, sdPortDir, sdRobotDir],
                    sources=[speedDreamsPythonFile])
setup(ext_modules=[module])
