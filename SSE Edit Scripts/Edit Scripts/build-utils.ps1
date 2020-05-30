$utils = Get-Content utils-src\_notation.pas | Out-String
$utils += "`n`n"
$utils += Get-Content utils-src\_globals.pas | Out-String
$utils += "`n`n"

$utils += Get-Content -Path utils-src\* -Exclude _notation.pas, _globals.pas | Out-String
$utils += "`n"
$utils += 'end.'
Set-Content SkyrimUtils.pas $utils
