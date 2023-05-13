@rem Example Usage:
@rem runs pwsh powershell script with same name as this .cmd file
@pwsh -executionpolicy unrestricted -File "%~dp0%~n0.ps1" %*