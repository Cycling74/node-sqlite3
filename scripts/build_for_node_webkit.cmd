echo Platform: %1
echo The list of environment variables:
set
if not "%nwjs_version%" == "" goto nwjs
if not "%1" == "x86" goto end
if "%nw_version%" == "" goto end
call npm install nw-gyp
call cinst wget 7zip.commandline
call wget http://dl.nwjs.io/v%nw_version%/node-webkit-v%nw_version%-win-ia32.zip
call 7z e -onw node-webkit-v%nw_version%-win-ia32.zip
dir nw
set PATH=nw;%PATH%
call node-pre-gyp rebuild --runtime=node-webkit --target=%nw_version% --target_arch=ia32
call node-pre-gyp package testpackage --runtime=node-webkit --target=%nw_version% --target_arch=ia32
if not "%CM%" == "%CM:[publish binary]=%" call node-pre-gyp publish --msvs_version=2013 --runtime=node-webkit --target=%nw_version% --target_arch=ia32
goto end

:nwjs
if not "%1" == "x86" goto nwjs_64
call npm install nw-gyp
call cinst wget 7zip.commandline
call wget http://dl.nwjs.io/v%nwjs_version%/nwjs-v%nwjs_version%-win-ia32.zip
call 7z e -onw nwjs-v%nwjs_version%-win-ia32.zip
dir nw
set PATH=nw;%PATH%
call node-pre-gyp rebuild --runtime=node-webkit --target=%nwjs_version% --target_arch=ia32
call node-pre-gyp package testpackage --runtime=node-webkit --target=%nwjs_version% --target_arch=ia32
if not "%CM%" == "%CM:[publish binary]=%" call node-pre-gyp publish --msvs_version=2013 --runtime=node-webkit --target=%nwjs_version% --target_arch=ia32
goto end

:nwjs_64
call npm install nw-gyp
call cinst wget 7zip.commandline
call wget http://dl.nwjs.io/v%nwjs_version%/nwjs-v%nwjs_version%-win-x64.zip
call 7z e -onw nwjs-v%nwjs_version%-win-x64.zip
dir nw
set PATH=nw;%PATH%
call node-pre-gyp rebuild --runtime=node-webkit --target=%nwjs_version% --target_arch=x64
call node-pre-gyp package testpackage --runtime=node-webkit --target=%nwjs_version% --target_arch=x64
if not "%CM%" == "%CM:[publish binary]=%" call node-pre-gyp publish --msvs_version=2013 --runtime=node-webkit --target=%nwjs_version% --target_arch=x64
goto end

:end