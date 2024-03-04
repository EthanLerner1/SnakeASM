.code

modulu proc, num1:dword, num2:dword
;creating mod "%" function 
	push edx
	; eax = num1 % num2
	mov eax, num1
	xor edx, edx
	mov ecx, num2
	div ecx ; edx = num1 % num2
	mov eax, edx

	pop edx
	ret
modulu endp

random proc, min:dword, max:dword
;; getting a random number between two numbers
;; returnd value in eax
	rdrand eax ;; from .686
	inc eax
	mov edx, max
	sub edx, min
	invoke modulu, eax, edx
	add eax, min ; random value

	ret
random endp

stop proc drc: dword
;; stop the Dmoot from moving on both X and Y directions
	mov edx,0
	mov ecx, drc
	mov [ebx+ecx],edx
ret
stop endp

change_drc proc key:dword,drc:dword,drcAdd:dword
;; changing the drc of the Dmoot according to the Key
	mov eax,ebx
	add eax,key
	invoke GetAsyncKeyState,[eax]
	cmp eax,0
	jne Tag_Go

ret

	Tag_Go:
		mov eax,drc
		mov ecx,drcAdd
		mov [ebx+eax],ecx
		;;;;; stop y ;;;;;
		cmp eax, drcX
		je stopY

		;;;;; stop x ;;;;;
		cmp eax, drcY
		je stopX
	stopY:
		invoke stop, drcY
ret

	stopX:
		invoke stop, drcX
ret
change_drc endp

collision_Dmoot proc adrObj1:DWORD ,adrObj2:DWORD 
;; check if there is collision and prevent over drawing and return 1 in esi if there is colission
	mov ebx,adrObj2
	mov eax,adrObj1
	mov ecx,[ebx+centerX] ;x2
	sub ecx,[eax+centerX] ;x2-x1
	mov edx,[ebx+centerY] ;y2
	sub edx,[eax+centerY] ;y2-y1
	imul ecx,ecx
	imul edx,edx
	add ecx,edx      ; ecx: dx^2+dy^2
	mov edx,[eax+rad] ; r1
	add edx,[ebx+rad] ; r1+r2
	imul edx,edx
	cmp ecx,edx
	jle TAG_collide

ret

TAG_collide:
	mov esi,1 ; so I could know in  the function that calls this 1 if there was a colission

ret
collision_Dmoot endp

ltl_setDmootPoints proc startT:DWORD
;; set center point XoY
	add edx,startT
	mov eax,edx
	add eax,CENTERx
	mov eax,[eax];CENTER X or Y
	mov ecx,edx
	add ecx,posX
	mov ecx,[ecx]; pos X or Y
	add ecx,eax ;pos+CENTER
	mov [edx+centerX],ecx ; mov to center

ret
ltl_setDmootPoints endp

limitCheckDownRight proc adrObj: dword, side: dword, xOy: dword
;; checking limit in right and down
;; right and down beacuse they are both positive
	mov ebx, adrObj 
	add ebx, xOy
	mov eax, [ebx+posX]
	cmp eax, side
	jge reachedLim

ret
	reachedLim:
		mov eax, -30
		mov [ebx+drcX],eax
		mov gameInfo.tochedWall,1
ret
limitCheckDownRight endp

limitCheckUpLeft proc adrObj: dword, side: dword, xOy: dword
;; checking limit in up and left
;; up and left because both are negative
	mov ebx, adrObj 
	add ebx, xOy
	mov eax, [ebx+posX]
	cmp eax, side
	jle reachedLim
ret
	reachedLim:
		mov eax, 30
		mov [ebx+drcX],eax
		mov gameInfo.tochedWall,1
ret
limitCheckUpLeft endp

randomLocSetForR proc adrObj: dword
;; setting random location for Dmoot in the restart function
	mov ebx, adrObj
	;;;;;; x ;;;;;;
	invoke random, 40 , 880
	mov [ebx+posX],eax
	;;;;;; y ;;;;;;
	invoke random, 45 , 500
	mov [ebx+posY], eax
ret
randomLocSetForR endp

ltl_restart proc
;; restarting all variables and counters changed throw
;; the game
	mov gameInfo.points, 0 ;poins
	mov gameInfo.won, 0 ; won
	mov gameInfo.lost, 0 ; lost
	mov gameInfo.tochedWall, 0 ; lim
	mov gameInfo.started, 0 ; start
	mov timeI.timeEnd,0 ; timeOver
	mov timeI.startTime,0 ; Start time
	mov timeI.LapTime,0 ; Lap time
	mov movemetSlow, 0 ; slow
	mov changeLost,0 ; boom delay
	mov slowBY,60 ; slow movment by
	mov snake.drc.x,0 ; drcX
	mov snake.drc.y, 0 ; drcY
	invoke randomLocSetForR, offset snake ; random loc snake
	invoke randomLocSetForR, offset bomb ; random loc bomb
	invoke randomLocSetForR, offset apple ; random loc apple
	
ret
ltl_restart endp

timeToDec proc,  hour:dword, min:dword, sec:dword 
;; transforms the hour and min to sec => eax = hour*3600 + min*60 + sec
; the result is inside eax
; this is a help function for setTimeTo
	mov eax, hour ; eax=hour
	mov ebx,3600 ; ebx=3600
	mul ebx ; eax*ebx = hour*3600 
	mov ebx, min ; ebx=minute
	mov ecx,60 ; ecx=60
	imul ebx,ecx ; ebx*ecx = min*60
	add eax, ebx ; eax+ebx = hour*3600 + min*60 
	mov ecx, sec ; ecx=secomds
	add eax,ecx ; eax+ecx = hour*3600 + min*60 + sec
ret
timeToDec endp

setTimeTo proc, lapOstart: dword
;; setting time to the time struct and to the lap or start time info
; using timeToDec function to load a dword into the struct and not a comlite
; time struct
	invoke GetSystemTime,offset timeStruct ; time struct contains hour,min,sec,milisec
	mov ebx, lapOstart ; is it start time or lap time?
	push ebx
	invoke timeToDec, timeStruct.wHour,timeStruct.wMinute,timeStruct.wSecond ; in eax there is a decimal value for the time
	pop ebx
	mov [ebx], eax 
ret
setTimeTo endp