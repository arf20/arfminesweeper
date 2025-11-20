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

    swing.java: Java Swing frontend

*/

import java.io.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

class MinesweeperSwing extends JFrame implements MouseListener {
    final int size, wWidth, wHeight;

    /* interface to C core */
    final ArfBackend backend;

    final ImageIcon flag;

    final JLabel flagLabel;
    JButton[] cells = new JButton[1]; // java sucks
    JLabel[] numbers = new JLabel[1]; // java sucks

    public MinesweeperSwing(int lsize) {
        backend = new ArfBackend();

        size = lsize;

        wWidth = (2 * backend.W_MARGIN) + (size * backend.CELL_SIZE) +
            ((size - 1) * backend.CELL_MARGIN);
        wHeight = backend.HEADER_HEIGHT + backend.W_MARGIN +
            (size * backend.CELL_SIZE) + ((size - 1) * backend.CELL_MARGIN);

        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        setResizable(false);
        setLayout(null);
        setTitle(backend.TXT_TITLE);

        setVisible(true);
        Insets insets = getInsets();
        setSize(wWidth + insets.left + insets.right, wHeight + insets.top + insets.bottom);

        JLabel title = new JLabel(backend.TXT_TITLE);
        title.setBounds(5, 15, 100, 20);
        add(title);

        flagLabel = new JLabel(Integer.toString(backend.gameGetFlagsLeft()));
        flagLabel.setBounds(wWidth - 25, 35, 50, 20);
        add(flagLabel);

        flag = new ImageIcon(backend.FLAG_PNG_PATH);

        cells = new JButton[size*size];
        numbers = new JLabel[size*size];

        for (int y = 0; y < size; y++) {
            for (int x = 0; x < size; x++) {
                int cX = backend.W_MARGIN + (x * (backend.CELL_SIZE + backend.CELL_MARGIN));
                int cY = backend.HEADER_HEIGHT + (y * (backend.CELL_SIZE + backend.CELL_MARGIN));
                int btni = (y*size) + x;

                cells[btni] = new JButton();
                cells[btni].setBounds(cX, cY, backend.CELL_SIZE, backend.CELL_SIZE);
                cells[btni].addMouseListener(this);
                add(cells[btni]);

                numbers[btni] = new JLabel();
                numbers[btni].setBounds(cX, cY, backend.CELL_SIZE, backend.CELL_SIZE);

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
                numbers[btni].setVisible(false);
                add(numbers[btni]);
            }
        }

        SwingUtilities.updateComponentTreeUI(this);
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

        updateBoard();
    }

    private void updateBoard() {
        flagLabel.setText(Integer.toString(backend.gameGetFlagsLeft()));

        for (int y = 0; y < size; y++) {
            for (int x = 0; x < size; x++) {
                int btni = (y * size) + x;

                if (backend.CHECK_CLEAR(backend.BOARDXY(x, y))) {
                    cells[btni].setVisible(false);
                    numbers[btni].setVisible(true);
                } else if (backend.CHECK_FLAG(backend.BOARDXY(x, y))) {
                    cells[btni].setIcon(flag);
                } else {
                    cells[btni].setIcon(null);
                }
            }
        }
        SwingUtilities.updateComponentTreeUI(this);

        int state = backend.gameGetState();
        if (state == backend.STATE_LOST) {
            JOptionPane.showMessageDialog(this, backend.TXT_LOST, backend.TXT_TITLE, JOptionPane.ERROR_MESSAGE);
            dispose();
            System.exit(0);
        } else if (state == backend.STATE_WON) {
            JOptionPane.showMessageDialog(this, backend.TXT_WON, backend.TXT_TITLE, JOptionPane.INFORMATION_MESSAGE);
            dispose();
            System.exit(0);
        }
    }

    public void mouseExited(MouseEvent e) { }
    public void mouseEntered(MouseEvent e) { }
    public void mousePressed(MouseEvent e) { }
    public void mouseReleased(MouseEvent e) { }
}

public class swing {
    static void main(int lsize)  {
        MinesweeperSwing f = new MinesweeperSwing(lsize);

    }
}

