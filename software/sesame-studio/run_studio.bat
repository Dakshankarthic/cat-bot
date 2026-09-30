@echo off
cd /d "%~dp0"
echo ==============================================
echo   Starting Animation Studio for Cat Bot
echo ==============================================
python -m pip install -q -r requirements.txt
python sesame_studio.py
if errorlevel 1 (
    echo.
    echo Make sure Python 3 is installed and added to PATH.
    pause
)
