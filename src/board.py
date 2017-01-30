from support import *

class Board(object):
    """docstring for Board
    these codes are for BLACK"""
    board = []
    turn = 0
    passed = False
    Moved = []
    def __init__(self, other=None):
        # super(Board, self).__init__()
        if other == None:
            self.board = [BLANK] * 100
        else:
            self.board = other.board[:]
            self.turn = other.turn
            self.passed = False
            self.Moved = other.Moved[:]

    def __eq__(self, other):
        if other == None:
            return False
        if set(self.Moved) != set(other.Moved):
            return False
        # slow
        # if self.board == other.board
        for i in range(1, 9):
            for j in range(1, 9):
                mass = i*10 + j
                if self.board[mass] != other.board[mass]:
                    return False

        if self.passed != self.passed:
            return False
        return True

    def setBoard(self):
        for i in range(1, 9):
            self.board[i*10] = LRWALL
            self.board[i*10+9] = LRWALL
        for i in range(1, 10):
            self.board[i] = FBWALL
            self.board[i+90] = FBWALL

        self.board[D4] = WHITE;
        self.board[E4] = BLACK;
        self.board[D5] = BLACK;
        self.board[E5] = WHITE;


    def revBoard(self):
        for i in range(1, 9):
            for j in range(1, 9):
                mass = i*10 + j
                if self.board[mass] == WHITE:
                    self.board[mass] = BLACK
                elif self.board[mass] == BLACK:
                    self.board[mass] = WHITE

    def dispBoard(self):
        for i in range(100):
            print(self.board[i], end="")
            if i % 10 == 9:
                print("")

    def checkBoard(self):
        if BLACK in self.board and WHITE in self.board:
            return False
        else:
            return True

    def getNextMove(self, mass):
        if self.board[mass] != BLANK:
            return False

        else:
            # around the mass
            for i in range(-1, 2):
                for j in range(-1, 2):
                    direction = i + 10 * j
                    # in the direction, no WHITE near the mass
                    if self.board[mass+direction] != WHITE:
                        continue
                    # check along the direction
                    else:
                        Reach = 2
                        while True:
                            if self.board[mass+Reach*direction] == WHITE:
                                Reach += 1
                                continue
                            if self.board[mass+Reach*direction] == BLACK:
                                return True
                            # wall or blank
                            else:
                                break
            return False

    def getNextMoves(self):
        return [(i*10+j) for i in range(1, 9) for j in range(1, 9) if self.getNextMove(i*10+j)]
        # NextMoves = []
        # for i in range(1, 9):
        #     for j in range(1, 9):
        #         mass = i*10 + j
        #         if self.getNextMove(mass):
        #             NextMoves.append(mass)
        # return NextMoves

    def getNextBoard(self, NextMove):
        NextBoard = Board(self)
        # around the mass
        for i in range(-1, 2):
            for j in range(-1, 2):
                # in the direction, no WHITE near the mass
                direction = i + 10 * j
                if NextBoard.board[NextMove+direction] != WHITE:
                    continue
                # check along the direction
                else:
                    Reach = 2
                    while True:
                        if NextBoard.board[NextMove+Reach*direction] == WHITE:
                            Reach += 1
                            continue
                        if NextBoard.board[NextMove+Reach*direction] == BLACK:
                            for k in range(Reach):
                                NextBoard.board[NextMove+k*direction] = BLACK
                            break
                        # wall or blank
                        else:
                            break
        NextBoard.turn += 1
        NextBoard.Moved.append(NextMove)
        return NextBoard
