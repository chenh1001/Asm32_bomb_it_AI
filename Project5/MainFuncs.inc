includelib drd.lib
include drd.inc
include data.inc
include funcs.inc
.code


init proc ;gives all init values for every object and imageloadFiles all
	invoke drd_init, 700, 700, 0;open window
	mov hasGameEnded,0

	mov eax, offset player2wonScreen
	mov ebx,0
	mov [eax+20],ebx ;set screens as not alive
	invoke drd_imageLoadFile,offset player2Won, offset player2wonScreen ;set screens
	invoke drd_imageSetTransparent,offset player2wonScreen,0ffffffffh
	mov eax, offset player1wonScreen
	mov [eax+20],ebx
	invoke drd_imageLoadFile, offset player1Won, offset player1wonScreen
	invoke drd_imageSetTransparent,offset player1wonScreen,0ffffffffh
	
	invoke drd_imageLoadFile,offset grass,offset reka ;set reka
	
	mov ebx, offset players[0]
	invoke drd_imageLoadFile,offset blueballByte,ebx ;set players
	invoke drd_imageSetTransparent,ebx,0ffffffffh

	add ebx,DmootSize
	invoke drd_imageLoadFile,offset redballByte,ebx
	invoke drd_imageSetTransparent,ebx,0ffffffffh
	
	add ebx,DmootSize
	invoke drd_imageLoadFile, offset greenBallByte, ebx
	invoke drd_imageSetTransparent,ebx,0ffffffffh

	add ebx,DmootSize
	invoke drd_imageLoadFile, offset yellowBall, ebx
	invoke drd_imageSetTransparent,ebx,0ffffffffh

	mov eax, offset players[0]
	mov ecx,1
	
	loopiPlayers:
		cmp ecx,1
		je updatePos1
		cmp ecx,2 
		je updatePos2
		cmp ecx,3
		je updatePos3
		cmp ecx,4
		je updatePos4
		retUpdatePos:
		
		mov ebx,Lives
		mov [eax+60],ebx;resetlives
		mov ebx,1
		mov [eax+48],ebx;reset bombs availible
		mov ebx,100
		mov	[eax+64],ebx ;reset bullet power
		mov ebx,0
		mov [eax+68],ebx; reset speed factor
		mov ebx,1
		mov [eax+72],ebx; reset max bombs at once
		mov ebx,0
		mov [eax+56],ebx ;reset cd to bomb

		add ecx,1
		add eax,DmootSize
		cmp ecx,numberOfPlayers
	jle loopiPlayers

	invoke breakValues, offset breakArr[0] ;reset breaks info
	mov edi, offset breakArr[0]
	mov ebx,0
	loopi:
		mov edx,0 ;set with no powerUp
		mov [edi+48],edx		
		mov edx,1 ;set as alive
		mov [edi+40],edx

		pusha
		invoke isSpecialFunc,edi ;check if is special if yes continue to next break 
		cmp eax,1
		je isSpecialLine
		popa

		;Checking if X of break%2 is 1 to check if is destructible
		mov eax,[edi+20];breakPosX into ecx
		xor edx,edx
		mov ecx,70;BREAK SIZE CHANGE IN NEED 
		div ecx
		mov ecx,2
		xor edx,edx
		div ecx
		mov eax, edx; now x%y is in eax
		cmp eax,1
		je isntDestructibleMaybe
		retISdestructable:

		invoke Random,probabilityOfPowerUp ;random if has powerUp inside or not and if yes randoms which powerUp
		cmp eax,0
		je hasPowerUp
		retPowerUp:

		mov eax,1;set as destructible
		mov [edi+44],eax
		invoke drd_imageLoadFile, offset breaks,edi
		retisntDestructible:

		mov ecx,edi
		add ecx,52
		invoke drd_imageSetTransparent,edi,0ffffffffh
		add edi,BreakSize
		add ebx,1
		cmp ebx, lengthOfBreaksArr
	jl loopi
	mov eax,0
	mov edi,offset players[0]
	add edi,BombArrStarts
	mov esi,edi
	loopPlayers:
		mov ebx,0
		mov edi,esi
		loopBomb: ; reset bombs 
			pusha
			invoke drd_imageLoadFile, offset bombByte, edi
			mov ecx, 0
			mov [edi+40],ecx
			invoke drd_imageSetTransparent,edi,0ffffffffh
			mov esi,44;bulletStartsOnBomb
			add esi,edi
			mov ecx, 0
			mov [esi+48],ecx
			invoke drd_imageLoadFile, offset bulletDownByte, esi
			invoke drd_imageSetTransparent,esi,0ffffffffh
			add esi,BulletSize
			mov ecx, 0
			mov [esi+48],ecx
			invoke drd_imageLoadFile, offset bulletLeftByte, esi
			invoke drd_imageSetTransparent,esi,0ffffffffh
			add esi,BulletSize
			mov ecx, 0
			mov [esi+48],ecx
			invoke drd_imageLoadFile, offset bulletUpByte, esi
			invoke drd_imageSetTransparent,esi,0ffffffffh
			add esi,BulletSize
			mov ecx, 0
			mov [esi+48],ecx
			invoke drd_imageLoadFile, offset bulleRightByte, esi
			invoke drd_imageSetTransparent,esi,0ffffffffh
			popa
			
			add edi,BombSize
			add ebx,1
			cmp ebx,MaxBombSize
		jl loopBomb
		add eax,1
		add esi, DmootSize
		cmp eax, numberOfPlayers
	jl loopPlayers


	invoke drd_getMainHwnd
	
	
	invoke drd_imageLoadFile, offset yellowBall,offset testy
	invoke drd_imageSetTransparent,offset testy,0ffffffffh
	ret

	updatePos1:
	mov ebx,0
	mov [eax+20],ebx;reset x
	mov ebx,0
	mov [eax+24],ebx;reset y
	jmp retUpdatePos

	updatePos2:
	mov ebx,650
	mov [eax+20],ebx;reset x
	mov ebx,650
	mov [eax+24],ebx;reset y
	jmp retUpdatePos

	updatePos3:
	mov ebx,0
	mov [eax+20],ebx;reset x
	mov ebx,650
	mov [eax+24],ebx;reset y
	jmp retUpdatePos

	updatePos4:
	mov ebx,650
	mov [eax+20],ebx;reset x
	mov ebx,0
	mov [eax+24],ebx;reset y
	jmp retUpdatePos


	isSpecialLine:
	invoke drd_imageLoadFile, offset breaks,edi;hasSomething Loaded ;doesnt matter cause isnt alive allways
	popa
	jmp retisntDestructible ;popa only if is special, jump to end of loop 

	isntDestructibleMaybe:;Checking if Y of break%2 is 0
	mov eax,[edi+24];breakPosX into ecx
	xor edx,edx
	mov ecx,70;BREAK SIZE CHANGE IN NEED 
	div ecx
	mov ecx,2
	xor edx,edx
	div ecx
	mov eax, edx; now x%y is in eax
	cmp eax,0
	je isntDestructible
	jmp retISdestructable

	isntDestructible:
	mov eax,0
	mov [edi+44],eax
	invoke drd_imageLoadFile,offset breaksNotDestructible,edi
	jmp retisntDestructible


	;; PROB DEVIDE:
	;thunder: 1,2  
	;speed: 3
	;add bomb: 4,5

	hasPowerUp:
	invoke Random, NumberOfPowerUps;random powerUp number in eax(randoms which powerUp does the break hold)
	add eax,1;be 1 + no 0
	cmp eax,1;loadFile the right image for the right powerUp
	je loadThunder
	cmp eax,2
	je loadThunder
	cmp eax,3
	je loadSpeedBoost
	cmp eax,4
	je addBombAvialiable
	cmp eax,5
	je addBombAvialiable
	jmp retPowerUp

	loadThunder:
	mov eax,1
	mov [edi+48],eax
	mov ecx,edi;loadFile the powerUp if has 1
	add ecx,52
	invoke drd_imageLoadFile,offset lightningByte,ecx
	mov ecx,edi
	add ecx,52
	invoke drd_imageSetTransparent,ecx,0ffffffffh
	jmp retPowerUp

	loadSpeedBoost:
	mov eax,2
	mov [edi+48],eax
	mov ecx,edi;loadFile the powerUp if has 2
	add ecx,52
	invoke drd_imageLoadFile,offset speedBoostByte,ecx
	mov ecx,edi
	add ecx,52
	invoke drd_imageSetTransparent,ecx,0ffffffffh
	jmp retPowerUp

	addBombAvialiable:
	mov eax,3
	mov [edi+48],eax
	mov ecx,edi;loadFile the powerUp if has 3
	add ecx,52
	invoke drd_imageLoadFile,offset addBombByte,ecx
	mov ecx,edi
	add ecx,52
	invoke drd_imageSetTransparent,ecx,0ffffffffh
	jmp retPowerUp

init endp

update proc ;
	add Counter,1 ;go update only once in GameSpeed
	mov eax, Counter 
	invoke modulu, eax, GameSpeed
	cmp eax , 0
	jne dontUpdate

	cmp hasGameEnded,1
	je dontUpdate

	startUpdate: ; call necesery funcs
	mov eax,1
	mov ebx, offset players[0]
	loopi:
		mov edi,0
		cmp [ebx+60],edi 
		jle dontUpdatePlayer
		pusha
		invoke bombValue,ebx
		popa
		pusha
		invoke bulletValues, ebx
		popa
		pusha
		invoke dmootCenter,ebx
		popa
		
		pusha
		invoke callCollision,ebx ;funcs player1
		popa
		pusha
		invoke dmootUpdateCD, ebx
		popa
		pusha
		invoke hasCollisionedPower, ebx
		popa
	
		pusha
		cmp eax,1 
		je dmoot1Keyboard
		cmp eax,2
		je dmoot2Keyboard
		cmp eax,3
		je isBot
		cmp eax,4 
		je isBot
		
		DmootKeyboardRet:
		popa
		pusha
		invoke move, ebx
		popa
		

		dontUpdatePlayer:
		add eax,1
		add ebx, DmootSize
		cmp eax,numberOfPlayers
	jle loopi

	invoke drawTitle
	invoke checkForEndGame

	ret
	dmoot1Keyboard:
	mov edx,1
	cmp [ebx+76],edx
	je isBot
	invoke DmootKeyboard,ebx
	jmp DmootKeyboardRet

	dmoot2Keyboard:
	mov edx,1
	cmp [ebx+76],edx
	je isBot
	invoke DmootKeyboard2,ebx
	jmp DmootKeyboardRet

	isBot: ;botFuncs
		invoke greenBallMovment,ebx
	jmp DmootKeyboardRet

	dontUpdate:
	ret
update endp


draw proc
	invoke drd_pixelsClear,0
	invoke drd_imageDraw, offset reka,0,0
	invoke drawBreaks
	invoke drawPowerUps

	mov ebx, offset players[0]
	mov eax,0 
	loopiPlayers:
		mov edi,0
		cmp [ebx+60],edi 
		jle dontUpdatePlayer
		pusha
		invoke drawBombs,ebx
		popa
		pusha
		invoke drawBullets,ebx
		popa
		pusha
		mov eax,[ebx+20]
		mov ecx,[ebx+24]
		invoke drd_imageDraw, ebx,eax,ecx
		popa
		dontUpdatePlayer:
		add eax,1
		add ebx,DmootSize
		cmp eax,numberOfPlayers
	jl loopiPlayers

	;mov eax, offset testy
	;invoke drd_imageDraw,eax,[eax+20],[eax+24]
	invoke drawScreens
	invoke drd_flip
	invoke drd_processMessages

	ret
draw endp
