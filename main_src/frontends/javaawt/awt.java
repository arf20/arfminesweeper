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
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

class awt_frame extends Frame {   
    final int size, wWidth, wHeight;

    /* macro values from native frontconf.h */
    final String TXT_TITLE, TXT_LOST, TXT_WON;
    final int HEADER_HEIGHT, CELL_SIZE, CELL_MARGIN, W_MARGIN;

    /* interface to C functions */
    final Backend backend;
    
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

        Button[] buttons = new Button[size*size];

        for (int y = 0; y < size; y++) {
            for (int x = 0; x < size; x++) {
                int cX = W_MARGIN + (x * (CELL_SIZE + CELL_MARGIN));
                int cY = HEADER_HEIGHT + (y * (CELL_SIZE + CELL_MARGIN));
                int idx = (y*size) + x;
                buttons[idx] = new Button();
                buttons[idx].setBounds(cX, cY, CELL_SIZE, CELL_SIZE);
                add(buttons[idx]);
            }
        }

        setSize(wWidth, wHeight);
        setUndecorated(true);
        setTitle(TXT_TITLE);
        setLayout(null);
        setVisible(true);
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

