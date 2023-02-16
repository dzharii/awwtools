param(
    [Parameter(Mandatory=$true)]
    [string]$FileName
)

$ErrorActionPreference = "Stop"

$host.ui.RawUI.WindowTitle = "My Title"
$ThisScriptPath = Split-Path -Parent $MyInvocation.MyCommand.Definition

Write-Host "Hello World"

