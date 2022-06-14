from distutils.core import setup, Extension
import os
import sys

userDir = os.getcwd()
newDir = userDir.split('SDALib-CPP')[0]
os.chdir('C:\\')

sdaLibDir = os.path.join(newDir, 'SDALib-CPP')
sdaLibCppDir = os.path.join(sdaLibDir, 'SDALib-CPP')
sdaLibPyDir = os.path.join(sdaLibDir, 'SDALib-Python')
sdaLibTestDir = os.path.join(sdaLibDir, 'SDALibTests')
sdaLibSDDir = os.path.join(sdaLibDir, 'SpeedDreams')
libDir = os.path.join(newDir, 'libraries')
libIncludeDir = os.path.join(libDir, 'include')
libLibDir = os.path.join(libDir, 'lib')
libcmake = os.path.join(sdaLibDir, 'cmake-build-debug')
libcmakeSDALib = os.path.join(libcmake, 'SDALib-CPP')

sdTgfDir = os.path.join(sdaLibSDDir, 'tgf')
sdMathDir = os.path.join(sdaLibSDDir, 'math')
sdSimDir = os.path.join(sdaLibSDDir, 'simuv4')
sdPortDir = os.path.join(sdaLibSDDir, 'portability')
sdRobotDir = os.path.join(sdaLibSDDir, 'robottools')

speedDreamsPythonFile = os.path.join(sdaLibPyDir, 'SpeedDreamsPython.cpp')
sdaTypesConverterFile = os.path.join(sdaLibPyDir, 'SDATypesConverter.cpp')
sdaSpeedDreamsFile = os.path.join(sdaLibCppDir, 'SDASpeedDreams.cpp')
sdaDataFile = os.path.join(sdaLibCppDir, 'SDAData.cpp')

aeroFile = os.path.join(sdSimDir, 'aero.cpp')
atmosphereFile = os.path.join(sdSimDir, 'atmosphere.cpp')
axleFile = os.path.join(sdSimDir, 'axle.cpp')
brakeFile = os.path.join(sdSimDir, 'brake.cpp')
carFile = os.path.join(sdSimDir, 'car.cpp')

categoriesFile = os.path.join(sdSimDir, 'categories.cpp')
collideFile = os.path.join(sdSimDir, 'collide.cpp')
differentialFile = os.path.join(sdSimDir, 'differential.cpp')
engineFile = os.path.join(sdSimDir, 'engine.cpp')
suspFile = os.path.join(sdSimDir, 'susp.cpp')
simuFile = os.path.join(sdSimDir, 'simu.cpp')
wheelFile = os.path.join(sdSimDir, 'wheel.cpp')
steerFile = os.path.join(sdSimDir, 'steer.cpp')
transmissionFile = os.path.join(sdSimDir, 'transmission.cpp')

rttrackFile = os.path.join(sdRobotDir, 'rttrack.cpp')
rtutilFile = os.path.join(sdRobotDir, 'rtutil.cpp')

portabilityFile = os.path.join(sdPortDir, 'portability.cpp')

sgGeoFile = os.path.join(sdMathDir, 'sg_geodesy.cpp')
sgRandomFile = os.path.join(sdMathDir, 'sg_random.c')
sgGeodesyFile = os.path.join(sdMathDir, 'SGGeodesy.cpp')
vectorFile = os.path.join(sdMathDir, 'vector.cpp')

paramsFile = os.path.join(sdTgfDir, 'params.cpp')
tgfFile = os.path.join(sdTgfDir, 'tgf.cpp')
directoryFile = os.path.join(sdTgfDir, 'directory.cpp')
fileFile = os.path.join(sdTgfDir, 'file.cpp')
fileSetupFile = os.path.join(sdTgfDir, 'filesetup.cpp')
formulaFile = os.path.join(sdTgfDir, 'formula.cpp')
hashFile = os.path.join(sdTgfDir, 'hash.cpp')
legacymoduleFile = os.path.join(sdTgfDir, 'legacymodule.cpp')
linuxspecFile = os.path.join(sdTgfDir, 'linuxspec.cpp')
windowsspecFile = os.path.join(sdTgfDir, 'windowsspec.cpp')
memmanagerFile = os.path.join(sdTgfDir, 'memmanager.cpp')
osFile = os.path.join(sdTgfDir, 'os.cpp')
profilerFile = os.path.join(sdTgfDir, 'profiler.cpp')
schedulespyFile = os.path.join(sdTgfDir, 'schedulespy.cpp')
traceFile = os.path.join(sdTgfDir, 'trace.cpp')

macros = [('PYTHON_MODULE', '1')]
if sys.platform == "win32":
    macros.append(('WIN32', '1'))

module = Extension('SpeedDreamsFunction',
                   define_macros = macros,
                   include_dirs = [sdaLibPyDir, sdaLibCppDir, sdaLibTestDir, sdaLibSDDir, libcmakeSDALib,
                                   sdTgfDir, sdMathDir, sdSimDir, sdPortDir, sdRobotDir, libIncludeDir,
                                   libLibDir],
                   library_dirs = [libLibDir, "C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.19041.0\\um\\x86"],
                   libraries = ['sg', 'solid2', 'ul', 'IPCLib', 'libexpat', 'js', 'libssl', 'sl', 'ssg', 'User32'],
                   sources=[sdaDataFile, simuFile, sdaTypesConverterFile, transmissionFile, sdaSpeedDreamsFile,
                            speedDreamsPythonFile, wheelFile, rttrackFile, rtutilFile, portabilityFile,
                            sgGeoFile, sgRandomFile, sgGeodesyFile, vectorFile, carFile, brakeFile, axleFile,
                            atmosphereFile, aeroFile, steerFile, tgfFile, paramsFile, directoryFile, fileFile,
                            fileSetupFile, formulaFile, hashFile, legacymoduleFile, windowsspecFile,
                            memmanagerFile, osFile, profilerFile, schedulespyFile, traceFile,
                            categoriesFile, collideFile, differentialFile, engineFile, suspFile])

setup(name='SpeedDreamsFunction',
      ext_modules=[module])
