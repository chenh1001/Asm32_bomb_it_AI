.const
DmootSize=1440
BreakSize=72
BombSize=272
BulletSize=56
BombArrStarts=80
MaxBombSize=5
bombGainTime=300
lengthOfBreaksArr=100
PowerUpSize=48
NumberOfPowerUps=5;total with size of prob
probabilityOfPowerUp=3
Lives=5
bulletRangeGrowing=70
GameSpeed=15 ; change to diffrent pcs cause visual studio runs in diffrent speeds
sensorR=13

numberOfPlayers=4
closeLinesConst=1600
bombTimer=400
cooldownToBombConst=200
.data
grass byte "grassy.png",0
blueballByte byte "blueBall.bmp",0
redballByte byte "redBall.bmp",0
greenBallByte byte "player3.bmp",0
breaks byte "break.bmp",0
breaksNotDestructible byte "unDestructibleBreak.bmp",0
bombByte byte "bomb50.bmp",0
bulletDownByte byte "bulletDown.bmp",0
bulletLeftByte byte "bulletLeft.bmp",0
bulletUpByte byte "bulletUp.bmp",0
bulleRightByte byte "bulletRight.bmp",0
player2Won byte "Player2Won.bmp",0;450*110
player1Won byte "player1Won.bmp",0;450*110
lightningByte byte "lightning.bmp",0;70*70
speedBoostByte byte "speedBoost.bmp",0;70*70
addBombByte byte "addBomb.bmp",0;70*70
yellowBall byte "yellowBall.bmp",0

player1Lives byte "player1       lives: ",0
player1LivesCounter byte 64 dup(0)
player1Bombs byte " bombs available:  ",0
player1BombsCount byte 64 dup(0)

player2Lives byte "                        player2       lives: ",0
player2LivesCounter byte 64 dup(0)
player2Bombs byte " bombs available: ",0
player2BombsCount byte 64 dup(0)

bigBuffer byte 2048 dup(0)

;locals
Counter dword ?
hasGameEnded dword ?
hasColidedBreak dword ?

reka Img<>

testDot struct 
	info Img<>;0
	pos POINT<0,0>;20
testDot ends

Bullet struct ;35*35
	info Img<>;0
	pos POINT<-50,-50>;20
	center POINT<>;28
	origin POINT<>;36
	speed dword 5;44
	isAlive dword 0;48
	power dword 100;52
Bullet ends; TOTAL 56

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
	timer dword 600;268
Bomb ends

Player struct ;50*50
	info Img<>;0
	pos POINT<0,0>;20
	dir POINT<>;28
	center POINT<>;36
	rad dword 25; 44
	bombsAvailable dword MaxBombSize;48
	cooldownToBomb dword cooldownToBombConst;52
	cooldownToNewBomb dword 0;56
	lives dword 5;60
	buletPower dword 50;64
	speedFactor dword 0;68
	maxBombsAtOnce dword 1;72
	isBot dword 0;76
	bombArr Bomb MaxBombSize dup(<>);80
Player ends

Break struct;70*70
	info Img<>;0
	pos POINT<>;20
	center POINT<>;28
	rad dword 38;36 ;was 28
	isAlive dword 1;40
	isDestructible dword 0;44
	powerUp dword 0;48
	infoPowerUpImg Img<>;52
Break ends

PlayerWonScreens struct
	playerWonScreen Img<>;0
	isAlive dword 0;20
PlayerWonScreens ends

breakArr Break lengthOfBreaksArr dup(<>) 
players Player <>,<>,<>,<>
player2wonScreen PlayerWonScreens<>
player1wonScreen PlayerWonScreens<>
testy testDot<>


