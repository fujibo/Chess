import queue
import time
from support import *
from board import *

if __name__ == '__main__':
    Turn = 0
    Start = time.time()

    print("\nthis is a analysis for fool's mate")
    FirstBoard = Board()
    FirstBoard.setBoard()
    FirstBoard.dispBoard()
    FirstMove = F5;
    OPEN = queue.Queue()
    CLOSE = queue.Queue()

    OPEN.put(FirstBoard.getNextBoard(FirstMove));
    counter = 0
    while not OPEN.empty():
        counter += 1
        NowBoard = OPEN.get();
        # input()
        CLOSE.put(NowBoard)
        if NowBoard.turn != Turn:
            if Turn != 0:
                print("turn {} end.".format(Turn))
                print("{} boards have checked in this turn".format(counter))
                print("{}s taken".format(time.time() - Start))
            Turn += 1
            counter = 0
            # NowBoard.dispBoard()
            # print(NowBoard.Moved)

        if counter % 1000 == 0:
            print("count = {}".format(counter))
        #     NowBoard.dispBoard()

        if NowBoard.turn % 2 == 1:
            NowBoard.revBoard()

        MovesList = NowBoard.getNextMoves()
        # print(MovesList)
        # must pass
        if MovesList == []:
            if NowBoard.passed:
                # end of this game
                NextBoard.turn += 1
                NowBoard.Moved.append("Pass")
                NowBoard.dispBoard()
                print(NowBoard.Moved)
                continue
            else:
                print("passed")
                NextBoard = Board(NowBoard)
                NextBoard.turn += 1
                NextBoard.passed = True
                NextMoved.append("Pass")
                NextBoardsList = [NextBoard]
        else:
            NextBoardsList = [NowBoard.getNextBoard(move) for move in MovesList]

        if NowBoard.turn % 2 == 1:
            NowBoard.revBoard()

        for aNextBoard in NextBoardsList:
            if NowBoard.turn % 2 == 1:
                aNextBoard.revBoard()

            if aNextBoard.checkBoard():
                print("find!!")
                aNextBoard.dispBoard()
                print(NowBoard.Moved)
                exit()

            if aNextBoard in list(OPEN.queue):
                # print("in OPEN")
                continue

            if aNextBoard in list(CLOSE.queue):
                # print("in CLOSE")
                continue
            OPEN.put(aNextBoard)
