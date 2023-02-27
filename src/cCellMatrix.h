#include <vector>
#include <array>

// Forward declaration of the base cell class.
// Since only a cLife pointer is used in this file, the whole cLife class isn't necessary to include
class cLife;

// The Cell data structure forms the basis of the Cell Matrix. Each cell contains one life class type,
//  alive or dead. The cell provides a wrapper interface 
struct sCell
{
public:  // by default, all struct members are public anyway...
    sCell();
    ~sCell();
    int     getIndex() { return m_idx; }
    void    setup(int x, int y);
    void    reset(int x, int y);
    void    draw();

    static int ms_count;            // class-static shared counter of all cells created. It should be exactly width * height
    cLife*  mp_life;                // pointer to the Life in this cell. Must be cLife or type derived from cLife
    int     m_healthChange{ 0 };   // used to determinte result of generation of simulation - added to each cell's health after all cells are simulated
private:
    int     m_idx;                  // the cell unique id/index of this cell. not really used.
};


const int CELL_SIZE = 16;

class cCellMatrix {
public:
    cCellMatrix() = delete;     // no default constructor - must set width & height
    cCellMatrix(int width, int height);
    virtual ~cCellMatrix();
    void    setup();
    void    reset();
    void    update();
    void    draw();
    cLife*  setCellLife(int row, int col, cLife* pNewLife);
    cLife*  getCellLife(int row, int col) const { return mp_cells[row][col].mp_life; }
    int     getWidth() const { return m_cellsWidth; }
    int     getHeight() const { return m_cellsHeight; }
    int     getLivingCellCount(void) const;

private:
    // the cell matrix is a container for a 2D array of cells
    sCell** mp_cells{ nullptr };

    // grid drawing data
    int	m_cellsWidth{ 16 };
    int m_cellsHeight{ 16 };
};





#if 0
using CellVector = std::vector<sCell>;
using CellVectorItr = std::vector<std::vector<sCell>>::iterator;
std::vector<std::vector<sCell>> mp_cells;


int main() {
    std::cout << "Hello World!\n";

    std::array<std::array<cCellMatrix, 48>, 32> cellMatrix;
    using Array2DItr = std::array<std::array<cCellMatrix, 48>, 32>::iterator;

    for (Array2DItr rowItr = cellMatrix.begin(); rowItr != cellMatrix.end();
        rowItr++) {
        for (auto itr = rowItr->begin(); itr != rowItr->end(); itr++)
            std::cout << itr->getId() << " ";
    }
}


#endif