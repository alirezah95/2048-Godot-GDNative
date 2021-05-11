#ifndef __MATRIXINDEX_H__
#define __MATRIXINDEX_H__

namespace godot
{
    class MatrixIndex
    {
    public:
        int row;
        int col;

    public:
        MatrixIndex()
        {
            row = 0;
            col = 0;
        }
        MatrixIndex(int __row, int __col)
        {
            row = __row;
            col = __col;
            return;
        }

        /* Copy consructor */
        MatrixIndex(const MatrixIndex &__other)
        {
            row = __other.row;
            col = __other.col;
            return;
        }

        /* Move constructor */
        MatrixIndex(const MatrixIndex &&__other)
        {
            row = __other.row;
            col = __other.col;
            return;
        }
    };
}

#endif // __MATRIXINDEX_H__