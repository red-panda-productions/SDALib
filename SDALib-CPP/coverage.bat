@echo off
            cd /d E:\source\repos\SDALib-CPP\SDALib-CPP\out\build\x86-Installer-Release-Premade\SDALibTests
            "E:/OpenCppCoverage/OpenCppCoverage.exe" --sources=E:\source\repos\SDALib-CPP\SDALib-CPP\SDALib-CPP --export_type=html:E:\source\repos\SDALib-CPP\SDALib-CPP\out\build\x86-Installer-Release-Premade\SDALibTests\coverage --excluded_sources  --excluded_line_regex  -- E:\source\repos\SDALib-CPP\SDALib-CPP\out\build\x86-Installer-Release-Premade\SDALibTests\SDATests.exe
            @start E:\source\repos\SDALib-CPP\SDALib-CPP\out\build\x86-Installer-Release-Premade\SDALibTests\coverage\index.html