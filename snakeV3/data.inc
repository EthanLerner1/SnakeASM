.const
; those constants represent constant info about the location of the data in the struct
posX = 20
posY = 24

drcX = 28
drcY = 32

dimenX = 36
dimenY = 40

GL = 44
GR = 48
GU = 52
GD = 56

CENTERx = 60
CENTERy = 64

centerX = 68
centerY = 72

rad = 76

bodyAmount = 150
DmootLength = 80
heartAmount = 3

limR = 896
limL = 35
limU = 45
limD = 505

.data

;;;;; counters ;;;;;
movemetSlow dword 0 ; slow the movment of the Dmoot
slowBY dword 60 ; cmp the movemetSlow to
changeLost dword 0 ; wait interval between the boom and lost
slowSelfCol dword 100 ; after he toached himself, wait so hes points will go down only by one

;;;;;; structs ;;;;;;;
;; time 
TimeData struct
	startTime DWORD 0 ; the time when game started
	LapTime dword 0 ; time at every lap
	timeEnd dword 0 ; has the time ended (5min=300sec)
TimeData ends

timeI TimeData<> ; the struct I made for time info
timeStruct SYSTEMTIME<> ; struct from msdn

;; game info
gameData struct
	points dword 0 ; how many times the snake reached the apple
	won dword 0  ; did he won
	lost dword 0 ; he lost
	tochedWall dword 0 ; did he touched the wall
	started dword 0 ; has the game started yet?
gameData ends

gameInfo gameData <>

;;keys
KEYS struct
	left dword ? ; 44 
	right dword ? ; 48
	up dword ? ; 52
	down dword ? ; 56
KEYS ends

;;dmoots
Dmoot struct
	info Img <> ; struct from drd
	pos POINT <> ; 20
	drc POINT <> ; 28
	dimentions POINT <> ; 36
	keys KEYS <> ; 44 struct before
	CENTER POINT <30,30> ; 60
	center POINT <> ; 68
	rad dword 20 ; 76
Dmoot ends

;;;;; Dmoots ;;;;;;;;
snake Dmoot <<>,<>,<>,<>,<VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN>,<30,30>,<>,30>
apple Dmoot <<>,<>,<>,<>,<>,<18,13>,<>,15>
body Dmoot 150 dup (<>)
background Dmoot <>
gameOver Dmoot <<>,<100,100>>
win Dmoot <>
bomb Dmoot <<>,<>,<>,<>,<>,<18,25>,<>,20>
heart Dmoot 3 dup (<<>,<0,0>,<>,<33,30>,<>,<>,<>,<>>)
startScreen Dmoot <>
boom Dmoot<>
timeScreen Dmoot<>

;;;;; hardDisk locations

hdlocBack byte "img/background.png",0
hdlocHead byte "img/head.png",0
hdlocBody byte "img/body.png",0
hdlocApple byte "img/apple.png",0
hdlocGameOver byte "img/GameOver.png",0
hdlocGameWon byte "img/win.jpg",0
hdlocBomb byte "img/bomb.png",0
hdlocHeart byte "img/heart.png",0
hdlocStart byte "img/startScreen.png",0
hdlocBoom byte "img/boom.jpg",0
hdlocTIme byte "img/TIME.png",0
hdlocdemon byte "img/demon.png",0

