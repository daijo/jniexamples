class MyNewString {
    private static native String nativeMethod();
    public static void main(String args[]) {
        System.out.println("Native method returns: " + nativeMethod());
    }
    static {
        System.loadLibrary("MyNewString");
    }
}
