include \masm32\include\masm32rt.inc
include MainFuncs.inc
include drd.inc

includelib drd.lib


.code
main PROC
	invoke init
	MainLoop:
		invoke update
		invoke draw
	jmp MainLoop
	ret
main ENDP
end main

