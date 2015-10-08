# GameMap
Game Map Assignment Challenge

## Introduction
This project was created to solve the challenge raised in the ``assignment`` folder. The project enables the following functionality:<br>
1) Create Random Game Map <br>
2) Serialise the map into six different formats<br>
3) Write the serialised data to file<br>
4) Read files in<br>
5) Deserialise the six formats into maps<br>
6) Find optimal path from entrance to exit (if one exists)<br>

Running the code in linux, basically requires to run ./GameMap. The output will be the display of a 10x10 randomly generated game map, followed by the optimal path from entrance to exit. If no exit exists, then the output will display this accordingly.<br>
The representation in the map is as follows:<br>
enum ROOM_TYPE{<br>
    eEMPTY = 0,<br>
    eENTRANCE = 1,<br>
    eMONSTER = 2,<br>
    eTREASURE = 3,<br>
    eEXIT = 4,<br>
    eUNDEFINED = 5,<br>
    eTOTAL = 6<br>
};<br>
``Note: eTOTAL is not used in the presentation.``<br>

Further, the code will output the randomly generated game map into six different serialized formats, within the ``dataFiles`` folder, the formats are:<br>
* ```mapBinary.dat``` - Plain binary, where the height and width take 64-bits each, followed by 8-bits per element for a complete 2D array, using the values as defined above (ROOM_TYPE)
* ```mapCompressedBinary.dat``` - First two 64-bits are for height and width of whole map, followed by 8-bit element and count of number of times a 16-bit combo is repeated (which can happen a lot for eUNDEFINED spaces). This is similiar to applying Run-length encoding  
* ```mapBinaryPosition.dat``` - First two 64-bits are for height and width of whole map, followed by 8-bit element and its absolute x and y position on the map - thus dropping all the unused eUNDEFINED.
* ```mapString.dat``` - Plain string characters, where the height and widht and seperated by "(", "," and ")", followed by the complete 2D array values (as per ROOM_TYPE)
* ```mapStringCompressed.txt``` - Starts with height and widht seperated by "(", "," and ")", followed by ROOM_TYPE and its sequential count in brackets "(" and ")". This is similiar to run-length encoding
* ```mapStringPosition.txt``` - Starts with height and widht seperated by "(", "," and ")", followed by ROOM_TYPE and its absolute position seperated by "(", "," and ")" - thus dropping all the unused eUNDEFINED.

## Code Design Overview

## Discussion on Code
###In which area(s) is the code deficient?
###What would you change and why?
###How close do you think this is to what you would call "perfect" code, on a scale of 1 to 10

