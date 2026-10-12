@echo off
chcp 65001 >nul
setlocal EnableDelayedExpansion

:: ========== Config ==========
set "ENGINE=R:\texlive\2026\bin\windows"
set "TEXFILE=main.tex"
set "OUTNAME=Periodicity Mathematics British English"

:: ========== Compile Info ==========
echo ============================================
echo   Compile: %TEXFILE%
echo   Output : "%OUTNAME%.pdf"
echo   Engine : XeLaTeX, 3 passes
echo   Note   : Using thebibliography, no BibTeX/Biber needed
echo ============================================

for %%P in (1 2 3) do (
    echo [%%P/3] XeLaTeX...
    "%ENGINE%\xelatex.exe" -synctex=1 -interaction=nonstopmode -file-line-error -jobname="%OUTNAME%" "%TEXFILE%"
    if errorlevel 1 (
        echo [Error] XeLaTeX pass %%P failed! Check "%OUTNAME%.log"
        goto :end
    )
)

echo ============================================
echo [Success] Compile finished: "%OUTNAME%.pdf"
echo ============================================

:end
endlocal
pause