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

import java.io.*;
import java.util.*;
import java.lang.*;
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
    private final Image flag;
    private boolean paintFlag = false;

    public Cell(Image flag) {
        super();
        this.flag = flag;
    }

    public void setPaintFlag(boolean paint) {
        paintFlag = paint;
    }

    public void update(Graphics g) {
        super.update(g);

        if (!paintFlag)
            return;

        g.drawImage(flag, 0, 0, flag.getWidth(null), flag.getHeight(null), null);
    }
}

class MinesweeperAwt extends Frame implements MouseListener {   
    final int size, wWidth, wHeight;

    /* interface to C core */
    final ArfBackend backend;

    final Image flag;
    
    final Label flagLabel;
    Cell[] cells = new Cell[1]; // java sucks
    Label[] numbers = new Label[1]; // java sucks
    
    MinesweeperAwt(int lsize) {
        backend = new ArfBackend();

        /* initialize values from native */
        

        size = lsize;

        wWidth = (2 * backend.W_MARGIN) + (size * backend.CELL_SIZE) +
            ((size - 1) * backend.CELL_MARGIN);
        wHeight = backend.HEADER_HEIGHT + backend.W_MARGIN +
            (size * backend.CELL_SIZE) + ((size - 1) * backend.CELL_MARGIN);

        setResizable(false);
        setLayout(null);
        setTitle(backend.TXT_TITLE);
        
        setVisible(true);
        Insets insets = getInsets();
        setSize(wWidth + insets.left + insets.right, wHeight + insets.top + insets.bottom);

        Label titleLabel = new Label(backend.TXT_TITLE);
        titleLabel.setBounds(5 + insets.left, 15 + insets.top, 100, 20);
        add(titleLabel);

        flagLabel = new Label(Integer.toString(backend.gameGetFlagsLeft()));
        flagLabel.setBounds(wWidth - 25 + insets.left, 35 + insets.top, 50, 20);
        add(flagLabel);

        flag = Toolkit.getDefaultToolkit().getImage(backend.FLAG_PNG_PATH);

        cells = new Cell[size*size];
        numbers = new Label[size*size];

        for (int y = 0; y < size; y++) {
            for (int x = 0; x < size; x++) {
                int cX = backend.W_MARGIN + (x * (backend.CELL_SIZE + backend.CELL_MARGIN)) + insets.left;
                int cY = backend.HEADER_HEIGHT + (y * (backend.CELL_SIZE + backend.CELL_MARGIN)) + insets.top;
                int btni = (y*size) + x;

                cells[btni] = new Cell(flag);
                cells[btni].setBounds(cX, cY, backend.CELL_SIZE, backend.CELL_SIZE);
                cells[btni].addMouseListener(this);
                add(cells[btni]);

                numbers[btni] = new Label();
                numbers[btni].setBounds(cX, cY, backend.CELL_SIZE, backend.CELL_SIZE);
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
            if (cells[i] == btn) {
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
                    cells[btni].setVisible(false);
                    numbers[btni].setVisible(true);
                } else if (CHECK_FLAG(BOARDXY(x, y))) {
                    cells[btni].setPaintFlag(true);
                } else {
                    cells[btni].setPaintFlag(false);
                }

                cells[btni].repaint(1);
            }
        }
        repaint(1);

        int state = backend.gameGetState();
        if (state == backend.STATE_LOST) {
            MessageBox dialog = new MessageBox(this, backend.TXT_TITLE, backend.TXT_LOST);
            dispose();
            System.exit(0);
        } else if (state == backend.STATE_WON) {
            MessageBox dialog = new MessageBox(this, backend.TXT_TITLE, backend.TXT_WON);
            dispose();
            System.exit(0);
        }
    }

    /* bit test macros */
    private boolean CHECK_MINE(int x) { return  (((x) >> backend.CELL_BIT_MINE) & 1) == 1; };
    private boolean CHECK_FLAG(int x) { return  (((x) >> backend.CELL_BIT_FLAG) & 1) == 1; };
    private boolean CHECK_CLEAR(int x) { return (((x) >> backend.CELL_BIT_CLEAR) & 1) == 1; };

    private int BOARDXY(int x, int y) {
        return backend.getBoardCopy(size)[(y * size) + x];
    }
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

