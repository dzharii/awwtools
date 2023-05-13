@rem Example Usage:
@rem runs powershell (v5) script with same name as this .cmd file
@powershell -executionpolicy unrestricted -File "%~dp0%~n0.ps1" %*
