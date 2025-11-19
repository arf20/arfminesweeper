/*

    arfminesweeper: Cross-plataform multi-frontend game
    Copyright (C) 2023 arf20 (Ángel Ruiz Fernandez)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    awt.java: Java AWT frontend

*/

import java.util.Arrays;
import java.awt.*;
import java.awt.event.*;

class awt_frame extends Frame implements MouseListener {   
    final int size, wWidth, wHeight;

    /* macro values from native frontconf.h */
    final String TXT_TITLE, TXT_LOST, TXT_WON;
    final int HEADER_HEIGHT, CELL_SIZE, CELL_MARGIN, W_MARGIN;
    final int CELL_BIT_MINE, CELL_BIT_FLAG, CELL_BIT_CLEAR;

    /* interface to C functions */
    final Backend backend;
    
    Button[] buttons = new Button[1]; // java sucks
    
    awt_frame(int lsize) {
        backend = new Backend();

        /* initialize values from native */
        TXT_TITLE = backend.getTextTitle();
        TXT_LOST = backend.getTextLost();
        TXT_WON = backend.getTextWon();
        HEADER_HEIGHT = backend.getHeaderHeight();
        CELL_SIZE = backend.getCellSize();
        CELL_MARGIN = backend.getCellMargin();
        W_MARGIN = backend.getWindowMargin();
        CELL_BIT_MINE = backend.getCellBitMine();
        CELL_BIT_FLAG = backend.getCellBitFlag();
        CELL_BIT_CLEAR = backend.getCellBitClear(); 

        size = lsize;

        wWidth = (2 * W_MARGIN) + (size * CELL_SIZE) + ((size - 1) * CELL_MARGIN);
        wHeight = HEADER_HEIGHT + W_MARGIN + (size * CELL_SIZE) +
            ((size - 1) * CELL_MARGIN);

        setResizable(false);

        Label titleLabel = new Label(TXT_TITLE);
        titleLabel.setBounds(5, 15, 100, 20);
        add(titleLabel);

        Label flagLabel = new Label("10");
        flagLabel.setBounds(wWidth - 25, 35, 50, 20);
        add(flagLabel);

        buttons = new Button[size*size];

        for (int y = 0; y < size; y++) {
            for (int x = 0; x < size; x++) {
                int cX = W_MARGIN + (x * (CELL_SIZE + CELL_MARGIN));
                int cY = HEADER_HEIGHT + (y * (CELL_SIZE + CELL_MARGIN));
                int btni = (y*size) + x;
                buttons[btni] = new Button();
                buttons[btni].setBounds(cX, cY, CELL_SIZE, CELL_SIZE);
                buttons[btni].addMouseListener(this);
                add(buttons[btni]);
            }
        }

        setSize(wWidth, wHeight);
        setUndecorated(true);
        setTitle(TXT_TITLE);
        setLayout(null);
        setVisible(true);
    }

    public void mouseClicked(MouseEvent e) {
        Object btn = e.getSource();
        int btni = -1;
        for (int i = 0; i < size * size; i++) {
            if (buttons[i] == btn) {
                btni = i;
                break;
            }
        }

        if (btni == -1)
            return;

        if (e.getButton() == MouseEvent.BUTTON1)
            backend.gameClearCell(btni % size, btni / size);
        else if (e.getButton() == MouseEvent.BUTTON3)
            backend.gameFlagCell(btni % size, btni / size);

        updateBoard();
    }

    public void mouseExited(MouseEvent e) { }
    public void mouseEntered(MouseEvent e) { }
    public void mousePressed(MouseEvent e) { }
    public void mouseReleased(MouseEvent e) { }

    private void updateBoard() {
        for (int y = 0; y < size; y++) {
            for (int x = 0; x < size; x++) {
                int btni = (y * size) + x;

                if (CHECK_CLEAR(BOARDXY(x, y))) {
                    buttons[btni].setVisible(false);
                } else if (CHECK_FLAG(BOARDXY(x, y))) {

                } else {

                }
            }
        }
    }

    private boolean CHECK_MINE(int x) { return  (((x) >> CELL_BIT_MINE) & 1) == 1; };
    private boolean CHECK_FLAG(int x) { return  (((x) >> CELL_BIT_FLAG) & 1) == 1; };
    private boolean CHECK_CLEAR(int x) { return (((x) >> CELL_BIT_CLEAR) & 1) == 1; };

    private int BOARDXY(int x, int y) {
        return backend.getBoardCopy()[(y * size) + x];
    }
}

class Backend {
    static {
        try {
            System.loadLibrary("arfminesweeper");
        } catch (UnsatisfiedLinkError e) {
            System.out.println(e);
        }

        try {
            System.loadLibrary("javaawt");
        } catch (UnsatisfiedLinkError e) {
            System.out.println(e);
        }
    }

    public native int[] getBoardCopy();
    public native String getTextLost();
    public native String getTextWon();
    public native String getTextTitle();
    public native int getHeaderHeight();
    public native int getCellSize();
    public native int getCellMargin();
    public native int getWindowMargin();
    public native int getCellBitMine();
    public native int getCellBitFlag();
    public native int getCellBitClear(); 

    /* game interface */
    public native int gameGetState();
    public native int gameGetSurroundingMines(int x, int y);
    public native int gameGetFlagsLeft();
    public native void gameClearCell(int x, int y);
    public native void gameFlagCell(int x, int y);
}

public class awt {
    static void main(int lsize)  {
        awt_frame f = new awt_frame(lsize);

        f.addWindowListener(new WindowAdapter() {    
            public void windowClosing(WindowEvent e) {    
                f.dispose();
                System.exit(0);
            }    
        });

        try {
            Thread.sleep(Long.MAX_VALUE);
        } catch (InterruptedException e) {
            System.out.println("awt::main main thread interrupted");
        }
    }
}

