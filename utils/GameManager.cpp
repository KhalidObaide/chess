#include "../lib.h"
#include <iostream>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <vector>

GameManager::GameManager(GameEngine *nGameEngine, const int CELL_SIZE,
                         BoardTheme theme)
    : GameObject({0, 0}, {0, 0}, {0, 0, 0, 0}, false), CS(CELL_SIZE),
      board(nGameEngine, CELL_SIZE, theme) {
  flipBoard = false;
  promotionInProgress = false;
  gameEngine = nGameEngine;
  gameEngine->registerGameObjects({this});
  setStandardBoard();
  setGameTurn(WHITE_SIDE);
}

void GameManager::setStandardBoard() {
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
}

void GameManager::reset() {
  // promotionBoard
  for (auto &option : promotionBoard.options) {
    gameEngine->deRegisterGameObject(&option);
  }
  promotionBoard = {};
  promotionInProgress = false;

  // remove all pieces
  for (auto &piece : pieces) {
    gameEngine->deRegisterGameObject(piece.get());
  }
  while (pieces.size()) {
    pieces.erase(pieces.begin());
  }
  history.records = {};
  history.capturedPieces.clear();
  history.capturedPiecesIndexes = {};
  setStandardBoard();
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
  case MOVE_MADE: {
    setGameTurn((Side)event.value == WHITE_SIDE ? BLACK_SIDE : WHITE_SIDE);
    // validate check-mate
    GameStatus status = getGameStatus(gameTurn);
    if (status == CHECKMATE) {
      std::cout << "CHECKMATE " << (gameTurn == WHITE_SIDE ? "WHITE" : "BLACK")
                << std::endl;
    } else if (status == DRAW) {
      std::cout << "DRAW" << std::endl;
    }
    break;
  }
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

  history.capturedPieces[history.records.size()] = std::move(pieces[index]);
  history.capturedPiecesIndexes[history.records.size()] = index;
  pieces.erase(pieces.begin() + index);
  gameEngine->deRegisterGameObject(capturedPiece);
}

bool GameManager::isSpotInCheck(Spot spot, Side side, bool includeKingChecks) {
  for (auto &piece : pieces) {
    if (piece->side == side || (!includeKingChecks && piece->type == KING))
      continue;

    for (auto &validMove : piece->getValidMoves(true)) {
      if (validMove.file == spot.file && validMove.rank == spot.rank) {
        return true;
      }
    }
  }

  return false;
}

std::vector<Spot> GameManager::getValidCastleMoves(Side side) {
  const Rank startingRank = side == WHITE_SIDE ? RANK_1 : RANK_8;
  // validate if in check
  if (isSpotInCheck({FILE_E, startingRank}, side, false)) {
    return {};
  }

  // convert to hashmap for quicker access when running validations
  std::unordered_map<std::string, Side> allPieces;
  for (auto &piece : pieces) {
    allPieces[std::to_string(piece->spot.file) +
              std::to_string(piece->spot.rank)] = piece->side;
  }

  bool isRightCastleValid = !isSpotInCheck({FILE_F, startingRank}, side, true);
  bool isLeftCastleValid = !isSpotInCheck({FILE_D, startingRank}, side, true);
  if (!isRightCastleValid && !isLeftCastleValid)
    return {};

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

void GameManager::addToHistory(Piece *piece, Spot end) {
  history.records.push_back({piece, piece->spot, end});
}

bool GameManager::makeMove(Piece *piece, Spot newSpot, bool switchTurn) {
  // castle
  if (piece->type == KING && abs(newSpot.file - piece->spot.file) == 2) {
    runCastleMove(newSpot);
  }
  // en-passant
  else if (piece->type == PAWN &&
           newSpot.rank == (piece->side == WHITE_SIDE ? RANK_6 : RANK_3) &&
           piece->spot.file != newSpot.file) {
    HistoryRecord lastMove = history.records[history.records.size() - 1];
    if (lastMove.end.rank ==
            newSpot.rank + (piece->side == WHITE_SIDE ? -1 : 1) &&
        lastMove.end.file == newSpot.file) {
      runCapture({newSpot.file,
                  (Rank)(newSpot.rank + (piece->side == WHITE_SIDE ? -1 : 1))});
    }
  }

  addToHistory(piece, newSpot);
  runCapture(newSpot);
  piece->move(newSpot);

  Spot kingSpot;
  for (auto &p : pieces) {
    if (p->type == KING && p->side == piece->side) {
      kingSpot = p->spot;
      break;
    }
  }
  if (!isSpotInCheck(kingSpot, piece->side, true)) {
    // successfull
    if (switchTurn) {
      // promotion ( last-rank )
      Rank promotionRank = piece->side == WHITE_SIDE ? RANK_8 : RANK_1;
      if (piece->type == PAWN && newSpot.rank == promotionRank) {
        runPromotion(piece->side, newSpot); // getNotified will be called after
                                            // selecting the new piece
      } else {
        getNotified({MOVE_MADE, piece->side});
      }
    }
    return true;
  } else {
    rollbackMove();
    return false;
  }
}

void GameManager::rollbackMove() {
  if (history.capturedPieces.find(history.records.size()) !=
      history.capturedPieces.end()) {
    int placement = history.capturedPiecesIndexes[history.records.size()];
    pieces.insert(pieces.begin() + placement,
                  std::move(history.capturedPieces[history.records.size()]));
    gameEngine->registerGameObjects({pieces[placement].get()});
    history.capturedPieces.erase(history.records.size());
    history.capturedPiecesIndexes.erase(history.records.size());
  }
  HistoryRecord lastRecord = history.records[history.records.size() - 1];
  lastRecord.piece->move(lastRecord.start);
  history.records.erase(history.records.begin() + history.records.size() - 1);
}

GameStatus GameManager::getGameStatus(Side side) {
  for (int i = 0; i < (int)pieces.size(); i++) {
    Piece *piece = pieces[i].get();
    if (piece->side != side) {
      continue;
    }
    std::vector<Spot> pieceValidMoves = piece->getValidMoves();
    for (auto &validMove : pieceValidMoves) {
      if (makeMove(piece, validMove, false)) {
        rollbackMove();
        return RUNNING;
      }
    }
  }

  Spot kingSpot;
  for (auto &p : pieces) {
    if (p->type == KING && p->side == side) {
      kingSpot = p->spot;
      break;
    }
  }
  if (isSpotInCheck(kingSpot, side, true)) {
    return CHECKMATE;
  } else {
    return DRAW;
  }
}

void GameManager::runPromotion(Side side, Spot spot) {
  // delete the piece
  int pawnIndex = 0;
  for (auto &piece : pieces) {
    if (piece->spot.file == spot.file && piece->spot.rank == spot.rank) {
      break;
    }
    pawnIndex++;
  }

  gameEngine->deRegisterGameObject(pieces[pawnIndex].get());
  pieces.erase(pieces.begin() + pawnIndex);
  displayPromotionBoard(side, spot);
}

void GameManager::displayPromotionBoard(Side side, Spot spot) {
  promotionInProgress = true;
  promotionBoard.options.push_back(PromotionOption(QUEEN, CS, this));
  promotionBoard.options.push_back(PromotionOption(ROOK, CS, this));
  promotionBoard.options.push_back(PromotionOption(BISHOP, CS, this));
  promotionBoard.options.push_back(PromotionOption(KNIGHT, CS, this));
  for (auto &option : promotionBoard.options) {
    option.display(side, spot);
    gameEngine->registerGameObjects({&option});
  }
}

void GameManager::confirmPromotion(PieceType type, Side side, Spot spot) {
  switch (type) {
  case KNIGHT:
    pieces.push_back(std::make_unique<Knight>(
        Knight(side, "a1", CS, gameEngine->renderer, *this)));
    break;
  case BISHOP:
    pieces.push_back(std::make_unique<Bishop>(
        Bishop(side, "a1", CS, gameEngine->renderer, *this)));
    break;
  case ROOK:
    pieces.push_back(std::make_unique<Rook>(
        Rook(side, "a1", CS, gameEngine->renderer, *this)));
    break;
  default:
    pieces.push_back(std::make_unique<Queen>(
        Queen(side, "a1", CS, gameEngine->renderer, *this)));
    break;
  }
  pieces[pieces.size() - 1]->move(spot);
  gameEngine->registerGameObjects({pieces[pieces.size() - 1].get()});
  getNotified({MOVE_MADE, side});

  for (auto &option : promotionBoard.options) {
    gameEngine->deRegisterGameObject(&option);
  }
  promotionBoard = {};
  promotionInProgress = false;
}

void GameManager::flip() {
  flipBoard = !flipBoard;
  for (auto &piece : pieces) {
    piece->move(piece->spot); // refresh the position
  }
}
