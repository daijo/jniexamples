/*
 * %W% %E%
 *
 * Copyright (c) 1998 Sun Microsystems, Inc. All Rights Reserved.
 *
 * See also the LICENSE file in this distribution.
 */

/**
 * A <code>CPointer</code> to memory obtained from the C heap via a call to
 * <code>malloc</code>.
 * <p>
 * In some cases it might be necessary to use memory obtained from
 * <code>malloc</code>.  For example, <code>CMalloc</code> helps accomplish
 * the following idiom:
 * <pre>
 * 		void *buf = malloc(BUF_LEN * sizeof(char));
 *		call_some_function(buf);
 *		free(buf);
 * </pre>
 * <p>
 * <b>Remember to <code>free</code> any <code>malloc</code> space
 * explicitly</b>.  This class could perhaps contain a <code>finalize</code>
 * method that does the <code>free</code>, but note that in Java you should
 * not use finalizers to free resources.
 *
 * @author Sheng Liang
 * @see CPointer
 */
public class CMalloc extends CPointer {

    /**
     * Allocate space in the C heap via a call to C's <code>malloc</code>.
     *
     * @param size number of <em>bytes</em> of space to allocate
     */
    public CMalloc(int size) {
        this.size = size;
        peer = malloc(size);
	if (peer == 0) {
	    throw new OutOfMemoryError();
	}
    }

    /**
     * De-allocate space obtained via an earlier call to <code>malloc</code>.
     */
    public native void free();

    /**
     * Indirect the C pointer to <code>malloc</code> space, a la
     * <code>CPointer.copyIn</code>.  But this method performs a bounds 
     * checks to
     * ensure that the indirection does not cause memory outside the
     * <code>malloc</code>ed space to be accessed.
     *
     * @see CPointer#copyIn(int,byte[],int,int) 
     */
    public void copyIn(int bOff, byte[] buf, int index, int length) {
        boundsCheck(bOff, length * 1);
	super.copyIn(bOff, buf, index, length);
    }

    /**
     * Indirect the C pointer to <code>malloc</code> space, a la
     * <code>CPointer.copyIn</code>.  But this method performs a bounds
     * checks to
     * ensure that the indirection does not cause memory outside the
     * <code>malloc</code>ed space to be accessed.
     *
     * @see CPointer#copyIn(int,short[],int,int)
     */
    public void copyIn(int bOff, short[] buf, int index, int length) {
        boundsCheck(bOff, length * 2);
	super.copyIn(bOff, buf, index, length);
    }

    /**
     * Indirect the C pointer to <code>malloc</code> space, a la
     * <code>CPointer.copyIn</code>.  But this method performs a bounds
     * checks to
     * ensure that the indirection does not cause memory outside the
     * <code>malloc</code>ed space to be accessed.
     *
     * @see CPointer#copyIn(int,char[],int,int)
     */
    public void copyIn(int bOff, char[] buf, int index, int length) {
        boundsCheck(bOff, length * 2);
	super.copyIn(bOff, buf, index, length);
    }

    /**
     * Indirect the C pointer to <code>malloc</code> space, a la
     * <code>CPointer.copyIn</code>.  But this method performs a bounds
     * checks to
     * ensure that the indirection does not cause memory outside the
     * <code>malloc</code>ed space to be accessed.
     *
     * @see CPointer#copyIn(int,int[],int,int) 
     */
    public void copyIn(int bOff, int[] buf, int index, int length) {
        boundsCheck(bOff, length * 4);
	super.copyIn(bOff, buf, index, length);
    }

    /**
     * Indirect the C pointer to <code>malloc</code> space, a la
     * <code>CPointer.copyIn</code>.  But this method performs a bounds
     * checks to
     * ensure that the indirection does not cause memory outside the
     * <code>malloc</code>ed space to be accessed.
     *
     * @see CPointer#copyIn(int,long[],int,int) 
     */
    public void copyIn(int bOff, long[] buf, int index, int length) {
        boundsCheck(bOff, length * 8);
	super.copyIn(bOff, buf, index, length);
    }

    /**
     * Indirect the C pointer to <code>malloc</code> space, a la
     * <code>CPointer.copyIn</code>.  But this method performs a bounds
     * checks to
     * ensure that the indirection does not cause memory outside the
     * <code>malloc</code>ed space to be accessed.
     *
     * @see CPointer#copyIn(int,float[],int,int)
     */
    public void copyIn(int bOff, float[] buf, int index, int length) {
        boundsCheck(bOff, length * 4);
	super.copyIn(bOff, buf, index, length);
    }

    /**
     * Indirect the C pointer to <code>malloc</code> space, a la
     * <code>CPointer.copyIn</code>.  But this method performs a bounds
     * checks to
     * ensure that the indirection does not cause memory outside the
     * <code>malloc</code>ed space to be accessed.
     *
     * @see CPointer#copyIn(int,double[],int,int) 
     */
    public void copyIn(int bOff, double[] buf, int index, int length) {
        boundsCheck(bOff, length * 8);
	super.copyIn(bOff, buf, index, length);
    }

    /**
     * Indirect the C pointer to <code>malloc</code> space, a la
     * <code>CPointer.copyOut</code>.  But this method performs a bounds
     * checks to
     * ensure that the indirection does not cause memory outside the
     * <code>malloc</code>ed space to be accessed.
     *
     * @see CPointer#copyOut(int,byte[],int,int) 
     */
    public void copyOut(int bOff, byte[] buf, int index, int length) {
        boundsCheck(bOff, length * 1);
	super.copyOut(bOff, buf, index, length);
    }

    /**
     * Indirect the C pointer to <code>malloc</code> space, a la
     * <code>CPointer.copyOut</code>.  But this method performs a bounds
     * checks to
     * ensure that the indirection does not cause memory outside the
     * <code>malloc</code>ed space to be accessed.
     *
     * @see CPointer#copyOut(int,short[],int,int)
     */
    public void copyOut(int bOff, short[] buf, int index, int length) {
        boundsCheck(bOff, length * 2);
	super.copyOut(bOff, buf, index, length);
    }

    /**
     * Indirect the C pointer to <code>malloc</code> space, a la
     * <code>CPointer.copyOut</code>.  But this method performs a bounds
     * checks to
     * ensure that the indirection does not cause memory outside the
     * <code>malloc</code>ed space to be accessed.
     *
     * @see CPointer#copyOut(int,char[],int,int) 
     */
    public void copyOut(int bOff, char[] buf, int index, int length) {
        boundsCheck(bOff, length * 2);
	super.copyOut(bOff, buf, index, length);
    }

    /**
     * Indirect the C pointer to <code>malloc</code> space, a la
     * <code>CPointer.copyOut</code>.  But this method performs a bounds
     * checks to
     * ensure that the indirection does not cause memory outside the
     * <code>malloc</code>ed space to be accessed.
     *
     * @see CPointer#copyOut(int,int[],int,int)
     */
    public void copyOut(int bOff, int[] buf, int index, int length) {
        boundsCheck(bOff, length * 4);
	super.copyOut(bOff, buf, index, length);
    }

    /**
     * Indirect the C pointer to <code>malloc</code> space, a la
     * <code>CPointer.copyOut</code>.  But this method performs a bounds
     * checks to
     * ensure that the indirection does not cause memory outside the
     * <code>malloc</code>ed space to be accessed.
     *
     * @see CPointer#copyOut(int,long[],int,int) 
     */
    public void copyOut(int bOff, long[] buf, int index, int length) {
        boundsCheck(bOff, length * 8);
	super.copyOut(bOff, buf, index, length);
    }

    /**
     * Indirect the C pointer to <code>malloc</code> space, a la
     * <code>CPointer.copyOut</code>.  But this method performs a bounds
     * checks to
     * ensure that the indirection does not cause memory outside the
     * <code>malloc</code>ed space to be accessed.
     *
     * @see CPointer#copyOut(int,float[],int,int) 
     */
    public void copyOut(int bOff, float[] buf, int index, int length) {
        boundsCheck(bOff, length * 4);
	super.copyOut(bOff, buf, index, length);
    }

    /**
     * Indirect the C pointer to <code>malloc</code> space, a la
     * <code>CPointer.copyOut</code>.  But this method performs a bounds
     * checks to
     * ensure that the indirection does not cause memory outside the
     * <code>malloc</code>ed space to be accessed.
     *
     * @see CPointer#copyOut(int,double[],int,int) 
     */
    public void copyOut(int bOff, double[] buf, int index, int length) {
        boundsCheck(bOff, length * 8);
	super.copyOut(bOff, buf, index, length);
    }

    /**
     * Indirect the C pointer to <code>malloc</code> space, a la
     * <code>CPointer.getByte</code>.  But this method performs a bounds
     * checks to
     * ensure that the indirection does not cause memory outside the
     * <code>malloc</code>ed space to be accessed.
     *
     * @see CPointer#getByte(int)
     */
    public byte getByte(int offset) {
        boundsCheck(offset, 1);
	return super.getByte(offset);
    }

    /**
     * Indirect the C pointer to <code>malloc</code> space, a la
     * <code>CPointer.getShort</code>.  But this method performs a bounds
     * checks to
     * ensure that the indirection does not cause memory outside the
     * <code>malloc</code>ed space to be accessed.
     *
     * @see CPointer#getShort(int)
     */
    public short getShort(int offset) {
        boundsCheck(offset, 2);
	return super.getShort(offset);
    }

    /**
     * Indirect the C pointer to <code>malloc</code> space, a la
     * <code>CPointer.getInt</code>.  But this method performs a bounds
     * checks to
     * ensure that the indirection does not cause memory outside the
     * <code>malloc</code>ed space to be accessed.
     *
     * @see CPointer#getInt(int)
     */
    public int getInt(int offset) {
        boundsCheck(offset, 4);
	return super.getInt(offset);
    }

    /**
     * Indirect the C pointer to <code>malloc</code> space, a la
     * <code>CPointer.getLong</code>.  But this method performs a bounds
     * checks to
     * ensure that the indirection does not cause memory outside the
     * <code>malloc</code>ed space to be accessed.
     *
     * @see CPointer#getLong(int)
     */
    public long getLong(int offset) {
        boundsCheck(offset, 8);
	return super.getLong(offset);
    }

    /**
     * Indirect the C pointer to <code>malloc</code> space, a la
     * <code>CPointer.getFloat</code>.  But this method performs a bounds
     * checks to
     * ensure that the indirection does not cause memory outside the
     * <code>malloc</code>ed space to be accessed.
     *
     * @see CPointer#getFloat(int)
     */
    public float getFloat(int offset) {
        boundsCheck(offset, 4);
	return super.getFloat(offset);
    }

    /**
     * Indirect the C pointer to <code>malloc</code> space, a la
     * <code>CPointer.getDouble</code>.  But this method performs a bounds
     * checks
     * to ensure that the indirection does not cause memory outside the
     * <code>malloc</code>ed space to be accessed.
     *
     * @see CPointer#getDouble(int)
     */
    public double getDouble(int offset) {
        boundsCheck(offset, 8);
	return super.getDouble(offset);
    }

    /**
     * Indirect the C pointer to <code>malloc</code> space, a la
     * <code>CPointer.getCPointer</code>.  But this method performs a bounds
     * checks to
     * ensure that the indirection does not cause memory outside the
     * <code>malloc</code>ed space to be accessed.
     *
     * @see CPointer#getCPointer(int)
     */
    public CPointer getCPointer(int offset) {
        boundsCheck(offset, SIZE);
	return super.getCPointer(offset);
    }

    /**
     * Indirect the C pointer to <code>malloc</code> space, a la
     * <code>CPointer.getString</code>.  But this method performs a bounds
     * checks
     * to ensure that the indirection does not cause memory outside the
     * <code>malloc</code>ed space to be accessed.
     *
     * @see CPointer#getString(int)
     */
    public String getString(int offset) {
        boundsCheck(offset, 0);
	return super.getString(offset);
    }

    /**
     * Indirect the C pointer to <code>malloc</code> space, a la
     * <code>CPointer.setByte</code>.  But this method performs a bounds
     * checks to
     * ensure that the indirection does not cause memory outside the
     * <code>malloc</code>ed space to be accessed.
     *
     * @see CPointer#setByte(int, byte)
     */
    public void setByte(int offset, byte value) {
        boundsCheck(offset, 1);
	super.setByte(offset, value);
    }

    /**
     * Indirect the C pointer to <code>malloc</code> space, a la
     * <code>CPointer.setShort</code>.  But this method performs a bounds
     * checks to
     * ensure that the indirection does not cause memory outside the
     * <code>malloc</code>ed space to be accessed.
     *
     * @see CPointer#setShort(int, short)
     */
    public void setShort(int offset, short value) {
        boundsCheck(offset, 2);
	super.setShort(offset, value);
    }

    /**
     * Indirect the C pointer to <code>malloc</code> space, a la
     * <code>CPointer.setInt</code>.  But this method performs a bounds
     * checks to
     * ensure that the indirection does not cause memory outside the
     * <code>malloc</code>ed space to be accessed.
     *
     * @see CPointer#setInt(int, int)
     */
    public void setInt(int offset, int value) {
        boundsCheck(offset, 4);
	super.setInt(offset, value);
    }

    /**
     * Indirect the C pointer to <code>malloc</code> space, a la
     * <code>CPointer.setLong</code>.  But this method performs a bounds
     * checks to
     * ensure that the indirection does not cause memory outside the
     * <code>malloc</code>ed space to be accessed.
     *
     * @see CPointer#setLong(int, long)
     */
    public void setLong(int offset, long value) {
        boundsCheck(offset, 8);
	super.setLong(offset, value);
    }

    /**
     * Indirect the C pointer to <code>malloc</code> space, a la
     * <code>CPointer.setFloat</code>.  But this method performs a bounds
     * checks to
     * ensure that the indirection does not cause memory outside the
     * <code>malloc</code>ed space to be accessed.
     *
     * @see CPointer#setFloat(int, float)
     */
    public void setFloat(int offset, float value) {
        boundsCheck(offset, 4);
	super.setFloat(offset, value);
    }

    /**
     * Indirect the C pointer to <code>malloc</code> space, a la
     * <code>CPointer.setDouble</code>.  But this method performs a bounds
     * checks
     * to ensure that the indirection does not cause memory outside the
     * <code>malloc</code>ed space to be accessed.
     *
     * @see CPointer#setDouble(int, double)
     */
    public void setDouble(int offset, double value) {
        boundsCheck(offset, 8);
	super.setDouble(offset, value);
    }

    /**
     * Indirect the C pointer to <code>malloc</code> space, a la
     * <code>CPointer.setCPointer</code>.  But this method performs a bounds
     * checks to
     * ensure that the indirection does not cause memory outside the
     * <code>malloc</code>ed space to be accessed.
     *
     * @see CPointer#setCPointer(int, CPointer)
     */
    public void setCPointer(int offset, CPointer value) {
        boundsCheck(offset, SIZE);
	super.setCPointer(offset, value);
    }

    /**
     * Indirect the C pointer to <code>malloc</code> space, a la
     * <code>CPointer.setString</code>.  But this method performs a bounds
     * checks
     * to ensure that the indirection does not cause memory outside the
     * <code>malloc</code>ed space to be accessed.
     *
     * @see CPointer#setString(int, String)
     */
    public void setString(int offset, String value) {
        byte[] bytes = value.getBytes();
	int length = bytes.length;
        boundsCheck(offset, length + 1);
	super.copyIn(offset, bytes, 0, length);
	super.setByte(offset + length, (byte)0);
    }

    /* Size of the malloc'ed space. */
    private int size;

    /* Call the real C malloc. */
    private static native long malloc(int size);

    /* Private to prevent creation of uninitialized malloc space. */
    private CMalloc() {}
    
    /* Check that indirection won't cause us to write outside the malloc'ed
       space. */
    private void boundsCheck(int off, int sz) {
        if (off < 0 || off + sz > size) {
	    throw new IndexOutOfBoundsException();
	}
    }
}



