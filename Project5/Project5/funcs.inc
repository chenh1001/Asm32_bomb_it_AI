.code
isSpeciall proc ofst: dword
	jmp isspecial
	retIsSpecial:
	mov eax,1
	ret
	retNoSpecial:
	mov eax,0
	ret
	isspecial:
		mov edi,ofst
		mov eax,0
		add eax,70
		cmp [edi+20],eax;70
		je mightSpecial
		mov eax, 770
		cmp [edi+20],eax;770
		je mightSpecial
		mov eax,0
		cmp [edi+20],eax;0
		je onSide
		mov eax,840
		cmp [edi+20],eax;840
		je onSide
		

	jmp retNoSpecial;if isnt special

	mightSpecial:; x is 770 or 70 check for y 0 or 840
	mov eax,0
	cmp [edi+24],eax;0
	je isSpecial
	add eax,840
	cmp [edi+24],eax;840
	je isSpecial
	jmp retNoSpecial

	onSide:
	mov eax,0
	cmp [edi+24],eax;0
	je isSpecial
	add eax,70
	cmp [edi+24],eax;70
	je isSpecial
	add eax,700
	cmp [edi+24],eax;770
	je isSpecial
	add eax,70
	cmp [edi+24],eax;840
	je isSpecial
	jmp retNoSpecial

	isSpecial:;if is special move to next block
	mov eax,0
	mov [edi+40],eax;CHANGE IF CHANING BREAK SIZE
	mov eax,70
	add [edi+20],eax;70
	jmp retIsSpecial

	ret
isSpeciall endp

dmootUpdateCD proc ofst:dword
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
	mov eax,MaxBombSize
	mov [ebx+48],eax
	jmp retResetBomb
dmootUpdateCD endp

explosionBombs proc ofst:dword
	mov esi, ofst
	mov ecx,ofst
	add esi,44;esi has first Bullet of the Bomb
		mov eax,0
		loopTheLoop:
			mov edx,[ecx+20]
			mov [esi+20],edx;x to x

			mov edx,[ecx+24]
			mov [esi+24],edx;y to y

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
	ret
explosionBombs endp

bombAllBombs proc ofst:dword
	mov ebx,ofst
	mov edi,BombArrStarts
	add edi,ebx ;edi now holds the offset of first bomb in arr
	mov eax,[ebx+48]
	mov esi,0
	cmp eax,esi
	je resetBombRenewTimer
	retRenewBomb:
	mov ebx,0
	loopi:
		mov eax, [edi+40];mov to eax if bomb is alive
		mov ecx,0
		cmp eax,ecx
		je notAlive
		;if IS alive invokeBOMB,  update to not alive
		invoke explosionBombs,edi
		mov eax,0
		mov [edi+40],eax
		notAlive:
		add ebx,1
		add edi, BombSize
		cmp ebx,MaxBombSize
	jl loopi
	ret

	resetBombRenewTimer:
	mov edx, bombGainTime
	mov [ebx+56],edx
	jmp retRenewBomb
bombAllBombs endp

drawBreaks proc
	mov edi, offset breakArr[0]
	mov ebx,0
	loopi:
		retIsSpecial:
		jmp checkSpecial
		retNoSpecial:
		mov ecx,0
		cmp [edi+40],ecx
		je notAlive
		invoke drd_imageDraw, edi,[edi+20],[edi+24]
		notAlive:
		add edi,BreakSize
		add ebx,1
		cmp ebx, 169
	jl loopi
	ret

	checkSpecial:
		mov eax,0
		add eax,70
		cmp [edi+20],eax;70
		je mightSpecial
		mov eax, 770
		cmp [edi+20],eax;770
		je mightSpecial
		mov eax,0
		cmp [edi+20],eax;0
		je onSide
		mov eax,840
		cmp [edi+20],eax;840
		je onSide
		

	jmp retNoSpecial;if isnt special

	mightSpecial:; x is 770 or 70 check for y 0 or 840
	mov eax,0
	cmp [edi+24],eax;0
	je isSpecial
	add eax,840
	cmp [edi+24],eax;840
	je isSpecial
	jmp retNoSpecial

	onSide:
	mov eax,0
	cmp [edi+24],eax;0
	je isSpecial
	add eax,70
	cmp [edi+24],eax;70
	je isSpecial
	add eax,700
	cmp [edi+24],eax;770
	je isSpecial
	add eax,70
	cmp [edi+24],eax;840
	je isSpecial
	jmp retNoSpecial

	isSpecial:;if is special move to next block
	mov eax,0
	mov [edi+40],eax
	mov eax,70
	add [edi+20],eax;70
	jmp retIsSpecial	
drawBreaks endp

hadCollisionedWall proc ofst:dword;;has colidided wall
	mov eax, ofst
	mov ebx, ofst
	mov ecx, [ebx+36] ;centerX
	mov edx, [ebx+ 44] ;rad
	cmp ecx,edx
	jle CLOSE_TO_WALLx1;CLOSE TO WALL X1
	mov ebx,910
	sub ebx,edx
	cmp ecx, ebx
	jge CLOSE_TO_WALLx2; CLOSE TO WALL X2
	mov ebx, ofst
	mov ecx, [ebx+40];centerY
	cmp ecx,edx
	jle CLOSE_TO_WALLy1;CLOSE TO WALL Y1
	mov ebx,910
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

hadCollisionedBreak proc offst:dword;has colided breakValues
	mov ecx,0
	mov edi,offst
	mov ebx, offset breakArr[0]
	loopi:
		mov eax,0
		cmp [ebx+40],eax
		je deadBreak
		mov eax, [ebx+28];centerX break
		mov edx, [ebx+32];centerY break
		cmp eax,[edi+36]
		jge breakGreater
		mov esi,[edi+36]
		sub esi,eax
		mov eax,esi
		retLine:
		imul eax,eax ;x*x in eax

		cmp edx,[edi+40]
		jge breakGreater2
		mov esi,[edi+40]
		sub esi,edx
		mov edx,esi
		retLine2:
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
		add ebx,BreakSize;CHANGE IF CHANING BREAK SIZE
		cmp ecx,169
		jl loopi
	ret
		breakGreater:
		sub eax, [edi+36];diff betwin centerX
		jmp retLine

		breakGreater2:
		sub edx,[edi+40]
		jmp retLine2
		
		areColiding:;are coliding, now checking from which diraction by comparing x,Y1
		;edi has offset of player
		;ebx has offset of break
		mov eax, [edi+36];player centerX
		mov edi,[edi+40];player centerY
		mov edx,[ebx+28];break centerX
		mov esi,[ebx+32];break centerY
		sub edx,eax
		cmp edx,[ebx+36]
		jge playerXsmaller
		
		mov edx,[ebx+28]
		sub eax,edx
		cmp eax,[ebx+36]
		jge playerXbigger

		sub esi,edi
		cmp esi,[ebx+36]
		jge playerYsmaller

		mov esi,[ebx+32]
		sub edi,esi
		cmp edi,[ebx+36]
		jge playerYbigger

		retX:
		cmp edi,esi;Comparing ys
		jg playerYbigger
		jmp playerYsmaller
		retY:

		jmp returnAfterColision

		playerXbigger:
		mov eax,1
		mov edx,offst
		mov [edx+28],eax
		jmp retX

		playerXsmaller:
		mov eax,-1
		mov edx,offst
		mov [edx+28],eax
		jmp retX

		playerYbigger:
		mov eax,1
		mov edi,offst
		mov [edi+32],eax
		jmp retY

		playerYsmaller:
		mov eax,-1
		mov edi,offst
		mov [edi+32],eax
		jmp retY
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
	mov edx,100
	mov [ebx+52],edx;update cooldown
	retLine:
	ret
throwBomb endp

DmootKeyboard proc ofst: dword
	mov eax,0
	mov ebx, ofst
	mov [ebx+28],eax
	mov [ebx+32],eax
	invoke hadCollisionedWall,ebx
	mov ebx,ofst
	invoke hadCollisionedBreak,ebx
	mov ebx,ofst
	invoke GetAsyncKeyState, VK_P
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
	mov [ebx+32], edx
	ret

	go_up_tag:
	mov ecx, ebx
	add ecx, 32
	mov edx,-1
	mov [ecx], edx
	ret

	go_right_tag:
	mov ecx, ebx
	add ecx, 28
	mov edx,1
	mov [ecx], edx
	ret

	go_left_tag:
	mov ecx, ebx
	add ecx, 28
	mov edx,-1
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
	invoke bombAllBombs,ebx
	jmp retBombtheBomb
	retTag:

	ret
DmootKeyboard endp

dmootCenter proc ofst:dword
	;x
	mov ecx,ofst
	mov ebx,ecx
	add ebx, 20;pos x
	mov ebx,[ebx]
	mov [ecx+36],ebx ; mov posX to centerX
	mov eax,[ecx+4] ;marioSize x 
	mov ebx, 2
	div ebx
	add [ecx+36], eax
	;y
	mov ebx,ecx
	add ebx, 24;pos Y
	mov ebx,[ebx]
	mov [ecx+40],ebx ; mov posY to centerY
	mov eax,[ecx+8] ;marioSize Y
	mov ebx, 2
	div ebx
	add [ecx+40], eax
	ret
dmootCenter endp

move proc ofst : dword
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
		cmp ebx,840
		je resetX
		add ebx,70
		retResetX:
		add esi,1
		cmp esi,169	
	jl forLine
	ret

	resetX:
		mov ebx,0
		add edx, 70
	jmp retResetX
breakValues endp

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

drawBullets proc ofst:dword
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

bulletValues proc ofst: dword
	mov ebx,ofst
	add ebx, BombArrStarts;ebx has first bomb
	mov edi,ebx
	mov eax,0
	loopi:
		mov ebx,edi
		add ebx,44;first bulet on ebx
		;mov ecx,0
		;cmp [ebx+48],ecx
		;je notalive1
		
		;down Arrow
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
		

		add ebx,BulletSize;left Arrow
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


		add ebx,BulletSize ; upArrow
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

		add ebx,BulletSize ;Right arrow
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

		notalive:
		add edi, BombSize
		add eax,1
		cmp eax,MaxBombSize
	jl loopi
	ret
bulletValues endp

bulletColosions proc ofstBomb: dword;checks for a collision in as spesific bombBullets
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

			mov esi,0;dont check if dead break
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

			cmp edx, esi;jump to collisionFunc if is in collision
			jle BulletCollisionedBreak
			retBulletCollisionedBreak:
		add edi,BreakSize
		add ecx,1
		cmp ecx, 169
		jl loopiBreaks



		add ebx, BulletSize
		add eax,1
		cmp eax,4
	jl loopi
	ret

	BulletCollisionedBreak:
	mov esi,0
	mov [edi+40],esi; set break not alive
	mov [ebx+48],esi; set bullet not alive
	jmp retBulletCollisionedBreak
bulletColosions endp

callCollision proc ofst: dword
	mov eax,ofst
	add eax, BombArrStarts;offset of first Bomb
	mov ebx,0
	loopis:
		pusha
		invoke bulletColosions,eax
		popa
		add eax, BombSize
		add ebx,1
		cmp ebx,MaxBombSize
	jl loopis
	ret
callCollision endp