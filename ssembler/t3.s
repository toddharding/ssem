; Calculate points on a parabola using a difference equation, and draw the parabola. By Magnus Olsson, Sweden.
; CI: 0
; PI: JMP 0               ; 0
; ACC: 0

JMP 0               ; 0
LDN 29              ; 16413
SUB 29              ; 32797
STO 29              ; 24605
CMP                 ; 49152
STP                 ; 57344
LDN 29              ; 16413
STO 29              ; 24605
LDN 22              ; 16406
SUB 29              ; 32797
STO 30              ; 24606
LDN 30              ; 16414
STO 22              ; 24598
LDN 8               ; 16392
SUB 28              ; 32796
STO 30              ; 24606
LDN 30              ; 16414
STO 8               ; 24584
SUB 27              ; 32795
STO 12              ; 24588
LDN 28              ; 16412
SUB 26              ; 32794
STO 28              ; 24604
LDN 28              ; 16412
STO 28              ; 24604
JMP 31              ; 31
JMP 1               ; 1
NUM -8192           ; STP 
NUM -6              ; STP 
NUM 131072          ; JMP 0
JMP 0               ; 0
JMP 0               ; 0

