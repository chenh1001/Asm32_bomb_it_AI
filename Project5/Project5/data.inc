.const
DmootSize=280
BreakSize=44
BombSize=252
BulletSize=52
BombArrStarts=60
MaxBombSize=5
bombGainTime=400

.data
grass byte "grassy.png",0
mario byte "blueBall.bmp",0
breaks byte "break.bmp",0
bombByte byte "bomb50.bmp",0
bulletDownByte byte "bulletDown.bmp",0
bulletLeftByte byte "bulletLeft.bmp",0
bulletUpByte byte "bulletUp.bmp",0
bulleRightByte byte "bulletRight.bmp",0

reka Img<>

Bullet struct ;35*35
	info Img<>;0
	pos POINT<>;20
	center POINT<>;28
	origin POINT<>;36
	speed dword 2;44
	isAlive dword 0;48
Bullet ends

Bomb struct;50*50
	info Img<>;0
	pos POINT<>;20
	center POINT<>;28
	rad dword 25;36
	isAlive dword 0;40
	downBullet Bullet <>;44
	leftBullet Bullet <>;+buleltsize
	upBullet Bullet <>;+buleltsize
	rightBullet Bullet <>;+buleltsize
Bomb ends

Dmoot struct ;50*50
	info Img<>;0
	pos POINT<0,0>;20
	dir POINT<>;28
	center POINT<>;36
	rad dword 25; 44
	bombsAvailable dword MaxBombSize;48
	cooldownToBomb dword 100;52
	cooldownToNewBomb dword 0;56
	bombArr Bomb MaxBombSize dup(<>);60
Dmoot ends

Break struct;70*70
	info Img<>;0
	pos POINT<>;20
	center POINT<>;28
	rad dword 35;36
	isAlive dword 1;40
Break ends



breakArr Break 169 dup(<>)
player Dmoot<>



