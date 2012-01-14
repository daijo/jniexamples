/*
 * %W% %E%
 *
 * Copyright (c) 1998 Sun Microsystems, Inc. All Rights Reserved.
 *
 * See also the LICENSE file in this distribution.
 */

/**
 * An abstraction for a C function pointer.  An instance of 
 * <code>CFunction</code>
 * repesents a pointer to some C function.  <code>callXXX</code> methods
 * provide means to call the function; select a <code>XXX</code> variant based
 * on the return type of the C function.
 *<p>
 * Beware that the <code>copyIn</code>, <code>copyOut</code>,
 * <code>setXXX</code>, and <code>getXXX</code> methods inherited from the
 * parent will indirect machine code.
 *
 * @author Sheng Liang
 * @see CPointer
 */
public class CFunction extends CPointer {
    private static final int CONV_C = 0;
    private static final int CONV_JNI = 1;

    /* calling convention */
    private int conv;

    /* Find names function in the named dll. */
    private native long find(String lib, String fname);

    private static native void initIDs();

    static {
        initIDs();
    }

    private void translateConv(String convname) {
        if (convname.equals("C")) {
	    conv = CONV_C;
	} else if (convname.equals("JNI")) {
	    conv = CONV_JNI;
	} else {
	    throw new IllegalArgumentException(
			  "unrecognized calling convention: " + convname);
	}
    }

    /**
     * Create a new <code>CFunction</code> that is linked with a C function
     * that follows a given calling convention.
     * <p>
     * The allocated instance represents a pointer to the named C function
     * from the named library, called with the named calling convention.
     * <p>
     * @param lib   library in which to find the C function
     * @param fname name of the C function to be linked with
     * @param conv  calling convention used by the C function ("C" or "JNI")
     */
    public CFunction(String lib, String fname, String conv) {
        translateConv(conv);
        peer = find(lib, fname);
    }

    /**
     * Create a new <code>CFunction</code> that is linked with a C function
     * that follows the standard "C" calling convention.
     * <p>
     * The allocated instance represents a pointer to the named C function
     * from the named library, called with the standard "C" calling
     * convention.
     * <p>
     * @param lib   library in which to find the C function
     * @param fname name of the C function to be linked with
     */
    public CFunction(String lib, String fname) {
        translateConv("C");
        peer = find(lib, fname);
    }

    /**
     * Create a new <code>CFunction</code> from a function pointer.
     * <p>
     * @param ptr   the function pointer
     * @param conv  calling convention ("C" or "JNI")
     */
    public CFunction(CPointer ptr, String conv) {
        translateConv(conv);
        peer = ptr.peer;
    }

    /**
     * Call the C function being represented by this object.
     *
     * @param  args arguments to pass to the C function
     * @return      <code>int</code> value returned by the underlying
     *		    C function
     */
    public native int callInt(Object[] args);
    
    /**
     * Call the C function being represented by this object.
     *
     * @param  args arguments to pass to the C function
     */
    public native void callVoid(Object[] args);

    /**
     * Call the C function being represented by this object.
     *
     * @param  args arguments to pass to the C function
     * @return      <code>float</code> value returned by the underlying
     *		    C function
     */
    public native float callFloat(Object[] args);

    /**
     * Call the C function being represented by this object.
     *
     * @param  args arguments to pass to the C function
     * @return      <code>double</code> value returned by the underlying
     *		    C function
     */
    public native double callDouble(Object[] args);

    /**
     * Call the C function being represented by this object.
     *
     * @param  args arguments to pass to the C function
     * @return      C pointer returned by the underlying C function
     */
    public native CPointer callCPointer(Object[] args);

    /* Don't allow creation of unitializaed CFunction objects. */
    private CFunction() {}

}
