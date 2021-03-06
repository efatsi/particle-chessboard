#include "Screen.h"
#include "Board.h"

Screen screen;
Board board;

int ledPin    = D7;
int playerPin = D6;

int currentPlayer = WHITE;
int waitingPlayer = BLACK;

int homePlayer;
int awayPlayer;

SYSTEM_MODE(MANUAL);

void setup() {
  Particle.function("screen", updateScreen);
  Particle.function("light", updateLight);

  pinMode(ledPin, OUTPUT);
  pinMode(playerPin, INPUT);

  if (digitalRead(playerPin)) {
    homePlayer = WHITE;
    awayPlayer = BLACK;
  } else {
    homePlayer = BLACK;
    awayPlayer = WHITE;
  }

  board.init(homePlayer, awayPlayer);
  screen.init(homePlayer == WHITE ? "WHITE" : "BLACK");
}

void loop() {
  board.determineState(currentPlayer);

  if (board.moveDetected()) {
    confirmChanges(board.moveString);
  }

  // board.printBinary();
  // board.printReadings();
  board.printFullStatus();
  delay(50);
}

void confirmChanges(String move) {
  // Print move locally
  String player = currentPlayer == WHITE ? "White" : "Black";
  if (homePlayer == currentPlayer) {
    screen.printMove(player, move);
  } else {
    screen.printMove(player, move);
    // screen.rawPrint("   satisfied");
  }

  // Reset board
  board.confirmChanges(currentPlayer);

  // Switch players
  waitingPlayer = currentPlayer;
  currentPlayer = WHITE + BLACK - currentPlayer;
}

// Particle Functions
int updateLight(String command) {
  if (command == "on") {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  return 1;
}

int updateScreen(String player) {
  // TODO: split on deliminator
  screen.printPlayerMove(player);
  return 1;
}
