!include LogicLib.nsh
!include MUI2.nsh
!include FileFunc.nsh

Name "InstallerDemoPKI"
OutFile "InstallerDemoPKISetup.exe" 

  !define MUI_WELCOMEPAGE_TEXT "Menu pemandu ini akan memandu memulai proses instalasi app InstallerDemoPKISetup Opensource.$\r$\n$\r$\n$\r$\nClick Next to continue."
  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES 
  !insertmacro MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES 
  
  
  !insertmacro MUI_LANGUAGE "English"

  !define UNINST_KEY \
  "Software\Microsoft\Windows\CurrentVersion\Uninstall\InstallerDemoPKI1"
Section
  SetOutPath "$InstDir\pki"
  File /r "app_pki"
  WriteRegStr HKLM  "Software\InstallerDemoPKI" "" "$InstDir\pki"
  WriteUninstaller "$InstDir\pki\uninstall.exe"
  CreateShortCut "$SMPROGRAMS\InstallerDemoPKI_opensource.lnk" "$InstDir\pki\app_pki\PKItest1.exe"
  CreateShortcut "$DESKTOP\InstallerDemoPKI_opensource.lnk" "$InstDir\pki\app_pki\PKItest1.exe"
  WriteRegStr HKLM  "${UNINST_KEY}" "DisplayName" "InstallerDemoPKI Opensource"
  WriteRegStr HKLM  "${UNINST_KEY}" "UninstallString" "$\"$InstDir\pki\uninstall.exe$\""
  WriteRegStr HKLM  "${UNINST_KEY}" "Publisher" "Ridwan Abdul Hafidh Opensource TM"
  ${GetSize} "$InstDir" "/S=0K" $0 $1 $2
  IntFmt $0 "0x%08X" $0
  WriteRegDWORD HKLM  "${UNINST_KEY}" "EstimatedSize" "$0"

SectionEnd
 
  

Section "Uninstall"

  RMDir /r "$InstDir"
  Delete "$SMPROGRAMS\InstallerDemoPKI_opensource.lnk"
  Delete "$desktop\InstallerDemoPKI_opensource.lnk"
  DeleteRegKey /ifempty HKLM "Software\InstallerDemoPKI1"
  DeleteRegKey HKLM "${UNINST_KEY}"

SectionEnd
