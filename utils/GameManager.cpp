#include "../lib.h"
#include <memory>
#include <tuple>
#include <unordered_map>
#include <vector>

std::unordered_map<std::string, BoardTheme> THEMES_SET = {
    {"Classic", {{255, 255, 255, 255}, {0, 0, 0, 255}}},
    {"Coral", {{177, 228, 185, 255}, {112, 162, 163, 255}}},
    {"Dusk", {{204, 183, 174, 255}, {112, 102, 119, 255}}},
    {"Marine", {{157, 172, 255, 255}, {111, 115, 210, 255}}},
    {"Wheat", {{234, 240, 206, 255}, {187, 190, 100, 255}}},
    {"Emerald", {{173, 189, 143, 255}, {111, 143, 114, 255}}},
    {"Sandcastle", {{227, 193, 111, 255}, {184, 139, 74, 255}}},
};

GameManager::GameManager(GameEngine *nGameEngine, const int CELL_SIZE)
    : GameObject({0, 0}, {0, 0}, {0, 0, 0, 0}, false), gameEngine(nGameEngine),
      board(nGameEngine, CELL_SIZE, THEMES_SET["Coral"]), CS(CELL_SIZE) {

  gameEngine->registerGameObjects({this});
  setBoard({
      {WHITE_SIDE, PAWN, "a2"},   {WHITE_SIDE, PAWN, "b2"},
      {WHITE_SIDE, PAWN, "c2"},   {WHITE_SIDE, PAWN, "d2"},
      {WHITE_SIDE, PAWN, "e2"},   {WHITE_SIDE, PAWN, "f2"},
      {WHITE_SIDE, PAWN, "g2"},   {WHITE_SIDE, PAWN, "h2"},
      {WHITE_SIDE, ROOK, "a1"},   {WHITE_SIDE, ROOK, "h1"},
      {WHITE_SIDE, KNIGHT, "b1"}, {WHITE_SIDE, KNIGHT, "g1"},
      {WHITE_SIDE, BISHOP, "c1"}, {WHITE_SIDE, BISHOP, "f1"},
      {WHITE_SIDE, QUEEN, "d1"},  {WHITE_SIDE, KING, "e1"},

      {BLACK_SIDE, PAWN, "a7"},   {BLACK_SIDE, PAWN, "b7"},
      {BLACK_SIDE, PAWN, "c7"},   {BLACK_SIDE, PAWN, "d7"},
      {BLACK_SIDE, PAWN, "e7"},   {BLACK_SIDE, PAWN, "f7"},
      {BLACK_SIDE, PAWN, "g7"},   {BLACK_SIDE, PAWN, "h7"},
      {BLACK_SIDE, ROOK, "a8"},   {BLACK_SIDE, ROOK, "h8"},
      {BLACK_SIDE, KNIGHT, "b8"}, {BLACK_SIDE, KNIGHT, "g8"},
      {BLACK_SIDE, BISHOP, "c8"}, {BLACK_SIDE, BISHOP, "f8"},
      {BLACK_SIDE, QUEEN, "d8"},  {BLACK_SIDE, KING, "e8"},
  });

  setGameTurn(WHITE_SIDE);
}

void GameManager::setGameTurn(Side nSide) {
  gameTurn = nSide;
  for (auto &piece : pieces) {
    piece->getNotified({GAME_TURN, nSide});
  }
}

void GameManager::setBoard(
    std::vector<std::tuple<Side, PieceType, std::string>> piecePlacements) {
  int initIndex = pieces.size();
  for (const auto &piecePlacement : piecePlacements) {
    const Side pSide = std::get<0>(piecePlacement);
    const PieceType pType = std::get<1>(piecePlacement);
    const std::string pInitPosition = std::get<2>(piecePlacement);
    switch (pType) {
    case PAWN:
      pieces.push_back(std::make_unique<Pawn>(
          Pawn(pSide, pInitPosition, CS, gameEngine->renderer, *this)));
      break;

    default:
      pieces.push_back(std::make_unique<Piece>(
          Piece(pSide, pType, pInitPosition, CS, gameEngine->renderer, *this)));
      break;
    }
  }
  for (int i = initIndex; i < (int)pieces.size(); i++) {
    gameEngine->registerGameObjects({pieces[i].get()});
  }
}

void GameManager::update(std::unordered_map<InputEventType, int> & /*events*/) {
  bool isDragging = false;
  for (auto &piece : pieces) {
    if (piece->isDragging) {
      isDragging = true;
      break;
    }
  }

  for (auto &piece : pieces) {
    piece->isReadyToDrag = !isDragging;
  }
}

void GameManager::getNotified(Event event) {
  switch (event.type) {
  case MOVE_MADE:
    setGameTurn((Side)event.value == WHITE_SIDE ? BLACK_SIDE : WHITE_SIDE);
    break;
  default:
    break;
  }
}
