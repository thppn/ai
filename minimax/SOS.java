//Athanasios Papanastasiou 3057

import java.util.Scanner;

public class SOS {
	
	static char e = '#';
	static char[] letter = new char[] {'S','O'};
	static String[] player = new String[] {"Computer", "Human"};
	static int N = 3;
	static int endVal = 1000;
	
	class Board {
		
		int free;
		char[][] grid;
		boolean isMax;
		
		Board() {
			free = N*N-1;
			grid = new char[][] {{e, e, e},{'O', e, e},{e, e, e}};
			isMax = false;
		}
		Board(Board other) {
			free = other.free;
			grid = new char[N][N];
			for(int i=0;i<N*N;i++) {
				grid[i/N][i%N] = other.grid[i/N][i%N];
			}
			isMax = !other.isMax;
		}
		boolean validMove(int move) {
			return grid[move/2/N][move/2%N] == e;
		}
		Board performMove(int move) {
			free--;
			grid[move/2/N][move/2%N] = letter[move%2];
			return this;
		}
		boolean isSOS() {//for 3x3
			for(int i=0;i < N;i++) {
				if(grid[i][0]=='S'&&grid[i][1]=='O'&&grid[i][2]=='S')//horizontal
					return true;
			}
			for(int j=0;j < N;j++) {
				if(grid[0][j]=='S'&&grid[1][j]=='O'&&grid[2][j]=='S')//vertical
					return true;
			}
			if(grid[0][0]=='S'&&grid[1][1]=='O'&&grid[2][2]=='S')//diagonal
				return true;
			if(grid[0][2]=='S'&&grid[1][1]=='O'&&grid[2][0]=='S')//anti-diagonal
				return true;
			else
				return false;
		}
		void printBoard() {
			for(char[] row: grid) {
				for(char cell: row) {
					System.out.print(cell);
				}
				System.out.println("");
			}
		}
	}
	
	Board gameBoard;
	Scanner userInput;
	
	SOS() {
		gameBoard = new Board();
		userInput = new Scanner(System.in);
	}
	
	int minimax(Board state, int depth, int alpha, int beta) {
		if (state.free == 0 || state.isSOS()) {
			return state.isMax?depth-endVal:endVal-depth;
		}
		
		int maxValue = -endVal, minValue = endVal, bestMove = -1;

		for(int move=0;move<N*N*2;move++) {
			
			if(!state.validMove(move)) continue;

			int moveValue = minimax((new Board(state)).performMove(move), depth+1, alpha, beta);
			
			if(state.isMax) {
				if(moveValue > maxValue) {
					maxValue = moveValue;
					bestMove = move;
				}
				if(alpha < maxValue) alpha = maxValue;
			} else {
				if(moveValue < minValue) {
					minValue = moveValue;
					bestMove = move;
				}
				if(beta > minValue) beta = minValue;
			}
			if(beta <= alpha) break;
		}
		return bestMove;
	}

	void startGame() {
		int r = 0;
		System.out.println("Welcome to SOS");
		while(!(gameBoard.free == 0 || gameBoard.isSOS())) {
			System.out.println(player[r%2]+" make a choice. Round:"+r);
			printBoard();
			if(r++%2==0)
				computerPlayer();
			else
				humanPlayer();
		}
		System.out.println("Game Over!");
		gameBoard.printBoard();
		if(gameBoard.isSOS())
			System.out.println(player[--r%2]+" has won!");
		else
			System.out.println("It's a draw!");
	}
	
	void computerPlayer() {
		int bestMove = minimax(gameBoard, 0, -endVal, endVal);
		gameBoard.performMove(bestMove);
	}
	
	void humanPlayer(){
		int x, y ,l, playerMove;
		do {
			System.out.println("X axis:");
			x = userInput.nextInt();
			System.out.println("Y axis:");
			y = userInput.nextInt();
			System.out.println("S(0) or O(1):");
			l = userInput.nextInt();
			playerMove = 6*x+2*y+l;
		} while(!gameBoard.validMove(playerMove));
		gameBoard.performMove(playerMove);
	}
	
	void printBoard() {
		for(char[] row: gameBoard.grid) {
			for(char cell: row) {
				System.out.print(cell);
			}
			System.out.println("");
		}
	}
	
	public static void main(String[] args) {
		SOS game = new SOS();
		game.startGame();
	}
}
