.686

.code
Random proc max:dword;return a random number in eax between 0 and max
	Rdrand eax
	Xor edx,edx
	div max
	mov eax,edx
	ret 
Random endp

modulu proc first:DWORD, sec:DWORD ; returns x%y
	xor edx, edx
	mov eax,first
	div sec
	mov eax,edx ; eax contains the value of x%y
	ret
modulu endp 

isSpecialFunc proc ofstBreak:dword ; returns to eax 1 or 0 deepends if the break is special or not (in the corners)
	mov edi,ofstBreak
	checkSpecial:
		mov eax,70
		cmp [edi+20],eax;70
		je mightSpecial
		mov eax, 560
		cmp [edi+20],eax;560
		je mightSpecial
		mov eax,0
		cmp [edi+20],eax;0
		je onSide
		mov eax,630
		cmp [edi+20],eax;630
		je onSide
		

	mov eax,0;if isnt special
	ret

	mightSpecial:; x is 560 or 70 check for y 0 or 840
	mov eax,0
	cmp [edi+24],eax;0
	je isSpecial
	add eax,630
	cmp [edi+24],eax;630
	je isSpecial
	mov eax,0
	ret

	onSide:
	mov eax,0
	cmp [edi+24],eax;0
	je isSpecial
	add eax,70
	cmp [edi+24],eax;70
	je isSpecial
	mov eax,560
	cmp [edi+24],eax;560
	je isSpecial
	add eax,70
	cmp [edi+24],eax;630
	je isSpecial
	mov eax,0
	ret

	isSpecial:;if is special move to next block
	mov eax,1
	ret 
isSpecialFunc endp

distancePlayers proc ofst1:dword, ofst2:dword
	local temp: dword
	pusha
	mov eax, ofst1
	mov ebx, ofst2
	
	mov ecx,[eax+36] ;center x 1
	mov edx,[ebx+36] ;center x 2
	mov eax,[eax+40] ;center y 1
	mov ebx,[ebx+40] ;center y 2

	;dist x
	sub edx, ecx
	imul edx,edx

	;dist y
	sub ebx,eax
	imul ebx,ebx

	add ebx,edx
	mov eax,ebx
	mov temp,eax
	popa

	mov eax,temp

	ret
distancePlayers endp

dmootUpdateCD proc ofst:dword ; updates all cooldowns of players
	mov ebx,ofst
	mov eax,1
	sub [ebx+52],eax
	sub [ebx+56],eax

	mov eax,0
	cmp [ebx+56],eax
	je ResetBomb
	retResetBomb:
	
	ret

	ResetBomb:
	mov ecx,[ebx+72]
	mov eax,ecx
	mov [ebx+48],eax
	jmp retResetBomb
dmootUpdateCD endp

explosionBombs proc ofst:dword ;gets a bombs offset and bombs it (lunch 4 bullets)
	pusha
	mov esi, ofst
	mov ecx,ofst
	add esi,44;esi has first Bullet of the Bomb
		mov eax,0
		loopTheLoop:
			cmp eax,0
			je down
			retDown:

			cmp eax,1
			je left
			retLeft:

			cmp eax,2
			je up
			retUp:

			cmp eax,3
			je right
			retRight:

			mov edx,1
			mov [esi+48],edx;update to ISalive

			mov edx,[ecx+20]
			mov [esi+36],edx;update originX

			mov edx,[ecx+24]
			mov [esi+40],edx;update originY
	
			mov edx,1
			mov [esi+48],edx;update to ISalive

			add esi, BulletSize
			add eax,1
			cmp eax,4
		jl loopTheLoop
	popa
	ret

	down:
		mov edx,[ecx+20]
		mov [esi+20],edx;x to x

		mov edx,[ecx+24]
		;add edx,20
		mov [esi+24],edx;y to y
	jmp retDown

	left:
		mov edx,[ecx+20]
		;sub edx, 20
		mov [esi+20],edx;x to x

		mov edx,[ecx+24]
		mov [esi+24],edx;y to y
	jmp retLeft

	up:
		mov edx,[ecx+20]
		mov [esi+20],edx;x to x

		mov edx,[ecx+24]
		;sub edx,20
		mov [esi+24],edx;y to y
	jmp retUp

	right:
		mov edx,[ecx+20]
		;add edx, 20
		mov [esi+20],edx;x to x

		mov edx,[ecx+24]
		mov [esi+24],edx;y to y
	jmp retRight
explosionBombs endp

bombAllBombs proc ofst:dword ;bombs all bombs (calls for each one of bombs the exposionBombs func)
;	local bombsBombed:dword
;	mov bombsBombed,0
;	mov ebx,ofst
;	mov edi,BombArrStarts
;	add edi,ebx ;edi now holds the offset of first bomb in arr
;	;mov eax,[ebx+48]
;	;mov esi,0
;	;cmp eax,esi
;	;je resetBombRenewTimer
;	;retRenewBomb:
;	mov ebx,0
;	loopi:
;		mov eax, [edi+40];mov to eax if bomb is alive
;		mov ecx,0
;		cmp eax,ecx
;		je notAlive
;		;if IS alive invokeBOMB,  update to not alive
;		invoke explosionBombs,edi
;		mov eax,0
;		mov [edi+40],eax
;		
;		mov ecx,ofst
;		mov ecx,[ecx+48]
;		cmp ecx,0
;		je resetBombRenewTimer
;		retRenewBomb:
;		notAlive:
;		add ebx,1
;		add edi, BombSize
;		cmp ebx,MaxBombSize
;	jl loopi
;ret
;
;	resetBombRenewTimer:
;	mov edx, bombGainTime
;	mov ecx,ofst
;	mov [ecx+56],edx
;	jmp retRenewBomb
bombAllBombs endp

drawBreaks proc ;draws the breaks 
	mov edi, offset breakArr[0]
	mov ebx,0
	loopi:
		invoke isSpecialFunc,edi
		cmp eax,1
		je isSpecialLine
		retIsSpecialLine:

		mov ecx,0
		cmp [edi+40],ecx
		je notAlive
		invoke drd_imageDraw, edi,[edi+20],[edi+24]
		notAlive:
		add edi,BreakSize
		add ebx,1
		cmp ebx, lengthOfBreaksArr
	jl loopi
	ret	

	isSpecialLine:;if is special move to next block
	mov eax,0
	mov [edi+40],eax
	jmp retIsSpecialLine
drawBreaks endp

hadCollisionedWall proc ofst:dword;;has player colidided wall
	mov eax, ofst
	mov ebx, ofst
	mov ecx, [ebx+36] ;centerX
	mov edx, [ebx+ 44] ;rad
	cmp ecx,edx
	jle CLOSE_TO_WALLx1;CLOSE TO WALL X1
	mov ebx,700
	sub ebx,edx
	cmp ecx, ebx
	jge CLOSE_TO_WALLx2; CLOSE TO WALL X2
	mov ebx, ofst
	mov ecx, [ebx+40];centerY
	cmp ecx,edx
	jle CLOSE_TO_WALLy1;CLOSE TO WALL Y1
	mov ebx,700
	sub ebx,edx
	cmp ecx,ebx
	jge CLOSE_TO_WALLy2; CLOSE TO WALL Y2

	ret

	CLOSE_TO_WALLx1:
	mov ebx,1
	mov [eax+28],ebx
	ret
	CLOSE_TO_WALLx2:
	mov ebx,-1
	mov [eax+28],ebx
	ret
	CLOSE_TO_WALLy1:
	mov ebx,1
	mov[eax+32],ebx
	ret
	CLOSE_TO_WALLy2:
	mov ebx,-1
	mov [eax+32],ebx
	ret
hadCollisionedWall endp

hadCollisionedBreak proc offst:dword;checking for a collision with a break and acting to it
	mov hasColidedBreak,0
	mov ecx,0
	mov edi,offst
	mov ebx, offset breakArr[0]
	loopi:
		mov eax,0
		cmp [ebx+40],eax
		je deadBreak
		mov eax, [ebx+28];centerX break
		mov edx, [ebx+32];centerY break
		mov esi,[edi+36]
		sub esi,eax
		mov eax,esi
		imul eax,eax ;x*x in eax

		mov esi,[edi+40]
		sub esi,edx
		mov edx,esi
		imul edx,edx ;y*y in edx

		add eax,edx
		mov edx,0
		add edx,[ebx+36]
		add edx,[edi+44]
		imul edx,edx
		cmp eax,edx; Comparing betwin sum of radiuses and dist of centers
		jle areColiding
		returnAfterColision:
		deadBreak:
		add ecx,1
		add ebx,BreakSize
		cmp ecx,lengthOfBreaksArr
		jl loopi
	ret
		areColiding:;are coliding, now checking from which diraction by comparing x,Y1
		;edi has offset of player
		;ebx has offset of break
		mov hasColidedBreak,1
		mov eax, [edi+36];player centerX
		mov edi,[edi+40];player centerY
		mov edx,[ebx+20];break posX
		mov esi,[ebx+24];break posY
		
		;lets get SHIT DONE
		cmp eax,edx
		jle playerXsmaller
		retPlayerXsmaller:

		mov esi,[ebx+36]
		add esi,esi
		mov edx,[ebx+20]
		add esi, edx
		mov edi, offst
		mov eax, [edi+36]
		cmp eax,esi
		jge playerXbigger
		retPlayerXbigger:

		mov esi,[ebx+24]
		mov edi, offst
		mov edi,[edi+40]
		cmp edi,esi
		jle playerYsmaller
		retplayerYsmaller:

		mov esi,[ebx+36]
		add esi,esi
		add esi, [ebx+24]
		mov edi, offst
		mov edi,[edi+40]
		cmp edi,esi
		jge playerYbigger
		retplayerYbigger:
		
		mov edi,offst
		jmp returnAfterColision

	

		playerXbigger:
		mov eax,1
		mov edx,offst
		mov [edx+28],eax
		jmp retPlayerXbigger

		playerXsmaller:
		mov eax,-1
		mov edx,offst
		mov [edx+28],eax
		jmp retPlayerXsmaller

		playerYbigger:
		mov eax,1
		mov edi,offst
		mov [edi+32],eax
		jmp retplayerYbigger

		playerYsmaller:
		mov eax,-1
		mov edi,offst
		mov [edi+32],eax
		jmp retplayerYsmaller
hadCollisionedBreak endp

throwBomb proc ofst: dword;called when a player throws a bomb
	mov ebx, ofst 
	mov eax,0
	cmp [ebx+48],eax;return if there are no bombs avialeible
	jle retLine
	mov eax,[ebx+48];mov bomb count to eax
	imul eax,BombSize
	add eax,BombArrStarts
	sub eax,BombSize   
	add eax,ebx ;now there is the start of the last bomb offset in eax
	mov ecx,[ebx+20]
	mov [eax+20],ecx ;bombX=playerX
	mov ecx,[ebx+24]
	mov [eax+24],ecx ;bombY=playerY
	mov ecx,1
	mov [eax+40],ecx ;update bomb to be alive
	mov edx,1 
	sub [ebx+48],edx;minus 1 on bombCount
	mov edx,cooldownToBombConst
	mov [ebx+52],edx;update cooldown
	retLine:
	ret
throwBomb endp

DmootKeyboard proc ofst: dword ;movment player 1
	mov eax,0
	mov ebx, ofst
	mov [ebx+28],eax ;reset diractions
	mov [ebx+32],eax
	invoke hadCollisionedWall,ebx ;checking for all collisions before moving
	mov ebx,ofst
	invoke hadCollisionedBreak,ebx
	mov ebx,ofst
	invoke GetAsyncKeyState, VK_P ;checking for bomb action
	cmp eax,0
	jne throwBombTag
	retThrowBomb:
	invoke GetAsyncKeyState,VK_O
	mov ebx,ofst
	cmp eax,0
	jne bombTheBomb
	retBombtheBomb:
	mov ebx,ofst
	mov ecx, [ebx+28];dont check keys if colided
	cmp ecx, 0
	jne retTag
	mov ecx, [ebx+32]
	cmp ecx,0
	jne retTag

	invoke GetAsyncKeyState, VK_DOWN
	cmp eax,0
	jne go_down_tag

	invoke GetAsyncKeyState, VK_UP
	cmp eax,0
	jne go_up_tag

	invoke GetAsyncKeyState, VK_RIGHT
	cmp eax,0
	jne go_right_tag
	
	invoke GetAsyncKeyState, VK_LEFT
	cmp eax,0
	jne go_left_tag

	jmp retTag

	go_down_tag:
	mov edx,1
	add edx,[ebx+68];speed factor
	mov [ebx+32], edx
	ret

	go_up_tag:
	mov ecx, ebx
	add ecx, 32
	mov edx,-1
	sub edx,[ebx+68];speed factor
	mov [ecx], edx
	ret

	go_right_tag:
	mov ecx, ebx
	add ecx, 28
	mov edx,1
	add edx,[ebx+68];speed factor
	mov [ecx], edx
	ret

	go_left_tag:
	mov ecx, ebx
	add ecx, 28
	mov edx,-1
	sub edx,[ebx+68];speed factor
	mov [ecx],edx
	ret

	throwBombTag:
	mov ebx, ofst
	mov eax,0
	cmp [ebx+52],eax;;check if cooldown is lower than 0, if not return
	jge retThrowBomb
	invoke throwBomb,ebx
	jmp retThrowBomb

	bombTheBomb:
	;invoke bombAllBombs,ebx
	jmp retBombtheBomb
	retTag:

	ret
DmootKeyboard endp

DmootKeyboard2 proc ofst: dword ;movment player 2
	mov eax,0
	mov ebx, ofst
	mov [ebx+28],eax
	mov [ebx+32],eax
	invoke hadCollisionedWall,ebx
	mov ebx,ofst
	invoke hadCollisionedBreak,ebx
	mov ebx,ofst
	invoke GetAsyncKeyState, VK_H
	cmp eax,0
	jne throwBombTag
	retThrowBomb:
	invoke GetAsyncKeyState,VK_G
	mov ebx,ofst
	cmp eax,0
	jne bombTheBomb
	retBombtheBomb:
	mov ebx,ofst
	mov ecx, [ebx+28];dont check keys if colided
	cmp ecx, 0
	jne retTag
	mov ecx, [ebx+32]
	cmp ecx,0
	jne retTag

	invoke GetAsyncKeyState, VK_S
	cmp eax,0
	jne go_down_tag

	invoke GetAsyncKeyState, VK_W
	cmp eax,0
	jne go_up_tag

	invoke GetAsyncKeyState, VK_D
	cmp eax,0
	jne go_right_tag
	
	invoke GetAsyncKeyState, VK_A
	cmp eax,0
	jne go_left_tag

	jmp retTag

	go_down_tag:
	mov edx,1
	add edx,[ebx+68];speed factor
	mov [ebx+32], edx
	ret

	go_up_tag:
	mov ecx, ebx
	add ecx, 32
	mov edx,-1
	sub edx,[ebx+68];speed factor
	mov [ecx], edx
	ret

	go_right_tag:
	mov ecx, ebx
	add ecx, 28
	mov edx,1
	add edx,[ebx+68];speed factor
	mov [ecx], edx
	ret

	go_left_tag:
	mov ecx, ebx
	add ecx, 28
	mov edx,-1
	sub edx,[ebx+68];speed factor
	mov [ecx],edx
	ret

	throwBombTag:
	mov ebx, ofst
	mov eax,0
	cmp [ebx+52],eax;;check if cooldown is lower than 0, if not return
	jge retThrowBomb
	invoke throwBomb,ebx
	jmp retThrowBomb

	bombTheBomb:
	;invoke bombAllBombs,ebx
	jmp retBombtheBomb
	retTag:

	ret
DmootKeyboard2 endp

dmootCenter proc ofst:dword ;updates the center values of a player
	mov ebx, ofst
	mov eax,[ebx+20]
	mov [ebx+36],eax
	mov ecx,[ebx+44];rad
	add [ebx+36],ecx

	mov eax,[ebx+24]
	mov [ebx+40],eax
	mov ecx,[ebx+44];rad
	add [ebx+40],ecx
	ret
dmootCenter endp

move proc ofst : dword ;updates pos with diraction for a player
	mov eax, ofst
	mov ebx, [eax+28] 
	add [eax+20],ebx

	mov ebx, [eax+32] 
	add [eax+24],ebx
	ret
move endp

breakValues proc ofst: dword ;give the break their init values
	mov esi, 0
	mov ebx,0;x
	mov edx,0;y
	mov edi, ofst
	forLine:
		mov [edi+20],ebx;update break x
		mov[edi+24],edx;update break y
		mov ecx, [edi+20];update center x
		add ecx,35
		mov [edi+28],ecx
		mov ecx, [edi+24];update center y
		add ecx,35
		mov [edi+32],ecx
		add edi, BreakSize
		cmp ebx,630
		je resetX
		add ebx,70
		retResetX:
		add esi,1
		cmp esi,lengthOfBreaksArr	
	jl forLine
	ret

	resetX:
		mov ebx,0
		add edx, 70
	jmp retResetX
breakValues endp

bombValue proc ofst:dword
	mov ebx,ofst
	mov edi,BombArrStarts
	add edi,ebx ;edi now holds the offset of first bomb in arr
	mov ebx,0
	loopi:
		mov ecx,[edi+40]
		cmp ecx,1;update only if bomb is alive
		je bombAlive
		mov eax,bombTimer
		mov [edi+268],eax
		jmp bombNotAlive
		bombAlive:

		mov eax,-1
		add [edi+268],eax
		mov eax,0
		cmp [edi+268],eax
		je bombThisBomb
		retbombThisBomb:

		mov eax,[edi+20] 
		mov [edi+28],eax
		mov eax,[edi+24] 
		mov [edi+32],eax

		mov eax,[edi+36] ;radius
		add [edi+28],eax
		add [edi+32],eax
		bombNotAlive:
		add edi,BombSize
		add ebx,1
		cmp ebx,MaxBombSize
	jl loopi
	ret

	bombThisBomb:
	invoke explosionBombs,edi
	mov eax,0
	mov [edi+40],eax
		
	mov ecx,ofst
	mov ecx,[ecx+48]
	cmp ecx,0
	je resetBombRenewTimer
	retRenewBomb:
	jmp retbombThisBomb

	resetBombRenewTimer:
	mov edx, bombGainTime
	mov ecx,ofst
	mov [ecx+56],edx
	jmp retRenewBomb
bombValue endp

drawBombs proc ofst:dword;DRAWS ALL ALIVE BOMBS OF A PLAYER
	mov ebx,ofst
	mov edi,BombArrStarts
	add edi,ebx ;edi now holds the offset of first bomb in arr
	mov ebx,0
	loopi:
		mov ecx,[edi+40]
		cmp ecx,1;draw only if bomb is alive
		jge bombAlive
		jmp bombNotAlive
		bombAlive:
		mov eax, [edi+20];bombX
		mov ecx, [edi+24];bombY 
		invoke drd_imageDraw,edi,eax,ecx ;changes eax,ecx,edx!
		bombNotAlive:
		add edi,BombSize
		add ebx,1
		cmp ebx,MaxBombSize
	jl loopi
	ret
drawBombs endp

drawBullets proc ofst:dword ;goes throw all bullets of a player and check if need to be drawn and if yes draws them (if alive or not)
	mov ebx, ofst
	add ebx,BombArrStarts;ebx holds first bomb
	mov edi,ebx
	mov esi,0
	loopi:
		mov ebx,edi
		mov edx,0
		add ebx,44 ;bulet STARTS
		
		mov edx,0
		cmp [ebx+48],edx
		je bulet1isntAlive
		mov eax, [ebx+20]
		mov ecx, [ebx+24]
		invoke drd_imageDraw, ebx,eax,ecx
		bulet1isntAlive:
		
		mov edx,0
		add ebx,BulletSize ;bulet STARTS
		cmp [ebx+48],edx
		je bulet2isntAlive
		mov eax, [ebx+20]
		mov ecx, [ebx+24]
		invoke drd_imageDraw, ebx,eax,ecx
		bulet2isntAlive:

		mov edx,0
		add ebx,BulletSize ;bulet STARTS
		cmp [ebx+48],edx
		je bulet3isntAlive
		mov eax, [ebx+20]
		mov ecx, [ebx+24]
		invoke drd_imageDraw, ebx,eax,ecx
		bulet3isntAlive:

		mov edx,0  
		add ebx,BulletSize ;bulet STARTS
		cmp [ebx+48],edx
		je bulet4isntAlive
		mov eax, [ebx+20]
		mov ecx, [ebx+24]
		invoke drd_imageDraw, ebx,eax,ecx
		bulet4isntAlive:

		add esi,1
		add edi,BombSize
		cmp esi,MaxBombSize
	jl loopi
	ret
drawBullets endp

bulletValues proc ofst: dword ;updates the bullets values (center,pos, checks if behind distance (power))
	mov ebx,ofst
	mov esi,ofst
	add ebx, BombArrStarts;ebx has first bomb
	mov edi,ebx
	mov eax,0
	loopi:
		mov ebx,edi
		add ebx,44;first bulet on ebx
		
		;down Arrow
		mov ecx,[esi+64]
		mov [ebx+52],ecx
		mov ecx,[ebx+44]
		add [ebx+24],ecx
		mov ecx,[ebx+20]
		mov [ebx+28],ecx
		mov edx,17
		add [ebx+28],edx
		mov edx,35
		mov ecx,[ebx+24]
		mov [ebx+32],ecx
		add [ebx+32],edx
		
		mov ecx,[ebx+24];pos Y of bulet
		mov edx,[ebx+40];originY of bulet
		sub ecx,edx
		cmp ecx,[ebx+52]
		jle bulletDidntPastDistance1
		mov ecx,0
		mov [ebx+48],ecx; kill bullet 
		bulletDidntPastDistance1:


		add ebx,BulletSize;left Arrow
		mov ecx,[esi+64]
		mov [ebx+52],ecx
		mov ecx,[ebx+44]
		sub [ebx+20],ecx
		mov ecx,[ebx+20]
		mov [ebx+28],ecx
		mov edx,0
		add [ebx+28],edx;update x center

		mov ecx,[ebx+24];update y center
		mov [ebx+32],ecx
		mov ecx,17
		add [ebx+32],ecx;17

		mov ecx,[ebx+20];pos X of bulet
		mov edx,[ebx+36];originX of bulet
		sub edx,ecx
		cmp edx,[ebx+52]
		jle bulletDidntPastDistance2
		mov ecx,0
		mov [ebx+48],ecx; kill bullet 
		bulletDidntPastDistance2:


		add ebx,BulletSize ; upArrow
		mov ecx,[esi+64]
		mov [ebx+52],ecx
		mov ecx,[ebx+44]
		sub [ebx+24],ecx
		mov ecx,[ebx+20]
		mov [ebx+28],ecx
		mov edx,17
		add [ebx+28],edx;update x center

		mov ecx,[ebx+24];update y center
		mov [ebx+32],ecx
		mov ecx,0
		add [ebx+32],ecx;

		mov ecx,[ebx+24];pos Y of bulet
		mov edx,[ebx+40];originY of bulet
		sub edx,ecx
		cmp edx,[ebx+52]
		jle bulletDidntPastDistance3
		mov ecx,0
		mov [ebx+48],ecx; kill bullet 
		bulletDidntPastDistance3:

		add ebx,BulletSize ;Right arrow
		mov ecx,[esi+64]
		mov [ebx+52],ecx
		mov ecx,[ebx+44]
		add [ebx+20],ecx
		mov ecx,[ebx+20]
		mov [ebx+28],ecx
		mov edx,35
		add [ebx+28],edx;update x center

		mov ecx,[ebx+24];update y center
		mov [ebx+32],ecx
		mov ecx,17
		add [ebx+32],ecx;17

		mov ecx,[ebx+20];pos X of bulet
		mov edx,[ebx+36];originX of bulet
		sub ecx,edx
		cmp ecx,[ebx+52]
		jle bulletDidntPastDistance4
		mov ecx,0
		mov [ebx+48],ecx; kill bullet 
		bulletDidntPastDistance4:

		add edi, BombSize
		add eax,1
		cmp eax,MaxBombSize
	jl loopi
	ret
bulletValues endp

bulletColosions proc ofstBomb: dword;checks for a collision of single bombBullets with breaks and destroys them  and breaks if colided
	mov ebx,ofstBomb ;where Bomb starts
	add ebx,44;where bulletsStart
	mov eax,0;counter
	loopi:
		mov edi, offset breakArr[0];offset of first break 
		mov ecx,0
		loopiBreaks:
			mov esi,0;dont check if dead break
			cmp [edi+40],esi
			je retBulletCollisionedBreak

			mov esi,0;dont check if dead bullet
			cmp [ebx+48],esi
			je retBulletCollisionedBreak

			mov edx,[edi+28];centerX of break 
			sub edx, [ebx+28];centerXBreak-centerXBullet
			imul edx,edx

			mov esi,[edi+32]
			sub esi, [ebx+32];same with Y
			imul esi,esi
			add edx,esi;sum in edx

			mov esi,[edi+36];radius of break 
			imul esi,esi;radius squered in esi

			;add esi,1000
			cmp edx, esi;jump to collisionFunc if is in collision
			jle BulletCollisionedBreak
			retBulletCollisionedBreak:
		add edi,BreakSize
		add ecx,1
		cmp ecx, lengthOfBreaksArr
		jl loopiBreaks

		add ebx, BulletSize
		add eax,1
		cmp eax,4
	jl loopi
	ret

	BulletCollisionedBreak:
	mov esi,0
	mov [ebx+48],esi; set bullet not alive
	; dont destroy break if indestructible break
	cmp [edi+44],esi
	je	retBulletCollisionedBreak
	mov [edi+40],esi; set break not alive
	jmp retBulletCollisionedBreak
bulletColosions endp

bulletColideMan proc ofstBomb: dword ;checks for a collision in as spesific bombBullets
	mov ebx,ofstBomb ;where Bomb starts
	add ebx,44;where bulletsStart
	mov eax,0;counter
	loopi:
		pusha
		mov eax,0
		mov ecx, offset players[0]
		playerLoop:
			mov edx,0
			cmp [ecx+60],edx
			jle retBulletCollisionedPlayer

			mov edi, ecx
			mov esi,0;dont check if dead bullet
			cmp [ebx+48],esi
			je retBulletCollisionedPlayer

			mov edx,[edi+36];centerX of player 
			sub edx, [ebx+28];centerXplayer-centerXBullet
			imul edx,edx

			mov esi,[edi+40]
			sub esi, [ebx+32];same with Y
			imul esi,esi
			add edx,esi;sum in edx

			mov esi,[edi+44];radius of player 
			imul esi,esi;radius squered in esi

			cmp edx, esi;jump to collisionFunc if is in collision
			jle BulletCollisionedPlayer
			retBulletCollisionedPlayer:

			add ecx, DmootSize
			add eax,1
			cmp eax,numberOfPlayers
		jl playerLoop
		popa

		add ebx, BulletSize
		add eax,1
		cmp eax,4
	jl loopi
	ret

	BulletCollisionedPlayer:
	mov esi,0
	mov [ebx+48],esi; set bullet not alive
	mov esi,1
	sub [edi+60],esi; minus health on player
	jmp retBulletCollisionedPlayer
bulletColideMan endp

callCollision proc ofst: dword ;checks all kind of collisions with bullets for a player
	mov eax,ofst
	add eax, BombArrStarts;offset of first Bomb
	mov ebx,0
	loopis:
		pusha
		invoke bulletColosions,eax
		popa
		pusha
		mov ecx,0
		mov edx, offset players[0]
		playersLoop:
			pusha
			invoke bulletColideMan,eax
			popa
			add edx, DmootSize
			add ecx,1
			cmp ecx,numberOfPlayers
		jl playersLoop
		popa
		add eax, BombSize
		add ebx,1
		cmp ebx,MaxBombSize
	jl loopis
	ret
callCollision endp;checks all colisions

checkForEndGame proc ;checks if the game has ended and if yes making the needed screens alive
	local winnerIndex:dword
	mov ecx,0
	mov eax,1
	mov ebx,offset players[0]
	loopi:
		mov edx,0
		cmp [ebx+60],edx
		jg addAlive
		retaddAlive:

		add ebx,DmootSize
		add eax,1
		cmp eax, numberOfPlayers
	jle loopi
	cmp ecx,1
	je WeGotAwinner
	

	ret
	addAlive:
		add ecx,1
		mov winnerIndex,eax
	jmp retaddAlive 

	WeGotAwinner:
	cmp winnerIndex,1 
	je player1Wonline
	cmp winnerIndex,2 
	je player2Wonline
	cmp winnerIndex,3
	je player2Wonline


	player2Wonline:;changes alive to be true
	mov ecx, offset player2wonScreen
	mov edx,1
	mov [ecx+20],edx
	mov hasGameEnded,1
	ret

	player1Wonline:;changes alive to be true
	mov ecx, offset player1wonScreen
	mov edx,1
	mov [ecx+20],edx
	mov hasGameEnded,1
	ret
checkForEndGame endp

drawScreens proc ;draws screens if alive

	mov ecx, offset player2wonScreen ;draw end game screens only if some1 won (only if screen isAlive=1)
	mov eax,1
	cmp [ecx+20],eax;check if the screen is alive and if yes draws it
	je drawScreen

	mov ecx, offset player1wonScreen
	mov eax,1
	cmp [ecx+20],eax
	je drawScreen
	ret	

	drawScreen:
	invoke drd_imageDraw,ecx,130,270
	ret
drawScreens endp

drawPowerUps proc ;draws alive powerUps 
	mov ebx,0
	mov edi, offset breakArr[0]
	loopi:
		mov eax,0
		cmp[edi+48],eax
		jg hasPowerUp
		retHasPowerUp:
		add edi,BreakSize
		add ebx,1
		cmp ebx,lengthOfBreaksArr
	jl loopi
	ret


	hasPowerUp:
	mov eax,1;if break is still alive return
	cmp [edi+40],eax ;make as mark if want to debug powerUp locations
	jge retHasPowerUp

	mov eax,1
	cmp[edi+48],eax
	je HasThunder
	mov eax,2
	cmp[edi+48],eax
	je hasSpeedBoost
	mov eax,3
	cmp[edi+48],eax
	je hasAddBomb
	jmp retHasPowerUp


	HasThunder:
	;invoke drd_imageSetTransparent,edi,0ffffffffh
	mov eax, edi
	add eax, 52
	invoke drd_imageDraw,eax,[edi+20],[edi+24]
	jmp retHasPowerUp

	hasSpeedBoost:
	;invoke drd_imageSetTransparent,edi,0ffffffffh
	mov eax, edi
	add eax, 52
	invoke drd_imageDraw,eax,[edi+20],[edi+24]
	jmp retHasPowerUp

	hasAddBomb:
	invoke drd_imageSetTransparent,edi,0ffffffffh
	mov eax, edi
	add eax, 52
	invoke drd_imageDraw,eax,[edi+20],[edi+24]
	jmp retHasPowerUp
drawPowerUps endp
  
hasCollisionedPower proc ofst:dword ;checking if a player has coldied a powerUp
	mov ecx,0
	mov edi,ofst
	mov ebx, offset breakArr[0]
	loopi:
		mov eax,0
		cmp [ebx+48],eax
		je noPowerUp

		mov eax, [ebx+28];centerX break
		mov edx, [ebx+32];centerY break
		mov esi,[edi+36];centerX of player
		sub esi,eax
		mov eax,esi
		imul eax,eax ;x*x in eax
		
		mov esi,[edi+40];centerY of player
		sub esi,edx
		mov edx,esi
		imul edx,edx ;y*y in edx

		add eax,edx;sum of distxy squered
		add eax,700; safeDist to not take if didnt bomb the break

		mov edx,[ebx+36]
		add edx,[edi+44]
		imul edx,edx
		cmp eax,edx; Comparing betwin sum of radiuses and dist of centers
		jle areColiding
		retColision:

		noPowerUp:
		add ecx,1
		add ebx,BreakSize
		cmp ecx,lengthOfBreaksArr
		jl loopi
	ret

	areColiding:
	mov eax,1
	cmp [ebx+48],eax;thunder
	je activeThunder
	mov eax,2
	cmp [ebx+48],eax;speedBoost
	je activeSpeedBoost
	mov eax,3
	cmp [ebx+48],eax;addBomb
	je activeAddBomb
	jmp retColision

	activeThunder:
		mov eax,bulletRangeGrowing;factor of making the dist of bullet further
		add [edi+64],eax
		mov eax,0; set break to dont have power anymore
		mov [ebx+48],eax
	jmp retColision

	activeSpeedBoost:
		mov eax,0 ; set break to dont have power anymore
		mov [ebx+48],eax
		mov eax,3
		cmp[edi+68],eax
		jge retColision
		mov eax,1
		add [edi+68],eax
	jmp retColision

	activeAddBomb:
		mov eax,0
		mov [ebx+48],eax
		mov eax,MaxBombSize
		cmp[edi+72],eax
		jge retColision
		mov eax,1
		add [edi+72],eax
	jmp retColision
hasCollisionedPower endp

bufferLength proc bufferOfst:dword ;gets the length of a buffer
	local Blength: dword
	mov Blength,0 
	pusha
	mov ebx, bufferOfst
	xor ecx,ecx
	cmp byte ptr [ebx],0 
	je retSign
	loopCheckLength:
		add ecx,1
		cmp byte ptr [ebx+ecx],0
		jne loopCheckLength
		retSign:
		mov Blength,ecx
		popa
		mov eax,Blength
		ret
bufferLength endp

smallBufferToBigBuffer proc ofstSmallBuffer:dword, ofstBigBuffer:dword ;adds a buffer content to other buffer
	local smallBufferLength:dword
	local bigBufferLength:dword
	pusha
	mov ebx,ofstSmallBuffer
	mov ecx, ofstBigBuffer 
	invoke bufferLength, ecx
	mov bigBufferLength,eax
	invoke bufferLength, ebx
	mov smallBufferLength,eax 
	add ecx, bigBufferLength
	xor edi,edi
	loopiCopyToBigBuffer:
		mov byte ptr dl, byte ptr [ebx+edi]
		mov byte ptr [ecx+edi], byte ptr dl
		add edi,1
		cmp edi, smallBufferLength
	jne loopiCopyToBigBuffer
	popa
	ret
smallBufferToBigBuffer endp

deleteBufferContent proc ofstBuffer: dword ;delets the buffers content
	pusha
	mov ebx, ofstBuffer
	invoke bufferLength, ebx
	cmp eax,0
	jne loopiClear
	popa
	ret
	loopiClear:
		mov byte ptr [ebx+eax-1],0 
		add eax,-1
		cmp eax, 0
	jne loopiClear
	popa
	ret
deleteBufferContent endp

drawTitle proc ;draws the title with the buffer funcs
	invoke deleteBufferContent, offset bigBuffer

	invoke smallBufferToBigBuffer,offset player1Lives, offset bigBuffer
	invoke dwtoa, players[0].lives,offset player1LivesCounter
	invoke smallBufferToBigBuffer,offset player1LivesCounter, offset bigBuffer
	
	invoke smallBufferToBigBuffer, offset player1Bombs, offset bigBuffer
	invoke dwtoa, players[0].bombsAvailable, offset player1BombsCount
	invoke smallBufferToBigBuffer, offset player1BombsCount, offset bigBuffer
	

	invoke smallBufferToBigBuffer,offset player2Lives, offset bigBuffer
	invoke dwtoa, players[DmootSize+DmootSize].lives, offset player2LivesCounter
	invoke smallBufferToBigBuffer,offset player2LivesCounter, offset bigBuffer

	invoke smallBufferToBigBuffer, offset player2Bombs, offset bigBuffer
	invoke dwtoa, players[DmootSize+DmootSize].bombsAvailable, offset player2BombsCount
	invoke smallBufferToBigBuffer, offset player2BombsCount, offset bigBuffer
	

	invoke drd_setWindowTitle, offset bigBuffer
	ret
drawTitle endp

;bot funcs

closetPlayerToBot proc ofst:dword
	local ofstRet:dword
	local minDist:dword
	pusha
	mov minDist,1000000
	mov eax,ofst
	mov ebx,0
	mov edi, offset players[0]
	playersLoop:
		
		cmp edi,eax
		je samePlayer
		
		mov ecx,0
		cmp [edi+60],ecx
		je samePlayer

		mov ecx,[edi+36];centerX player
		sub ecx,[eax+36];centerX bot
		imul ecx,ecx

		mov edx,[edi+40];centerY player
		sub edx,[eax+40];centerY bot
		imul edx,edx

		add ecx,edx
		cmp ecx, minDist
		jle lowerDist
		retlowerDist:
		

		samePlayer:

		add ebx,1
		add edi,DmootSize
		cmp ebx,numberOfPlayers
	jl playersLoop
	popa
	mov eax,ofstRet
	ret

	lowerDist:
		mov minDist,ecx
		mov ofstRet,edi
		sub minDist,1000
	jmp retlowerDist
closetPlayerToBot endp
hasSensorCollisionedBreak proc ofst:dword, sensor:POINT ;returns to eax 1 if colided any break 2 if colided indestructable break
	local hasColided : dword

	mov hasColided,0
	pusha
	mov ebx, ofst
	mov eax,0
	mov edi, offset breakArr[0]
	loopi:
		mov ecx,0
		cmp [edi+40],ecx
		je notAlive

		mov ecx, [edi+28];centerX
		mov edx, [edi+32]; centerY

		sub ecx,sensor.x
		imul ecx,ecx

		sub edx, sensor.y
		imul edx,edx

		add ecx,edx
		mov edx,[edi+36]
		add edx,5
		imul edx,edx

		cmp ecx,edx
		jle colidedBreak

		notAlive:
		add edi,BreakSize
		add eax,1
		cmp eax, lengthOfBreaksArr
	jl loopi
	retColidedBreak:
	popa
	mov eax, hasColided
	ret

	colidedBreak:
	mov hasColided,1
	mov esi,0
	cmp [edi+44],esi
	je colidedNonDistructable
	jmp retColidedBreak

	colidedNonDistructable:
	mov hasColided,2
	jmp retColidedBreak
hasSensorCollisionedBreak endp

distancePointPlayer proc sensor:POINT, ofst:dword ;return the distance to eax
	local sensorX:dword
	local sensorY:dword
	pusha

	mov eax,sensor.x
	mov sensorX,eax
	mov eax,sensor.y
	mov sensorY, eax

	mov ebx,[ofst+36];centerX player
	mov ecx,[ofst+40];centerY player

	sub ebx,sensorX
	imul ebx,ebx

	sub ecx,sensorY
	imul ecx,ecx

	add ebx,ecx
	mov sensorX,ebx
	popa
	mov eax,sensorX
	ret
distancePointPlayer endp

isThereAbreakBetweenPlayerAndPlayerInLine proc ofst:dword, ofstPlayer2:dword, closeXorY:dword ;-1= x is close, 1=y is close return 0 or 1 to eax
	local counter:dword
	local IsBetweenBoolean:dword
	pusha
	mov IsBetweenBoolean,0
	mov eax,0
	mov ebx, ofst
	mov edx,ofstPlayer2
	mov ecx, offset breakArr[0]
	loopi:
		mov edi,0
		cmp [ecx+40],edi
		je retCloseY

		mov edi,1
		cmp [ecx+44],edi
		je retCloseY

		mov counter,0
		mov edi,closeXorY
		cmp edi,-1 
		je closeX
		;close Y
		;check if break is on Y Line
		mov esi,[ecx+32]
		sub esi,[ebx+40]
		imul esi,esi
		cmp esi,2000
		jg diffrentYline
		mov esi,[ecx+32]
		sub esi,[edx+40]
		imul esi,esi
		cmp esi,2000
		jg diffrentYline
		;sameYline
		;check if between two Xs
		;if x is greater than some1 add 1 to counter if counter equals 1 is between
		mov edi,[ecx+28]
		cmp edi,[ebx+36]
		jl dontAddCounter
		add counter,1
		dontAddCounter:

		cmp edi,[edx+36]
		jl dontAddCounter2
		add counter,1
		dontAddCounter2:

		cmp counter,1
		je IsBetween

		diffrentYline:
		jmp retCloseY
		closeX:
		;check if break is on X Line
		mov esi,[ecx+28]
		sub esi,[ebx+36]
		imul esi,esi
		cmp esi,2000
		jg retCloseY
		mov esi,[ecx+28]
		sub esi,[edx+36]
		imul esi,esi
		cmp esi,2000
		jg retCloseY
		;sameXline
		;check if between two Ys
		;if y is greater than some1 add 1 to counter if counter equals 1 is between
		mov edi,[ecx+32]
		cmp edi,[ebx+40]
		jl dontAddCounter3
		add counter,1
		dontAddCounter3:

		cmp edi,[edx+40]
		jl dontAddCounter4
		add counter,1
		dontAddCounter4:

		cmp counter,1
		je IsBetween

		retCloseY:


		add ecx,BreakSize
		add eax,1
		cmp eax, lengthOfBreaksArr
	jl loopi
	popa
	mov eax,0
	ret

	IsBetween:
		mov IsBetweenBoolean,1
		popa
		mov eax,IsBetweenBoolean
	ret
isThereAbreakBetweenPlayerAndPlayerInLine endp

isThereAbreakBetweenPlayerAndBombInLine proc ofst:dword, ofstBomb:dword, closeXorY:dword ;-1= x is close, 1=y is close return 0 or 1 to eax
	local counter:dword
	local IsBetweenBoolean:dword
	pusha
	mov IsBetweenBoolean,0
	mov eax,0
	mov ebx, ofst
	mov edx,ofstBomb
	mov ecx, offset breakArr[0]
	loopi:
		mov edi,0
		cmp [ecx+40],edi
		je retCloseY
		mov counter,0
		mov edi,closeXorY
		cmp edi,-1 
		je closeX
		;close Y
		;check if break is on Y Line
		mov esi,[ecx+32]
		sub esi,[edx+32]
		imul esi,esi
		cmp esi,closeLinesConst
		jg diffrentYline
		;sameYline
		;check if between two Xs
		;if x is greater than some1 add 1 to counter if counter equals 1 is between
		mov edi,[ecx+28]
		cmp edi,[ebx+36]
		jl dontAddCounter
		add counter,1
		dontAddCounter:

		cmp edi,[edx+28]
		jl dontAddCounter2
		add counter,1
		dontAddCounter2:

		cmp counter,1
		je IsBetween

		diffrentYline:
		jmp retCloseY
		closeX:
		;check if break is on X Line
		mov esi,[ecx+28]
		sub esi,[edx+28]
		imul esi,esi
		cmp esi,closeLinesConst
		jg retCloseY
		;sameXline
		;check if between two Ys
		;if y is greater than some1 add 1 to counter if counter equals 1 is between
		mov edi,[ecx+32]
		cmp edi,[ebx+40]
		jl dontAddCounter3
		add counter,1
		dontAddCounter3:

		cmp edi,[edx+32]
		jl dontAddCounter4
		add counter,1
		dontAddCounter4:

		cmp counter,1
		je IsBetween

		retCloseY:


		add ecx,BreakSize
		add eax,1
		cmp eax, lengthOfBreaksArr
	jl loopi
	popa
	mov eax,0
	ret

	IsBetween:
		mov IsBetweenBoolean,1
		popa
		mov eax,IsBetweenBoolean
	ret
isThereAbreakBetweenPlayerAndBombInLine endp

shouldBotEscapeFromBomb proc ofst:dword ;checks if in danger from bomb, if no danger mov eax,0. if is in danger mov eax 1 , moves to ebx the x of escape, moves to ecx the y of escape
	local closetBreakofst :dword
	local minDist:dword
	local power:dword
	local dangerBombOfst:dword
	local counterPlayers:dword
	local counterBombs:dword
	local counterBreaks:dword
	local isBadBreak:dword
	mov isBadBreak,0
	mov minDist,1000000
	;go throw all players and all of their bombs and check if a bomb is inRange of bot
	mov eax,0
	mov edx,ofst
	mov edi,offset players[0]
	add edi,BombArrStarts
	mov esi,edi
	loopPlayers:
		mov ebx,0
		mov edi,esi
		loopBomb: ; reset bombs ;esi for bullets edi for bombs
			mov ecx,0
			cmp [edi+40],ecx
			je deadBomb
			pusha
			
			mov esi,44;bulletStartsOnBomb
			add esi,edi
			mov ecx,[esi+52] ;bullet power to ecx
			add ecx,35;SAFE ZONE
			imul ecx,ecx ;bullet power +30 squered

			;offset of bomb in edi
			mov edx,ofst
			mov esi, [edi+28] ;bomb centerX
			mov edx, [edx+36] ;player centerX
			sub edx,esi
			imul edx,edx
			cmp edx,3200
			jg farX1
			
			;close Xs
			mov edx,ofst
			invoke isThereAbreakBetweenPlayerAndBombInLine,edx,edi,-1
			cmp eax,1
			je farX1
			mov esi, [edi+32] ;bomb centerY
			mov edx, [edx+40] ;player centerY
			sub edx,esi
			imul edx,edx
			cmp edx,ecx
			jle InDanger

			farX1:
			mov edx,ofst
			mov esi, [edi+32] ;bomb centerY
			mov edx, [edx+40] ;player centerY
			sub edx,esi
			imul edx,edx
			cmp edx,3200
			jg farY1

			;close ys
			mov edx,ofst
			invoke isThereAbreakBetweenPlayerAndBombInLine,edx,edi,1
			cmp eax,1
			je farY1
			mov esi, [edi+28] ;bomb centerX
			mov edx, [edx+36] ;player centerX
			sub edx,esi
			imul edx,edx
			cmp edx,ecx
			jle InDanger
			farY1:

			mov edx,ofst
			mov esi,[edi+28]
			sub esi,[edx+36]
			imul esi,esi
			mov ecx,[edi+32]
			sub ecx,[edx+40]
			imul ecx,ecx
			add esi,ecx

			mov ecx,[edi+36]
			add ecx,[edx+44]
			imul ecx,ecx
			cmp esi,ecx
			jle InDanger

			;mov edx,ofst
			;mov esi,[edi+28] ;bombCenterX
			;mov edx,[edx+36] ;player centerX
			;sub edx,esi
			;imul edx,edx

			;mov ebx,ofst
			;mov eax,[edi+32] ;bombCenterY
			;mov esi,[ebx+40] ;player centerY
			;sub esi,eax
			;imul esi,esi

			;add esi,edx
			;cmp esi,ecx
			;jle InDanger


			popa

			deadBomb:
			add edi,BombSize
			add ebx,1
			cmp ebx,MaxBombSize
		jl loopBomb
		add eax,1
		add esi, DmootSize
		cmp eax, numberOfPlayers
	jl loopPlayers
	
	mov eax,0
	ret

	InDanger:
	mov dangerBombOfst,edi
	mov esi,44;bulletStartsOnBomb
	add esi,edi
	mov ecx,[esi+52]
	mov power,ecx
	popa

	
	mov ebx,ofst
	mov ecx, offset breakArr[0]
	mov edi, offset players[0]

	mov counterBreaks,0 
	mov counterPlayers,0 
	mov counterBombs,0 
	loopiBreaks:
		mov isBadBreak,0
		mov eax,1 
		cmp [ecx+40],eax
		je badBreak

		mov edi, offset players[0]
		mov counterPlayers,0 
		loopiplayers:
			mov edx, edi ;edx holds bomb offset
			add edx, BombArrStarts
			mov eax,0
			cmp [edi+60],eax
			jle deadPlayer
			mov counterBombs,0 
			loopibombs:
				mov eax,edx
				pusha
				mov ebx,0
				cmp [edx+40],ebx
				jle deadBomb2
				
				mov esi,44;bulletStartsOnBomb
				add esi,edx
				mov ebx,[esi+52]
				mov power,ebx

				mov esi,[eax+28]
				sub esi,[ecx+28]
				imul esi,esi
				cmp esi,300 ;range of bomb X exlposion
				jg farXs
				;close Xs
				mov esi,ofst
				invoke isThereAbreakBetweenPlayerAndBombInLine,esi,edx,-1
				cmp eax,1
				je farXs
				mov eax,edx
				mov esi,[eax+32]
				sub esi,[ecx+32]
				imul esi,esi
				mov edi,power
				add edi,150
				imul edi,edi
				cmp esi,edi ;is y hefresh bigger than power
				jle badBreak

				farXs:
				mov eax,edx
				mov esi,[eax+32]
				sub esi,[ecx+32]
				imul esi,esi
				cmp esi,300 ;range of bomb Y exlposion
				jg farYs
				;close ys
				mov esi,ofst
				invoke isThereAbreakBetweenPlayerAndBombInLine,esi,edx,1
				cmp eax,1
				je farYs
				mov eax,edx
				mov esi,[eax+28]
				sub esi,[ecx+28]
				imul esi,esi
				mov edi,power
				add edi,150
				imul edi,edi
				cmp esi,edi ;is x hefresh bigger than power
				jle badBreak

				farYs:
				mov eax,edx
				;IF GOT HERE BREAK IS ALIVE AND BOT IN RANGE OF Bomb

				deadBomb2:
				popa

				add edx,BombSize
				add counterBombs,1
				mov eax,MaxBombSize
				cmp counterBombs,eax
			jl loopibombs

			deadPlayer:

			add edi,DmootSize
			add counterPlayers,1 
			mov eax, numberOfPlayers
			cmp counterPlayers,eax
		jl loopiplayers
		

		retBadBreak:
		mov eax,0
		cmp isBadBreak,eax
		je checkForMinBreak
		retcheckForMinBreak:

		add ecx, BreakSize
		add counterBreaks,1
		mov eax,lengthOfBreaksArr
		cmp counterBreaks,eax
	jl loopiBreaks
		mov edx,closetBreakofst
		mov eax,offset testy
		mov ebx,[edx+20]
		mov [eax+20],ebx
		mov ebx,[edx+24]
		mov [eax+24],ebx
		mov eax,1
		mov ebx,[edx+28]
		mov ecx,[edx+32]
	ret

	badBreak:
	mov isBadBreak,1 
	jmp retBadBreak

	newMin:
		mov closetBreakofst,ecx
		mov minDist,esi
	jmp retnewMin

	checkForMinBreak: ;the break isnt in danger check if in min distance
	mov eax,ofst ;eax has ofst of player
	mov esi,[eax+36] ;centerX player
	sub esi,[ecx+28]
	imul esi,esi

	mov edi,[eax+40] ;centerY player
	sub edi,[ecx+32] 
	imul edi,edi

	add esi,edi ;distance between break and player in esi
	cmp esi,minDist
	jle newMin
	retnewMin:

	jmp retcheckForMinBreak

shouldBotEscapeFromBomb endp

shouldBotThrowBomb proc ofst:dword
	local power:dword
	local dangerBombOfst:dword
	;go check all players and all of their bombs and check if a bomb is inRange of bot
	mov ecx,0
	mov edx,ofst
	mov edi,offset players[0]
	
	loopPlayers:
		pusha
			cmp edx,edi
			je samePlayer

			mov ecx,[edi+64] ;bullet power to ecx
			add ecx,35;SAFE ZONE
			imul ecx,ecx ;bullet power +30 squered

			;offset of victim in edi
			mov edx,ofst
			mov esi, [edi+36] ;victim centerX
			mov edx, [edx+36] ;player centerX
			sub edx,esi
			imul edx,edx
			cmp edx,3200
			jg farX1
			
			;close Xs
			mov edx,ofst
			invoke isThereAbreakBetweenPlayerAndPlayerInLine,edx,edi,-1
			cmp eax,1
			je farX1
			mov esi, [edi+40] ;victim centerY
			mov edx, [edx+40] ;player centerY
			sub edx,esi
			imul edx,edx
			cmp edx,ecx
			jle throwKillBomb

			farX1:
			mov edx,ofst
			mov esi, [edi+40] ;victim centerY
			mov edx, [edx+40] ;player centerY
			sub edx,esi
			imul edx,edx
			cmp edx,3200
			jg farY1

			;close ys
			mov edx,ofst
			invoke isThereAbreakBetweenPlayerAndPlayerInLine,edx,edi,1
			cmp eax,1
			je farY1
			mov esi, [edi+36] ;victim centerX
			mov edx, [edx+36] ;player centerX
			sub edx,esi
			imul edx,edx
			cmp edx,ecx
			jle throwKillBomb
			farY1:

			mov edx,ofst
			mov esi,[edi+36]
			sub esi,[edx+36]
			imul esi,esi
			mov ecx,[edi+40]
			sub ecx,[edx+40]
			imul ecx,ecx
			add esi,ecx

			mov ecx,[edi+44]
			add ecx,[edx+44]
			imul ecx,ecx
			cmp esi,ecx
			jle throwKillBomb
			
			samePlayer:
			popa

			
			add edi,DmootSize
			add ecx,1
			cmp ecx,numberOfPlayers
	jl loopPlayers
	
	mov eax,0
	ret

	throwKillBomb:
	mov ebx, ofst
	mov eax,0
	cmp [ebx+52],eax;;check if cooldown is lower than 0, if not return
	jge samePlayer
	invoke throwBomb,ebx
	jmp samePlayer
	ret	
shouldBotThrowBomb endp

greenBallMovment proc ofst:dword   ;, chaserX:dword,chaserY:dword
	;currently the bot to chase after is player1 change to closest bot
	local bestRoute1:dword
	local bestRoute2:dword
	local bestRoute3:dword
	local bestRoute4:dword

	local Xpreff: dword
	local Xleast: dword
	local Ypreff: dword
	local Yleast: dword

	local upSensor:POINT
	local rightSensor:POINT
	local downSensor:POINT
	local leftSensor:POINT

	local routeCounter: dword
	local currentRoute: dword

	local XtoChase: dword
	local YtoChase: dword

	local hasStuck: dword
	mov hasStuck,0
	mov bestRoute1,0
	mov bestRoute2,0
	mov bestRoute3,0
	mov bestRoute4,0

	mov routeCounter,1
	mov ebx, ofst
	
	jmp updateSensorValues
	retupdateSensorValues:

	mov eax,0
	mov [ebx+28],eax ;reset diractions
	mov [ebx+32],eax
	mov ebx,ofst
	invoke hadCollisionedBreak,ebx
	mov ebx,ofst
	invoke hadCollisionedWall,ebx ;checking for all collisions before moving
	mov ebx,ofst

	mov ecx, [ebx+28];dont check movment if colided
	cmp ecx, 0
	jne retTag
	mov ecx, [ebx+32]
	cmp ecx,0
	jne retTag
	
	mov ebx,ofst
	invoke shouldBotThrowBomb,ebx
	
	mov ebx,ofst
	invoke closetPlayerToBot,ebx
	mov edi, eax
	;check if is on Stuck position
	;isThereAbreakBetweenPlayerAndPlayerInLine
	;mov edi,offset players[0]
	mov ebx,ofst
	mov eax,[edi+36]
	sub eax,[ebx+36] 
	imul eax,eax
	cmp eax, closeLinesConst
	jg farXs
	closeXs:
	invoke isThereAbreakBetweenPlayerAndPlayerInLine,ebx,edi,-1 ;check if there is a break inLine
	cmp eax,1
	je StuckCloseX

	farXs:
	mov eax,[edi+40]
	sub eax,[ebx+40] 
	imul eax,eax
	cmp eax, closeLinesConst
	jg farYs
	
	closeYs:
	invoke isThereAbreakBetweenPlayerAndPlayerInLine,ebx,edi,1 ;check if there is a break inLine
	cmp eax,1
	je StuckCloseY
	farYs:


	mov ebx,ofst
	invoke shouldBotEscapeFromBomb,ebx
	cmp eax,1 
	je isInDanger

	mov ebx,ofst
	invoke closetPlayerToBot,ebx
	mov edi, eax
	mov eax,[edi+36]
	mov XtoChase,eax
	mov eax,[edi+40]
	mov YtoChase,eax

	retUpdateXy:
	arentClose:
	mov ebx,ofst
	
	changeX: 
	mov ecx,[ebx+36] ;centerxBot
	cmp ecx, XtoChase 
	jge XpreffLeft
	mov Xpreff,4
	mov Xleast, 2

	changeY:
	mov ecx,[ebx+40]
	cmp ecx, YtoChase
	jge YpreffUp
	mov Ypreff,1
	mov Yleast,3
	retY:

	mov ebx,ofst
	mov ecx,[ebx+36] ;centerxBot
	sub ecx, XtoChase ;centerxBot-centerxPlayer in ecx
	imul ecx,ecx

	mov edx, [ebx+40]
	sub edx, YtoChase; centerYbot-centerYplayer in edx
	imul edx,edx

	cmp ecx, edx
	jge preffX
	jl preffY
	retPreffXY:
	mov ebx,ofst
	StartGoingToRouts:
	;now move to the bestRoute1 
	;invoke hasSensorCollisionedBreak,ebx,sensors[bestRoute1]
	loopi:
		mov eax,routeCounter
		cmp eax,1
		je route1
		cmp eax,2
		je route2
		cmp eax,3
		je route3
		cmp eax,4 
		je route4
		retRoute:

		mov eax,1
		cmp currentRoute,eax
		je cmpCollisionDown
		mov eax,2
		cmp currentRoute,eax
		je cmpCollisionLeft
		mov eax,3
		cmp currentRoute,eax
		je cmpCollisionUp
		mov eax,4
		cmp currentRoute,eax
		je cmpCollisionRight

		BadRoute:

		add routeCounter,1
		cmp routeCounter,3
	jle loopi
		mov ebx,ofst
		mov eax,1
		cmp currentRoute,eax
		je goDown
		mov eax,2
		cmp currentRoute,eax
		je goLeft
		mov eax,3
		cmp currentRoute,eax
		je goUp
		mov eax,4
		cmp currentRoute,eax
		je goRight

	route1:
	mov esi,bestRoute1
	mov currentRoute,esi
	jmp retRoute

	route2:
	mov esi,bestRoute2
	mov currentRoute,esi
	jmp retRoute
	
	route3:
	mov esi,bestRoute3
	mov currentRoute,esi
	jmp retRoute

	route4:
	mov esi,bestRoute4
	mov currentRoute,esi
	jmp retRoute


	retTag:
	ret
	StuckCloseX:
		mov hasStuck,1
		invoke hasSensorCollisionedBreak,ebx,rightSensor
		cmp eax,1
		jge LeftBestRightLess
		mov bestRoute1,4 
		mov bestRoute2,2 
		mov bestRoute3,1 
		mov bestRoute2,3 
		retLeftBestRightLess:
	jmp StartGoingToRouts

	LeftBestRightLess:
	mov bestRoute1,2 
	mov bestRoute2,4 
	mov bestRoute3,1 
	mov bestRoute2,3 
	jmp retLeftBestRightLess

	StuckCloseY:
		mov hasStuck,1
		invoke hasSensorCollisionedBreak,ebx,downSensor
		cmp eax,1
		jge UpBestDownLess
		mov bestRoute1,1  
		mov bestRoute2,3
		mov bestRoute3,2 
		mov bestRoute4,4 
		retUpBestDownLess:
	jmp StartGoingToRouts

	UpBestDownLess:
	mov bestRoute1,3 
	mov bestRoute2,1 
	mov bestRoute3,2 
	mov bestRoute4,4 
	jmp retUpBestDownLess


	isInDanger:
		mov XtoChase,ebx
		mov YtoChase,ecx
	jmp retUpdateXy

	cmpCollisionDown:
	invoke hasSensorCollisionedBreak,ebx,downSensor
	cmp eax,1
	jl goDown
	cmp eax,2
	je BadRoute
	cmp routeCounter,1 
	je throwBombTag
	cmp routeCounter,2 
	je throwBombTag
	jmp BadRoute

	cmpCollisionLeft:
	invoke hasSensorCollisionedBreak,ebx,leftSensor
	cmp eax,1
	jl goLeft
	cmp eax,2
	je BadRoute
	cmp routeCounter,1 
	je throwBombTag
	cmp routeCounter,2 
	je throwBombTag
	jmp BadRoute

	cmpCollisionUp:
	invoke hasSensorCollisionedBreak,ebx,upSensor
	cmp eax,1
	jl goUp
	cmp eax,2
	je BadRoute
	cmp routeCounter,1 
	je throwBombTag
	cmp routeCounter,2 
	je throwBombTag
	jmp BadRoute

	cmpCollisionRight:
	invoke hasSensorCollisionedBreak,ebx,rightSensor
	cmp eax,1
	jl goRight
	cmp eax,2
	je BadRoute
	cmp routeCounter,1 
	je throwBombTag
	cmp routeCounter,2 
	je throwBombTag
	jmp BadRoute

	preffX:
	mov eax,Xpreff
	mov bestRoute1,eax
	mov eax,Ypreff
	mov bestRoute2, eax
	mov eax,Yleast
	mov bestRoute3, eax
	mov eax, Xleast
	mov bestRoute4,eax
	jmp retPreffXY

	preffY:
	mov eax,Ypreff
	mov bestRoute1,eax
	mov eax,Xpreff
	mov bestRoute2, eax
	mov eax,Xleast
	mov bestRoute3, eax
	mov eax,Yleast
	mov bestRoute4, eax
	jmp retPreffXY

	XpreffLeft:
	mov Xpreff,2 
	mov Xleast,4
	jmp changeY

	YpreffUp:
	mov Ypreff,3
	mov Yleast,1
	jmp retY

	goLeft:
		mov ebx,ofst
		mov edx,-1
		mov [ebx+28],edx
	jmp retTag
		
	goRight:
		mov ebx,ofst
		mov edx,1
		mov [ebx+28],edx
	jmp retTag

	goDown:
		mov edx,1
		mov ebx,ofst
		mov [ebx+32],edx
	jmp retTag

	goUp:
		mov ebx,ofst
		mov edx,-1
		mov [ebx+32],edx
	jmp retTag

	throwBombTag:
	mov ebx, ofst
	mov eax,0
	cmp [ebx+52],eax;;check if cooldown is lower than 0, if not return
	jge BadRoute
	invoke throwBomb,ebx
	jmp BadRoute
	
	updateSensorValues:
	mov ebx,ofst

	;up sensor
	mov eax,[ebx+20]
	mov upSensor.x,eax
	mov eax,[ebx+44]
	add upSensor.x, eax
	mov eax,[ebx+24]
	mov upSensor.y,eax
	mov eax,sensorR
	sub upSensor.y,eax

	;right sensor
	mov eax,[ebx+20]
	mov rightSensor.x,eax
	mov eax,[ebx+44]
	add eax,eax
	add eax,sensorR
	add rightSensor.x, eax
	mov eax,[ebx+24]
	mov rightSensor.y,eax
	mov eax,[ebx+44]
	add rightSensor.y,eax

	;down sensor
	mov eax,[ebx+20]
	mov downSensor.x,eax
	mov eax,[ebx+44]
	add downSensor.x, eax
	mov eax,[ebx+24]
	mov downSensor.y,eax
	mov eax,[ebx+44]
	add eax,eax
	add eax,sensorR
	add downSensor.y,eax

	;left sensor
	mov eax,[ebx+20]
	mov leftSensor.x,eax
	mov eax,sensorR
	sub leftSensor.x, eax
	mov eax,[ebx+24]
	mov leftSensor.y,eax
	mov eax,[ebx+44]
	add leftSensor.y,eax
	jmp retupdateSensorValues
greenBallMovment endp