@echo off

if exist %2\shaders.h del %2\shaders.h

for %%F in (%1\*.glsl) do (
  echo const GLchar *%%~nF = >> %2\shaders.h
  for /F "tokens=*" %%A in (%%F) do echo "%%A\n" >> %2\shaders.h
  echo "\0"; >> %2\shaders.h
  echo. >> %2\shaders.h
)

echo ***** Successfully generated shader header. *****