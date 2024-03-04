.686
include ltl_funcs.inc
.code

randomLocSet proc adrObj: dword
;;- setting random location for Dmoot
; this function works by using the random function created earlier to get a random number
;between the X limit and d Y limit and put it inside the DMOOT that the method 
;recived from the "caller"
	mov ebx, adrObj
	;;;;;; x ;;;;;;
	invoke random, 40 , 880 
	mov [ebx+posX],eax
	;;;;;; y ;;;;;;
	invoke random, 45 , 500
	mov [ebx+posY], eax
ret
randomLocSet endp

move proc adrobj:dword
;; makes the Dmoot move
; by adding the drc which is the dimention of the dmott
; to the dmoot pos
	;;;; X POS ;;;;
	mov ebx, adrobj
	mov eax,[ebx+drcX]
	add [ebx+posX],eax ; adding to the pos drc to move
	;;;; Y POS ;;;;
	mov eax,[ebx+drcY]
	add [ebx+posY],eax; adding to the pos drc to move

	ret
move endp

keyboard proc objadr:dword
;; checking which key was pressed and according to that change the drc
; using change_drc function 
	mov ebx, objadr
	invoke change_drc,GL,drcX,-30 ; left key
	invoke change_drc,GR,drcX,30 ; right key
	invoke change_drc,GU,drcY,-30 ; up key
	invoke change_drc,GD,drcY,30 ; down key
	ret
keyboard endp

setDmootPoints proc adrObj:DWORD 
;; finding current center point of the Dmoot
; by using ltl_setDmootPoints func
	mov ebx,adrObj
	mov edx,ebx
	invoke ltl_setDmootPoints,0
	invoke ltl_setDmootPoints,4
ret
setDmootPoints endp

collision_Apple proc 
;; check if there is collision between snake and apple
; using colission_Dmoot function between snake and apple
; if in esi, 1 then increase the player points and set a 
; new random location for the apple
	mov esi,0
	invoke collision_Dmoot,offset apple, offset snake; checks collision between Dmoots return esi=1 is colide
	cmp esi,1
	jne returnu
		inc gameInfo.points; increase the points player got so far
		invoke randomLocSet, offset apple ; setting new random location
	returnu:
ret
collision_Apple endp

colission_Bomb_snake proc
;; colission between the bomb and the head of the snake
; this function checks colision between bomb and snake using  
; collision_Dmoot func ' if collision_Dmoot returns 1 in esi
; set new random location and take 1 life off
	mov esi,0
	invoke collision_Dmoot,offset bomb, offset snake ; checkin colission
	cmp esi,1
	jne finish
		inc gameInfo.lost ; take life
		invoke randomLocSet, offset bomb ; new random location settings
	finish:
ret
colission_Bomb_snake endp

SnakeBody proc
;; this function sets the body pos
;this function works *once in a while*, not every time 1/50
; it sets the pos of the first body to the pos of the head 
; the reason for the slow by 1/50 is to slow the movment of the snake 
; else its too fast and you cant play
	cmp gameInfo.points,1
	jge firstBody
retutu:
	cmp gameInfo.points,1 ;if there is only one
	jle finish
	mov ebx, offset body
	mov eax, DmootLength
	mul gameInfo.points ; acourding to the points
	sub eax, DmootLength
	add ebx, eax
	add ebx,posX
	redo:
		;;;;;;;;;; set X ;;;;;;;;;;;;
		mov ecx, [ebx-DmootLength]
		mov [ebx],ecx
		;;;;;;;;;; set Y ;;;;;;;;;;;;
		add ebx,4
		mov ecx, [ebx-DmootLength]
		mov [ebx],ecx
		;; loop cmp
		mov edx, offset body
		add edx, posY
		add edx, DmootLength
		cmp ebx ,edx 
		jle finish
		sub ebx, 4
		sub ebx, DmootLength
	jmp redo

	finish:
ret
	firstBody: ; for the first one
		mov ebx, offset snake
		mov eax, offset body

		mov ecx, [ebx+posX]
		mov [eax+posX], ecx

		mov ecx, [ebx+posY]
		mov [eax+posY],ecx

	jmp retutu
SnakeBody endp

getObject proc index:dword, dmootLength:dword, arrAd:dword
;; getting a specific object from an array of objects
	mov eax, index ; eax->index of dmoot in array
	mov ebx,dmootLength ; ebx-> the length of dmoot struct
	mul ebx ; eax*ebx
	add eax,arrAd ; inital offset of dmoot array
	mov ebx,eax ; now in  abx the adrees of dmoot in array
ret
getObject endp 

limitCheck proc adrObj: dword
;; this function checks if the dmoot reached the lim and if so, prevent the dmoot the go over the lim
; using defrent functions for up,left and down,right because"
; right and down are positive so the cmp is: jle
; up and left are negative so the cmp is: jge
; 0 or 4 repesente x or y
;x=0. y=4
	mov esi,0
	invoke limitCheckDownRight, adrObj, limR ,0
	invoke limitCheckDownRight , adrObj , limD , 4
	invoke limitCheckUpLeft , adrObj , limL , 0
	invoke limitCheckUpLeft , adrObj , limU , 4
ret	
limitCheck endp

selfCollision proc 
;; this function checks if the snake touced his own tale an if he did his life is down by 1 
; the checking starts only from the 4th body, because the first bodys suposed to touch
; the head at all times,  this is why this functuin works only if the player earned more then 4 
; point=> 4 bodys
	cmp gameInfo.points,4
	jle finish

	mov eax, DmootLength
	mul gameInfo.points
	mov ebx, offset body
	add ebx, eax
	again:
		sub ebx, DmootLength
		;;; loop conditions
		mov ecx, offset body
		mov eax, DmootLength
		imul eax,3
		add ecx, eax
		cmp ecx,ebx
		jge finish

		mov esi, 0
			push ebx
		invoke collision_Dmoot, ebx, offset snake
			pop ebx
		cmp esi,1
		je losttt
		
	jmp again

	finish:
ret
	losttt:
		cmp slowSelfCol, 0
		jne slowSelfColl
			inc gameInfo.lost
			mov slowSelfCol,100
ret
	slowSelfColl:
		dec slowSelfCol
ret
selfCollision endp

bodySetCenterPoint proc
;; this function sets the center point of the body so we could do colissions
;also usung king of "for" loop to go over all the bodys thats in the game 
;(I know how is in the game and how is not by the points in the gameData struct)
;and setting there center location using the setDmootPoints from before
	cmp gameInfo.points,1
	jge firstBody
retutu:
	cmp gameInfo.points,1
	jle finish
	mov ebx, offset body
	mov eax, DmootLength
	mul gameInfo.points
	sub eax, DmootLength
	add ebx, eax
	redo:
		invoke setDmootPoints, ebx
		sub ebx, DmootLength
		cmp ebx, offset body
		jle finish

	jmp redo
	finish:
ret
	firstBody:
		invoke setDmootPoints, offset body
	jmp retutu
bodySetCenterPoint endp

startCheck proc  
;; checking if the user pressed "x" and started the game
; if "x" is pressed put 1 in gameInfo.started so i 
;could know the game started
; and deturmening the start time
	invoke GetAsyncKeyState,VK_X
	cmp eax,0
	jne started

ret

	started:
		;; setting the starting time of the game
		invoke setTimeTo,offset timeI.startTime
		; so I could know game Started
		mov gameInfo.started,1 
		
ret
startCheck endp

restart proc
;; restarting the game proc
; chcking if R key was pressed using function from msdn
;if it was pressed restart all the counters and values that chnaged
;during the game
	invoke GetAsyncKeyState,VK_R
	cmp eax,0; if R key pressed call te ltl restart function
	je notRestarted
	invoke ltl_restart
notRestarted:
ret
restart endp

CheckWin proc
;; checking if the player won
; he won if he reached 20 points, and if he did put
; in gameInfo.won, 1
	mov eax,20 ; 20 points to win
	cmp gameInfo.points, eax; reached 20 points?
	jge wonn

ret
	wonn:
		mov gameInfo.won, 1 ; put in won 1 so i could know he won
		mov gameInfo.lost, 0 ; so the things in the backround of the screen wont affect the screen

ret
CheckWin endp

drawBoom proc
;; this function shows an explosion before the lost page
; works 1/1000 laps
	cmp changeLost, 1000 ; delay counter so we could see the boom 
	je lostttt ; if delay over jmp to the lost screen
	invoke drd_imageDraw, offset boom.info, boom.pos.x, boom.pos.y ; boom
	inc changeLost ; increase delay
ret
	lostttt:
		invoke drd_imageDraw, offset gameOver.info, gameOver.pos.x, gameOver.pos.y
ret
drawBoom endp

drawLive proc
;; drawing the amount of lives (hearts) on the screen acourding to the amount 
;; of times the player died
; this function determens how many times to draw the hearts, and does so
	mov edi,0
	mov edx, heartAmount ;3
	retau:
			push edx

			cmp edx, gameInfo.lost
			je flipi

			invoke getObject,edi, DmootLength, offset heart ; object offet in ebx
			cmp edi,0
			je notG

			mov eax, edi
			mov ecx, dimenX
			mul ecx
			add [ebx+posX], eax
			invoke drd_imageDraw, offset heart.info, eax, heart.pos.y
			jmp G

			notG:
			invoke drd_imageDraw, offset heart.info, heart.pos.x, heart.pos.y ; first one in X dimen

			G:
		inc edi
		pop edx
		dec edx
		cmp edi, heartAmount
		jne retau

ret
	flipi:
		pop edx
	ret
drawLive endp

bodyDraw proc
;; draw the body after head/body before
; function checks how many bodys need to be drawen
; and does so acourding to the loction setted in 
; the SnakeBody proc
	cmp gameInfo.points,1
	jge firstBody
retutu:
	cmp gameInfo.points,1
	jle finish
	mov ebx, offset body
	mov eax, DmootLength
	mul gameInfo.points
	sub eax, DmootLength
	add ebx, eax
	add ebx,posX
	redo:
		;;;;;;;;;; get X ;;;;;;;;;;;;
		mov ecx, [ebx]
		;;;;;;;;;; get Y ;;;;;;;;;;;;
		add ebx,4
		mov edx, [ebx]
		push ebx
		;;;;;;;;;; draw ;;;;;;;;;;;;;
		invoke drd_imageDraw, offset body.info, ecx, edx ; draw
		;; loop cmp
		mov edx, offset body
		add edx, posY
		add edx, DmootLength
		pop ebx
		cmp ebx ,edx
		jle finish
		sub ebx, 4
		sub ebx, DmootLength
	jmp redo

	finish:
ret
	firstBody: ; private case for first body
		invoke drd_imageDraw, offset body.info, body.pos.x, body.pos.y
	jmp retutu

ret
bodyDraw endp

checkTimeEnd proc
;; checking if the time of the game is finished
; checking by comparing the start time of the game- determened in the startCheck fun
; to the LapTime - dtermened every lap in the update function
; if the start time "sub" ths lap time is greater than 5min=300sec
; put in timeI.timeEnd,1 so I could know time ended
	mov eax, timeI.LapTime ; eax=lap
	mov ebx, timeI.startTime ; ebx - start
	sub eax,ebx ; lap-start
	mov ebx, 300 ; 5 min is 300 in sec
	cmp ebx,eax
	jle enddd
ret
	enddd:
		mov timeI.timeEnd,1; so I could know time ended
ret
checkTimeEnd endp