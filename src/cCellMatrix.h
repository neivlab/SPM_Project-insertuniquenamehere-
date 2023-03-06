#include <vector>
#include <array>

// Forward declaration of the base cell class.
// Since only a cLife pointer is used in this file, the whole cLife class isn't necessary to include
class cLife;

// The Cell data structure forms the basis of the Cell Matrix. Each cell contains one life class type,
//  alive or dead. The cell provides a wrapper interface 



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
    cLife*  getCellLife(int row, int col) const { return m_cells[row][col].mp_life; }
    int     getWidth() const { return m_cellsWidth; }
    int     getHeight() const { return m_cellsHeight; }
    int     getLivingCellCount(void) const;

private:
    class cCell
    {
    public:
        cCell();
        cCell(int xCentre, int yCentre);
        cCell(const cCell& other);
        ~cCell();
        void    setup(int xCentre, int yCentre);    // must be called if the default constructor is used
        void    reset();

        cLife*  mp_life{ nullptr };         // pointer to the Life in this cell. Must be cLife or type derived from cLife
        int     m_healthChange{ 0 };        // used to determinte result of generation of simulation - added to each cell's health after all cells are simulated
    private:
        int     m_xCentre{ 0 };             // xy centre of the cell, calculated one and passed to the life for drawing
        int     m_yCentre{ 0 };
    };

    const int CELL_SIZE = 16;
    // grid drawing data
    int	m_cellsWidth{ 0 };              // width of the matrix, in cells
    int m_cellsHeight{ 0 };             // height of the matrix, in cells
    
    std::vector <std::vector<cCell>> m_cells;   // the cell matrix is a container for a 2D array of cells
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