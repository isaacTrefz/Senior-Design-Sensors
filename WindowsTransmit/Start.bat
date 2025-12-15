@echo off
REM This script forces PowerShell to run Launch.ps1 without permission errors
PowerShell -NoProfile -ExecutionPolicy Bypass -Command "& './Launch.ps1'"
pause