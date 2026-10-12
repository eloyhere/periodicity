@echo off
chcp 65001 >nul
setlocal

set TEXNAME=%~n1
if "%~1"=="" (
    set TEXNAME=main
) else (
    set TEXNAME=%~n1
)

echo ============================================
echo   Clean auxiliary files for: %TEXNAME%
echo ============================================

:: LaTeX 辅助文件
del /q "*.aux" 2>nul
del /q "*.log" 2>nul
del /q "*.out" 2>nul
del /q "*.toc" 2>nul
del /q "*.lof" 2>nul
del /q "*.lot" 2>nul
del /q "*.synctex.gz" 2>nul
del /q "*.fls" 2>nul
del /q "*.xdv" 2>nul
del /q "*.fdb_latexmk" 2>nul

:: Biber 缓存
del /q "*.bbl" 2>nul
del /q "*.blg" 2>nul
del /q "*.bcf" 2>nul
rd /s /q "*_biber_tmp" 2>nul

:: 章节级辅助文件（如果你的子文件也生成了 .aux）
del /q chapters\*.aux 2>nul
del /q sections\*.aux 2>nul

:: 其他常见辅助文件
del /q "*.idx" 2>nul
del /q "*.ind" 2>nul
del /q "*.ilg" 2>nul
del /q "*.nav" 2>nul
del /q "*.snm" 2>nul
del /q "*.vrb" 2>nul
del /q "*.thm" 2>nul
del /q "*.xdv" 2>nul
del /q "*.run.xml" 2>nul

del /q "*.run.xml" 2>nul

del /q "*.pdf" 2>nul

echo ✅ Clean complete.
echo ============================================

endlocal