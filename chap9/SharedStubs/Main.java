/*
 * %W% %E%
 *
 * Copyright (c) 1998 Sun Microsystems, Inc. All Rights Reserved.
 *
 * See also the LICENSE file in this distribution.
 */

/**
 * Provides a <code>main()</code> method for running the shared stubs example.
 * Shared stubs allow you to call native code (such as the standard C
 * library's <code>printf</code> function) from Java code, with very little
 * work.
 * <p>
 * This class uses our prototype implementation of the shared stubs
 * distributed with this example.
 *
 * @author Sheng Liang
 * @see    CFunction
 * @see	   CPointer
 * @see    CMalloc
 */
public class Main {

    /**
     * Demonstrates calling <code>printf</code>, <code>scanf</code>, etc from
     * the C library using shared stubs.
     * <p>
     * Note that this is an example demonstrating the use of shared stubs, and
     * must not be construed as us encouraging the use of <code>printf</code>
     * from Java code!  Infact, we strongly discourage you from doing so ---
     * the Java platform APIs provide powerful, type-safe and portable
     * alternatives for these C functions, and the Java versions will be a
     * performance win.  We hope that shared stubs will be useful to you if
     * you absolutely must write native methods.
     *<p>
     * Output from C's printf is enclosed in <>, to distinguish it from things
     * we print with System.out.println.
     *
     * @param main_args Arguments passed from the command line. Currently
     * 			unused.
     */
    public static void main(String[] main_args) {

	/* Which OS are we running on? */
	String osName = System.getProperty("os.name");
	String libc, libm;
	/* JDK1.1 returns "Solaris", 1.2 returns "SunOS". */
	if (osName.equals("SunOS") || osName.equals("Solaris")) {
	    libc = "libc.so";
	    libm = "libm.so";
	} else {
	    libc = libm = "msvcrt.dll";		  // Win32
	}

	try {
	    // Popup a message box on Win32
	    CFunction messageBox = new CFunction("user32.dll", "MessageBoxA", "JNI");
	    messageBox.callInt(new Object[] {new Integer(0), 
					     "Click OK to continue",
					     "Testing MessageBox",
					     new Integer(0)});
	} catch (UnsatisfiedLinkError e) {
	    // We'll get here on Solaris.
	}
	
	/* Printing a message with printf(). Note that we first create an
	   instance of CFunction that wraps around C's printf(). Then we do the
	   actual call, dispatching to one of the "callXXX" methods on this
	   instance, based on the return type; in this case it happens to be
	   callInt.  Arguments to the C function are passed as an array of
	   Objects.  Notice we use the "anonymous array creation" syntax for
	   creating the array of arguments. */
	CFunction printf = new CFunction(libc, "printf");
	int ires = printf.callInt(new Object[]
		  {"\n<output from printf(): Running %s, eh?>\n", osName});
	System.out.println("printf() returned " + ires);

	
	/* Call time() with a NULL pointer. */
	CFunction time = new CFunction(libc, "time");
	ires = time.callInt(new Object[]{ null });
	System.out.println("\ntime() reports seconds since 1/1/70 as " + ires);


	/* Little more complicated.  Firstly, ctime() takes a "int *" which
           points to an int containing the elapsed seconds since the epoch.
           So we will malloc() a 4 byte chunk with C's malloc, and initialize
           it the value we just got from time().  Secondly, ctime() returns a
           string, so be aware of that. */
	CMalloc timePtr = new CMalloc(4);
	try {
	    timePtr.setInt(0, ires);
	    CFunction ctime = new CFunction(libc, "ctime");
	    CPointer result = ctime.callCPointer(new Object[]{ timePtr });
	    System.out.print("\nctime() reports " + result.getString(0));
	} finally {
	    /* We malloc()ed something from C heap, so we have to free() it. */
	  timePtr.free();
	}

	/* Read first word from stdin with scanf(). */
	System.out.println("\nPlease type something and then hit <return>");
	CFunction scanf = new CFunction(libc, "scanf");
	CMalloc cbuf = new CMalloc(128);
	try {
	    ires = scanf.callInt(new Object[]{ "%s", cbuf });
	    System.out.println("scanf() says first word you typed is \"" +
			       cbuf.getString(0) + "\"");
	} finally {
	    /* malloc()ed memory must be freed! */
	    cbuf.free();
	}

	/* Caculate C's sin(2.0) with Math.sin(2.0). */
	CFunction sin = new CFunction(libm, "sin");
	double dres = sin.callDouble(new Object[]{new Double(2.0) });
	System.out.println("\nC's  sin(2.0) = " + dres);
	System.out.println("Math.sin(2.0) = " + Math.sin(2.0));
	

	/* clock().  Takes no arguments. */
	CFunction clock = new CFunction(libc, "clock");
	System.out.println("\nclock() returned " + 
			   clock.callInt(new Object[0]));
    }
}

class Win32 {
    private static CFunction c_CreateFile = 
        new CFunction ("kernel32.dll",   // native library name
                       "CreateFileA",    // native function
                       "JNI");           // calling convention

    public static int CreateFile(
        String fileName,          // file name
        int desiredAccess,        // access (read-write) mode 
        int shareMode,            // share mode 
        int[] secAttrs,           // security attributes 
        int creationDistribution, // how to create 
        int flagsAndAttributes,   // file attributes 
        int templateFile)         // file with attr. to copy
    {
        CMalloc cSecAttrs = null;
        if (secAttrs != null) {
            cSecAttrs = new CMalloc(secAttrs.length * 4);
            cSecAttrs.copyIn(0, secAttrs, 0, secAttrs.length);
        }
        try {
            return c_CreateFile.callInt(new Object[] {
                           fileName,
                           new Integer(desiredAccess),
                           new Integer(shareMode),
                           cSecAttrs,
                           new Integer(creationDistribution),
                           new Integer(flagsAndAttributes),
                           new Integer(templateFile)});
        } finally {
            if (secAttrs != null) {
                cSecAttrs.free();
            }
        }
    }
}

class C {
    private static CFunction c_atol =
        new CFunction("msvcrt.dll", // native library name
                      "atol",       // C function name
                      "C");         // calling convention
    public static int atol(String str) {
        return c_atol.callInt(new Object[] {str});
    }
}
