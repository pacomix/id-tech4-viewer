SET SHADERCOMP=%~1
SET RELDIR=%~2
SET FILENAME=%~3
SET EXT=%~4
SET DIR_OUT=%~5
SET LOGFILE=%DIR_OUT%%FILENAME%%EXT%.log

IF %EXT%==.ps (
	SET SHADER_TYPE=ps_3_0
) ELSE IF %EXT%==.vs (
	SET SHADER_TYPE=vs_3_0
) ELSE (
	SET SHADER_TYPE=unknown
)

IF EXIST %LOGFILE% del %LOGFILE%
echo	Compiling shader =^> [%RELDIR%%FILENAME%%EXT%]
echo 		Compiler:	[%SHADERCOMP%] > %LOGFILE% 2>&1
echo 		File:		[%RELDIR%%FILENAME%%EXT%] >> %LOGFILE% 2>&1
echo 		Type:		[%SHADER_TYPE%] >> %LOGFILE% 2>&1


"%SHADERCOMP%" /E"main" /Fo"%DIR_OUT%%FILENAME%%EXT%c" /T"%SHADER_TYPE%" "%RELDIR%%FILENAME%%EXT%" >> %LOGFILE% 2>&1

findstr error %LOGFILE% >nul 2>&1 && (
	echo 	ERRORS detected. Please check output:
	type %LOGFILE%
	echo.
	echo.
	goto exit
) || (
	findstr warning %LOGFILE% >nul 2>&1 && (
		echo 	WARNINGS detected. Please check output:
		type %LOGFILE%
		echo.
		echo.
	)
	VERIFY > nul
	goto exit
)

:exit
EXIT /B %ERRORLEVEL%
