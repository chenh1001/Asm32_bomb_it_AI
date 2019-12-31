include \masm32\include\masm32rt.inc
include MainFuncs.inc
include drd.inc

includelib drd.lib


.code
main PROC
	restartLine:
	invoke init
	MainLoop:
		invoke update
		invoke draw
		invoke GetAsyncKeyState, VK_R ;check if r is pressed and if is pressed check if game ended
		cmp eax,0
		jne checkForRestart
		retCheck:
	jmp MainLoop
	ret

	checkForRestart:
	cmp hasGameEnded,1
	je restartLine
	jmp retCheck
main ENDP
end main

