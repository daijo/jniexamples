class ThrowByName {
    private native void doit();
    public static void main(String args[]) {
        ThrowByName c = new ThrowByName();
        try {
            c.doit();
        } catch (Throwable e) {
            System.out.println("In Java:\n\t" + e);
        }
    }
    static {
        System.loadLibrary("ThrowByName");
    }
}
