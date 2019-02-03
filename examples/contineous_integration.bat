@echo off
arduino_debug --verify --verbose-build .\E00_Minimal\E00_Minimal.ino
REM echo The return value is %errorlevel%

if %errorlevel% NEQ 0 ( 
	Echo Error status: %errorlevel%
	goto error_handler
)

arduino_debug --verify --verbose-build .\E01_HelloBootCamp\E01_HelloBootCamp.ino
REM echo The return value is %errorlevel%
if %errorlevel% NEQ 0 ( 
	Echo Error status %errorlevel%
	goto error_handler
)

arduino_debug --verify --verbose-build .\E02_ReadMaxCommBufferSize\E02_ReadMaxCommBufferSize.ino
REM echo The return value is %errorlevel%
if %errorlevel% NEQ 0 ( 
	Echo Error status %errorlevel%
	goto error_handler
)

arduino_debug --verify --verbose-build .\E03_GetUniqueID\E03_GetUniqueID.ino
REM echo The return value is %errorlevel%
if %errorlevel% NEQ 0 ( 
	Echo Error status %errorlevel%
	goto error_handler
)

arduino_debug --verify --verbose-build .\E04_GetRandom\E04_GetRandom.ino
REM echo The return value is %errorlevel%
if %errorlevel% NEQ 0 ( 
	Echo Error status %errorlevel%
	goto error_handler
)

arduino_debug --verify --verbose-build .\E05_GetCertificate\E05_GetCertificate.ino
REM echo The return value is %errorlevel%
if %errorlevel% NEQ 0 ( 
	Echo Error status %errorlevel%
	goto error_handler
)

arduino_debug --verify --verbose-build .\E06_CalculateHash\E06_CalculateHash.ino
REM echo The return value is %errorlevel%
if %errorlevel% NEQ 0 ( 
	Echo Error status %errorlevel%
	goto error_handler
)

arduino_debug --verify --verbose-build .\E07_SignVerify\E07_SignVerify.ino
REM echo The return value is %errorlevel%
if %errorlevel% NEQ 0 ( 
	Echo Error status %errorlevel%
	goto error_handler
)

arduino_debug --verify --verbose-build .\E08_OneWayAuth\E08_OneWayAuth.ino
REM echo The return value is %errorlevel%
if %errorlevel% NEQ 0 ( 
	Echo Error status %errorlevel%
	goto error_handler
)

arduino_debug --verify --verbose-build .\E09_GenerateKeypair\E09_GenerateKeypair.ino
REM echo The return value is %errorlevel%
if %errorlevel% NEQ 0 ( 
	Echo Error status %errorlevel%
	goto error_handler
)

arduino_debug --verify --verbose-build .\E10_PseudoTLS\E10_PseudoTLS.ino
REM echo The return value is %errorlevel%
if %errorlevel% NEQ 0 ( 
	Echo Error status %errorlevel%
	goto error_handler
)

arduino_debug --verify --verbose-build .\H01_ObjectDump\H01_ObjectDump.ino
REM echo The return value is %errorlevel%
if %errorlevel% NEQ 0 ( 
	Echo Error status %errorlevel%
	goto error_handler
)

arduino_debug --verify --verbose-build .\H02_ChangeI2CAddress\H02_ChangeI2CAddress.ino
REM echo The return value is %errorlevel%
if %errorlevel% NEQ 0 ( 
	Echo Error status %errorlevel%
	goto error_handler
)
arduino_debug --verify --verbose-build .\H03_SearchI2CAddress\H03_SearchI2CAddress.ino
REM echo The return value is %errorlevel%
if %errorlevel% NEQ 0 ( 
	Echo Error status %errorlevel%
	goto error_handler
)
arduino_debug --verify --verbose-build .\H04_CheckValidPublicKey\H04_CheckValidPublicKey.ino
REM echo The return value is %errorlevel%
if %errorlevel% NEQ 0 ( 
	Echo Error status %errorlevel%
	goto error_handler
)
arduino_debug --verify --verbose-build .\H05_LoopReadUID\H05_LoopReadUID.ino
REM echo The return value is %errorlevel%
if %errorlevel% NEQ 0 ( 
	Echo Error status %errorlevel%
	goto error_handler
)
arduino_debug --verify --verbose-build .\H06_SecurityCounter\H06_SecurityCounter.ino
REM echo The return value is %errorlevel%
if %errorlevel% NEQ 0 ( 
	Echo Error status %errorlevel%
	goto error_handler
)
	exit 0

:error_handler
if %errorlevel% == 1 ( 
	Echo Build failed or upload failed
	exit 0
)
if %errorlevel% == 2 (
	Echo Sketch not found
	exit 0
)
if %errorlevel% == 3 (
	Echo Invalid (argument for) commandline option
	exit 0
)
if %errorlevel% == 4 (
	Echo Preference passed to --get-pref does not exist
	exit 0
)
	
:End