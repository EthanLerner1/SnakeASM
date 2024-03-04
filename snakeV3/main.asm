include \masm32\include\masm32rt.inc
include drd.inc
includelib drd.lib
include data.inc 
include main_funcs.inc

.code
	main proc
		invoke init; loading things that need to be loaded or changed only once
		loopi:
			invoke update; things that change every time the loop is starting
			invoke draw; everything that is tied to drawing
		jmp loopi
		
		ret
	main endp
end main