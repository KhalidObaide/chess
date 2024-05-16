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
    case KNIGHT:
      pieces.push_back(std::make_unique<Knight>(
          Knight(pSide, pInitPosition, CS, gameEngine->renderer, *this)));
      break;
    case KING:
      pieces.push_back(std::make_unique<King>(
          King(pSide, pInitPosition, CS, gameEngine->renderer, *this)));
      break;
    case ROOK:
      pieces.push_back(std::make_unique<Rook>(
          Rook(pSide, pInitPosition, CS, gameEngine->renderer, *this)));
      break;
    case BISHOP:
      pieces.push_back(std::make_unique<Bishop>(
          Bishop(pSide, pInitPosition, CS, gameEngine->renderer, *this)));
      break;
    case QUEEN:
      pieces.push_back(std::make_unique<Queen>(
          Queen(pSide, pInitPosition, CS, gameEngine->renderer, *this)));
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

void GameManager::runCapture(Spot nSpot) {
  int index = 0;
  Piece *capturedPiece = nullptr;
  for (auto &piece : pieces) {
    if (piece->spot.file == nSpot.file && piece->spot.rank == nSpot.rank) {
      capturedPiece = piece.get();
      break;
    }
    index++;
  }
  if (index == (int)pieces.size())
    return;

  pieces.erase(pieces.begin() + index);
  gameEngine->deRegisterGameObject(capturedPiece);
}

bool GameManager::isSpotInCheck(Spot spot, Side side, bool includeKingChecks) {
  for (auto &piece : pieces) {
    if (piece->side == side ||
        (includeKingChecks ? false : piece->type == KING))
      continue;
    std::vector<Spot> pieceValidMoves = piece->getValidMoves(true);

    for (auto &validMove : pieceValidMoves) {
      if (validMove.file == spot.file && validMove.rank == spot.rank) {
        return true;
      }
    }
  }

  return false;
}

std::vector<Spot> GameManager::getValidCastleMoves(Side side) {
  // convert to hashmap for quicker access when running validations
  std::unordered_map<std::string, Side> allPieces;
  for (auto &piece : pieces) {
    allPieces[std::to_string(piece->spot.file) +
              std::to_string(piece->spot.rank)] = piece->side;
  }
  const Rank startingRank = side == WHITE_SIDE ? RANK_1 : RANK_8;
  bool isRightCastleValid = !isSpotInCheck({FILE_F, startingRank}, side, true);
  bool isLeftCastleValid = !isSpotInCheck({FILE_D, startingRank}, side, true);

  // validate the history
  for (auto &record : history.records) {
    if (record.start.rank == startingRank && record.start.file == FILE_E) {
      isRightCastleValid = false;
      isLeftCastleValid = false;
      break;
    } else if ((record.start.rank == startingRank &&
                record.start.file == FILE_H) ||
               (record.end.rank == startingRank && record.end.file == FILE_H)) {
      isRightCastleValid = false;
    } else if ((record.start.rank == startingRank &&
                record.start.file == FILE_A) ||
               (record.end.rank == startingRank && record.end.file == FILE_H)) {
      isLeftCastleValid = false;
    }
  }

  // validate if in check
  if (isSpotInCheck({FILE_E, startingRank}, side, false)) {
    return {};
  }

  // right & left castle ( validate if no piece in the middle )
  std::vector<Spot> validCastleMoves;
  if (isRightCastleValid) {
    std::vector<File> rightMiddleFiles = {FILE_F, FILE_G};
    for (auto &file : rightMiddleFiles) {
      std::string spotString =
          std::to_string(file) + std::to_string(startingRank);
      if (allPieces.find(spotString) != allPieces.end()) {
        isRightCastleValid = false;
        break;
      }
    }
    if (isRightCastleValid) {
      validCastleMoves.push_back({FILE_G, startingRank});
    }
  }
  if (isLeftCastleValid) {
    std::vector<File> leftMiddleFiles = {FILE_B, FILE_C, FILE_D};
    for (auto &file : leftMiddleFiles) {
      std::string spotString =
          std::to_string(file) + std::to_string(startingRank);
      if (allPieces.find(spotString) != allPieces.end()) {
        isLeftCastleValid = false;
        break;
      }
    }
    if (isLeftCastleValid) {
      validCastleMoves.push_back({FILE_C, startingRank});
    }
  }

  return validCastleMoves;
}

void GameManager::runCastleMove(Spot nSpot) {
  for (auto &piece : pieces) {
    // right
    if (nSpot.file == FILE_G) {
      if (piece->spot.file == FILE_H && piece->spot.rank == nSpot.rank) {
        piece->move({FILE_F, piece->spot.rank});
        break;
      }
    } else if (nSpot.file == FILE_C) {
      // left
      if (piece->spot.file == FILE_A && piece->spot.rank == nSpot.rank) {
        piece->move({FILE_D, piece->spot.rank});
        break;
      }
    }
  }
}

void GameManager::addToHistory(Spot start, Spot end) {
  history.records.push_back({start, end});
}
