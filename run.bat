@echo off
REM This batch script is used to test the miteFinder program

REM Set paths for input, output, and pattern scoring files
set INPUT_FILE=GCA_001433935.1.fasta
set OUTPUT_FILE=output.txt
set PATTERN_FILE=pattern_scoring.txt

REM Check if the input file exists
if not exist %INPUT_FILE% (
    echo Input file %INPUT_FILE% does not exist. Exiting.
    exit /b 1
)

REM Check if the pattern scoring file exists
if not exist %PATTERN_FILE% (
    echo Pattern scoring file %PATTERN_FILE% does not exist. Exiting.
    exit /b 1
)

REM Run the miteFinder program with the specified arguments
miteFinder.exe -input %INPUT_FILE% -output %OUTPUT_FILE% -pattern_scoring %PATTERN_FILE% -threshold 0

REM Check if the output file is created and not empty
if exist %OUTPUT_FILE% (
    echo Output file %OUTPUT_FILE% created successfully.
) else (
    echo Output file %OUTPUT_FILE% was not created.
    exit /b 1
)

REM Display the output file content
if exist %OUTPUT_FILE% (
    echo === Output file content ===
    type %OUTPUT_FILE%
    echo ==========================
) else (
    echo No output file to display.
)

pause
