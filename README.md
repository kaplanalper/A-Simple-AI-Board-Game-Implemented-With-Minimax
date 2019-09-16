# A-Simple-AI-Board-Game-Implemented-With-Minimax
A simple board game written for CSE 562 Artificial Intelligence Master's Course Assignment (Yeditepe University, Istanbul, Turkey).

Written in C, implemented with Minimax algorithm.

# Board Game Rules

There is a 7x7 board, a parametric turn limit and a parametrical number of pieces for each player.
Initial positions of these pieces are random.

On each turn current player makes a single move: Moving his piece to one of its four neighbours. Don’t forget the board is the same board for both players.

Game ends when one of the players has no place to move a piece or the turn limit is reached.

Game is played between AI and a user. In the beginning, the program asks if the user will be Player1 or Player2. If user is Player1, the first move will be user’s. Otherwise computer will make the first move. If user decides to play first, the input must be 1 otherwise it is 2.

At the start of the game the number of pieces for a single player and the maximum number of turns will be given by the user.

After board is initialized and at the end of each turn coordinate system and the squares occupied with pieces labeled by their player indicator are printed.

## Computer's turn

Computer moves its piece to an available square and print the new position as shown below:

Computer moves the piece at g4 to g5

## User's turn

If it is user’s turn, user will type the desired move in same structure as the computer. First the coordinate of the piece that is wanted to be moved and then the new coordinate of the piece.

Choose piece to move: c4
Choose the new position for c4:
d4 Player moves the piece at c4 to d4

Game checks whether the desired moves and the piece coordinates are valid or not.
If not, game asks user for a valid move or a valid piece coordinate again.

# Move Rules
Player1’s indicator defined as “X” and Player2’s as “O” in the printed representation.
Players can not move outside of the field.
Players can not move to the squares occupied by the their opponent or by their own pieces.
Players can only move their piece to one of its four neighbours. Move can be vertical or horizontal.

# Coordinate System
Columns are represented as numbers between “1 and 7”.
Rows are represented as letters between “a-g”.
(“a1” is top left square and “g7” is bottom right.)

## Ending & Winning
The game ends when one of the players can not make a valid move or the turn limit has been reached.
When one of the players cannot make a valid move, the game ends and the other player will be the winner.
When the turn limit is reached, the game ends and the player with more valid moveable space will be the winner.

## Examples

![](in-game%20board%20view%20examples.png)

In the DRAW case "X" can move to "c2,b3,e3,d4,f4,e5" and "O" can move to "c1,a3,a5,b5,c6,d7". P1 moveable space count is 6 and P2 moveable space count is also 6. So, the game is a draw.

In the Player2 Wins case "X" can not move so Player2(O) wins.
In the Player1 Wins case "X" can move to "b3,c2,d1,e3,f4,g3" and "O" can move to "a3,c1,e2,g3".

P1 moveable space count is 6 and P2 moveable space count is 4. So Player1(X) wins.
