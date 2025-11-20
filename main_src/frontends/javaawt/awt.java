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

class MessageBox extends Dialog implements ActionListener {
    public MessageBox(Frame parent, String title, String text) {
        super(parent, title, true);
        setSize(400, 250);
        TextArea txt = new TextArea(text, 80, 10, TextArea.SCROLLBARS_NONE);
        txt.setEditable(false);
        add(txt, BorderLayout.CENTER);
        Button ok = new Button("Ok");
        ok.addActionListener(this);
        add(ok, BorderLayout.SOUTH);
        doLayout();
        setVisible(true);
    }

    public void actionPerformed(ActionEvent e) {  
        dispose();
    }
}

class Cell extends Button {

}

class MinesweeperAwt extends Frame implements MouseListener {   
    final int size, wWidth, wHeight;

    /* macro values from native frontconf.h */
    final String TXT_TITLE, TXT_LOST, TXT_WON, FLAG_PNG_PATH;
    final int HEADER_HEIGHT, CELL_SIZE, CELL_MARGIN, W_MARGIN,
          CELL_BIT_MINE, CELL_BIT_FLAG, CELL_BIT_CLEAR, STATE_LOST, STATE_WON;

    /* interface to C functions */
    final Backend backend;

    final Image flag;
    
    final Label flagLabel;
    Button[] buttons = new Button[1]; // java sucks
    Label[] numbers = new Label[1]; // java sucks
    
    MinesweeperAwt(int lsize) {
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
        FLAG_PNG_PATH = backend.getFlagPNGPath(); 
        STATE_LOST = backend.getStateLost();
        STATE_WON = backend.getStateWon();

        size = lsize;

        wWidth = (2 * W_MARGIN) + (size * CELL_SIZE) + ((size - 1) * CELL_MARGIN);
        wHeight = HEADER_HEIGHT + W_MARGIN + (size * CELL_SIZE) +
            ((size - 1) * CELL_MARGIN);

        setResizable(false);
        setLayout(null);
        setTitle(TXT_TITLE);
        
        setVisible(true);
        Insets insets = getInsets();
        setSize(wWidth + insets.left + insets.right, wHeight + insets.top + insets.bottom);

        Label titleLabel = new Label(TXT_TITLE);
        titleLabel.setBounds(5 + insets.left, 15 + insets.top, 100, 20);
        add(titleLabel);

        flagLabel = new Label(Integer.toString(backend.gameGetFlagsLeft()));
        flagLabel.setBounds(wWidth - 25 + insets.left, 35 + insets.top, 50, 20);
        add(flagLabel);

        flag = Toolkit.getDefaultToolkit().getImage(FLAG_PNG_PATH);

        buttons = new Button[size*size];
        numbers = new Label[size*size];

        for (int y = 0; y < size; y++) {
            for (int x = 0; x < size; x++) {
                int cX = W_MARGIN + (x * (CELL_SIZE + CELL_MARGIN)) + insets.left;
                int cY = HEADER_HEIGHT + (y * (CELL_SIZE + CELL_MARGIN)) + insets.top;
                int btni = (y*size) + x;

                buttons[btni] = new Button();
                buttons[btni].setBounds(cX, cY, CELL_SIZE, CELL_SIZE);
                buttons[btni].addMouseListener(this);
                add(buttons[btni]);

                numbers[btni] = new Label();
                numbers[btni].setBounds(cX, cY, CELL_SIZE, CELL_SIZE);
                numbers[btni].setVisible(false);

                int n = backend.gameGetSurroundingMines(x, y);
                if (n > 0) {
                    numbers[btni].setText(Integer.toString(n));

                    switch (n) {
                        case 1: numbers[btni].setForeground(Color.blue); break;
                        case 2: numbers[btni].setForeground(Color.green); break;
                        case 3: numbers[btni].setForeground(Color.red); break;
                        case 4: numbers[btni].setForeground(Color.cyan.darker()); break;
                        case 5: numbers[btni].setForeground(Color.red.darker()); break;
                        case 6: numbers[btni].setForeground(Color.cyan); break;
                        case 7: numbers[btni].setForeground(Color.black); break;
                        case 8: numbers[btni].setForeground(Color.gray); break;
                    }
                }
                    
                add(numbers[btni]);
            }
        }
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

        update();
    }

    public void mouseExited(MouseEvent e) { }
    public void mouseEntered(MouseEvent e) { }
    public void mousePressed(MouseEvent e) { }
    public void mouseReleased(MouseEvent e) { }

    private void update() {
        flagLabel.setText(Integer.toString(backend.gameGetFlagsLeft()));

        for (int y = 0; y < size; y++) {
            for (int x = 0; x < size; x++) {
                int btni = (y * size) + x;

                if (CHECK_CLEAR(BOARDXY(x, y))) {
                    buttons[btni].setVisible(false);
                    numbers[btni].setVisible(true);
                } else if (CHECK_FLAG(BOARDXY(x, y))) {
                    buttons[btni].setLabel("F");
                } else {
                    buttons[btni].setLabel("");
                }
            }
        }

        int state = backend.gameGetState();
        if (state == STATE_LOST) {
            MessageBox dialog = new MessageBox(this, TXT_TITLE, TXT_LOST);
            dispose();
            System.exit(0);
        } else if (state == STATE_WON) {
            MessageBox dialog = new MessageBox(this, TXT_TITLE, TXT_WON);
            dispose();
            System.exit(0);
        }
    }

    public void paint(Graphics g) {
        //g.drawImage(flag, 0, 0, 200, 200, null);
    }

    private boolean CHECK_MINE(int x) { return  (((x) >> CELL_BIT_MINE) & 1) == 1; };
    private boolean CHECK_FLAG(int x) { return  (((x) >> CELL_BIT_FLAG) & 1) == 1; };
    private boolean CHECK_CLEAR(int x) { return (((x) >> CELL_BIT_CLEAR) & 1) == 1; };

    private int BOARDXY(int x, int y) {
        return backend.getBoardCopy()[(y * size) + x];
    }
}

class Backend {
    boolean linked = false;

    Backend() {
        UnsatisfiedLinkError exception = new UnsatisfiedLinkError();

        try {
            System.loadLibrary("arfminesweeper");
            linked = true;
        } catch (UnsatisfiedLinkError e) {
            exception = e;
        }

        if (!linked) {
            try {
                System.loadLibrary("javaawt");
                linked = true;
            } catch (UnsatisfiedLinkError e) {
                exception = e;
            }
        }

        if (!linked) {
            System.out.println("Could not find library to link against or error linking with it");
            throw exception;
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
    public native String getFlagPNGPath(); 
    public native int getStateLost(); 
    public native int getStateWon(); 

    /* game interface */
    public native int gameGetState();
    public native int gameGetSurroundingMines(int x, int y);
    public native int gameGetFlagsLeft();
    public native void gameClearCell(int x, int y);
    public native void gameFlagCell(int x, int y);
}

public class awt {
    static void main(int lsize)  {
        MinesweeperAwt f = new MinesweeperAwt(lsize);

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

