@echo off
cd /d "%~dp0"
echo ==============================================
echo     Starting Cat Bot Python Controller
echo ==============================================
python -m pip install -q requests
python cat_bot_controller.py
pause
