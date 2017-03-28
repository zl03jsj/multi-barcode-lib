del *.map /S
del *.trg /S
del *.tlb /S
del *.sym /S
del *.vxd /S
del *.386 /S
del *.exp /S
del *.pel /S
del *.rsp /S
del *.res /S
del *.pdb /S
del *.pch /S
del *.sbr /S
del *.bsc /S
del *.plg /S
del *.scc /S
del *.ncb /S
del *.opt /S
del *.aps /S
del *.tmp /S
del *.ilk /S
del *.idb /S
del *.sup /S
del *.tlh /S
del *.tli /S
@for /F "delims=" %%i in ('dir *.obj /S /B^|find "\Release\"') do del "%%i"
@for /F "delims=" %%i in ('dir *.obj /S /B^|find "\Debug\"') do del "%%i"
@for /f "delims=" %%i in ('dir /S /B /AD^|find "\Release"' ) do rd /q "%%i"
@for /f "delims=" %%i in ('dir /S /B /AD^|find "\Debug"' ) do rd /q "%%i"