include drd.inc
include funcs.inc
.code
init proc
	invoke drd_init,1000,600,0
	;; loading img to ram and setting transparent using drd to all picture
		;;;;;;;;;;;;;;;; background ;;;;;;;;;;;;;;
		invoke drd_imageLoadFile, offset hdlocBack,offset background.info
		invoke drd_imageSetTransparent,offset background.info,0000000h

		;;;;;;;;;;;;;; HEAD ;;;;;;;;;;;;;;;;;;
		invoke drd_imageLoadFile, offset hdlocHead,offset snake.info
		invoke drd_imageSetTransparent,offset snake.info,0FFFFFFh

		;;;;;;;;;;;;; BODY ;;;;;;;;;;;;;;;;;;
		;this is kinf of a "for" lopp in asm I wrote
		; for (int edi=0 ; edi!=bodyAmount ; edi++)
		mov edi,0
		again:
			invoke getObject,edi, DmootLength, offset body
			push ebx
			invoke drd_imageLoadFile, offset hdlocBody, ebx
			pop ebx
			invoke drd_imageSetTransparent, ebx , 0FFFFFFh
		inc edi
		cmp edi, bodyAmount
		jne again
		;;;;;;;;;;;;; APPLE ;;;;;;;;;;;;;;
		invoke drd_imageLoadFile, offset hdlocApple,offset apple.info
		invoke drd_imageSetTransparent,offset apple.info,0FFFFFFh

		;;;;;;;;;;;; heart ;;;;;;;;;;;;;;;
		;this is kinf of a "for" lopp in asm I wrote
		; for (int edi=0 ; edi!=bodyAmount ; edi++)
		mov edi,0
		retau:
			invoke getObject,edi, DmootLength, offset heart
			push ebx
			invoke drd_imageLoadFile, offset hdlocHeart, ebx
			pop ebx
			invoke drd_imageSetTransparent, ebx , 0FFFFFFh
				cmp edi, 1
				jne skip
				mov eax, [ebx+dimenX]
				add [ebx+posX],eax
			skip:
		inc edi
		cmp edi, heartAmount
		jne retau
		;;;;;;;;;;;;; BOMB ;;;;;;;;;;;;;;;;;;;;
		invoke drd_imageLoadFile, offset hdlocBomb,offset bomb.info
		invoke drd_imageSetTransparent,offset bomb.info,0FFFFFFh

		;;;;;;;;;;;;;  game over ;;;;;;;;;;;;;;;;;;;;;;
		invoke drd_imageLoadFile, offset hdlocGameOver, offset gameOver.info
		invoke drd_imageSetTransparent,offset gameOver.info,0FFFFFFh

		 ;;;;;;;;;;; win ;;;;;;;;;;;;;;;;;;;;;
		 invoke drd_imageLoadFile, offset hdlocGameWon, offset win
		 invoke drd_imageSetTransparent,offset win.info,0000000h

		 ;;;;;;;;;;; startScreen ;;;;;;;;;;;;;
		 invoke drd_imageLoadFile, offset hdlocStart, offset startScreen

		 ;;;;;;;;;;;; boom ;;;;;;;;;;;;;;;;;;;
		 invoke drd_imageLoadFile, offset hdlocBoom, offset boom

		 ;;;;;;;;;;;; time ended screen ;;;;;;;;;;;
		 invoke drd_imageLoadFile, offset hdlocTIme, offset timeScreen


	 ;;;;;;;;;; setting first random location for Dmoots ;;;;;;;;;;
	 invoke randomLocSet, offset apple
	 invoke randomLocSet, offset snake
	 invoke randomLocSet, offset bomb

	
ret
init endp


update proc

invoke startCheck
cmp gameInfo.started,0
je GNS
	;;;;;;;;;;; stting Dmoots center points ;;;;;;;;;;;;;;;
	invoke setDmootPoints, offset snake
	invoke setDmootPoints, offset apple
	invoke setDmootPoints, offset bomb


	invoke keyboard, offset snake;; changing drc of the head according to the key that is pressed
	invoke limitCheck, offset snake ; checking and preventing the head to pass the lim 

	
	
	;;;;;;;;;;; movment ;;;;;;;;;;;;;;;
	mov eax,slowBY
	cmp movemetSlow,eax ;my own sleep function- less bugs
	jne LineAfterMove
		invoke SnakeBody ; setting pos
		invoke bodySetCenterPoint ; setting center pos
		invoke move , offset snake ; moving the head- body folows head
		mov movemetSlow,0
	LineAfterMove:
	inc movemetSlow

	;;;;;;;;;;;;;;;;;;;; colissions ;;;;;;;;;;;;;;;;;;;;;;;;;
	invoke collision_Apple ; colisiion between apple and head
	invoke colission_Bomb_snake ; colission between bomb and snake
	invoke selfCollision ; checking colisiion between body and head 

	;;;;;;;;;;;;;;;;;; check remain time ;;;;;;;;;;;;;;
	invoke setTimeTo,offset timeI.LapTime ;; setting current lap time
	invoke checkTimeEnd ;; checking if reached maximum time

	;;;;;;;;;;;;;;;;;;; restart & end ;;;;;;;;;;;;;;;;;;;;;
	invoke restart ; restarting the game if R is pressed
	invoke CheckWin ; checking if player reached point amount
GNS:
ret
update endp


draw proc
	invoke drd_pixelsClear,0

	cmp gameInfo.started,0 ; game started yet
	je GNSD

	cmp timeI.timeEnd,0 ; timeOut?
	jnz timeOut

	cmp gameInfo.lost,3 ; lost?
	jge losttt

	cmp gameInfo.won, 1 ; won?
	je wonn

	invoke drd_imageDraw,offset background.info,background.pos.x,background.pos.y
	invoke drd_imageDraw,offset snake.info,snake.pos.x,snake.pos.y
	invoke drd_imageDraw,offset apple.info,apple.pos.x,apple.pos.y
	invoke drd_imageDraw, offset bomb.info, bomb.pos.x, bomb.pos.y
	invoke bodyDraw
	invoke drawLive

flip:
;; always need to happen
	invoke drd_processMessages
	invoke drd_flip
ret
	losttt:
		invoke drawBoom
		jmp flip

		GNSD:
			invoke drd_imageDraw, offset startScreen.info, startScreen.pos.x, startScreen.pos.y ; drawing start screen and directions
			jmp flip

		wonn:
			invoke drd_imageDraw, offset win.info, win.pos.x, win.pos.y
			jmp flip

		timeOut:
			invoke drd_imageDraw, offset timeScreen.info, timeScreen.pos.x, timeScreen.pos.y
			jmp flip
draw endp
