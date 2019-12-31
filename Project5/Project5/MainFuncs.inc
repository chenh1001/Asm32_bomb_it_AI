includelib drd.lib
include drd.inc
include data.inc
include funcs.inc
.code


init proc 
	invoke drd_init, 700, 700, INIT_WINDOW
	invoke drd_imageLoadFile,offset grass,offset reka
	invoke drd_imageLoadFile,offset mario,offset player
	invoke breakValues, offset breakArr[0]
	mov edi, offset breakArr[0]
	mov ebx,0
	loopi:
		mov edx,1
		mov [edi+40],edx
		invoke drd_imageLoadFile, offset breaks,edi
		invoke drd_imageSetTransparent,edi,0ffffffffh
		add edi,BreakSize
		add ebx,1
		cmp ebx, 169
	jl loopi
	mov edi,offset player
	add edi,BombArrStarts
	mov ebx,0
	loopBomb:
		invoke drd_imageLoadFile, offset bombByte, edi
		invoke drd_imageSetTransparent,edi,0ffffffffh
		mov esi,44;bulletStartsOnBomb
		add esi,edi
		invoke drd_imageLoadFile, offset bulletDownByte, esi
		invoke drd_imageSetTransparent,esi,0ffffffffh
		add esi,BulletSize
		invoke drd_imageLoadFile, offset bulletLeftByte, esi
		invoke drd_imageSetTransparent,esi,0ffffffffh
		add esi,BulletSize
		invoke drd_imageLoadFile, offset bulletUpByte, esi
		invoke drd_imageSetTransparent,esi,0ffffffffh
		add esi,BulletSize
		invoke drd_imageLoadFile, offset bulleRightByte, esi
		invoke drd_imageSetTransparent,esi,0ffffffffh
		add edi,BombSize
		add ebx,1
		cmp ebx,MaxBombSize
	jl loopBomb

	invoke drd_imageSetTransparent,offset player,0ffffffffh
	invoke drd_getMainHwnd
	ret
init endp

update proc 
	invoke Sleep,5
	invoke callCollision,offset player
	invoke dmootUpdateCD, offset player
	invoke dmootCenter,offset player
	invoke DmootKeyboard,offset player
	invoke move, offset player
	ret
update endp


draw proc
	invoke drd_pixelsClear,0
	invoke drd_imageDraw, offset reka,0,0
	invoke drawBreaks
	invoke drawBombs,offset player
	invoke bulletValues, offset player
	invoke drawBullets,offset player
	invoke drd_imageDraw, offset player.info,player.pos.x,player.pos.y
	invoke drd_flip
	invoke drd_processMessages
	ret
draw endp
