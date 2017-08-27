/*
 * Реализован алгоритм похожий на описанный тут:
 * "Эвристический решатель судоку"
 * http://habrahabr.ru/post/134071/
*/
#include <iostream>
#include <fstream>
#include <vector>

using std::vector;

void Load(vector<int>**&);
void Solve(vector<int>**&);
bool RulesSolver(vector<int>**&);
bool IncludSolver(vector<int>**&);
bool DoubleSolver(vector<int>**&);
void RecSolver(vector<int>**&);
void Print(vector<int>**&);
bool isValid(vector<int>**&);

bool DelByData(vector<int>& vec, int data);

bool UnicInLine(vector<int>**&, int, int, int);
bool UnicInColumn(vector<int>**&, int, int, int);
bool UnicInBlock(vector<int>**&, int, int, int);
void DelInLine(vector<int>**&, int, int, int);
void DelInColumn(vector<int>**&, int, int, int);
void DelInBlock(vector<int>**&, int, int, int);

vector<int> FindDoubleLine(vector<int>**&, int, int);
vector<int> FindDoubleColumn(vector<int>**&, int, int);
vector<int> FindDoubleBlock(vector<int>**&, int, int);
bool DelDoubleLine(vector<int>**&, int, int, vector<int>);
bool DelDoubleColumn(vector<int>**&, int, int, vector<int>);
bool DelDoubleBlock(vector<int>**&, int, int, vector<int>);

bool isValidLine(vector<int>**&, int, int);
bool isValidColumn(vector<int>**&, int, int);
bool isValidBlock(vector<int>**&, int, int);

int main()
{
    vector<int>** sudoku;
    Load(sudoku);
    Print(sudoku);
    std::cout << "\n";
    Solve(sudoku);
    Print(sudoku);

    char c;
    std::cin >> c;
    return 0;
}

void Solve(vector<int>**& sudoku)
{

    // Контроль правил
    while (RulesSolver(sudoku));
    if(isValid(sudoku)) return;
    // Контроль единственных включений
    while (IncludSolver(sudoku));
    if(isValid(sudoku)) return;
    // Контроль дублей вариантов
    while (DoubleSolver(sudoku));
    if(isValid(sudoku)) return;
    // Частичный перебор
    RecSolver(sudoku);



}

bool isValid(vector<int>**& sudoku)
{
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if(sudoku[i][j].size()==1)
            {
                if(!isValidBlock(sudoku,i,j) ||
                   !isValidColumn(sudoku,i,j) ||
                   !isValidLine(sudoku,i,j)  )
                    return false;
            } else return false;
    return true;
}

bool RulesSolver(vector<int>**& sudoku)
{
    // контроль правил
    bool result = false;
    // блоки
    for (int i = 0; i < 9; i+=3)
        for (int j = 0; j < 9; j+=3)
        {
            for (int k = 0; k < 3; k++)
                for (int t = 0; t < 3; t++)
                    for (int k2 = 0; k2 < 3; k2++)
                        for (int t2 = 0; t2 < 3; t2++)
                        {
                            if(k==k2 && t==t2) continue;
                            if(sudoku[i+k2][j+t2].size()==1)
                            {
                                if(DelByData(sudoku[i+k][j+t],sudoku[i+k2][j+t2][0]))
                                    result = true;
                            }
                        }
        }

    // строки и столбцы
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
        {
            if(sudoku[i][j].size()==1) continue;
            // строки
            for (int k = 0; k < 9; k++)
            {
                if(j==k) continue;
                if(sudoku[i][k].size()==1)
                {
                    if(DelByData(sudoku[i][j],sudoku[i][k][0]))
                        result = true;
                }
            }
            // столбцы
            for (int k = 0; k < 9; k++)
            {
                if(i==k) continue;
                if(sudoku[k][j].size()==1)
                {
                    if(DelByData(sudoku[i][j],sudoku[k][j][0]))
                            result = true;
                }
            }

        }
    return result;

}

bool IncludSolver(vector<int>**& sudoku)
{
    bool result = false;
    // поиск единственных включений
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            for (int k = 0; k < (int)sudoku[i][j].size(); k++)
            {
                if(sudoku[i][j].size()==1)
                    continue;
                if(UnicInLine(sudoku,i,j,k))
                {
                    //std::cout << "DETECT in line!\n";
                    int tmp = sudoku[i][j][k];
                    sudoku[i][j].clear();
                    sudoku[i][j].push_back(tmp);
                    DelInColumn(sudoku,i,j,tmp);
                    DelInBlock(sudoku,i,j,tmp);
                    result = true;
                    //std::cout << sudoku[i][j].size() << "\n";
                }
                if( UnicInColumn(sudoku,i,j,k) )
                {
                    //std::cout << "DETECT in column!\n";
                    int tmp = sudoku[i][j][k];
                    sudoku[i][j].clear();
                    sudoku[i][j].push_back(tmp);
                    DelInLine(sudoku,i,j,tmp);
                    DelInBlock(sudoku,i,j,tmp);
                    result = true;
                }
                if( UnicInBlock(sudoku,i,j,k) )
                {
                    //std::cout << "DETECT in column!\n";
                    int tmp = sudoku[i][j][k];
                    sudoku[i][j].clear();
                    sudoku[i][j].push_back(tmp);
                    DelInLine(sudoku,i,j,tmp);
                    DelInColumn(sudoku,i,j,tmp);
                    result = true;
                }
            }
    while (RulesSolver(sudoku));
    return result;
}

bool DoubleSolver(vector<int>**& sudoku)
{
    bool result = false;
    // поиск вариативных дублей
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
        {
             if(DelDoubleLine(sudoku,i,j, FindDoubleLine(sudoku,i,j)))
                     result = true;

             if(DelDoubleColumn(sudoku,i,j, FindDoubleColumn(sudoku,i,j)))
                     result = true;

             if(DelDoubleBlock(sudoku,i,j, FindDoubleBlock(sudoku,i,j)))
                     result = true;
        }
    while (RulesSolver(sudoku));
    while (IncludSolver(sudoku));
    return result;
}

void RecSolver(vector<int>**& sudoku)
{
    if(isValid(sudoku)) return;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if(sudoku[i][j].size()>1)
                for (int k = 0; k < (int)sudoku[i][j].size(); k++)
                {
                    vector<int>** sudokuCopy;
                    sudokuCopy = new vector<int>*[9];
                    for (int i2 = 0; i2 < 9; i2++)
                    {
                        sudokuCopy[i2] = new vector<int>[9];
                        for (int j2 = 0; j2 < 9; j2++)
                            sudokuCopy[i2][j2]=vector<int>(sudoku[i2][j2]);
                    }

                    int itmp = sudokuCopy[i][j][k];

                    sudokuCopy[i][j].clear();
                    sudokuCopy[i][j].push_back(itmp);

                    while (RulesSolver(sudokuCopy));
                    while (IncludSolver(sudokuCopy));
                    while (DoubleSolver(sudokuCopy));

                    if(isValid(sudokuCopy))
                    {
                        vector<int>** stmp = sudoku;
                        sudoku = sudokuCopy;
                        for (int i2 = 0; i2 < 9; i2++)
                            delete[] stmp[i2] ;
                        delete[] stmp;
                        return;
                    }

                    RecSolver(sudokuCopy);

                    if(isValid(sudokuCopy))
                    {
                        if(sudokuCopy==sudoku) return;
                        vector<int>** stmp = sudoku;
                        sudoku = sudokuCopy;
                        for (int i2 = 0; i2 < 9; i2++)
                            delete[] stmp[i2] ;
                        delete[] stmp;
                        return;
                    }

                    for (int i2 = 0; i2 < 9; i2++)
                        delete[] sudokuCopy[i2] ;
                    delete[] sudokuCopy;
                }
}

void Load(vector<int>**& sudoku)
{
    try
    {
        sudoku = new vector<int>*[9];
        for (int i = 0; i < 9; i++)
            sudoku[i] = new vector<int>[9];

        std::ifstream inp("sudoku.in");
        if(!inp.is_open()) throw true;

        //sudoku = new vector<int>[9];
        int itmp;
        char ctmp;

        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++)
            {
                inp >> ctmp;
                itmp = (int)(ctmp-48);
                if(itmp!=0)
                    sudoku[i][j].push_back(itmp);
                else
                    for (int k = 0; k < 9; k++)
                        sudoku[i][j].push_back(k+1);
            }

        inp.close();
    } catch(bool e)
    {
        std::cout << "File not found\n";
    }
}

void Print(vector<int>**& sudoku)
{
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
                if(sudoku[i][j].size()==1)
                    std::cout << sudoku[i][j][0];
                else
                    std::cout << "*";
            std::cout << "\n";
        }
}

bool DelByData(vector<int>& vec, int data)
{
    if(vec.size()==1) return false;
    for (int i = 0; i < (int)vec.size(); i++)
        if(vec[i]==data)
        {
            vec.erase(vec.begin()+i);
            return true;
        }
    return false;
}

bool UnicInLine(vector<int>**& sudoku, int i, int j, int k)
{
    //std::cout << "i " << i << "| " << j <<"\n";
    for (int t = 0; t < 9; t++)
    {
        if(j==t) continue;
        if(sudoku[i][t].size()==1) continue;
        for (int f = 0; f < (int)sudoku[i][t].size(); f++)
        {
            if(sudoku[i][t][f]==sudoku[i][j][k])
                return false;
        }
    }
    return true;
}

void DelInLine(vector<int>**& sudoku, int i, int j, int data)
{
    for (int t = 0; t < 9; t++)
    {
        if(j==t) continue;
        if(sudoku[i][t].size()==1) continue;
        for (int f = 0; f < (int)sudoku[i][t].size(); f++)
        {
            if(sudoku[i][t][f]==data)
                sudoku[i][t].erase(sudoku[i][t].begin()+f);
        }
    }
}

bool UnicInColumn(vector<int>**& sudoku, int i, int j, int k)
{
    for (int t = 0; t < 9; t++)
    {
        if(i==t) continue;
        if(sudoku[t][j].size()==1) continue;
        for (int f = 0; f < (int)sudoku[t][j].size(); f++)
            if(sudoku[t][j][f]==sudoku[i][j][k])
                return false;
    }
    return true;
}

void DelInColumn(vector<int>**& sudoku, int i, int j, int data)
{
    for (int t = 0; t < 9; t++)
    {
        if(i==t) continue;
        if(sudoku[t][j].size()==1) continue;
        for (int f = 0; f < (int)sudoku[t][j].size(); f++)
            if(sudoku[t][j][f]==data)
                sudoku[t][j].erase(sudoku[t][j].begin()+f);
    }
}

bool UnicInBlock(vector<int>**& sudoku, int i, int j, int k)
{
    int BlockLineBeg;
    int BlockColuBeg;
    if(i>2)
        if(i>5)
            BlockLineBeg=6;
        else
            BlockLineBeg=3;
    else
        BlockLineBeg=0;
    if(j>2)
        if(j>5)
            BlockColuBeg=6;
        else
            BlockColuBeg=3;
    else
        BlockColuBeg=0;

    for (int t = 0; t < 3; t++)
        for (int f = 0; f < 3; f++)
            for (int g = 0; g < (int)sudoku[BlockLineBeg+t][BlockColuBeg+f].size(); g++)
            {
                if(sudoku[BlockLineBeg+t][BlockColuBeg+f].size()==1)
                    continue;
                if((BlockLineBeg+t)==i && (BlockColuBeg+f)==j)
                    continue;

                if(sudoku[BlockLineBeg+t][BlockColuBeg+f][g]==sudoku[i][j][k])
                    return false;
            }
    return true;

}

void DelInBlock(vector<int>**& sudoku, int i, int j, int data)
{
    int BlockLineBeg;
    int BlockColuBeg;
    if(i>2)
        if(i>5)
            BlockLineBeg=6;
        else
            BlockLineBeg=3;
    else
        BlockLineBeg=0;
    if(j>2)
        if(j>5)
            BlockColuBeg=6;
        else
            BlockColuBeg=3;
    else
        BlockColuBeg=0;

    for (int t = 0; t < 3; t++)
        for (int f = 0; f < 3; f++)
            for (int g = 0; g < (int)sudoku[BlockLineBeg+t][BlockColuBeg+f].size(); g++)
            {
                if(sudoku[BlockLineBeg+t][BlockColuBeg+f].size()==1)
                    continue;
                if((BlockLineBeg+t)==i && (BlockColuBeg+f)==j)
                    continue;

                if(sudoku[BlockLineBeg+t][BlockColuBeg+f][g]==data)
                    sudoku[BlockLineBeg+t][BlockColuBeg+f].erase(
                                sudoku[BlockLineBeg+t][BlockColuBeg+f].begin()+g);
            }
}

vector<int> FindDoubleLine(vector<int>**& sudoku, int i, int j)
{
    vector<int> result;
    for (int t = 0; t < 9; t++)
    {
        if(j==t) continue;
        if(sudoku[i][t].size() != sudoku[i][j].size())
            continue;
        int count = 0;
        for (int f = 0; f < (int)sudoku[i][t].size(); f++)
        {
            for (int g = 0; g < (int)sudoku[i][t].size(); g++)
            {
                if(sudoku[i][j][g]==sudoku[i][t][f])
                    count++;
            }
        }
        if( count == (int)sudoku[i][j].size()) result.push_back(t);
    }
    if((int)result.size()==(int)sudoku[i][j].size()-1)
        return result;
    else
    {
        result.clear();
        return result;
    }
}

vector<int> FindDoubleColumn(vector<int>**& sudoku, int i, int j)
{
    vector<int> result;
    for (int t = 0; t < 9; t++)
    {
        if(i==t) continue;
        if(sudoku[t][j].size() != sudoku[i][j].size())
            continue;
        int count = 0;
        for (int f = 0; f < (int)sudoku[t][j].size(); f++)
        {
            for (int g = 0; g < (int)sudoku[t][j].size(); g++)
            {
                if(sudoku[i][j][f]==sudoku[t][j][g])
                    count++;
            }
        }
        if( count == (int)sudoku[i][j].size()) result.push_back(t);
    }
    if(result.size()==sudoku[i][j].size()-1)
        return result;
    else
    {
        result.clear();
        return result;
    }
}

vector<int> FindDoubleBlock(vector<int>**& sudoku, int i, int j)
{
    int BlockLineBeg;
    int BlockColuBeg;
    if(i>2)
        if(i>5)
            BlockLineBeg=6;
        else
            BlockLineBeg=3;
    else
        BlockLineBeg=0;
    if(j>2)
        if(j>5)
            BlockColuBeg=6;
        else
            BlockColuBeg=3;
    else
        BlockColuBeg=0;

    vector<int> result;
    for (int t = 0; t < 3; t++)
        for (int f = 0; f < 3; f++)
            for (int g = 0; g < (int)sudoku[BlockLineBeg+t][BlockColuBeg+f].size(); g++)
            {
                if(i==BlockLineBeg+t && j==BlockColuBeg+f) continue;
                if(sudoku[BlockLineBeg+t][BlockColuBeg+f].size() != sudoku[i][j].size())
                    continue;

                int count = 0;
                for (int f = 0; f < (int)sudoku[BlockLineBeg+t][BlockColuBeg+f].size(); f++)
                {
                    for (int g = 0; g < (int)sudoku[i][t].size(); g++)
                    {
                        if(sudoku[i][j][f]==sudoku[i][t][g])
                            count++;
                    }
                }
                if( count == (int)sudoku[i][j].size())
                {
                    result.push_back(t);
                    result.push_back(f);
                }
            }
    if(result.size()/2==sudoku[i][j].size()-1)
        return result;
    else
    {
        result.clear();
        return result;
    }
}

bool DelDoubleLine(vector<int>**& sudoku, int i, int j, vector<int> column)
{
    bool returnFlag = false;
    if (column.size()==0) return returnFlag;
    vector<int> delnum = sudoku[i][j];
    for (int k = 0; k < 9; k++)
    {
        bool continueFlag=false;
        for (int k2 = 0; k2 < (int)column.size(); k2++)
            if(k==column[k2] || k==j) continueFlag=true;
        if(continueFlag) continue;

        if(sudoku[i][k].size() == 1) continue;

        for (int t = 0; t < (int)sudoku[i][k].size(); t++)
            for (int f = 0; f < (int)delnum.size(); f++)
                if(sudoku[i][k].size()>1)
                    if(sudoku[i][k][t]==delnum[f])
                    {
                        sudoku[i][k].erase(sudoku[i][k].begin()+t);
                        returnFlag = true;
                        t--;
                    }
    }
    return returnFlag;
}

bool DelDoubleColumn(vector<int>**& sudoku, int i, int j, vector<int> line)
{
    bool returnFlag = false;
    if (line.size()==0) return returnFlag;
        vector<int> delnum = sudoku[i][j];
        for (int k = 0; k < 9; k++)
        {
            bool continueFlag=false;
            for (int k2 = 0; k2 < (int)line.size(); k2++)
                if(k==line[k2] || k==j) continueFlag=true;
            if(continueFlag) continue;

            if(sudoku[k][j].size() == 1) continue;

            for (int t = 0; t < (int)sudoku[k][j].size(); t++)
                for (int f = 0; f < (int)delnum.size(); f++)
                    if(sudoku[k][j].size()>1)
                        if(sudoku[k][j][t]==delnum[f])
                        {
                            sudoku[k][j].erase(sudoku[k][j].begin()+t);
                            returnFlag=true;
                            t--;
                        }
        }
        return returnFlag;
}

bool DelDoubleBlock(vector<int>**& sudoku, int i, int j, vector<int> block)
{
    int BlockLineBeg;
    int BlockColuBeg;
    if(i>2)
        if(i>5)
            BlockLineBeg=6;
        else
            BlockLineBeg=3;
    else
        BlockLineBeg=0;
    if(j>2)
        if(j>5)
            BlockColuBeg=6;
        else
            BlockColuBeg=3;
    else
        BlockColuBeg=0;

    bool returnFlag = false;
    if (block.size()==0) return returnFlag;
    vector<int> delnum = sudoku[i][j];

    for (int t = 0; t < 3; t++)
        for (int f = 0; f < 3; f++)
        {
            bool continueFlag=false;
            for (int k2 = 0; k2 < (int)block.size(); k2+=2)
                if( (BlockLineBeg+t==block[k2] && BlockColuBeg+f==block[k2+1])
                        || (t==i && f==j)) continueFlag=true;
            if(continueFlag) continue;

            if(sudoku[BlockLineBeg+t][BlockLineBeg+f].size() == 1) continue;

            for (int t2 = 0; t2 < (int)delnum.size(); t2++)
                for (int k2 = 0; k2 < (int)delnum.size(); k2++)
                    if(sudoku[BlockLineBeg+t][BlockLineBeg+f].size()>1)
                        if(sudoku[BlockLineBeg+t][BlockLineBeg+f][t2]==delnum[k2])
                        {
                            sudoku[BlockLineBeg+t][BlockLineBeg+f].erase(
                                        sudoku[BlockLineBeg+t][BlockLineBeg+f].begin()+t2);
                            returnFlag = true;
                            t2--;
                        }
        }
        return returnFlag;
}

bool isValidLine(vector<int>**& sudoku, int i, int j)
{
    for (int t = 0; t < 9; t++)
    {
        if(j==t) continue;
        if(sudoku[i][t][0]==sudoku[i][j][0])
            return false;
    }
    return true;
}

bool isValidColumn(vector<int>**& sudoku, int i, int j)
{
    for (int t = 0; t < 9; t++)
    {
        if(i==t) continue;
        if(sudoku[t][j][0]==sudoku[i][j][0])
            return false;
    }
    return true;
}

bool isValidBlock(vector<int>**& sudoku, int i, int j)
{
    int BlockLineBeg;
    int BlockColuBeg;
    if(i>2)
        if(i>5)
            BlockLineBeg=6;
        else
            BlockLineBeg=3;
    else
        BlockLineBeg=0;
    if(j>2)
        if(j>5)
            BlockColuBeg=6;
        else
            BlockColuBeg=3;
    else
        BlockColuBeg=0;

    for (int t = 0; t < 3; t++)
        for (int f = 0; f < 3; f++)
        {
            if((BlockLineBeg+t)==i && (BlockColuBeg+f)==j)
                continue;

            if(sudoku[BlockLineBeg+t][BlockColuBeg+f][0]==sudoku[i][j][0])
                return false;
        }
    return true;
}
