# Install libwebp on Windows.
# Downloads and installs the specified prebuilt version from Google's servers.

$LibVer = $args[0]
$LibArchive = "libwebp-$LibVer-windows-x64"

#Download and extract the release package
$DownloadPath = New-TemporaryFile
$LibArchiveURL = "https://storage.googleapis.com/downloads.webmproject.org/releases/webp/$LibArchive.zip"
Invoke-WebRequest -OutFile $DownloadPath $LibArchiveURL
Expand-Archive -Path $DownloadPath -DestinationPath $Env:TEMP
Remove-Item $DownloadPath

#Add the directory to the search paths
Add-Content $Env:GITHUB_ENV "LIBWEBP_DIR=$Env:TEMP\$LibArchive"
Add-Content $Env:GITHUB_ENV "INCLUDE=$Env:TEMP\$LibArchive\include"
Add-Content $Env:GITHUB_ENV "LIB=$Env:TEMP\$LibArchive\lib"
