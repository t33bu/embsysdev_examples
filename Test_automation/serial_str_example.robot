*** Settings ***
Library   String
Library   SerialLibrary

*** Variables ***
${com}   	COM7
${baud} 	115200
${board}	nRF
${seq}      RYGX
${error}    -1X

*** Test Cases ***
Connect Serial
	Log To Console  Connecting to ${board}
	Add Port  ${com}  baudrate=${baud}  encoding=ascii
	Port Should Be Open  ${com}
	Reset Input Buffer
	Reset Output Buffer

Serial Led Control
	# lähetetään RYG ja lopetusmerkki X
	Write Data   ${seq}   encoding=ascii 
	Log To Console   Send sequence ${seq}

	# vastaanotetaan merkkijono kunnes lopetusmerkki X (58) 
	${read} =   Read Until   terminator=58   encoding=ascii 

	# konsolille näkyviin vastaanotettu merkkijono
	Log To Console   Received ${read}
	
	# vertaillaan merkkijonoa
	Should Be Equal As Strings   ${read}    ${error}
	Log To Console   Tested ${read} is same as ${error}

	# astetta hankalampi tehdä testaus numeroina
	# koska lopetusmerkki X pitää ensin poistaa merkkijonosta
	# tai vaihtaa lopetusmerkki esim \0
	# Should Be Equal As Integers   ${read}    -1
	
Disconnect Serial
	Log To Console  Disconnecting ${board}
	[TearDown]  Delete Port  ${com}


	
	
	

