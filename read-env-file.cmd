@echo off
for /f "eol=# delims=" %%a in (.env) do (
    set "%%a"
)
