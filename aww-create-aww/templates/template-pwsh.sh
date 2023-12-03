#!/bin/env bash
# Example Usage:
# - ...
# runs pwsh powershell script with same name as this .cmd file
pwsh -executionpolicy unrestricted -File "${0%.*}.ps1" $@
