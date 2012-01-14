class C {
    public static native int atol(String str);
}

class Win32 {
    public static native int CreateFile(
        String fileName,          // file name
        int desiredAccess,        // access (read-write) mode 
        int shareMode,            // share mode 
        int[] secAttrs,           // security attributes 
        int creationDistribution, // how to create 
        int flagsAndAttributes,   // file attributes 
        int templateFile);        // file with attr. to copy
}

class OneToOne {
    public static void main(String[] args) {
	System.out.println("atol(\"123\") = " + C.atol("123"));
	System.out.println("Creating a file called TestFile.tst in the current directory...");
	Win32.CreateFile("TestFile.tst",
			 0x40000000, // GENERIC_WRITE
			 0,          // not sharable
			 null,       // no security
			 2,          // CREATE_ALWAYS
			 0x00000080, // FILE_ATTRIBUTE_NORMAL
			 0);         // no template file
    }
    static {
	System.loadLibrary("OneToOne");
    }
}

