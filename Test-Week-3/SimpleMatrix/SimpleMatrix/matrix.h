/*!
 * @file matrix.h
 * @author Tony Andrioli, The Hague University of Applied Sciences
 * @date June 2022
 * 
 * Contains a (verry simple) template class Matrix, for simple matrix calculation.
 * This class is made for educational purposes only. If you're looking for a serious matrix library, keep looking :-)
 * 
 */
/*! @mainpage
 * @ref Matrix "the matrix template class"  
 *
 * Created by: Tony Andrioli
 */

#pragma once

#include <cstdlib>
#include <iostream>
#include <cstring>

//#define DEBUG

#ifdef DEBUG
int idcount = 0;
#endif

/*!
 * @class Matrix
 * @brief Basic Matrix template class. 
 *  
 * Very basic Matrix class for addition, multiplication and
 * transposing of matrices. This is a template class so to declare
 * a matrix of integers, declare:
 * @code{.cpp}
 * Matrix<int> m;
 * @endcode
 * 
 * Some operations are inplace, that is the current matrix will be updated,
 * while other operations return new matrices, leaving the current matrix
 * unaltered. 
 * 
 * Matrices need to have the same type in order to do calculation on them.
 * So for example a multiplication of Matrix<int> times Matrix<float> won't
 * compile. (With an exception for convertTo, naturally)
 * 
 * Class T must have * and + operators. ConvertTo uses only standard casting.
 */
template <class T>
class Matrix {

private:
    T* m;                    // The data of the matrix
    int max_row, max_col;    // Matrix dimensions

    int id;
public:
    // ----------------------------------------------------------------------
    // constructors / destructors
    // ----------------------------------------------------------------------

    /*!
     * @brief Constructs empty matrix
     *
     * Normally you create only an empty matrix if you want to
     * store the result of a matrix calculation in it later.
     */
    Matrix() {
        m = NULL;
        max_col = 0;
        max_row = 0;
        
    #ifdef DEBUG 
        id = ++idcount;
        std::cout << "construct :" << max_row << "X" << max_col << "[" << id << "]" << std::endl;
    #endif
    };

    /*!
     * @brief Copy construct
     *
     * Create new matrix based on the parameter. The matrix given by the 
     * parameter will be (deep) copied, so you really end up having to separate 
     * matrices.
     * @param[in] other the matrix to be copied from
     */
    Matrix(const Matrix& other) {
        m = NULL;
        max_col = other.max_col;
        max_row = other.max_row;
    #ifdef DEBUG     
        id = ++idcount;
        std::cout << "construct :" << max_row << "X" << max_col << "[" << id << "]" << std::endl;
    #endif
        if (max_row*max_col > 0) {
            m = (T*) calloc(max_row*max_col, sizeof(T));
            memcpy(m,other.m,max_row*max_col*sizeof(T)); 
        }   
    };

    /*!
     * @brief Default constructor
     *
     * Create a new matrix based on the given dimensions
     * @param[in] rows    the number of rows of the matrix
     * @param[in] columns the number of columns of the matrix
     * @param[in] values  optional: an 1-dimensional array with the matrix values 
     *                    stored from left to right and top to bottom. 
     *                    
     */
    Matrix(unsigned int rows, unsigned int columns, const T* values = NULL){
        m = NULL;
        max_col = columns;
        max_row = rows;
    #ifdef DEBUG 
        id = ++idcount;
        std::cout << "construct :" << max_row << "X" << max_col << "[" << id << "]" << std::endl;
    #endif
        if (rows*columns > 0)
            m = (T*) calloc(rows*columns, sizeof(T));
        if (values != NULL) 
            memcpy(m,values,rows*columns*sizeof(T));
    };

    /*!
     * @brief Default destructor
     *
     * Free the memory in use by this matrix
     */
    ~Matrix(){
        if ( m != NULL)
            free(m);
        m = NULL;
    #ifdef DEBUG 
        std::cout << "destroy :" << max_row << "X" << max_col << "[" << id << "]" << std::endl;
    #endif
    };

    // ----------------------------------------------------------------------
    // setters &  getters
    // ----------------------------------------------------------------------

    /*!
     * @brief Set one matrix element
     *
     * @param[in] row the row position of the element to set.
     * @param[in] col the column position of the element to set.
     * @param[in] val the value the element must be set to.
     * @exception invalid_argument thrown when row or col is out of bounds.
     */
    void set(unsigned int row, unsigned int col, T val) {
        if (row < max_row && col < max_col)
            m[(row*max_col)+col] = val;
        else
            throw std::invalid_argument( "Set: out of bounds" );
    }

    /*!
     * @brief Get one matrix element
     *
     * @param[in] row the row position of the element to set.
     * @param[in] col the column position of the element to set.
     * @return    the value of the requested element.
     * @exception invalid_argument thrown when row or col is out of bounds.
     */
    T get(unsigned int row, unsigned int col) const {    
        if (row < max_row && col < max_col)
            return m[(row*max_col)+col];
        else
            throw std::invalid_argument( "Get: out of bounds" );

        return (T)0;
    } 

    /*!
     * @brief Index operator
     *
     * Returns an array with the matrix-row. As this is a regular array, 
     * it can be indexed again. So  
     * @code{.cpp}
     * A.get(1,2)
     * @endcode
     * equals
     * @code{.cpp}
     * A[1][2]
     * @endcode
     * 
     * Warning! As you get a pointer to the internal data structure of the matrix
     * as return value, you mess the matrix up. So don't reallocate,
     * free memory and such..
     * 
     * @param i the first (row) index
     * @return a pointer to the requested row
     */
    T* operator [] (unsigned int i) {
        if (i < max_row)
            return &m[i*max_col];

        throw std::invalid_argument( "[]: out of bounds" );
        return NULL;
    }

    /*!
     * @brief Return the size of the matrix
     * @return an pointer to an int[2] array with the { row, col } value.
     */
    int* size() const {
        static int s[2];
        s[0] = max_row;
        s[1] = max_col;
        return s;
    };  

    // ----------------------------------------------------------------------
    // addition
    // ----------------------------------------------------------------------

    /*!
     * @brief Inplace addition, add 'other' to current matrix.
     *
     * Matrices must have he same dimensions for an addition.
     * 
     * @param[in] other the matrix to add.
     * @exception invalid_argument thrown matrix dimension don't match.
     */
    void addInPlace(const Matrix<T>& other) {
        if ( (max_row != other.max_row) || (max_col != other.max_col) ) {
            throw std::invalid_argument( "Addition: matrices must have the same size." );
        }
        for (int r=0; r<max_row; r++) 
            for (int c=0; c<max_col; c++) {
                m[(r*max_col)+c] += other.m[(r*max_col)+c];
            }
    };
    /*!
     * @brief addition, add 'other' to current matrix, and return new result-matrix.
     *
     * Matrices must have he same dimensions for an addition.
     * 
     * @param[in] first the first matrix to add.
     * @param[in] second the second matrix to add.
     * @return The new result-matrix
     * @exception invalid_argument thrown matrix dimension don't match.
     */    
    static Matrix<T> add(const Matrix<T>& first, const Matrix<T>& second) {
        Matrix<T> nieuw(first);
        nieuw.addInPlace(second);
        return nieuw;
    };

    /*!
     * @brief addition operator
     *
     * For convenience calls 
     * @code{.cpp}
     * Matrix::add(first, second)
     * @endcode
     */    
    Matrix<T> operator+ (const Matrix<T>& other) {
        return Matrix::add( (*this), other);
    }

    // ----------------------------------------------------------------------
    // multiplication
    // ----------------------------------------------------------------------

    /*!
     * @brief inplace multiplication
     *
     * inplace multiplication with scalar, multiply every element with 'scalar'
     * @param[in] scalar the multiplication factor
    */
    void multiplyInPlace(T scalar) {
        for (int r=0; r<max_row; r++) 
            for (int c=0; c<max_col; c++) {
                m[(r*max_col)+c] *= scalar;
            }
    };

     /*!
     * @brief multiplication with scalar
     *
     * Multiplication of  a matrix with  a scalar
     * @param[in] first  the matrix to multiply
     * @param[in] scalar the multiplication factor
     * @return Return new matrix with the result of first*scalar
    */   
    static Matrix<T> multiply(const Matrix<T>& first, T scalar) {
        Matrix<T> nieuw(first);
        nieuw.multiplyInPlace(scalar);
        return nieuw;
    }

     /*!
     * @brief multiplication of two matrices
     *
     * Multiplication of two matrices: first*second
     * @param[in] first  the first matrix to multiply
     * @param[in] second the second matrix to multiply
     * @return Return new matrix with the result of first*second
    */ 
    static Matrix<T> multiply(const Matrix<T>& first, const Matrix<T>& second) {
        if ( first.max_col != second.max_row ) {
            throw std::invalid_argument( "Multiplication: matrices sizes don't alow multiplication." );
        }

        Matrix<T> ret(first.max_row, second.max_col);
        for (int r=0; r < ret.max_row; r++) 
            for (int c=0; c < ret.max_col; c++) {
                T temp = 0;
                for ( int i=0; i < first.max_col; i++)
                    temp += first.m[r*first.max_col+i] * second.m[i*second.max_col+c];
                ret.m[(r*ret.max_col)+c] = temp;
            } 
        return ret;
    };

    /*!
     * @brief inplace multiplication operator
     *
     * See multiplyInPlace()
    */
    Matrix<T> operator* (const T scalar) {
        return Matrix::multiply((*this), scalar);
    }

    /*!
     * @brief multiplication operator
     *
     * Multiplication of two matrices: this*other
     * @param[in] other the second matrix to multiply
     * @return Return new matrix with the result of this*other
    */
    Matrix<T> operator* (const Matrix<T>& other) {
        return Matrix::multiply((*this), other);
    }

    /*!
     * @brief Hadamard operation
     *
     * Hadamard (element-wise) product. Executed inplace.
     * @param[in] other the second matrix to multiply
    */
    void hadamardInPlace(const Matrix<T>& other) {
        if ( (max_row != other.max_row) || (max_col != other.max_col) ) {
            throw std::invalid_argument( "hadamard: matrices must have the same size." );
        }
        for (int r=0; r<max_row; r++) 
            for (int c=0; c<max_col; c++) {
                m[(r*max_col)+c] *= other.m[(r*max_col)+c];
            }
    };

    /*!
     * @brief Hadamard operation
     *
     * Hadamard (element-wise) product. Returns new result matrix.
     * @param[in] other the second matrix to multiply
     * @return Return new matrix with the result of the hadamard product of this*other
    */
    Matrix<T> hadamard(const Matrix<T>& other) {
        Matrix nieuw((*this));
        nieuw.hadamardInPlace(other);
        return nieuw; 
    };

    // ----------------------------------------------------------------------
    // Transpose
    // ----------------------------------------------------------------------

    /*!
     * @brief Inplace transpose
     *
     * transposes the current matrix.
     */
    void transposeInPlace() {
        T* nieuw = (T*) calloc(max_row*max_col, sizeof(T));
        int tmp = 0;
        for (int c=0; c < max_col; c++) 
            for (int r=0; r < max_row; r++) {
                nieuw[tmp++] = get(r,c); 
            }
        tmp = max_col;
        max_col = max_row;
        max_row = tmp;
        free(m);
        m = nieuw;
    };

    /*!
     * @brief Transpose the current matrix
     *
     * @return A new matrix with the transposed values.
     */
    Matrix<T> transpose() {
        Matrix<T> ret(max_col, max_row);
        for (int r=0; r < max_row; r++) 
            for (int c=0; c < max_col; c++) {
                ret.set(c,r, get(r,c)); 
            }

        return ret;
    };

    // ----------------------------------------------------------------------
    // Other operations
    // ----------------------------------------------------------------------

    /*! @brief assignment operator
     *
     *  @param[in] other the matrix to copy into this
     */
    void operator= (const Matrix<T>& other) {
    #ifdef DEBUG 
        std::cout << " operator = (free " << max_row << "X" << max_col << "[" << id << "])" << 
          "-> (" << other.max_row << "X" << other.max_col << "[" << other.id << "])" << std::endl;
    #endif
        if (m != NULL) 
            free(m);
        m = NULL;
        max_row = other.max_row;
        max_col = other.max_col;
        if (max_row*max_col > 0) {
            m = (T*) calloc(max_row*max_col, sizeof(T));
            memcpy(m, other.m, max_row*max_col*sizeof(T));
        }
    };

    /*! @brief equality operator
     * 
     * @param[in] other the matrix to compare to
     * @return true is matrices are equal
     */
    bool operator== (const Matrix<T>& other){
        if ( (max_col != other.max_col) || (max_row != other.max_row) )
            return false;
        for (int r=0; r < max_row; r++) {
            for (int c=0;  c <max_col; c++) {
                if (get(r,c) != other.get(r,c))
                    return false;
            }
        };
        return true;
    }

    /*! @brief print matrix to cout, for debug purposes.
     */
    void debug() const {
        for (int r=0; r < max_row; r++) {
            for (int c=0;  c <max_col; c++) {
                std::cout << get(r,c) << " ";
            }
            std::cout << std::endl;
        }
    }

    /*! @brief create duplicate matrix of with different type
     *
     * Create duplicate but with different type. So, you can for example 
     * convert int matrices to float matrices 
     * usage: matrixa.convertTo(matrixOut), where matrixOut
     * is the matrix where the data is copied to.
     * Type conversion is just done by typecasting!! (No rounding)
     * 
     * @param[out] out the new matrix
     */
    
    template <class To>
    void convertTo(Matrix<To>& out) {
        // Not very efficient method, but create a new matrix first.
        // The inefficiency is that 'out=tmp' will copy it again!        
        Matrix<To> tmp = Matrix<To>(max_row, max_col);
        for (int r=0; r<max_row; r++) 
            for (int c=0; c<max_col; c++) {
                tmp.set(r,c, (To) get(r,c));
            }
        out = tmp;    
    }
};
