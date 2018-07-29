#include <stdio.h>
#include <unistd.h> /* for STDIN_FILENO */
#include <termios.h>

void printBoard(void);
void readCh(void);

// globals
char keys[] = {0, 0, 0, 0};  // the keys entered 
/*  (remember: some keys produce multiple characters;
    i.e. left = \x1b [ D (3 chars)
*/
int curpos = 0;             // position of the cursor
int i;                      // helper
int p;                      // position for printing board in loop
char board[] = {            // board
                ' ', ' ', ' ', 
                ' ', ' ', ' ', 
                ' ', ' ', ' ', };
int move = 0;              // (move % 2 == 0) = X; (move % 2 == 1) = O
char winner;

struct termios old, t;


int main() {
    tcgetattr(STDIN_FILENO, &old);
    t = old;

    // format Unix terminal (move cursor to (0,0), clear to bottom, change title to "Tic-Tac-Toe", turn off cursor (prevents flickering and looks better)
    printf("\x1b[0;0H\x1b[J\x1b]0;Tic-Tac-Toe\x07\x1b[?25l");

    printBoard();

    while (keys[0] != 'q') {
        readCh();

        // UP
        if (keys[2] == 27 && keys[1] == 91 && keys[0] == 65) {
            // cursor curpos on first row (move to last)
            if (curpos / 3 == 0) {
                curpos += 2 * 3;
            } else {
                curpos -= 3;
            }
        // DOWN
        } else if (keys[2] == 27 && keys[1] == 91 && keys[0] == 66) {
            // cursor curpos on last row (move to first)
            if (curpos / 3 == 2) {
                curpos -= 2 * 3;
            } else {
                curpos += 3;
            }
        // RIGHT
        } else if (keys[2] == 27 && keys[1] == 91 && keys[0] == 67) {
            // cursor curpos on right-most column (move to left-most)
            if (curpos % 3 == 2) {
                curpos -= 2;
            } else {
                curpos += 1;
            }
        // LEFT
        } else if (keys[2] == 27 && keys[1] == 91 && keys[0] == 68) {
            // cursor curpos on left-most column (move to right-most)
            if (curpos % 3 == 0) {
                curpos += 2;
            } else {
                curpos -= 1;
            }
        // ENTER or SPACE
        } else if (keys[0] == 13 || keys[0] == 32) {
            if (board[curpos] == ' ') {
                if (move % 2 == 0) {
                    board[curpos] = 'X';
                } else {
                    board[curpos] = 'O';
                }
                move++;
            }
        } else if (keys[0] == 'r') {
            printf("\nrestart? [y/N] ");
            readCh();
            printf("\x1b[0;0H\x1b[J");
            if (keys[0] == 'y' || keys[0] == 'Y') {
                move = 0;
                for (i = 0; i < 9; i++) {
                    board[i] = ' ';
                }
            }
        } else if (keys[0] == 'p') {
            printf("\x1b[0;0H\x1b[J");
        }

        printBoard();

        // check for winner
        // ----------------
        if ((board[0] == 'X' && board[1] == 'X' && board[2] == 'X') || // row 1
           (board[3] == 'X' && board[4] == 'X' && board[5] == 'X') ||  // row 2
           (board[6] == 'X' && board[7] == 'X' && board[8] == 'X') ||  // row 3
           (board[0] == 'X' && board[3] == 'X' && board[6] == 'X') ||  // col 1
           (board[1] == 'X' && board[4] == 'X' && board[7] == 'X') ||  // col 2
           (board[2] == 'X' && board[5] == 'X' && board[8] == 'X') ||  // col 3
           (board[0] == 'X' && board[4] == 'X' && board[8] == 'X') ||  // diag top-left to bottom-right
           (board[2] == 'X' && board[4] == 'X' && board[6] == 'X')) {  // diag top-right to bottom-left
            printf("\n\n\x1b[34;1mX\x1b[m wins!           [enter]");
            for (i = 0; i < 4; i++) {
                keys[i] = 0;
            }
            while (keys[0] != 13) {
                readCh();
            }
                printf("\x1b[0;0H\x1b[J");
            for (i = 0; i < 9; i++) {
                board[i] = ' ';
            }
            move = 0;
            printBoard();
        }
        if ((board[0] == 'O' && board[1] == 'O' && board[2] == 'O') || // row 1
           (board[3] == 'O' && board[4] == 'O' && board[5] == 'O') ||  // row 2
           (board[6] == 'O' && board[7] == 'O' && board[8] == 'O') ||  // row 3
           (board[0] == 'O' && board[3] == 'O' && board[6] == 'O') ||  // col 1
           (board[1] == 'O' && board[4] == 'O' && board[7] == 'O') ||  // col 2
           (board[2] == 'O' && board[5] == 'O' && board[8] == 'O') ||  // col 3
           (board[0] == 'O' && board[4] == 'O' && board[8] == 'O') ||  // diag top-left to bottom-right
           (board[2] == 'O' && board[4] == 'O' && board[6] == 'O')) {  // diag top-right to bottom-left
            printf("\n\n\x1b[33;1mO\x1b[m wins!           [enter]");
            for (i = 0; i < 4; i++) {
                keys[i] = 0;
            }
            while (keys[0] != 13) {
                readCh();
            }
            printf("\x1b[0;0H\x1b[J");
            for (i = 0; i < 9; i++) {
                board[i] = ' ';
            }
            move = 0;
            printBoard();
        }
        
        // check for tie
        if (move == 9) {
            printf("\n\nIt's a tie!          [Enter]");
            for (i = 0; i < 4; i++) {
                keys[i] = 0;
            }
            while (keys[0] != 13) {
                readCh();
            }
            printf("\x1b[0;0H\x1b[J");
            for (i = 0; i < 9; i++) {
                board[i] = ' ';
            }
            move = 0;
            printBoard();

        }
    }
        
    printf("\x1b[?25h");
    return 0;
}

void readCh() {
    // move all chars up one to make way for next char
    keys[3] = keys[2];
    keys[2] = keys[1];
    keys[1] = keys[0];

    // wait for next char
    cfmakeraw(&t); // set terminal to raw mode
    tcsetattr(STDIN_FILENO, TCSANOW, &t); // implement
    keys[0] = getc(stdin); // read char and store to index 0
    tcsetattr(STDIN_FILENO, TCSANOW, &old); // set terminal to normal mode (old settings)
}


void printBoard() {
    printf("\x1b[0;0HKEYS\n----\n  'q' quit\n  'r' restart\n  'p' repaint\n\n");
    for (p = 0; p < 9; p++) {
        printf("|");

        if (p == curpos) {
            printf("\x1b[7m"); // turn on rev. color (if cursor on this pos)
        }

        if (board[p] == 'X') {
            printf("\x1b[34;1m");
        } else if (board[p] == 'O') {
            printf("\x1b[33;1m");
        }

        putchar(board[p]);

        if (board[p] == 'X' || board[p] == 'O') {
            printf("\x1b[m");
        }

        if (p == curpos) {
            printf("\x1b[27m"); // turn off rev. color (if cursor on this pos)
        }

        if (p % 3 == 2) {
            printf("|\n");
        }
    }
    fflush(stdin);
}