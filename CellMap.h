#ifndef MY_CLASS_H  
#define MY_CLASS_H

class CellMap{
public:
    CellMap(int width, int height);
    ~CellMap();
    void SetCell(int x, int y);
    void ClearCell(int x, int y);
    void Init(double prob_start);
    int CellState(int x, int y);
    void NextGen();
    void Scope();
    void Reset();
    void Text();
private:
    char *cells;
    char *temp_cells;
    int w,h;
    int length;
};

#endif