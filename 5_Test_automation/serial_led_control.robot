*** Settings ***
Library  SerialLibrary

*** Variables ***
${com}   MUUTA_TAHAN_OMA
${board}   nRF5340
${ledon}   1
${ledoff}  0

*** Test Cases ***
Connect Serial
	Log To Console  Connecting to ${com} ${board}
	Add Port  ${com}  baudrate=115200   encoding=ascii
	Port Should Be Open  ${com}
	Reset Input Buffer
	Reset Output Buffer

Serial Led ON
	Send Led On
	${read} =   Read Until  terminator=0  encoding=ascii
	Log To Console  Received ${read}
	Should Be Equal As Integers  ${read}  ${ledon}
	#Should Be Equal As Strings  ${read}  ${ledon}

Sleep 3s
	Log To Console  Sleeping for 3s
	Sleep  3s

Serial Led OFF
	Send Led Off
	${read} =  Read Until  terminator=0  encoding=ascii
	Log To Console  Received ${read}
	Should Be Equal As Integers  ${read}  ${ledoff}
	#Should Be Equal As Strings  ${read}  ${ledoff}

Disconnect Serial
	Log To Console  Disconnecting ${board}
	[TearDown]  Delete Port  ${com}

*** Keywords ***
Send Led On
	Log To Console  Send Led ON
	Write Data  ${ledon}  encoding=ascii
	Log To Console  Send Command ${ledon} to ${com}

Send Led Off
	Log To Console  Send Led OFF
	Write Data  ${ledoff}  encoding=ascii
	Log To Console  Send Command ${ledoff} to ${com}

	
	
	

