# GameMap
Game Map Assignment Challenge

## Introduction
This project was created to solve the challenge raised in the ``assignment`` folder. 
###Features
The project enables the following functionality:<br>
1) Create Random Game Map <br>
2) Serialise the map into six different formats<br>
3) Write the serialised data to file<br>
4) Read files in<br>
5) Deserialise the six formats into maps<br>
6) Find optimal path from entrance to exit (if one exists)<br>

###Running the Code
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

###Serialization
Further, the code will output the randomly generated game map into six different serialized formats, within the ``dataFiles`` folder, the formats are:<br>
* ```mapBinary.dat``` - Plain binary, where the height and width take 64-bits each, followed by 8-bits per element for a complete 2D array, using the values as defined above (ROOM_TYPE)
* ```mapCompressedBinary.dat``` - First two 64-bits are for height and width of whole map, followed by 8-bit element and count of number of times a 16-bit combo is repeated (which can happen a lot for eUNDEFINED spaces). This is similiar to applying Run-length encoding  
* ```mapBinaryPosition.dat``` - First two 64-bits are for height and width of whole map, followed by 8-bit element and its absolute x and y position on the map - thus dropping all the unused eUNDEFINED.
* ```mapString.dat``` - Plain string characters, where the height and widht and seperated by "(", "," and ")", followed by the complete 2D array values (as per ROOM_TYPE)
* ```mapStringCompressed.txt``` - Starts with height and widht seperated by "(", "," and ")", followed by ROOM_TYPE and its sequential count in brackets "(" and ")". This is similiar to run-length encoding
* ```mapStringPosition.txt``` - Starts with height and widht seperated by "(", "," and ")", followed by ROOM_TYPE and its absolute position seperated by "(", "," and ")" - thus dropping all the unused eUNDEFINED.<br>

###Map Data Structures
Two map data structures where used, namely: <br>
* ``2D Array`` - this is fine for small maps, but will be a concern for large maps as a lot of space is potentially used for undefined space <br>
* ``Graph`` - each room is linked to its neighbours in typical graph form.  <br>

###Path Finder
Depth-First Search was applied to obtain the list of valid routes from the entrance to the exits. The best, in terms of total score, was picked from the list of valid routes. Similiarly Breath-First Search could have been used. 

###Test Suite
A Test suite was created that basically runs the following sequence of tests:<br>
1) create random map<br>
2) display map <br>
3) serialize map to plain string and save to file<br>
4) serialize map to plain binary and save to file<br>
5) serialize map to plain string and save to file<br>
6) serialize map to plain binary and save to file<br>
7) serialize map to plain string and save to file<br>
8) serialize map to plain binary and save to file<br>
9-14) read-in each file and convert back to map<br>
15) find optimal path<br>
16) create graph map<br>

## Code Design

The intention, while writing the code, was to show case patterns that can be utilized, within the giving content. Design patterns used, included: <br>
* ``Factory``
* ``Interface``
* ``Singleton``
* ``Receiver``

Further notes on the code: <br>
* a logging header was integrated, from a third party, to make it easier to manage the debug levels, which can be controlled with the ``LOG_LEVEL`` #define. 
* The ``unique_ptr`` was also used.
* const was used to ensure functions and parameters can not work outside their remit

## Discussion on Code
###In which area(s) is the code deficient?
The following needs to be looked at: <br>
* unhappy paths - unit testing should be implemented to expose elements like the following: <br>
a) upper and lower bound values<br>
b) negative values<br>
c) making calls out of sequence<br>
d) ingesting bad data (wrong format, missing header, longer than defined in header, etc) <br>
* namespaces & code seperation - ideally the classes should be grouped into their own namespaces to help scope the data and avoid future conflicts
* use the Point struct instead of using a pair. Point can be futher extended if needed, like working in 3D space, but a pair is very limited
* threads - ideally the receiver and test suite should run in their own thread context. Due to time limitations this was not implemented
* implement test tool to compare serialized and deserialized maps automatically
* error handling - currently the error handling is very basic, expections should be thrown and handled 

###What would you change and why?
The following changes are at the front:
* GameMap2DArray and GameMapGraph classes shared duplicate code, which should be moved into the parent. 
* DataConverter.cpp - there is duplicate code for the different file formats, especially around header parsing. This should be moved into a shared function(s)

###How close do you think this is to what you would call "perfect" code, on a scale of 1 to 10
I think the structure of the good is great (close to 10), due to the following reasons:
* using the factory makes is easy to extend supported compression formats and allow for dependency inversion by obscuring the low-level implementation from the feature user
* seperating the implementation of the different key parts (serialization, File IO, game maps, path finding) using Interfaces makes it easy to integrate the code for external use - good encapsulation & re-use
* the receiver singleton seperates the execution layer from the requested client, this allows for good low coupling between components, as interaction goes through one, well defined, part
* High cohesion is demonstrated by the relationship between the PathFinder and GameMap objects, as the PathFinder lifespan is tied to a 1:1 lifespan of a GameMap object

Testability is pretty good as well, as demonstrated by the TestSuite class, maybe an 8.

Coding style and convention are good in terms of variable, function and class names. The use of const is also well utilized. The code does require more comments and should go through Doxygen testing before completing the code. 

Hardening of the code has not been much of a focus yet and will only get a 5 now.

To conclude then, I will rate the code around 7-8, with the note that this code will only be completed once hardening as improved. 

## Future Work
The following features can be added:
* JSON file import and export
* Breath-First Search for 2D Array
* DFS and BFS For Graph
