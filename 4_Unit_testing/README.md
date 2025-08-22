# Instructions how to build test program with googletest and test cases

1. Pura zip-tiedosto ja mene hakemistoon `parser`

2. `cd build`

3. `cmake -G "MinGW Makefiles" ..` (Huomaa kaksi pistettä lopussa)

4. `cmake --build .` (Huomaa yksi piste lopussa)

5. `cd ..\test_program`

6. Aja testiohjelma `TimeParserTest.exe`

> [!CAUTION]
> Always make sure you are in the project ´build´ directory before making cmake commands. Otherwise build process will fail..

