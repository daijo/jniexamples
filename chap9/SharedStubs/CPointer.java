/*
 * %W% %E%
 *
 * Copyright (c) 1998 Sun Microsystems, Inc. All Rights Reserved.
 *
 * See also the LICENSE file in this distribution.
 */

/**
 * An abstraction for a C pointer data type.  A CPointer instance represents,
 * on the Java side, a C pointer.  The C pointer could be any <em>type</em>
 * of C pointer.  Methods such as <code>copyIn</code>, <code>copyOut</code>,
 * <code>getXXX</code>, and <code>setXXX</code>, provide
 * means to indirect the underlying C pointer. 
 *
 * @author Sheng Liang
 * @see    CFunction
 */
public class CPointer {

    /**
     * The size of a C pointer on the platform this Java virtual machine is
     * running on.
     */
    public static final int SIZE;
    
    /**
     * Compares this <code>CPointer</code> to the specified object.
     *
     * @param other a <code>CPointer</code>
     * @return      true if the class of this <code>CPointer</code> object 
     *              and the class of <code>other</code> are exactly equal, 
     *              and the C pointers being pointed to by these objects are
     *              also equal. Returns false otherwise.
     */
    public boolean equals(Object other) {
        if (other == null)
	    return false;
	if (other == this)
	    return true;
	if (CPointer.class != other.getClass())
	    return false;
	return peer == ((CPointer)other).peer;
    }

    /**
     * Returns a hashcode for the C pointer represented by this
     * <code>Cptr</code> object.
     *
     * @return a hash code value for the represented C pointer.
     */
    public int hashCode() {
        return (int)((peer >>> 32) + (peer & 0xFFFFFFFF));
    }

    /**
     * Indirect the C pointer, copying <em>into</em> memory pointed to by C
     * pointer, from the specified array.
     *
     * @param bOff   byte offset from pointer into which data is copied
     * @param buf    <code>byte</code> array from which to copy
     * @param index  array index from which to start copying
     * @param length number of elements from <code>buf</code> that must be
     *               copied
     */
    public native void copyIn(int bOff, byte[] buf, int index, int length);

    /**
     * Indirect the C pointer, copying <em>into</em> memory pointed to by C
     * pointer, from the specified array.
     *
     * @param bOff   byte offset from pointer into which data is copied
     * @param buf    <code>short</code> array from which to copy
     * @param index  array index from which to start copying
     * @param length number of elements from <code>buf</code> that must be
     *               copied
     */
    public native void copyIn(int bOff, short[] buf, int index, int length);

    /**
     * Indirect the C pointer, copying <em>into</em> memory pointed to by C
     * pointer, from the specified array.
     *
     * @param bOff   byte offset from pointer into which data is copied
     * @param buf    <code>char</code> array from which to copy
     * @param index  array index from which to start copying
     * @param length number of elements from <code>buf</code> that must be
     *               copied
     */
    public native void copyIn(int bOff, char[] buf, int index, int length);

    /**
     * Indirect the C pointer, copying <em>into</em> memory pointed to by C
     * pointer, from the specified array.
     *
     * @param bOff   byte offset from pointer into which data is copied
     * @param buf    <code>int</code> array from which to copy
     * @param index  array index from which to start copying
     * @param length number of elements from <code>buf</code> that must be
     *               copied
     */
    public native void copyIn(int bOff, int[] buf, int index, int length);

    /**
     * Indirect the C pointer, copying <em>into</em> memory pointed to by C
     * pointer, from the specified array.
     *
     * @param bOff   byte offset from pointer into which data is copied
     * @param buf    <code>long</code> array from which to copy
     * @param index  array index from which to start copying
     * @param length number of elements from <code>buf</code> that must be
     *               copied
     */
    public native void copyIn(int bOff, long[] buf, int index, int length);

    /**
     * Indirect the C pointer, copying <em>into</em> memory pointed to by C
     * pointer, from the specified array.
     *
     * @param bOff   byte offset from pointer into which data is copied
     * @param buf    <code>float</code> array from which to copy
     * @param index  array index from which to start copying
     * @param length number of elements from <code>buf</code> that must be
     *               copied
     */
    public native void copyIn(int bOff, float[] buf, int index, int length);

    /**
     * Indirect the C pointer, copying <em>into</em> memory pointed to by C
     * pointer, from the specified array.
     *
     * @param bOff   byte offset from pointer into which data is copied
     * @param buf    <code>double</code> array from which to copy
     * @param index  array index from which to start copying
     * @param length number of elements from <code>buf</code> that must be
     *               copied
     */
    public native void copyIn(int bOff, double[] buf, int index, int length);

    /**
     * Indirect the C pointer, copying <em>from</em> memory pointed to by C
     * pointer, into the specified array.
     *
     * @param bOff   byte offset from pointer into which data is copied
     * @param buf    <code>byte</code> array into which data is copied
     * @param index  array index from which to start copying
     * @param length number of elements from C pointer that must be copied
     */
    public native void copyOut(int bOff, byte[] buf, int index, int length);

    /**
     * Indirect the C pointer, copying <em>from</em> memory pointed to by C
     * pointer, into the specified array.
     *
     * @param bOff   byte offset from pointer from which data is copied
     * @param buf    <code>short</code> array into which data is copied
     * @param index  array index to which data is copied
     * @param length number of elements from C pointer that must be copied
     */
    public native void copyOut(int bOff, short[] buf, int index, int length);

    /**
     * Indirect the C pointer, copying <em>from</em> memory pointed to by C
     * pointer, into the specified array.
     *
     * @param bOff   byte offset from pointer from which data is copied
     * @param buf    <code>char</code> array into which data is copied
     * @param index  array index to which data is copied
     * @param length number of elements from C pointer that must be copied
     */
    public native void copyOut(int bOff, char[] buf, int index, int length);

    /**
     * Indirect the C pointer, copying <em>from</em> memory pointed to by C
     * pointer, into the specified array.
     *
     * @param bOff   byte offset from pointer from which data is copied
     * @param buf    <code>int</code> array into which data is copied
     * @param index  array index to which data is copied
     * @param length number of elements from C pointer that must be copied
     */
    public native void copyOut(int bOff, int[] buf, int index, int length);

    /**
     * Indirect the C pointer, copying <em>from</em> memory pointed to by C
     * pointer, into the specified array.
     *
     * @param bOff   byte offset from pointer from which data is copied
     * @param buf    <code>long</code> array into which data is copied
     * @param index  array index to which data is copied
     * @param length number of elements from C pointer that must be copied
     */
    public native void copyOut(int bOff, long[] buf, int index, int length);

    /**
     * Indirect the C pointer, copying <em>from</em> memory pointed to by C
     * pointer, into the specified array.
     *
     * @param bOff   byte offset from pointer from which data is copied
     * @param buf    <code>float</code> array into which data is copied
     * @param index  array index to which data is copied
     * @param length number of elements from C pointer that must be copied
     */
    public native void copyOut(int bOff, float[] buf, int index, int length);

    /**
     * Indirect the C pointer, copying <em>from</em> memory pointed to by C
     * pointer, into the specified array.
     *
     * @param bOff   byte offset from pointer from which data is copied
     * @param buf    <code>double</code> array into which data is copied
     * @param index  array index to which data is copied
     * @param length number of elements from C pointer that must be copied
     */
    public native void copyOut(int bOff, double[] buf, int index, int length);

    /**
     * Indirect the C pointer as a pointer to <code>byte</code>.  This is
     * equivalent to the expression 
     * <code>*((jbyte *)((char *)cptr + * offset))</code>.
     *
     * @param offset offset from pointer to perform the indirection
     * @return the <code>byte</code> value being pointed to
     */
    public native byte getByte(int offset);

    /**
     * Indirect the C pointer as a pointer to <code>short</code>.  This is
     * equivalent to the expression
     * <code>*((jshort *)((char *)cptr + offset))</code>.
     *
     * @param offset byte offset from pointer to perform the indirection
     * @return the <code>short</code> value being pointed to
     */
    public native short getShort(int offset);

    /**
     * Indirect the C pointer as a pointer to <code>int</code>.  This is
     * equivalent to the expression
     * <code>*((jint *)((char *)cptr + offset))</code>.
     *
     * @param offset byte offset from pointer to perform the indirection
     * @return the <code>int</code> value being pointed to
     */
    public native int getInt(int offset);

    /**
     * Indirect the C pointer as a pointer to <code>long</code>.  This is
     * equivalent to the expression
     * <code>*((jlong *)((char *)cptr + offset))</code>.
     *
     * @param offset byte offset from pointer to perform the indirection
     * @return the <code>long</code> value being pointed to
     */
    public native long getLong(int offset);

    /**
     * Indirect the C pointer as a pointer to <code>float</code>.  This is
     * equivalent to the expression
     * <code>*((jfloat *)((char *)cptr + offset))</code>.
     *
     * @param offset byte offset from pointer to perform the indirection
     * @return the <code>float</code> value being pointed to
     */
    public native float getFloat(int offset);

    /**
     * Indirect the C pointer as a pointer to <code>double</code>.  This is
     * equivalent to the expression
     * <code>*((jdouble *)((char *)cptr + offset))</code>.
     *
     * @param offset byte offset from pointer to perform the indirection
     * @return the <code>double</code> value being pointed to
     */
    public native double getDouble(int offset);

    /**
     * Indirect the C pointer as a pointer to pointer.  This is equivalent to
     * the expression <code>*((void **)((char *)cptr + offset))</code>.
     *
     * @param offset byte offset from pointer to perform the indirection
     * @return the <code>pointer</code> value being pointed to
     */
    public native CPointer getCPointer(int offset);

    /**
     * Indirect the C pointer as a pointer to <code>char *</code>, a
     * <code>NULL</code>-terminated C string. Convert the C string to a
     * <code>java.lang.String</code>.
     *
     * @param offset byte offset from pointer to obtain the C string
     * @return the <code>String</code> value being pointed to 
     */
    public native String getString(int offset);

    /**
     * Set <code>value</code> at location being pointed to. This is equivalent
     * to the expression
     * <code>*((jbyte *)((char *)cptr + offset)) = value</code>.
     *
     * @param offset byte offset from pointer at which <code>value</code>
     *		     must be set
     * @param value <code>byte</code> value to set
     */
    public native void setByte(int offset, byte value);

    /**
     * Set <code>value</code> at location being pointed to. This is equivalent
     * to the expression
     * <code>*((jshort *)((char *)cptr + offset)) = value</code>.
     *
     * @param offset byte offset from pointer at which <code>value</code>
     *		     must be set
     * @param value <code>short</code> value to set
     */
    public native void setShort(int offset, short value);

    /**
     * Set <code>value</code> at location being pointed to. This is equivalent
     * to the expression
     * <code>*((jint *)((char *)cptr + offset)) = value</code>.
     *
     * @param offset byte offset from pointer at which <code>value</code>
     *		     must be set
     * @param value <code>int</code> value to set
     */
    public native void setInt(int offset, int value);

    /**
     * Set <code>value</code> at location being pointed to. This is equivalent
     * to the expression
     * <code>*((jlong *)((char *)cptr + offset)) = value</code>.
     *
     * @param offset byte offset from pointer at which <code>value</code>
     *               must be set
     * @param value <code>long</code> value to set
     */
    public native void setLong(int offset, long value);

    /**
     * Set <code>value</code> at location being pointed to. This is equivalent
     * to the expression
     * <code>*((jfloat *)((char *)cptr + offset)) = value</code>.
     *
     * @param offset byte offset from pointer at which <code>value</code>
     *               must be set
     * @param value <code>float</code> value to set
     */
    public native void setFloat(int offset, float value);

    /**
     * Set <code>value</code> at location being pointed to. This is equivalent
     * to the expression
     * <code>*((jdouble *)((char *)cptr + offset)) = value</code>.
     *
     * @param offset byte offset from pointer at which <code>value</code>
     *               must be set
     * @param value <code>double</code> value to set
     */
    public native void setDouble(int offset, double value);

    /**
     * Set <code>value</code> at location being pointed to. This is equivalent
     * to the expression *((void **)((char *)cptr + offset)) = value</code>.
     *
     * @param offset byte offset from pointer at which <code>value</code> 
     *               must be set
     * @param value <code>CPointer</code> value to set
     */
    public native void setCPointer(int offset, CPointer value);

    /**
     * Copy string <code>value</code> to the location being pointed to.  Copy
     * each element in <code>value</code>, converted to native encoding, at an
     * <code>offset</code>from the location pointed to by this pointer.
     *
     * @param offset byte offset from pointer at which characters in
     * 		     <code>value</code> must be set
     * @param value  <code>java.lang.String</code> value to set 
     */
    public native void setString(int offset, String value);

    /* Initialize field and method IDs for native methods of this class. */
    private static native int initIDs();
    
    static {
        System.loadLibrary("disp");
        SIZE = initIDs();
    }

    /* Pointer value of the real C pointer. Use long to be 64-bit safe. */
    protected long peer;
}
