#SSEM

##Manchester Small-Scale Experimental Machine emulator

##Example usage:

./SSEM -f baby.text -s on 
 What the command line arguments mean: 
-f = the filename that contains the program to load -s = step through execution mode, can be either on or off

###What happens on execution? 

First the application creates a 
'''c++
vector<pair<string, string> >
''' 
called “ops”, then each of the command line arguments is parsed, and the valid options plus their value,  are then stored in a pair and added to the ops vector, then each of the arguments are executed in an if else statement, and the corresponding values are set. 
Next the file specified by filename is loaded and pushed into the data vector, if no filename is specified, a default program will be loaded. 
An instance of the SSEM class is then instantiated, powered on, the data vector is loaded into the SSEM, and the SSEM starts the fetch execute cycle. 
The SSEM class contains a Clock class that sends event signals when it is time for a new cycle, inturn the SSEM class then tells the CPU and VDU to update. 
The CPU phase is a switch case statement for halt, fetch, decode, and execute.  
During the fetch phase the CI is incremented and the PI is set to the value stored at the address of CI. 
During the decode phase, the decoded instruction and the decoded line number are bit shifted out of the PI. 
During the execute phase, a switch case statement is entered that contains all of the simulated instructions of the CPU, the relevant instruction is then executed. 
Finally the contents of the store and registers are sent to the VDU to be printed to the screen.  

#SSEMbler

##Manchester Small-Scale Experimental Machine assembler

###Example usage: 
./SSEMbler t1.s 
./SSEMbler <filename>

On successful completion SSEMbler will have created a “binary.ssem” file copy this to the directory of the simulator and run via typing ./SSEM -f binary.ssem 

Program includes 4 example assembly files, these are named t1-t4.s, t1 if the default add two numbers together function 
t2-t4 are translated from example applications at http://www.davidsharp.com/baby/ 
