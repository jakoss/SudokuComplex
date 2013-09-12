#include "RandomSudokuGenerator.h"
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include "HelperClass.h"

RandomSudokuGenerator::RandomSudokuGenerator()
{
    srand((unsigned)time(NULL));
}

SudokuStructure *RandomSudokuGenerator::getRandomSudoku()
{
    initiate(); //inicjacja tabel i podstawowych danych
    generatesolvedsudoku(); //generowanie losowego sudoku
    concealcells(); //przekształcanie do końcowego formatu
    SudokuPuzzle sudokuPuzzle = HelperClass::getSudokuPuzzleFromTable(sudoku);
    SudokuPuzzle originalSudoku = HelperClass::getSudokuPuzzleFromTable(sudoku);
    SudokuPuzzle solvedSudoku = HelperClass::getSudokuPuzzleFromTable(solution);
    SudokuStructure * ss = new SudokuStructure(sudokuPuzzle, solvedSudoku, originalSudoku);
    return ss;
}


void RandomSudokuGenerator::initiate()
{
    int i;
    int j;
    int k;
    int l;
    int cell;
    int box;
    //possible candidates for each cell are stored bitwise in array
    //cellscandidates[].If candidate 1 is present then bit 0 is set,
    //if candidate 2 is present then bit 1 is set, and so on...
    //The array countcandidates[] will return the number of candidates
    //if feeded with any cell's cellscandidates[]
    //Here, the array countcandidates[] is initiated.
    for(i=0;i<512;i++)
        {
        j=i;
        countcandidates[i]=0;
        while (j>0)
            {
            if (j%2>0) countcandidates[i]++;
            j=(j-(j%2))/2;
            }
        }
    //Boxes are adressed 0 to 8 from top left to bottom right
    //The array cellsboxnumber[] will return the boxnumber for
    //any cell, it acts as some kind of boxmap.
    //The array boxcellnumbers[][] holds the 9 cellnumbers
    //for each box. Here we initiate both arrays
    for(i=0;i<3;i++)
        {
        for(j=0;j<3;j++)
            {
            for(k=0;k<3;k++)
                {
                for(l=0;l<3;l++)
                    {
                    cell=(j*27)+(i*3)+(l*9)+k;
                    box=(j*3)+i;
                    cellsboxnumber[cell]=box;
                    boxcellnumbers[box][(l*3)+k]=cell;
                    }
                }
            }
        }
}

void RandomSudokuGenerator::generatesolvedsudoku()
{
    int i;
    int j;
    int k;
    int l;
    int currentcell;

    //prepare base data for empty grid
    for(i=0;i<81;i++)
        {
        unsolvedcells[i]=i;//collect all cellnumbers as unsolved cells
        cellscandidates[i]=511;//set all candidates as available for all cells
        solution[i]=0;//clear solution for all cells
        lastcandidate[i]=0;//clear last candidate indicator
        }

    //mix up the order of the cells kept in unsolvedcells[]
    for(i=0;i<500;i++)//500 times will be enough
        {
        j=(rand()%81);//generate a random number between 0 and 80
        k=(rand()%81);//generate another between 0 and 80
        //swap cell adresses kept in unsolvedcells[] pointed by i and j
        l=unsolvedcells[j];
        unsolvedcells[j]=unsolvedcells[k];
        unsolvedcells[k]=l;
        }

    cellptr=80;//initiate unsolved cells pointer
    logsteps=-1;//initiate candidate update counter

    //start brute force loop until grid is filled
    while (cellptr>-1)//if cellptr=-1 then the grid is filled
        {
        currentcell=unsolvedcells[cellptr];//get current cell from array unsolvedcells
        //check if the unsolved cell pointed by cellptr is allready under examination
            if (solution[currentcell]==0)//if 0 then the cell is not allready under examination
            {
            //seek through all unsolved cells for the one with the least available candidates
            //kinda bubble sort
            for(i=0;i<cellptr;i++)
                {
                if (countcandidates[cellscandidates[unsolvedcells[i]]]<countcandidates[cellscandidates[unsolvedcells[cellptr]]])
                    {
                    j=unsolvedcells[i];
                    unsolvedcells[i]=unsolvedcells[cellptr];
                    unsolvedcells[cellptr]=j;
                    }
                }
            //copy cellnumber with least available candidates to currentcell
            currentcell=unsolvedcells[cellptr];
            //seek for the first available candidate
            j=1;//prepare bitmask
            for(i=0;i<9;i++)//scan candidates
                {
                if ((cellscandidates[currentcell]&j)>0)//test with bitmask
                    {
                    //available candidate found
                    solution[currentcell]=i+1;//set as tempararely solution
                    break;//stop searching
                    }
                j=j*2;//update bitmask
                }
            //check if there is only one candidate, and if so set lastcandidate indicator
            if (countcandidates[cellscandidates[currentcell]]==1) lastcandidate[currentcell]=1;
            removecandidates(currentcell);//update candidates in unsolved cells sharing row, column or box of currentcell
            j=cellptr;//copy current value of cellptr to j
            cellptr--;//decrease cellptr (prepare for next unsolved cell)
            //if there are still cells unsolved check if there are empty cells among them
            if (cellptr>-1)
                {
                for(i=0;i<j;i++)//scan all unsolved cells
                    {
                    //check for empty cell
                    if (cellscandidates[unsolvedcells[i]]==0)//if 0 then empty cell
                        {
                        //empty cell caused by last altered cell
                        cellptr++;//increase cellptr, so it will point to the last altered cell again
                        break;//stop searching for empty cells
                        }
                    }
                }
            }
        else//currentcell is allready under examination, apparantly not the right solution
            {
            //check if the current value was the last available candidate
            if (lastcandidate[currentcell]==1)//check last candidate indicator
                {
                //no more candidates available for the current cell
                undo();//undo the last candidate update
                lastcandidate[currentcell]=0;//clear the last candidate indicator
                cellptr++;//increase cellptr (prepare for previous cell)
                }
            else//try next available candidate for the current cell
                {
                j=1;//prepare bitmask
                k=solution[currentcell];//get last used candidate
                undo();//undo the last candidate update
                for(i=0;i<k;i++) j=j*2;//update bitmask
                //seek for next available candidate
                for(i=k;i<9;i++)//scan next candidates
                    {
                    if ((cellscandidates[currentcell]&j)>0)//test with bitmask
                        {
                        //next available candidate found
                        solution[currentcell]=i+1;//set as temperarely solution
                        break;//stop searching
                        }
                    j=j*2;//update bitmask
                    }
                lastcandidate[currentcell]=1;//by default, set last candidate indicator
                //if current used candidate is not 9 then maybe it's not the last candidate used
                if (solution[currentcell]<9)//check if there are more available candidates
                    {
                    for(i=solution[currentcell];i<9;i++)//scan other candidates
                        {
                        j=j*2;//update bitmask
                        if ((cellscandidates[currentcell]&j)>0)//test with bitmask
                            {
                            //more candidates available
                            lastcandidate[currentcell]=0;//clear last candidate indicator
                            break;//stop searching
                            }
                        }
                    }
                removecandidates(currentcell);//update candidates in unsolved cells sharing row, column or box of currentcell
                j=cellptr;//copy current value of cellptr to j
                cellptr--;//decrease cellptr (prepare for next unsolved cell)
                //if there are still cells unsolved check if there are empty cells among them
                if (cellptr>-1)
                    {
                    for(i=0;i<j;i++)//scan all unsolved cells
                        {
                        //check for empty cell
                        if (cellscandidates[unsolvedcells[i]]==0)//if 0 then empty cell
                            {
                            //empty cell caused by last altered cell
                            cellptr++;//increase cellptr, so it will point to the last altered cell again
                            break;//stop searching for empty cells
                            }
                        }
                    }
                }
            }
        }
}

void RandomSudokuGenerator::concealcells()
{
    int i;
    int j;
    int k;
    int l;
    int solutionbackup[81];
    int countconcealedcells;
    int currentcell;
    int mixedorder[41];
    //This routine will remove values from the random ordered cell list, and checks
    //for uniqueness. To gain an 180° rotational sudoku cells are removed in pairs.
    //therefore only the first half of the cells are adressed directly. The 180° rotational
    //opposite cell is calculated: opposite cell = 80 - cell
    for(i=0;i<81;i++)
        {
        //make a backup of the solution, since the array solution is
        //used by the routine that checks the uniqueness
        solutionbackup[i]=solution[i];
        //copy the solution to handle the cells to conceal
        sudoku[i]=solution[i];
        //prepare an array to mix up the order of the first half of the cells
        if (i<41) mixedorder[i]=i;//setup first half of cells
        }

    for(i=0;i<250;i++)//mix up the order of the cellnumbers kept in array mixedorder[]
        {
        j=(rand()%41);
        k=(rand()%41);
        l=mixedorder[j];
        mixedorder[j]=mixedorder[k];
        mixedorder[k]=l;
        }

    countconcealedcells=0;//clear concealed cell counter

    for(i=0;i<41;i++)//scan half of the cells
        {
        //stop concealing if 58 cells are concealed succesful
        if (countconcealedcells==58) break;
        //get next cell
        currentcell=mixedorder[i];
        //conceal that cell (remove the value)
        sudoku[currentcell]=0;
        //conceal the 180° rotational cell
        sudoku[80-currentcell]=0;
        //check uniqueness
        if (checkuniqueness()==1)//if return value is 1, then there was one unique solution
            {
            //increase twice the concealed cell counter
            countconcealedcells++;
            countconcealedcells++;
            }
        else//if return value is not 1, then there was more than one solution
            {
            //restore the last concealed cells
            sudoku[currentcell]=solutionbackup[currentcell];
            sudoku[80-currentcell]=solutionbackup[80-currentcell];
            }
        }
    //all possible cells to conceal with a maximum of 58 are concealed,
    //and the sudoku to solve is in array sudoku[]
    //now restore the solution
    for(i=0;i<81;i++) solution[i]=solutionbackup[i];
}

int RandomSudokuGenerator::checkuniqueness()
{
    int i;
    int j;
    int k;
    int solvedcells;
    int row;
    int column;
    int box;
    int currentcell;
    int countsolutions;
    int boundary;
    //This routine will brute force solve the puzzle contained in sudoku[]
    //Since sudoku[] is based upon a solved grid, at least one solution will be found
    //When found the first and hopefully the only solution, the last solved cell
    //will be rejected so the search will continue. If another solution is found
    //the routine will be halted and countsolutions will be 2, indicating that there
    //are at least 2 solutions, and the brute force loop will be stopped.
    //If there is only one solution, this brute force will
    //eventually backtrack beyond the first solved cell, can't have that!
    //Therefore, a boundary is set to prevent this, and ends the brute force loop.
    //At last, the value from countsolutions is returned (1=1 solution, 2=at least 2 solutions)

    //initiation
    for(i=0;i<81;i++)
        {
        //transfer the sudoku puzzle to solution[]
        solution[i]=sudoku[i];
        //set all candidates available for all cells
        cellscandidates[i]=511;
        //clear last candidate indicator for all cells
        lastcandidate[i]=0;
        }

    solvedcells=0;//setup solved cells counter
    //update cellscandidates[]
    for(i=0;i<81;i++)//scan sudoku[]
        {
        if (solution[i]>0)//for each solved cell...
            {
            solvedcells++;//update solved cells counter
            //update cells candidates
            //remove candidates for each solved cell
            cellscandidates[i]=0;
            row=i/9;//calculate row from solved cell
            column=i%9;//calculate column from solved cell
            box=cellsboxnumber[i];//get boxnumber from solved cell
            j=1;//prepare candidate bitmask
            if (solution[i]>1)
                {
                for (k=1;k<solution[i];k++) j=j*2;//update candidate bitmask
                }
            for (k=0;k<9;k++)//for all positions in row, column or box...
                {
                //remove candidate from cells in row
                cellscandidates[(row*9)+k]=cellscandidates[(row*9)+k]&(511^j);
                //remove candidate from cells in column
                cellscandidates[(k*9)+column]=cellscandidates[(k*9)+column]&(511^j);
                //remove candidate from cells in box
                cellscandidates[boxcellnumbers[box][k]]=cellscandidates[boxcellnumbers[box][k]]&(511^j);
                }
            }
        }


    //solve naked singles first
    while (nakedsingle()==1) solvedcells++;//update solved cells counter

    if (solvedcells==81)//check if puzzle is solved
        {
        countsolutions=1;//set return value
        return (countsolutions);//return
        }

    //collect unsolved cells
    cellptr=-1;//reset unsolved cell pointer
    for(i=0;i<81;i++)//scan sudoku[]
        {
        if (solution[i]==0)//for each unsolved cell...
            {
            //collect all unsolved cells
            //increase cellptr for each unsolved cell
            cellptr++;
            //transfer cellnumber for each unsolved cell to array unsolvedcells[]
            unsolvedcells[cellptr]=i;
            }
        }

    boundary=cellptr+1;//set boundary
    logsteps=-1;//reset log stepcounter
    countsolutions=0;//clear solution counter

    while (cellptr<boundary)//do brute force loop but don't let it backtrack beyond the first unsolved cell
        {
        if (cellptr==-1)//check cellptr, if -1 then a solution is found
            {
            //solution found, increase solution counter
            countsolutions++;
            //if this is the second solution then break the brute force loop
            if (countsolutions==2) break;
            //at this point only one solution is found, now reject
            //the last solved cell by increasing cellptr, so the search
            //can continue for other solutions, if any
            cellptr++;
            }
        //FROM HERE THE CODE IS THE SAME AS IN THE BRUTE FORCE LOOP FROM generatesolvedsudoku()
        currentcell=unsolvedcells[cellptr];
        if (solution[unsolvedcells[cellptr]]==0)
            {
            for(i=0;i<cellptr;i++)
                {
                if (countcandidates[cellscandidates[unsolvedcells[i]]]<countcandidates[cellscandidates[unsolvedcells[cellptr]]])
                    {
                    j=unsolvedcells[i];
                    unsolvedcells[i]=unsolvedcells[cellptr];
                    unsolvedcells[cellptr]=j;
                    }
                }
            currentcell=unsolvedcells[cellptr];
            j=1;
            for(i=0;i<9;i++)
                {
                if ((cellscandidates[currentcell]&j)>0)
                    {
                    solution[currentcell]=i+1;
                    break;
                    }
                j=j*2;
                }
            if (countcandidates[cellscandidates[currentcell]]==1) lastcandidate[currentcell]=1;
            removecandidates(currentcell);
            j=cellptr;
            cellptr--;
            if (cellptr>-1)
                {
                for(i=0;i<j;i++)
                    {
                    if (cellscandidates[unsolvedcells[i]]==0)
                        {
                        cellptr++;
                        break;
                        }
                    }
                }
            }
        else
            {
            if (lastcandidate[currentcell]==1)
                {
                undo();
                lastcandidate[currentcell]=0;
                cellptr++;
                }
            else
                {
                j=1;
                k=solution[currentcell];
                undo();
                for(i=0;i<k;i++) j=j*2;
                for(i=k;i<9;i++)
                    {
                    if ((cellscandidates[currentcell]&j)>0)
                        {
                        solution[currentcell]=i+1;
                        break;
                        }
                    j=j*2;
                    }
                lastcandidate[currentcell]=1;
                if (solution[currentcell]<9)
                    {
                    for(i=solution[currentcell];i<9;i++)
                        {
                        j=j*2;
                        if ((cellscandidates[currentcell]&j)>0)
                            {
                            lastcandidate[currentcell]=0;
                            break;
                            }
                        }
                    }
                removecandidates(currentcell);
                j=cellptr;
                cellptr--;
                if (cellptr>-1)
                    {
                    for(i=0;i<j;i++)
                        {
                        if (cellscandidates[unsolvedcells[i]]==0)
                            {
                            cellptr++;
                            break;
                            }
                        }
                    }
                }
            }
        }
    //brute force loop ended, return countsolutions
    return (countsolutions);
}

void RandomSudokuGenerator::removecandidates(int currentcell)
{
    int i;
    int j;
    int k;
    int l;
    int row;
    int column;
    int box;
    //Each unsolved cell that shares the same row, column or box with the current cell
    //will loose the value last placed in the current cell as candidate
    //The current cell is saved, along with the positions of the cells losing the candidate,
    //related to their row, column or box.
    //If the brute force handler needs to backtrack, the undo() routine uses this information
    //to restore those candidates
    logsteps++;//increase stepcounter

    logcell[logsteps]=currentcell;//save current cell, this is the last altered cell
    logrow[logsteps]=0;//clear row log
    logcolumn[logsteps]=0;//clear column log
    logbox[logsteps]=0;//clear box log

    row=currentcell/9;//calculate the row where the current cell belongs to
    column=currentcell%9;//calculate the column where the current cell belongs to
    box=cellsboxnumber[currentcell];//get the boxnumber for the current cell

    j=1;//prepare candidate bitmask
    if (solution[currentcell]>1)
        {
        for(i=1;i<solution[currentcell];i++) j=j*2;//update candidate bitmask
        }
    for(i=0;i<cellptr;i++)//scan all unsolved cells
        {
        if ((cellscandidates[unsolvedcells[i]]&j)>0)//test if candidate is present
            {
            if (unsolvedcells[i]/9==row)//check if it shares row with current cell
                {
                l=1;//prepare position bitmask
                for(k=0;k<9;k++)//search position in row
                    {
                    if ((row*9)+k==unsolvedcells[i])//if position found...
                        {
                        logrow[logsteps]=logrow[logsteps]|l;//set position bit in row log
                        cellscandidates[unsolvedcells[i]]=cellscandidates[unsolvedcells[i]]&(511^j);//remove candidate from list
                        break;//stop position search
                        }
                    l=l*2;//update position bitmask
                    }
                }
            else if (unsolvedcells[i]%9==column)//check if it shares column with current cell
                {
                l=1;//prepare position bitmask
                for(k=0;k<9;k++)//search position in column
                    {
                    if ((k*9)+column==unsolvedcells[i])//if position found...
                        {
                        logcolumn[logsteps]=logcolumn[logsteps]|l;//set position bit in column log
                        cellscandidates[unsolvedcells[i]]=cellscandidates[unsolvedcells[i]]&(511^j);//remove candidate from list
                        break;//stop position search
                        }
                    l=l*2;//update position bitmask
                    }
                }
            else if (cellsboxnumber[unsolvedcells[i]]==box)//check if it shares box with current cell
                {
                l=1;//prepare position bitmask
                for(k=0;k<9;k++)//search position in box
                    {
                    if (boxcellnumbers[box][k]==unsolvedcells[i])//if position found...
                        {
                        logbox[logsteps]=logbox[logsteps]|l;//set position bit in box log
                        cellscandidates[unsolvedcells[i]]=cellscandidates[unsolvedcells[i]]&(511^j);//remove candidate from list
                        break;//stop position search
                        }
                    l=l*2;//update position bitmask
                    }
                }
            }
        }
}

void RandomSudokuGenerator::undo()
{
    int i;
    int j;
    int k;
    int l;
    int row;
    int column;
    int box;
    //This routine uses the information stored by removecandidates[] to replace
    //the last removed candidates from the unsolved cells whenever the brute force
    //handler needs to backtrack
    k=solution[logcell[logsteps]];//get the candidate value
    solution[logcell[logsteps]]=0;//clear the solution

    row=logcell[logsteps]/9;//calculate row
    column=logcell[logsteps]%9;//calculate column
    box=cellsboxnumber[logcell[logsteps]];//get boxnumber

    j=1;//prepare candidate bitmask
    if (k>1)
        {
        for(i=1;i<k;i++) j=j*2;//update candidate bitmask
        }
    l=1;//prepare position bitmask
    for(i=0;i<9;i++)//scan all positions
        {
        //if position is in row log then replace candidate
        if ((logrow[logsteps]&l)>0) cellscandidates[(row*9)+i]=cellscandidates[(row*9)+i]|j;
        //if position is in column log then replace candidate
        if ((logcolumn[logsteps]&l)>0) cellscandidates[(i*9)+column]=cellscandidates[(i*9)+column]|j;
        //if position is in box log then replace candidate
        if ((logbox[logsteps]&l)>0) cellscandidates[boxcellnumbers[box][i]]=cellscandidates[boxcellnumbers[box][i]]|j;
        l=l*2;//update position bitmask
        }
    logsteps--;//decrease logcounter (for next backtrack, if needed)
}

int RandomSudokuGenerator::nakedsingle()
{
    int i;
    int j;
    int k;
    int row;
    int column;
    int box;
    int foundnaked;

    foundnaked=0;//clear return value
    for (i=0;i<81;i++)//scan all cells
        {
        //search for unsolved cell with only one possible candidate (naked single)
        if(countcandidates[cellscandidates[i]]==1)
            {
            foundnaked=1;//set return value
            row=i/9;//calculate row
            column=i%9;//calculate column
            box=cellsboxnumber[i];//get boxnumber
            j=1;//prepare bitmask
            for (k=0;k<9;k++)//scan candidate list
                {
                if ((cellscandidates[i]&j)>0)//test bit for available candidate
                    {
                    //if found...
                    solution[i]=k+1;//assign to solution
                    break;//stop scanning candidate list
                    }
                j=j*2;//update bitmask
                }
            cellscandidates[i]=0;//clear candidate list from naked single cell
            for (k=0;k<9;k++)//for each position in same row, column or box
                {
                //update cellscandidates for each cell in same row
                cellscandidates[(row*9)+k]=cellscandidates[(row*9)+k]&(511^j);
                //update cellscandidates for each cell in same column
                cellscandidates[(k*9)+column]=cellscandidates[(k*9)+column]&(511^j);
                //update cellscandidates for each cell in same box
                cellscandidates[boxcellnumbers[box][k]]=cellscandidates[boxcellnumbers[box][k]]&(511^j);
                }
            break;
            }
        }
    return(foundnaked);
}
