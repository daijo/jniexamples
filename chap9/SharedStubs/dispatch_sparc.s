/*
 * %W% %E%
 *
 * Copyright (c) 1998 Sun Microsystems, Inc. All Rights Reserved.
 *
 * See also the LICENSE file in this distribution.
 */

	.section	".text",#alloc,#execinstr
	.file		"dispatch_sparc.s"

	.section	".text",#alloc,#execinstr
	.align	4
	
	.global asm_dispatch

!
! Copies the arguments from the given array to C stack, invoke the
! target function, and copy the result back.
!
! Arguments:
!
! i0		function
! i1		# of words in arguments
! i2		argument types, ignored
! i3		arguments
! i4		return type
! i5		address to store return value
! [sp+92]	calling convention, must be set to 0
!

asm_dispatch:

! Set up a big enough stack frame (and make sure it's 8-byte aligned):

	subcc	%o1,6,%g1	! We can freely use %g1.
	bpos	more_than_6_args
	nop
	mov	%g0,%g1

more_than_6_args:
	add	%g1,1,%g1
	sra	%g1,1,%g1
        sll	%g1,3,%g1
	sub	%g0,%g1,%g1	! negate %g1
        sub	%g1,96,%g1
	save	%sp,%g1,%sp

! transferring arguments
!
! first 6 in register

        ld      [%i3],%o0
        deccc   %i1
        ble     args_done       ! this handles #args == 0 case too.
        add     %i3,4,%i3
	
        ld      [%i3],%o1
        deccc   %i1
        be      args_done
        add     %i3,4,%i3

        ld      [%i3],%o2
        deccc   %i1
        be      args_done
        add     %i3,4,%i3

        ld      [%i3],%o3
        deccc   %i1
        be      args_done
        add     %i3,4,%i3

        ld      [%i3],%o4
        deccc   %i1
        be      args_done
        add     %i3,4,%i3

        ld      [%i3],%o5
        deccc   %i1
        be      args_done
        add     %i3,4,%i3

! push the remaining arguments onto the stack

	add	%sp,92,%l0
args_loop:
	ld	[%i3],%l3
	deccc	%i1
	st	%l3,[%l0]
	add	%l0,4,%l0
	bne	args_loop
	add	%i3,4,%i3

args_done:

	jmpl	%i0,%o7
	nop
	
	set	ret_jumps, %l1	
	sll	%i4,2,%i4
	
	ld	[%l1 + %i4],%g1		! thread the return address to the
	jmp	%g1			! proper code for our return type
	nop

ret_f64:	
	st      %f0,[%i5]
        st      %f1,[%i5 + 4]
        ret
        restore %g0,%g0,%o0

ret_f32:
	st      %f0,[%i5]
	ret
        restore %g0,%g0,%o0

ret_i32:
	st	%o0,[%i5]
	ret
        restore %g0,%g0,%o0

ret_p64:
	st	%g0,[%i5]
	st	%o0,[%i5 + 4]
	ret
        restore %g0,%g0,%o0

ret_jumps:		
	.word	ret_p64
	.word	ret_i32
	.word	ret_f32
	.word	ret_f64

	.type	asm_dispatch,2
	.size	asm_dispatch,(.-asm_dispatch)
