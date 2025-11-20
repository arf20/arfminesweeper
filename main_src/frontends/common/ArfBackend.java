public class ArfBackend {
    ArfBackend() {
        System.loadLibrary("arfjavabindings");

        TXT_TITLE = getTextTitle();
        TXT_LOST = getTextLost();
        TXT_WON = getTextWon();
        HEADER_HEIGHT = getHeaderHeight();
        CELL_SIZE = getCellSize();
        CELL_MARGIN = getCellMargin();
        W_MARGIN = getWindowMargin();
        CELL_BIT_MINE = getCellBitMine();
        CELL_BIT_FLAG = getCellBitFlag();
        CELL_BIT_CLEAR = getCellBitClear(); 
        FLAG_PNG_PATH = getFlagPNGPath(); 
        STATE_LOST = getStateLost();
        STATE_WON = getStateWon();
    }

    /* macro values from native frontconf.h */
    public final String TXT_TITLE, TXT_LOST, TXT_WON, FLAG_PNG_PATH;
    public final int HEADER_HEIGHT, CELL_SIZE, CELL_MARGIN, W_MARGIN,
          CELL_BIT_MINE, CELL_BIT_FLAG, CELL_BIT_CLEAR, STATE_LOST, STATE_WON;

    /* game interface */
    public native int[] getBoardCopy(int size);
    public native int gameGetState();
    public native int gameGetSurroundingMines(int x, int y);
    public native int gameGetFlagsLeft();
    public native void gameClearCell(int x, int y);
    public native void gameFlagCell(int x, int y);

    private native String getTextLost();
    private native String getTextWon();
    private native String getTextTitle();
    private native int getHeaderHeight();
    private native int getCellSize();
    private native int getCellMargin();
    private native int getWindowMargin();
    private native int getCellBitMine();
    private native int getCellBitFlag();
    private native int getCellBitClear(); 
    private native String getFlagPNGPath(); 
    private native int getStateLost(); 
    private native int getStateWon();
}

