/* 
 * CSE 562: Artificial Intelligence
 * Assignment 3
 * Alper Kaplan
 * 20174033006
 *
 * Prepared for the competition
 * that will be held on December 28, 2017
 * 
 * UPDATE: This version had won the competition.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>


// Define height and width of the board
#define     BOARD_HEIGHT          7
#define     BOARD_WIDTH           7
#define     P_INFINITY       100000.0
#define     N_INFINITY      -100000.0


/**
 * START: CONSTANT TEXTS
 **/
const char      BOARD_VERTICAL_LETTERS[7]               = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
const char      BOARD_HORIZONTAL_LINE[]                 = "\n   ___________________________\n";

const char      COMPUTER_MOVES_THE_PIECE_TEXT[]         = "\nComputer moves the piece at %c%c to %c%c\n\n";

const char      TURN_LIMIT_REACHED_TEXT[]               = "\n\nTurn limit is reached...\nFinal view of the board:\n\n";

const char      BOARD_AT_THE_BEGINNING_TURN_TEXT[]      = "\n\nBOARD at the beginning of TURN %d/%d:\n";

const char      PLAYER_1_WINS_TEXT[]                    = "PLAYER 1 (X) WINS!\n\n";
const char      PLAYER_2_WINS_TEXT[]                    = "PLAYER 2 (O) WINS!\n\n";
const char      DRAW_TEXT[]                             = "DRAW!\n\n";

const char      CHAR_HUMAN           = 'H';
const char      CHAR_COMPUTER        = 'C';

const char      CHAR_EMPTY_SPACE     = ' ';

const char      P1_PIECE_X           = 'X';
const char      P2_PIECE_O           = 'O';

const char      P1_1                 = '1';
const char      P2_2                 = '2';
/**
 * END: CONSTANT TEXTS
 **/


/**
 *  START: GLOBAL VARIABLES
 **/
char    BOARD[BOARD_HEIGHT][BOARD_WIDTH] = { "       ", "       ", "       ", "       ", "       ", "       ", "       " };

int     DEPTH   = 7;

int     number_of_pieces_for_each_player;
int     turn_limit;
char    p1;

int     turn_counter;
/**
 *  END: GLOBAL VARIABLES
 **/


// START: STRUCTS
typedef struct {
    char    next_states[BOARD_HEIGHT][BOARD_WIDTH];
    double  mv_counts;
} Expand;

typedef struct {
    char    STATE[BOARD_HEIGHT][BOARD_WIDTH];
    int     depth;
    double  max;
    double  min;
} Node;

typedef struct {
    int     encoded_value_X;
    int     encoded_value_O;
} Encoded_States;
// END: STRUCTS


void print_board(char BOARD[][BOARD_WIDTH]) {
	
    printf("\n    1");
    for (int i = 2; i < BOARD_HEIGHT + 1; i++)
        printf("   %d", i);

    printf("%s", BOARD_HORIZONTAL_LINE);

    for (int i = 0; i < BOARD_HEIGHT; i++) {

        printf("%c ", BOARD_VERTICAL_LETTERS[i]);

        for (int j = 0; j < BOARD_WIDTH; j++)
            printf("| %c ", BOARD[i][j]);

        printf("|%s", BOARD_HORIZONTAL_LINE);
    }

    printf("\n\n\n");
}

void print_computer_move(char BOARD[][BOARD_WIDTH], char NEXT_STATE[][BOARD_WIDTH]) {
    
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {

            int tmp_I, tmp_J, tmp_2I, tmp_2J;

            if (BOARD[i][j] != NEXT_STATE[i][j]) {
                
                tmp_I = i;
                tmp_J = j;

                if (i + 1 <= BOARD_HEIGHT - 1 && BOARD[i + 1][j] != NEXT_STATE[i + 1][j]) {
                    tmp_2I = i + 1;
                    tmp_2J = j;
                }
                else if (i - 1 >= 0 && BOARD[i - 1][j] != NEXT_STATE[i - 1][j]) {
                    tmp_2I = i - 1;
                    tmp_2J = j;
                }
                else if (j + 1 <= BOARD_WIDTH - 1 && BOARD[i][j + 1] != NEXT_STATE[i][j + 1]) {
                    tmp_2I = i;
                    tmp_2J = j + 1;
                }
                else if (j - 1 >= 0 && BOARD[i][j - 1] != NEXT_STATE[i][j - 1]) {
                    tmp_2I = i;
                    tmp_2J = j - 1;                        
                }

                char firstLoc[2], secLoc[2];

                for (int z = 0; z < BOARD_HEIGHT; z++) {

                    if (tmp_I == z)
                        firstLoc[0] = 'a' + z;
                    if (tmp_J == z)
                        firstLoc[1] = '1' + z;

                    if (tmp_2I == z)
                        secLoc[0] = 'a' + z;
                    if (tmp_2J == z)
                        secLoc[1] = '1' + z;                        
                }

                if (BOARD[i][j] == CHAR_EMPTY_SPACE) {
                    printf(COMPUTER_MOVES_THE_PIECE_TEXT, secLoc[0], secLoc[1], firstLoc[0], firstLoc[1]);
                    return;
                }
                else {
                    printf(COMPUTER_MOVES_THE_PIECE_TEXT, firstLoc[0], firstLoc[1], secLoc[0], secLoc[1]);
                    return;
                }
            }
        }
    }
}

void calculate_move_counts(char STATE[][BOARD_WIDTH], double mv_counts[], bool game_over) {
    
    char mv_P1[BOARD_HEIGHT][BOARD_WIDTH];
    char mv_P2[BOARD_HEIGHT][BOARD_WIDTH];
    
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        memcpy(&mv_P1[i], &STATE[i], sizeof(STATE[0]));
        memcpy(&mv_P2[i], &STATE[i], sizeof(STATE[0]));
    }
    
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            
            if (STATE[i][j] == P1_PIECE_X) {
                
                if (i < BOARD_HEIGHT - 1 && STATE[i + 1][j] == CHAR_EMPTY_SPACE)
                    mv_P1[i + 1][j] = P1_1;
                if (i != 0 && STATE[i - 1][j] == CHAR_EMPTY_SPACE)
                    mv_P1[i - 1][j] = P1_1;
                if (j < BOARD_WIDTH - 1 && STATE[i][j + 1] == CHAR_EMPTY_SPACE)
                    mv_P1[i][j + 1] = P1_1;
                if (j != 0 && STATE[i][j - 1] == CHAR_EMPTY_SPACE)
                    mv_P1[i][j - 1] = P1_1;
            }
            else if (STATE[i][j] == P2_PIECE_O) {

                if (i < BOARD_HEIGHT - 1 && STATE[i + 1][j] == CHAR_EMPTY_SPACE)
                    mv_P2[i + 1][j] = P2_2;
                if (i != 0 && STATE[i - 1][j] == CHAR_EMPTY_SPACE)
                    mv_P2[i - 1][j] = P2_2;
                if (j < BOARD_WIDTH - 1 && STATE[i][j + 1] == CHAR_EMPTY_SPACE)
                    mv_P2[i][j + 1] = P2_2;
                if (j != 0 && STATE[i][j - 1] == CHAR_EMPTY_SPACE)
                    mv_P2[i][j - 1] = P2_2;
            }
        }
    }
    
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {

            if (mv_P1[i][j] == P1_1)
                mv_counts[0]++;
            if (mv_P2[i][j] == P2_2)
                mv_counts[1]++;
        }
    }

    if (game_over) {
        
        printf("Player 1\n#Valid Moves: %d\n\n", (int)mv_counts[0]);
        print_board(mv_P1);
        printf("Player 2\n#Valid Moves: %d\n\n", (int)mv_counts[1]);
        print_board(mv_P2);
        
        if (mv_counts[0] == mv_counts[1])
            printf(DRAW_TEXT);
        else if (mv_counts[0] > mv_counts[1])
            printf("%s", PLAYER_1_WINS_TEXT);
        else
            printf("%s", PLAYER_2_WINS_TEXT);
    }
}

void check_who_wins() {

    print_board(BOARD);
    
    double mv_counts[2] = { 0.0, 0.0 };
    
    calculate_move_counts(BOARD, mv_counts, true);
}

void who_will_start_the_game() {
	
    while (1) {
        
        char p1_or_p2[100];

        printf("Enter 1 to become the Player 1 (X), 2 to Player 2 (O): ");
        scanf(" %s", p1_or_p2);		

        if (p1_or_p2[0] == '1' && (unsigned)strlen(p1_or_p2) == 1) {
            p1 = CHAR_HUMAN;

            printf("\nYou will start the game as Player 1 (X)!\nComputer is Player 2 (O)!\n\n");
            break;
        }
        else if (p1_or_p2[0] == '2' && (unsigned)strlen(p1_or_p2) == 1) {
            p1 = CHAR_COMPUTER;

            printf("\nComputer will start the game as Player 1 (X)!\nYou are Player 2 (O)!\n\n");
            break;
        }
        else
            printf("\nYou must enter either 1 or 2!\n\n");
    }
}

void assign_number_of_pieces() {

    while (1) {
        char local_num_of_pieces[100];

        printf("\nEnter the number of pieces that each player will have (1-24): ");
        scanf(" %s", local_num_of_pieces);

        if ((unsigned)strlen(local_num_of_pieces) == 1 && isdigit(local_num_of_pieces[0])) {

            number_of_pieces_for_each_player = atoi(local_num_of_pieces);
            break;
        }
        else if ((unsigned)strlen(local_num_of_pieces) == 2 && isdigit(local_num_of_pieces[0]) && isdigit(local_num_of_pieces[1])) {

            int temp_num_of_pieces = atoi(local_num_of_pieces);

            if (temp_num_of_pieces < 25) {
                number_of_pieces_for_each_player = atoi(local_num_of_pieces);
                break;
            }
        }

        printf("\nThe number of pieces for each player must be between 1 and 24!\n\n");
    }	
}

void assign_turn_limit() {
	
    while (1) {
        char local_turn_limit[100];

        printf("\nEnter a turn limit: ");
        scanf(" %s", local_turn_limit);

        bool is_number = true;
        for (int i = 0; i < (unsigned)strlen(local_turn_limit); i++) {

            if (!isdigit(local_turn_limit[i]))
                is_number = false;
        }

        int temp_turn_limit = atoi(local_turn_limit);

        if (!is_number)
                printf("\nYou must enter either a positive number or zero!\n");
        else if ((unsigned)strlen(local_turn_limit) == 1 && temp_turn_limit == 0) {

            turn_limit = temp_turn_limit;
            break;
        }
        else if (temp_turn_limit != 0 && temp_turn_limit > 0) {

            turn_limit = (int) temp_turn_limit;
            break;
        }
        else
            printf("\nYou must enter either a positive number or zero!\n");
    }
}

void set_a_piece_randomly(char piece) {
	
    while (1) {

        // random generate the board
        int x = rand() % BOARD_HEIGHT;
        int y = rand() % BOARD_WIDTH;

        if (BOARD[x][y] == CHAR_EMPTY_SPACE) {
            BOARD[x][y] = piece;
            break;
        }
    }
}

bool check_if_location_input_is_correct(char *location) {
	
    if (
        (unsigned)strlen(location) == 2
       &&
        (location[0] == 'a' || location[0] == 'b' || location[0] == 'c' || location[0] == 'd' ||
         location[0] == 'e' || location[0] == 'f' || location[0] == 'g')
       &&
        (location[1] == '1' || location[1] == '2' || location[1] == '3' || location[1] == '4' ||
         location[1] == '5' || location[1] == '6' || location[1] == '7')
       )
        return true;
    else
        return false;
}

void set_a_piece_manually(char piece) {
	
    while (1) {
        char location[100];

        if (piece == P1_PIECE_X)
            printf("\nEnter a location for Player 1 (X)!");
        else if (piece == P2_PIECE_O)
            printf("\nEnter a location for Player 2 (O)!");

        printf("\nEnter a location (a1-g7): ");
        scanf(" %s", location);

        if (check_if_location_input_is_correct(location))
        {
            int x = ((int) location[0]) % 97;
            int y = ((int) location[1]) % 49;

            if (BOARD[x][y] == CHAR_EMPTY_SPACE) {
                BOARD[x][y] = piece;
                break;
            }
            else
                printf("\n%s is already occupied!", location);
        }
        else
            printf("\n\nYou entered a wrong format. Please, try again.");
    }
}

void generate_board(int boardGenerationType) {
	
    if (boardGenerationType == 1)
        srand((unsigned int)time(NULL));
    
    for (int i = 1; i <= number_of_pieces_for_each_player; i++) {

        // If board is chosen as random generated
        // then, just send one of each player's pieces at a time
        if (boardGenerationType == 1) {
            set_a_piece_randomly(P1_PIECE_X);
            set_a_piece_randomly(P2_PIECE_O);
        }

        // If board is to be manually generated
        // then, send pieces to set_a_piece_manually function
        else if (boardGenerationType == 2) {

            printf("\n%d/%d", i, number_of_pieces_for_each_player);
            set_a_piece_manually(P1_PIECE_X);
        }
    }
    
    for (int i = 1; i <= number_of_pieces_for_each_player; i++) {
        
        if (boardGenerationType == 2) {

            printf("\n%d/%d", i, number_of_pieces_for_each_player);
            set_a_piece_manually(P2_PIECE_O);
        }
    }
}

void determine_board_generation_type() {
    
    while (1) {
        char arr_generate_board[100];

        printf("\nInput 1 to generate random board, 2 to manually enter locations: ");
        scanf(" %s", arr_generate_board);

        if (arr_generate_board[0] == '1' && (unsigned)strlen(arr_generate_board) == 1) {

            generate_board(1);
            printf("\nRandom board generated!\n");

            break;
        }
        else if (arr_generate_board[0] == '2' && (unsigned)strlen(arr_generate_board) == 1) {

            printf("\nGenerate the board manually...\n");
            generate_board(2);

            break;
        }
        else
            printf("\nYou must enter either 1 or 2!\n\n");
    }
}

void assign_depth() {
    
    while (1) {
        char local_depth[100];

        printf("\nEnter the depth for the computer's moves: ");
        scanf(" %s", local_depth);

        bool isNumber = true;
        for (int i = 0; i < (unsigned)strlen(local_depth); i++) {

            if (!isdigit(local_depth[i]))
                isNumber = false;
        }

        int temp_depth = atoi(local_depth);

        if (!isNumber)
            printf("\nYou must enter a number between 1 and 10!\n");
        else if (temp_depth > 9)
            printf("\nThe depth must be lower than 10!\n");
        else if (temp_depth > 0) {

            DEPTH = (int) temp_depth;
            break;
        }
        else
            printf("\nYou must enter a number between 1 and 10!\n");
    }    
}

void initialize_game() {
    
    printf("\n\nCSE 562: Artificial Intelligence Assignment 3 Demo...\n\n");	

    // Who will be the Player 1 (X) or Player 2 (O)?
    // Who will start the game?
    who_will_start_the_game();

    // How many number of pieces should each player have
    assign_number_of_pieces();

    // Turn limit
    assign_turn_limit();

    // Enter computer move depth
    assign_depth();
    
    // Generate board
    determine_board_generation_type();
}

bool can_make_a_move(int x, int y) {

    if ( (x - 1 >= 0 && BOARD[x - 1][y] == CHAR_EMPTY_SPACE) || (x + 1 <= BOARD_HEIGHT - 1 && BOARD[x + 1][y] == CHAR_EMPTY_SPACE) ||
         (y - 1 >= 0 && BOARD[x][y - 1] == CHAR_EMPTY_SPACE) || (y + 1 <= BOARD_WIDTH - 1 && BOARD[x][y + 1] == CHAR_EMPTY_SPACE) )
        return true;
    else
        return false;
}

bool can_opponent_make_a_move(char piece) {

    for (int x = 0; x < BOARD_HEIGHT; x++)
        for (int y = 0; y < BOARD_WIDTH; y++) {
            
            if (BOARD[x][y] == piece)
                if ( (x - 1 >= 0 && BOARD[x - 1][y] == CHAR_EMPTY_SPACE) || (x + 1 <= BOARD_HEIGHT - 1 && BOARD[x + 1][y] == CHAR_EMPTY_SPACE) ||
                     (y - 1 >= 0 && BOARD[x][y - 1] == CHAR_EMPTY_SPACE) || (y + 1 <= BOARD_WIDTH - 1 && BOARD[x][y + 1] == CHAR_EMPTY_SPACE) )
                    return true;
        }
    
    return false;
}

void human_move() {

    bool flag_finish = false;

    while (!flag_finish) {

        char piece_location[100];
		
	printf("Choose a piece to move: ");
	scanf(" %s", piece_location);
		
	if (check_if_location_input_is_correct(piece_location)) {
			
            int x = ((int) piece_location[0]) % 97;
            int y = ((int) piece_location[1]) % 49;
            
            if (p1 == CHAR_HUMAN && BOARD[x][y] == P1_PIECE_X) {
				
                if (can_make_a_move(x, y)) {

                    while (1) {
		
                        char piece_new_location[100];
								
			printf("\nChoose the new position for %s: ", piece_location);		
                        scanf(" %s", piece_new_location);
                        
                        if (check_if_location_input_is_correct(piece_new_location)) {
						
                            int newX = ((int) piece_new_location[0]) % 97;
                            int newY = ((int) piece_new_location[1]) % 49;
							
                            if ( x - 1 >= 0 && BOARD[x - 1][y] == CHAR_EMPTY_SPACE && x - 1 == newX && y == newY ) {
                                
                                BOARD[newX][newY] = P1_PIECE_X;
                                BOARD[x][y] = CHAR_EMPTY_SPACE;
                                
                                printf("Player moves the piece at %s to %s\n\n", piece_location, piece_new_location);				
				
                                flag_finish = true;
                                break;
                            }
                            else if ( x + 1 <= BOARD_HEIGHT - 1 && BOARD[x + 1][y] == CHAR_EMPTY_SPACE && x + 1 == newX && y == newY ) {
                                
                                BOARD[newX][newY] = P1_PIECE_X;
				BOARD[x][y] = CHAR_EMPTY_SPACE;
				
                                printf("Player moves the piece at %s to %s\n\n", piece_location, piece_new_location);								
				
                                flag_finish = true;
				break;
                            }
                            else if ( y + 1 >= 0 && BOARD[x][y + 1] == CHAR_EMPTY_SPACE && x == newX && y + 1 == newY ) {
								
                                BOARD[newX][newY] = P1_PIECE_X;
                                BOARD[x][y] = CHAR_EMPTY_SPACE;
				
                                printf("\nPlayer moves the piece at %s to %s\n\n", piece_location, piece_new_location);								
				
                                flag_finish = true;								
				break;
                            }
                            else if ( y - 1 <= BOARD_WIDTH - 1 && BOARD[x][y - 1] == CHAR_EMPTY_SPACE && x == newX && y - 1 == newY ) {
								
                                BOARD[newX][newY] = P1_PIECE_X;
				BOARD[x][y] = CHAR_EMPTY_SPACE;
				
                                printf("\nPlayer moves the piece at %s to %s\n\n", piece_location, piece_new_location);
				
                                flag_finish = true;								
				break;
                            }
                            else
                                printf("\nThat piece cannot move there!\n");
			}
			else
                            printf("\nThat piece cannot move there!\n");
                    }
                }
		else
                    printf("\nThat piece cannot make any moves!\n");
            }
            else if (p1 == CHAR_COMPUTER && BOARD[x][y] == P2_PIECE_O) {

                if (can_make_a_move(x, y)) {

                    while (1) {

                        char piece_new_location[100];

                        printf("\nChoose the new position for %s: ", piece_location);			
                        scanf(" %s", piece_new_location);

                        if (check_if_location_input_is_correct(piece_new_location)) {

                            int newX = ((int) piece_new_location[0]) % 97;
                            int newY = ((int) piece_new_location[1]) % 49;


                            if ( x - 1 >= 0 && BOARD[x - 1][y] == CHAR_EMPTY_SPACE && x - 1 == newX && y == newY ) {

                                BOARD[newX][newY] = P2_PIECE_O;
                                BOARD[x][y] = CHAR_EMPTY_SPACE;

                                printf("\nPlayer moves the piece at %s to %s", piece_location, piece_new_location);								

                                flag_finish = true;
                                break;
                            }
                            else if ( x + 1 <= BOARD_HEIGHT - 1 && BOARD[x + 1][y] == CHAR_EMPTY_SPACE && x + 1 == newX && y == newY ) {

                                BOARD[newX][newY] = P2_PIECE_O;
                                BOARD[x][y] = CHAR_EMPTY_SPACE;

                                printf("\nPlayer moves the piece at %s to %s", piece_location, piece_new_location);								

                                flag_finish = true;
                                break;
                            }
                            else if ( y + 1 >= 0 && BOARD[x][y + 1] == CHAR_EMPTY_SPACE && x == newX && y + 1 == newY ) {

                                BOARD[newX][newY] = P2_PIECE_O;
                                BOARD[x][y] = CHAR_EMPTY_SPACE;

                                printf("\nPlayer moves the piece at %s to %s", piece_location, piece_new_location);								

                                flag_finish = true;
                                break;
                            }
                            else if ( y - 1 <= BOARD_WIDTH - 1 && BOARD[x][y - 1] == CHAR_EMPTY_SPACE && x == newX && y - 1 == newY ) {

                                BOARD[newX][newY] = P2_PIECE_O;
                                BOARD[x][y] = CHAR_EMPTY_SPACE;

                                printf("\nPlayer moves the piece at %s to %s", piece_location, piece_new_location);								

                                flag_finish = true;
                                break;
                            }
                            else
                                printf("\nThat piece cannot move there!\n");
                        }
                        else
                            printf("\nThat piece cannot move there!\n");
                    }
                }
            }
        else {

            char humanPiece;

            if (p1 == CHAR_HUMAN)
                humanPiece = P1_PIECE_X;
            else
                humanPiece = P2_PIECE_O;

            printf("\nYou must pick one of your pieces (%c)!\n", humanPiece);
        }
    }
    else {

        char humanPiece;

        if (p1 == CHAR_HUMAN)
            humanPiece = P1_PIECE_X;
        else
            humanPiece = P2_PIECE_O;

            printf("\nYou must pick one of your pieces (%c)!\n", humanPiece);
        }
    }
    
    print_board(BOARD);
}

int calculate_state_number(char STATE[][BOARD_WIDTH], char piece) {

    int stateCounter = 0;
    
    for (int i = 0; i < BOARD_HEIGHT; i++)
        for (int j = 0; j < BOARD_WIDTH; j++) {

            if (piece == P1_PIECE_X && STATE[i][j] == P1_PIECE_X) {

                if (i + 1 < BOARD_HEIGHT && STATE[i + 1][j] == CHAR_EMPTY_SPACE)
                    stateCounter++;
                if (i - 1 >= 0 && STATE[i - 1][j] == CHAR_EMPTY_SPACE)
                    stateCounter++;
                if (j + 1 < BOARD_WIDTH && STATE[i][j + 1] == CHAR_EMPTY_SPACE)
                    stateCounter++;
                if (j - 1 >= 0 && STATE[i][j - 1] == CHAR_EMPTY_SPACE)
                    stateCounter++;
            }
            else if (piece == P2_PIECE_O && STATE[i][j] == P2_PIECE_O) {

                if (i + 1 < BOARD_HEIGHT && STATE[i + 1][j] == CHAR_EMPTY_SPACE)
                    stateCounter++;
                if (i - 1 >= 0 && STATE[i - 1][j] == CHAR_EMPTY_SPACE)
                    stateCounter++;
                if (j + 1 < BOARD_WIDTH && STATE[i][j + 1] == CHAR_EMPTY_SPACE)
                    stateCounter++;
                if (j - 1 >= 0 && STATE[i][j - 1] == CHAR_EMPTY_SPACE)
                    stateCounter++;
            }
        }
    
    return stateCounter;
}

void expand_a_state(char STATE[][BOARD_WIDTH], char piece, Expand *expand) {
    
    char tempState[BOARD_HEIGHT][BOARD_WIDTH];
    
    int k = 0;
    
    for (int i = 0; i < BOARD_HEIGHT; i++)
        memcpy(&tempState[i], &STATE[i], sizeof(STATE[0]));
    
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            
            if (piece == P1_PIECE_X && tempState[i][j] == P1_PIECE_X) {

                if (i + 1 <= BOARD_HEIGHT - 1 && tempState[i + 1][j] == CHAR_EMPTY_SPACE) {
                    
                    tempState[i][j] = CHAR_EMPTY_SPACE;
                    tempState[i + 1][j] = P1_PIECE_X;
                    
                    for (int m = 0; m < BOARD_HEIGHT; m++)
                        memcpy(&expand[k].next_states[m], &tempState[m], sizeof(tempState[0]));
                    
                    k++;
                    
                    tempState[i][j] = P1_PIECE_X;
                    tempState[i + 1][j] = CHAR_EMPTY_SPACE;
                }
                if (i - 1 >= 0 && tempState[i - 1][j] == CHAR_EMPTY_SPACE) {

                    tempState[i][j] = CHAR_EMPTY_SPACE;
                    tempState[i - 1][j] = P1_PIECE_X;
                    
                    for (int m = 0; m < BOARD_HEIGHT; m++)
                        memcpy(&expand[k].next_states[m], &tempState[m], sizeof(tempState[0]));
                    
                    k++;
                    
                    tempState[i][j] = P1_PIECE_X;
                    tempState[i - 1][j] = CHAR_EMPTY_SPACE;
                }
                if (j + 1 <= BOARD_WIDTH - 1 && tempState[i][j + 1] == CHAR_EMPTY_SPACE) {
                    
                    tempState[i][j] = CHAR_EMPTY_SPACE;
                    tempState[i][j + 1] = P1_PIECE_X;
                    
                    for (int m = 0; m < BOARD_HEIGHT; m++)
                        memcpy(&expand[k].next_states[m], &tempState[m], sizeof(tempState[0]));
                    
                    k++;
                    
                    tempState[i][j] = P1_PIECE_X;
                    tempState[i][j + 1] = CHAR_EMPTY_SPACE;
                }
                if (j - 1 >= 0 && tempState[i][j - 1] == CHAR_EMPTY_SPACE) {
                    
                    tempState[i][j] = CHAR_EMPTY_SPACE;
                    tempState[i][j - 1] = P1_PIECE_X;
                    
                    for (int m = 0; m < BOARD_HEIGHT; m++)
                        memcpy(&expand[k].next_states[m], &tempState[m], sizeof(tempState[0]));
                    
                    k++;
                    
                    tempState[i][j] = P1_PIECE_X;
                    tempState[i][j - 1] = CHAR_EMPTY_SPACE;
                }
            }
            else if (piece == P2_PIECE_O && tempState[i][j] == P2_PIECE_O) {

                if (i + 1 <= BOARD_HEIGHT - 1 && tempState[i + 1][j] == CHAR_EMPTY_SPACE) {
                    
                    tempState[i][j] = CHAR_EMPTY_SPACE;
                    tempState[i + 1][j] = P2_PIECE_O;
                    
                    for (int m = 0; m < BOARD_HEIGHT; m++)
                        memcpy(&expand[k].next_states[m], &tempState[m], sizeof(tempState[0]));
                    
                    k++;
                    
                    tempState[i][j] = P2_PIECE_O;
                    tempState[i + 1][j] = CHAR_EMPTY_SPACE;
                }
                if (i - 1 >= 0 && tempState[i - 1][j] == CHAR_EMPTY_SPACE) {

                    tempState[i][j] = CHAR_EMPTY_SPACE;
                    tempState[i - 1][j] = P2_PIECE_O;
                    
                    for (int m = 0; m < BOARD_HEIGHT; m++)
                        memcpy(&expand[k].next_states[m], &tempState[m], sizeof(tempState[0]));
                    
                    k++;
                    
                    tempState[i][j] = P2_PIECE_O;
                    tempState[i - 1][j] = CHAR_EMPTY_SPACE;
                }
                if (j + 1 <= BOARD_WIDTH - 1 && tempState[i][j + 1] == CHAR_EMPTY_SPACE) {

                    tempState[i][j] = CHAR_EMPTY_SPACE;
                    tempState[i][j + 1] = P2_PIECE_O;
                    
                    for (int m = 0; m < BOARD_HEIGHT; m++)
                        memcpy(&expand[k].next_states[m], &tempState[m], sizeof(tempState[0]));
                    
                    k++;
                    
                    tempState[i][j] = P2_PIECE_O;
                    tempState[i][j + 1] = CHAR_EMPTY_SPACE;
                }
                if (j - 1 >= 0 && tempState[i][j - 1] == CHAR_EMPTY_SPACE) {
                    
                    tempState[i][j] = CHAR_EMPTY_SPACE;
                    tempState[i][j - 1] = P2_PIECE_O;
                    
                    for (int m = 0; m < BOARD_HEIGHT; m++)
                        memcpy(&expand[k].next_states[m], &tempState[m], sizeof(tempState[0]));
                    
                    k++;
                    
                    tempState[i][j] = P2_PIECE_O;
                    tempState[i][j - 1] = CHAR_EMPTY_SPACE;
                }
            }
        }
    }
}

int calculate_manh_dist_to_super_tiles(char state[][BOARD_WIDTH], char piece) {
    
    char point_tiles[BOARD_HEIGHT][BOARD_WIDTH];
    
    for (int m = 0; m < BOARD_HEIGHT; m++)
        memcpy(&point_tiles[m], &state[m], sizeof(state[0]));
    
    for(int i = 1; i < BOARD_HEIGHT - 1; i++) {
        for(int j = 1; j < BOARD_HEIGHT - 1; j++) {
            
            if (piece == P1_PIECE_X && point_tiles[i][j] == P2_PIECE_O)
                continue;
            else if (piece == P2_PIECE_O && point_tiles[i][j] == P1_PIECE_X)
                continue;
            
            int score = 0;
            
            if (BOARD[i + 1][j] == ' ' && i != BOARD_HEIGHT - 1)
                score++;
            if (BOARD[i - 1][j] == ' ' && i != 0)
                score++;
            if (BOARD[i][j + 1] == ' ' && j != BOARD_WIDTH - 1)
                score++;
            if (BOARD[i][j - 1] == ' ' && j != 0)
                score++;
            
            point_tiles[i][j] = score + '0';
        }
    }
    
    int **piece_locations = malloc(number_of_pieces_for_each_player * sizeof(int*));
    
    for(int i = 0; i < number_of_pieces_for_each_player; i++)
        piece_locations[i] = malloc(2 * sizeof(int));
    
    int piece_index = 0;
    for(int i = 0; i < BOARD_HEIGHT; i++) {
        for(int j = 0; j < BOARD_HEIGHT; j++) {
            
            if (state[i][j] == piece) {
                piece_locations[piece_index][0] = i;
                piece_locations[piece_index][1] = j;
                
                piece_index++;
            }
        }   
    }
    
    int total_manhattan_distance = 0;
    
    for(int i = 1; i < BOARD_HEIGHT - 1; i++) {
        
        for(int j = 1; j < BOARD_HEIGHT - 1; j++) {
            
            if (point_tiles[i][j] == '4') {
                
                for (int m = 0; m < number_of_pieces_for_each_player; m++) {
                    
                    int i_piece_location, j_piece_location;
                    memcpy(&i_piece_location, &piece_locations[m][0], sizeof(int));
                    memcpy(&j_piece_location, &piece_locations[m][1], sizeof(int));
                    
                    int manhattan_dist = (i - j_piece_location) + (j - j_piece_location);
                    if (manhattan_dist < 0)
                        manhattan_dist *= -1;
                    
                    total_manhattan_distance += manhattan_dist;
                }
                
            }
        }
    }
    
    for (int m = 0; m < number_of_pieces_for_each_player; m++)
        free(piece_locations[m]);
    free(piece_locations);
    
    return total_manhattan_distance;
}

int subtract_walls_corners(char state[][BOARD_WIDTH], char piece) {
    
    int sum_subtract = 0;
    
    if (piece == 'X') {
        
        if (state[0][0] == piece)
            sum_subtract += 2;
        if (state[0][BOARD_WIDTH - 1] == piece)
            sum_subtract += 2;
        if (state[BOARD_HEIGHT - 1][0] == piece)
            sum_subtract += 2;
        if (state[BOARD_HEIGHT - 1][BOARD_WIDTH - 1] == piece)
            sum_subtract += 2;
        
        if (state[0][0] == P2_PIECE_O)
            sum_subtract -= 3;
        if (state[0][BOARD_WIDTH - 1] == P2_PIECE_O)
            sum_subtract -= 3;
        if (state[BOARD_HEIGHT - 1][0] == P2_PIECE_O)
            sum_subtract -= 3;
        if (state[BOARD_HEIGHT - 1][BOARD_WIDTH - 1] == P2_PIECE_O)
            sum_subtract -= 3;
    }
    else {
        
        if (state[0][0] == piece)
            sum_subtract += 2;
        if (state[0][BOARD_WIDTH - 1] == piece)
            sum_subtract += 2;
        if (state[BOARD_HEIGHT - 1][0] == piece)
            sum_subtract += 2;
        if (state[BOARD_HEIGHT - 1][BOARD_WIDTH - 1] == piece)
            sum_subtract += 2;
        
        if (state[0][0] == P1_PIECE_X)
            sum_subtract -= 3;
        if (state[0][BOARD_WIDTH - 1] == P1_PIECE_X)
            sum_subtract -= 3;
        if (state[BOARD_HEIGHT - 1][0] == P1_PIECE_X)
            sum_subtract -= 3;
        if (state[BOARD_HEIGHT - 1][BOARD_WIDTH - 1] == P1_PIECE_X)
            sum_subtract -= 3;
    }
    
    for(int i = 1; i < BOARD_HEIGHT - 1; i++) {
        
        if (state[0][i] == piece)
            sum_subtract++;
        if (state[i][0] == piece)
            sum_subtract++;
        if (state[BOARD_HEIGHT - 1][i] == piece)
            sum_subtract++;
        if (state[i][BOARD_WIDTH - 1] == piece)
            sum_subtract++;        
    }
    
    return sum_subtract;
}

double evaluation_function(char state[][BOARD_WIDTH]) {
    
    double mv_counts[2] = { 0.0, 0.0 };
    
    // according to move count
    calculate_move_counts(state, mv_counts, false);
    
    double subtract;
    double tile_scores;
    
    int turn_left = turn_limit - turn_counter + 1;
    
    if ( turn_left >= (turn_limit / 3)) {
        
        // according to walls and corners
        if (p1 == CHAR_COMPUTER)
            subtract = subtract_walls_corners(state, P1_PIECE_X);
        else
            subtract = subtract_walls_corners(state, P2_PIECE_O);

        
        // according to tile scores
        if (p1 == CHAR_COMPUTER) {
            tile_scores = calculate_manh_dist_to_super_tiles(state, P1_PIECE_X) -
                            calculate_manh_dist_to_super_tiles(state, P2_PIECE_O);
        }
        else {
            tile_scores = calculate_manh_dist_to_super_tiles(state, P2_PIECE_O) -
                            calculate_manh_dist_to_super_tiles(state, P1_PIECE_X);
        }
    }
    
    if (p1 == CHAR_COMPUTER) {
        
        if ( turn_left <= 7 /*turn_limit / 3*/ )
            return (mv_counts[0] - mv_counts[1]); // X minus O
        else if ( turn_left < (2 * turn_limit / 3) && (turn_left > (turn_limit / 3)) )
            return ((mv_counts[0] - mv_counts[1]) - (0.1 * subtract) + (0.1 * tile_scores)); // X minus O
        else if ( turn_left >= (2 * turn_limit / 3) )
            return ((mv_counts[0] - mv_counts[1]) - (0.05 * subtract) + (0.05 * tile_scores)); // X minus O
        else
            return (mv_counts[0] - mv_counts[1]);
    }
    else {
        if ( turn_left <= 7 /*turn_limit / 3*/ )
            return (mv_counts[1] - mv_counts[0]); // O minus X
        else if ( turn_left < (2 * turn_limit / 3) && (turn_left > (turn_limit / 3)) )
            return ((mv_counts[1] - mv_counts[0]) - (0.1 * subtract) + (0.1 * tile_scores)); // O minus X
        else if ( turn_left >= (2 * turn_limit / 3) )
            return ((mv_counts[1] - mv_counts[0]) - (0.05 * subtract) + (0.05 * tile_scores)); // O minus X
        else
            return (mv_counts[1] - mv_counts[0]);
    }
}

double computer_move(Node node, Encoded_States *encoded_states) {

    if (node.depth == DEPTH)
        return evaluation_function(node.STATE);
    
    int number_of_states;
    double *arr_evaluation_values;
    
    Expand *expands;
    
    // Expand Player 1's moves
    if ( (p1 == CHAR_COMPUTER && node.depth % 2 == 0) || (p1 == CHAR_HUMAN && node.depth % 2 != 0) ) {
        
        number_of_states = calculate_state_number(node.STATE, P1_PIECE_X);
        expands = (Expand*)malloc(number_of_states * sizeof(Expand));
        expand_a_state(node.STATE, P1_PIECE_X, expands);
    }
    
    // Expand Player 2's moves
    else if ( (p1 == CHAR_COMPUTER && node.depth % 2 != 0) || (p1 == CHAR_HUMAN && node.depth % 2 == 0) ) {
        
        number_of_states = calculate_state_number(node.STATE, P2_PIECE_O);
        expands = (Expand*)malloc(number_of_states * sizeof(Expand));
        expand_a_state(node.STATE, P2_PIECE_O, expands);
    }
    
    // sort expanded states according to their evaluation values
    if (node.depth == DEPTH) {

        int number_of_states_to_be_sorted;
        
        if (number_of_states / 3 < 6)
            number_of_states_to_be_sorted = number_of_states / 3;
        else
            number_of_states_to_be_sorted = 6;

        // first calculate the difference between move counts of two player's
        for (int i = 0; i < number_of_states_to_be_sorted; i++) {
            
            if (p1 == CHAR_COMPUTER)
                expands[i].mv_counts = evaluation_function(expands[i].next_states);
        }

        char char_arr_swap[BOARD_HEIGHT][BOARD_WIDTH];
        
        // bubble sort
        for (int c = 0 ; c < number_of_states_to_be_sorted; c++) {
            for (int d = 0 ; d < number_of_states_to_be_sorted - c; d++) {
                
                // increasing order OR decreasing order
                if ( (node.depth % 2 == 0 && expands[d].mv_counts > expands[d + 1].mv_counts) ||
                     (node.depth % 2 != 0 && expands[d].mv_counts < expands[d + 1].mv_counts) ) {

                    for (int g = 0; g < BOARD_HEIGHT; g++) {
                        memcpy(&char_arr_swap[g], &expands[d].next_states[g], sizeof(expands[d].next_states[0]));
                        memcpy(&expands[d].next_states[g], &expands[d + 1].next_states[g], sizeof(expands[d + 1].next_states[0]));
                        memcpy(&expands[d + 1].next_states[g], &char_arr_swap[g], sizeof(char_arr_swap[0]));
                    }
                }
            }
        }
    }
    
    arr_evaluation_values = (double*)malloc(number_of_states * sizeof(double));
    
    for (int i = 0; i < number_of_states; i++) {
        if (node.depth % 2 == 0)
            arr_evaluation_values[i] = N_INFINITY;
        else
            arr_evaluation_values[i] = P_INFINITY;
    }

    /*
    // encode Player 1 and Player 2 Pieces
    // use these values to check if later states are the same
    if (node.depth == 1) {
        
        encoded_states = (Encoded_States*)malloc(number_of_states * sizeof(Encoded_States));
        
        int encoded_value_X = 0;
        int encoded_value_O = 0;

        for (int k = 0; k < number_of_states; k++) {

            for (int m = 0; m < BOARD_HEIGHT; m++) {
                for (int n = 0; n < BOARD_WIDTH; n++) {

                    if (expands[k].next_states[m][n] == P1_PIECE_X)
                        encoded_value_X += m * BOARD_HEIGHT + n;
                    else if (expands[k].next_states[m][n] == P2_PIECE_O)
                        encoded_value_O += m * BOARD_HEIGHT + n;
                }
            }

            encoded_states[k].encoded_value_X = encoded_value_X;
            encoded_states[k].encoded_value_O = encoded_value_O;
        }
    }
    */
    
    double temp_max = N_INFINITY;
    double temp_min = P_INFINITY;
    
    // Iterate all expanded states
    for (int i = 0; i < number_of_states; i++) {
        
        // MAX
        if (node.depth % 2 == 0) {
            
            for (int j = 0; j < i; j++)
                if (temp_max < arr_evaluation_values[j])
                    memcpy(&temp_max, &arr_evaluation_values[j], sizeof(double));
            
            Node temp_node;
            
            for (int m = 0; m < BOARD_HEIGHT; m++)
                memcpy(&temp_node.STATE[m], &expands[i].next_states[m], sizeof(expands[i].next_states[0]));
            
            /*
            // if we reached to a same state; continue (do not expand it)
            if (node.depth > 3) {
            
                int encoded_value_X = 0;
                int encoded_value_O = 0;
                
                for (int m = 0; m < BOARD_HEIGHT; m++) {
                    for (int n = 0; n < BOARD_WIDTH; n++) {

                        if (temp_node.STATE[m][n] == P1_PIECE_X)
                            encoded_value_X += m * BOARD_HEIGHT + n;
                        else if (temp_node.STATE[m][n] == P2_PIECE_O)
                            encoded_value_O += m * BOARD_HEIGHT + n;
                    }
                }
                
                for (int g = 0; g < number_of_states; g++) {
                    
                    if (encoded_states[g].encoded_value_X == encoded_value_X && 
                        encoded_states[g].encoded_value_O == encoded_value_O)
                        continue;
                }
            }
            */
            
            int temp_node_depth = node.depth + 1;
            memcpy(&temp_node.depth, &temp_node_depth, sizeof(int));
            memcpy(&temp_node.max, &temp_max, sizeof(double));
            memcpy(&temp_node.min, &node.min, sizeof(double));
            
            double returned_evaluation_value = computer_move(temp_node, encoded_states);
            memcpy(&arr_evaluation_values[i], &returned_evaluation_value, sizeof(double));
            
            // pruned!!
            if (node.max >= arr_evaluation_values[i])
                break;
        }
        
        // MIN
        else {
            
            for (int j = 0; j < i; j++)
                if (temp_min > arr_evaluation_values[j])
                    memcpy(&temp_min, &arr_evaluation_values[j], sizeof(double));
            
            Node temp_node;
            
            for (int m = 0; m < BOARD_HEIGHT; m++)
                memcpy(&temp_node.STATE[m], &expands[i].next_states[m], sizeof(expands[i].next_states[0]));
            
            /*
            // if we reached to a same state; continue (do not expand it)
            if (node.depth > 3) {
                
                int encoded_value_X = 0;
                int encoded_value_O = 0;
                
                for (int m = 0; m < BOARD_HEIGHT; m++) {
                    for (int n = 0; n < BOARD_WIDTH; n++) {

                        if (temp_node.STATE[m][n] == P1_PIECE_X)
                            encoded_value_X += m * BOARD_HEIGHT + n;
                        else if (temp_node.STATE[m][n] == P2_PIECE_O)
                            encoded_value_O += m * BOARD_HEIGHT + n;
                    }
                }
                
                for (int g = 0; g < number_of_states; g++) {
                    
                    if (encoded_states[g].encoded_value_X == encoded_value_X &&
                        encoded_states[g].encoded_value_O == encoded_value_O)
                        continue;
                }
            }
            */
            
            int temp_node_depth = node.depth + 1;
            memcpy(&temp_node.depth, &temp_node_depth, sizeof(int));
            memcpy(&temp_node.min, &temp_min, sizeof(double));
            memcpy(&temp_node.max, &node.max, sizeof(double));
            
            double returned_evaluation_value = computer_move(temp_node, encoded_states);
            memcpy(&arr_evaluation_values[i], &returned_evaluation_value, sizeof(double));
            
            // pruned!!
            if (node.min <= arr_evaluation_values[i])
                break;
        }
    }
    
    // We have finished all the iterations
    // We are at Root node
    if (node.depth == 0) {

        double temp_max = N_INFINITY;
        int arr_index_of_evaluation;
        for (int i = 0; i < number_of_states; i++) {
            
            // print evaluation values for testing
            //printf("%d: %f\n", i + 1, arr_evaluation_values[i]);
            
            if (temp_max < arr_evaluation_values[i]) {
                arr_index_of_evaluation = i;
                memcpy(&temp_max, &arr_evaluation_values[i], sizeof(double));
            }
            
            if (temp_max == arr_evaluation_values[i]) {
                
                int prev_wall_corner_counter = 0;
                int curr_wall_corner_counter = 0;
                
                // count each piece at a corner
                if (p1 == CHAR_COMPUTER) {

                    if (expands[arr_index_of_evaluation].next_states[0][0] == P1_PIECE_X)
                        prev_wall_corner_counter += 2;
                    if (expands[arr_index_of_evaluation].next_states[0][BOARD_WIDTH - 1] == P1_PIECE_X)
                        prev_wall_corner_counter += 2;
                    if (expands[arr_index_of_evaluation].next_states[BOARD_HEIGHT - 1][0] == P1_PIECE_X)
                        prev_wall_corner_counter += 2;
                    if (expands[arr_index_of_evaluation].next_states[BOARD_HEIGHT - 1][BOARD_WIDTH - 1] == P1_PIECE_X)
                        prev_wall_corner_counter += 2;
                    
                    if (expands[i].next_states[0][0] == P1_PIECE_X)
                        curr_wall_corner_counter += 2;
                    if (expands[i].next_states[0][BOARD_WIDTH - 1] == P1_PIECE_X)
                        curr_wall_corner_counter += 2;
                    if (expands[i].next_states[BOARD_HEIGHT - 1][0] == P1_PIECE_X)
                        curr_wall_corner_counter += 2;
                    if (expands[i].next_states[BOARD_HEIGHT - 1][BOARD_WIDTH - 1] == P1_PIECE_X)
                        curr_wall_corner_counter += 2;                    
                }
                else {
                    
                    if (expands[arr_index_of_evaluation].next_states[0][0] == P2_PIECE_O)
                        prev_wall_corner_counter += 2;
                    if (expands[arr_index_of_evaluation].next_states[0][BOARD_WIDTH - 1] == P2_PIECE_O)
                        prev_wall_corner_counter += 2;
                    if (expands[arr_index_of_evaluation].next_states[BOARD_HEIGHT - 1][0] == P2_PIECE_O)
                        prev_wall_corner_counter += 2;
                    if (expands[arr_index_of_evaluation].next_states[BOARD_HEIGHT - 1][BOARD_WIDTH - 1] == P2_PIECE_O)
                        prev_wall_corner_counter += 2;
                    
                    if (expands[i].next_states[0][0] == P2_PIECE_O)
                        curr_wall_corner_counter += 2;
                    if (expands[i].next_states[0][BOARD_WIDTH - 1] == P2_PIECE_O)
                        curr_wall_corner_counter += 2;
                    if (expands[i].next_states[BOARD_HEIGHT - 1][0] == P2_PIECE_O)
                        curr_wall_corner_counter += 2;
                    if (expands[i].next_states[BOARD_HEIGHT - 1][BOARD_WIDTH - 1] == P2_PIECE_O)
                        curr_wall_corner_counter += 2;                    
                }
                
                // choose the move; if the board has less pieces at corners
                if (prev_wall_corner_counter >= curr_wall_corner_counter)
                    arr_index_of_evaluation = i;
            }
        }
        
        print_computer_move(BOARD, expands[arr_index_of_evaluation].next_states);
        
        // copy the move to BOARD
        for(int m = 0; m < BOARD_HEIGHT; m++)
            memcpy(&BOARD[m], &expands[arr_index_of_evaluation].next_states[m],
                        sizeof(expands[arr_index_of_evaluation].next_states[0]));
        
        print_board(BOARD);
        
        // free allocated memories
        free(arr_evaluation_values);
        free(expands);
        free(encoded_states);
        
        return 0;
    }
    
    // we're not at the root node
    // we're still iterating
    // this is for MAX
    else if (node.depth % 2 == 0) {
        
        double temp_max = N_INFINITY;
        for (int i = 0; i < number_of_states; i++)
            if (temp_max < arr_evaluation_values[i])
                memcpy(&temp_max, &arr_evaluation_values[i], sizeof(double));
        
        // free allocated memories
        free(arr_evaluation_values);
        free(expands);
        
        return temp_max;
    }
    
    // this is for MIN
    else {
        
        double tempMin = P_INFINITY;
        for (int i = 0; i < number_of_states; i++)
            if (tempMin > arr_evaluation_values[i])
                memcpy(&tempMin, &arr_evaluation_values[i], sizeof(double));
        
        // free allocated memories
        free(arr_evaluation_values);
        free(expands);
        
        return tempMin;
    }
}

void game_loop() {
    
    for (turn_counter = 1; turn_counter <= turn_limit; turn_counter++) {

        printf(BOARD_AT_THE_BEGINNING_TURN_TEXT, turn_counter, turn_limit);
        print_board(BOARD);

        int turn_left = turn_limit - turn_counter + 1;
        
        Node temp_node;
                    
        if (p1 == CHAR_HUMAN) {
            
            human_move();
            if (!can_opponent_make_a_move(P2_PIECE_O)) {
                
                check_who_wins();
                return;
            }
            
            for(int m = 0; m < BOARD_HEIGHT; m++)
                memcpy(&temp_node.STATE[m], &BOARD[m], sizeof(BOARD[0]));
            
            if (turn_left < DEPTH) {
                int temp_depth = turn_left * 2 - 1;
                
                if (temp_depth >= DEPTH)
                    DEPTH = 6;
                else
                    DEPTH = temp_depth;
            }
            
            temp_node.depth = 0;
            temp_node.max   = N_INFINITY;
            temp_node.min   = P_INFINITY;
            
            clock_t start = clock();
            computer_move(temp_node, NULL);
            printf("%.6f sec passed.\n", (double)(clock() - start) / CLOCKS_PER_SEC);
            
            if (!can_opponent_make_a_move(P1_PIECE_X)) {
                
                check_who_wins();
                return;
            }
        }
        else if (p1 == CHAR_COMPUTER) {
            
            for(int m = 0; m < BOARD_HEIGHT; m++)
                memcpy(&temp_node.STATE[m], &BOARD[m], sizeof(BOARD[0]));
            
            if (turn_left < DEPTH) {
                int temp_depth = turn_left * 2;
                
                if (temp_depth >= DEPTH)
                    DEPTH = 6;
                else
                    DEPTH = temp_depth;
            }
            
            temp_node.depth = 0;
            temp_node.max   = N_INFINITY;
            temp_node.min   = P_INFINITY;
            
            clock_t start = clock();
            computer_move(temp_node, NULL);
            printf("%.6f sec passed.\n\n", (double)(clock() - start) / CLOCKS_PER_SEC);
            
            if (!can_opponent_make_a_move(P2_PIECE_O)) {
                
                check_who_wins(); 
                return;
            }
            
            human_move();
            if (!can_opponent_make_a_move(P1_PIECE_X)) {
                
                check_who_wins();
                return;
            }
        }
    }

    printf(TURN_LIMIT_REACHED_TEXT);
    
    check_who_wins();
}

int main(int argc, char *argv[]) {
    
    initialize_game();
    game_loop();

    return (EXIT_SUCCESS);
}
