# Install libwebp on Windows.
# Downloads and installs the specified prebuilt version from Google's servers.

$LibVer = $args[0]
$LibArchive = "libwebp-$LibVer-windows-x64"

#Download and extract the release package
$LibArchiveURL = "https://storage.googleapis.com/downloads.webmproject.org/releases/webp/$LibArchive.zip"
(Invoke-WebRequest $LibArchiveURL).Content | Expand-Archive -DestinationPath $Env:TEMP

#Add the directory to the PATH
Add-Content $Env:GITHUB_PATH "$Env:TEMP\$LibArchive\lib"
Add-Content $Env:GITHUB_PATH "$Env:TEMP\$LibArchive\include"
