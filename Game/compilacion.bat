@REM Compilación y Enlace con biblioteca gráfica.
@cls
@echo ---------------------------------------------------
@echo  ESAT Curso 2023-2024 Asignatura PRG Primero
@echo ---------------------------------------------------
@echo  Proceso por lotes iniciado.
@echo ---------------------------------------------------
@echo off

cl /nologo /Zi /GR- /EHs /MD %2 %3 %4 %5 %6 %7 %8 %9 -I %1\Desarrollo\Lib_Graph\ESAT_rev248\include %1\Desarrollo\Lib_Graph\ESAT_rev248\bin\ESAT.lib   opengl32.lib user32.lib gdi32.lib shell32.lib Ws2_32.lib
 
@echo ---------------------------------------------------
@echo  Proceso por lotes finalizado.
@echo ---------------------------------------------------