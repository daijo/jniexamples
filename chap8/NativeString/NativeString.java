class NativeString {
    private static native String nativeMethod(String s);
    public static void main(String args[]) {
        System.out.println("Native method returns: " + nativeMethod("abcde"));
    }
    static {
        System.loadLibrary("NativeString");
    }
}
