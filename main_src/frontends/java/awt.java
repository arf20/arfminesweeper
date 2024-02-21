import java.awt.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

class awt_frame extends Frame {   
    int size, wWidth, wHeight;

    static final int HEADER_HEIGHT = 60;
    static final int CELL_SIZE     = 20;
    static final int CELL_MARGIN   = 2;
    static final int W_MARGIN      = 5;

    static final String TXT_TITLE = "arfminesweeper";

    static final String TXT_LOST = """
        \nYou died in a terrible explosion\n
        Killed from a german S-Mine in Denmark\n
        Your spontaneously dissasembled remains won't be recovered\n
        and you have no family left to cry for you,\n
        as they all died in the war.\n
        Among you, there were 2600 german POWs employed by the danish\n
        to clear mines between 1945 and 1947. As much as 500 died.\n
        The game is now over.\n""";

    static final String TXT_WON = """
        \nYou have finished your task.\n" \
        You have accopished the objective of clearing the 1.4M mines\n
        laid off by the germans during the occupation of Denmark.\n
        You won't be rewarded for your actions, and you will still be a POW\n
        until 1949.\n
        You have no family to return to, they all died in the war,\n
        There is no good ending in a war.\n
        But the game is now over.\n""";

    static final String TXT_QUIT = """
        \n\nYou tried sneaking out of the POW camp, how clever\n
        You were caught 5km away from it, and shot in situ.\n""";
    
    awt_frame(int lsize) {
        size = lsize;

        wWidth = (2 * W_MARGIN) + (size * CELL_SIZE) + ((size - 1) * CELL_MARGIN);
        wHeight = HEADER_HEIGHT + W_MARGIN + (size * CELL_SIZE) +
            ((size - 1) * CELL_MARGIN);

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

        //pack();

        setSize(wWidth, wHeight);
        //setBounds(0, 0, wWidth, wHeight + 100);
        setUndecorated(true);
        //setPreferredSize(new Dimension(wWidth, wHeight));
        setTitle(TXT_TITLE);
        setLayout(null);
        setVisible(true);
    }

    /* To make this class runnable standalone */
    public static void main(String[] args) {
        awt_frame f = new awt_frame(8);
    }
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
