#ifndef RANDOMSUDOKUGENERATOR_H
#define RANDOMSUDOKUGENERATOR_H

#include "Types.h"
#include "SudokuStructure.h"
#include "Cell.h"

class RandomSudokuGenerator {
private:
    void initiate();
    void generatesolvedsudoku();
    void concealcells();
    int checkuniqueness();
    void removecandidates(int currentcell);
    void undo();
    int nakedsingle();

    int unsolvedcells[81];
    int cellptr;
    int solution[81];
    int sudoku[81];
    int cellscandidates[81];
    int countcandidates[512];
    int cellsboxnumber[81];
    int boxcellnumbers[9][9];
    int lastcandidate[81];
    int logsteps;
    int logcell[81];
    int logrow[81];
    int logcolumn[81];
    int logbox[81];

public:
    RandomSudokuGenerator();
    SudokuStructure * getRandomSudoku();
};

#endif // RANDOMSUDOKUGENERATOR_H
