#SSEMbler

##Manchester Small-Scale Experimental Machine assembler

###Example usage:
```bash
./SSEMbler t1.s 
./SSEMbler <filename>
```

On successful completion SSEMbler will have created a “binary.ssem” file copy this to the directory of the simulator and run via typing ./SSEM -f binary.ssem 

Program includes 4 example assembly files, these are named t1-t4.s, t1 if the default add two numbers together function 
t2-t4 are translated from example applications at http://www.davidsharp.com/baby/ 
