/*
 * %W% %E%
 *
 * Copyright (c) 1998 Sun Microsystems, Inc. All Rights Reserved.
 *
 * See also the LICENSE file in this distribution.
 */

/*
 * The implementation here is more general than what is described
 * in the JNI book in handling return types other than "int". You
 * can, however, define the JNI_BOOK constant to compile in the
 * asm_dispatch_int implementation that is exactly the same as the
 * asm_dispatch implementation described in the JNI book.
 */

#ifdef JNI_BOOK

int asm_dispatch_int(void *func,  // pointer to the C function
                     int nwords,  // number of words in args array
                     long *args,  // start of the argument data
                     int conv)    // calling convention 0: C
                                  //                    1: JNI
{
    __asm {
        mov esi, args
        mov edx, nwords
        // word address -> byte address
        shl edx, 2
        sub edx, 4
        jc  args_done

        // push the last argument first
    args_loop:
        mov eax, DWORD PTR [esi+edx]
        push eax
        sub edx, 4
        jge SHORT args_loop

    args_done:
        call func

        // check for calling convention
        mov edx, conv
        or edx, edx
        jnz jni_call

        // pop the arguments
        mov edx, nwords
        shl edx, 2
        add esp, edx
    jni_call:
        // done, return value in eax
    }
}

#endif /* JNI_BOOK */

/*
 * Copies the arguments from the given array to C stack, invoke the
 * target function, and copy the result back.
 */
void asm_dispatch(void *func,
		  int nwords,
		  char *arg_types,
		  long *args,
		  int res_type,
		  long *resP,
		  int conv)
{
    __asm {

	mov esi, args
	mov edx, nwords
	// word address -> byte address
	shl edx, 2

        sub edx, 4
	jc  args_done

	// Push the last argument first.
    args_loop:
	mov eax, DWORD PTR [esi+edx]
	push eax
	sub edx, 4
	jge SHORT args_loop

    args_done:
	call func

	mov edx, conv
        or edx, edx
	jnz is_stdcall

	// pop arguments
	mov edx, nwords
        shl edx, 2
        add esp, edx

is_stdcall:
        mov esi, resP
	mov edx, res_type
	dec edx
	jge not_p64

	// p64
	mov [esi], eax
	mov [esi+4], 0
	jmp done

    not_p64:
	dec edx
        jge not_i32

	// i32
	mov [esi], eax
        jmp done

    not_i32:
	dec edx
        jge not_f32

        // f32
	fstp DWORD PTR [esi]
	jmp done

    not_f32:
        // f64
	fstp QWORD PTR [esi]

    done:
    }
}
