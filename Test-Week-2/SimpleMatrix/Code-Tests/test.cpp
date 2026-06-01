#include "pch.h"

template <typename T>
Matrix<T> makeMatrix(int rows, int columns, std::initializer_list<T> values) {
  Matrix<T> matrix(rows, columns);
  int index = 0;

  for (const T& value : values) {
    matrix.set(index / columns, index % columns, value);
    ++index;
  }

  return matrix;
}

TEST(MatrixCsvTests, ConstructorWithDimensionsCreatesZeroMatrix) {
  Matrix<int> matrix(2, 3);

  EXPECT_EQ(matrix.size()[0], 2);
  EXPECT_EQ(matrix.size()[1], 3);
  EXPECT_EQ(matrix.get(0, 0), 0);
  EXPECT_EQ(matrix.get(0, 1), 0);
  EXPECT_EQ(matrix.get(0, 2), 0);
  EXPECT_EQ(matrix.get(1, 0), 0);
  EXPECT_EQ(matrix.get(1, 1), 0);
  EXPECT_EQ(matrix.get(1, 2), 0);
}

TEST(MatrixCsvTests, DefaultConstructorCreatesEmptyMatrix) {
  Matrix<int> matrix;

  EXPECT_EQ(matrix.size()[0], 0);
  EXPECT_EQ(matrix.size()[1], 0);
}

TEST(MatrixCsvTests, CopyConstructorPerformsDeepCopy) {
  Matrix<int> source = makeMatrix<int>(2, 2, {1, 2, 3, 4});
  Matrix<int> copy(source);

  source.set(0, 0, 9);

  EXPECT_EQ(copy.get(0, 0), 1);
  EXPECT_EQ(copy.get(0, 1), 2);
  EXPECT_EQ(copy.get(1, 0), 3);
  EXPECT_EQ(copy.get(1, 1), 4);
}

TEST(MatrixCsvTests, AssignmentOperatorPerformsDeepCopy) {
  Matrix<int> source = makeMatrix<int>(2, 2, {1, 2, 3, 4});
  Matrix<int> target = makeMatrix<int>(2, 2, {9, 9, 9, 9});

  target = source;
  source.set(1, 1, 7);

  EXPECT_EQ(target.get(0, 0), 1);
  EXPECT_EQ(target.get(0, 1), 2);
  EXPECT_EQ(target.get(1, 0), 3);
  EXPECT_EQ(target.get(1, 1), 4);
}

TEST(MatrixCsvTests, SetGetAndIndexOperatorProvideValidAccess) {
  Matrix<int> matrix(2, 3);

  matrix.set(1, 2, 9);

  EXPECT_EQ(matrix.get(1, 2), 9);
  EXPECT_EQ(matrix[1][2], 9);
}

TEST(MatrixCsvTests, OutOfBoundsAccessThrowsInvalidArgument) {
  Matrix<int> matrix(2, 2);

  EXPECT_THROW(matrix.set(2, 0, 1), std::invalid_argument);
  EXPECT_THROW(matrix.get(0, 2), std::invalid_argument);
  EXPECT_THROW(matrix[2], std::invalid_argument);
}

TEST(MatrixCsvTests, SizeReturnsCorrectDimensions) {
  Matrix<int> matrix(4, 5);

  EXPECT_EQ(matrix.size()[0], 4);
  EXPECT_EQ(matrix.size()[1], 5);
}

TEST(MatrixCsvTests, AddInPlaceAddsEqualMatrices) {
  Matrix<int> a = makeMatrix<int>(2, 2, {1, 2, 3, 4});
  Matrix<int> b = makeMatrix<int>(2, 2, {10, 20, 30, 40});

  a.addInPlace(b);

  EXPECT_EQ(a.get(0, 0), 11);
  EXPECT_EQ(a.get(0, 1), 22);
  EXPECT_EQ(a.get(1, 0), 33);
  EXPECT_EQ(a.get(1, 1), 44);
}

TEST(MatrixCsvTests, OperatorPlusReturnsNewMatrixAndLeavesInputsUntouched) {
  Matrix<int> a = makeMatrix<int>(2, 2, {1, 2, 3, 4});
  Matrix<int> b = makeMatrix<int>(2, 2, {10, 20, 30, 40});

  Matrix<int> result = a + b;

  EXPECT_EQ(result.get(0, 0), 11);
  EXPECT_EQ(result.get(0, 1), 22);
  EXPECT_EQ(result.get(1, 0), 33);
  EXPECT_EQ(result.get(1, 1), 44);
  EXPECT_EQ(a.get(0, 0), 1);
  EXPECT_EQ(a.get(0, 1), 2);
  EXPECT_EQ(a.get(1, 0), 3);
  EXPECT_EQ(a.get(1, 1), 4);
  EXPECT_EQ(b.get(0, 0), 10);
  EXPECT_EQ(b.get(0, 1), 20);
  EXPECT_EQ(b.get(1, 0), 30);
  EXPECT_EQ(b.get(1, 1), 40);
}

TEST(MatrixCsvTests, MultiplyInPlaceWithScalarWorks) {
  Matrix<int> matrix = makeMatrix<int>(2, 2, {1, -2, 3, -4});

  matrix.multiplyInPlace(-2);

  EXPECT_EQ(matrix.get(0, 0), -2);
  EXPECT_EQ(matrix.get(0, 1), 4);
  EXPECT_EQ(matrix.get(1, 0), -6);
  EXPECT_EQ(matrix.get(1, 1), 8);
}

TEST(MatrixCsvTests, MatrixMultiplicationWorks) {
  Matrix<int> a = makeMatrix<int>(2, 3, {1, 2, 3, 4, 5, 6});
  Matrix<int> b = makeMatrix<int>(3, 2, {7, 8, 9, 10, 11, 12});

  Matrix<int> result = a * b;

  EXPECT_EQ(result.get(0, 0), 58);
  EXPECT_EQ(result.get(0, 1), 64);
  EXPECT_EQ(result.get(1, 0), 139);
  EXPECT_EQ(result.get(1, 1), 154);
}

TEST(MatrixCsvTests, EqualityOperatorComparesContentAndSize) {
  Matrix<int> a = makeMatrix<int>(2, 2, {1, 2, 3, 4});
  Matrix<int> b = makeMatrix<int>(2, 2, {1, 2, 3, 4});

  EXPECT_TRUE(a == b);

  b.set(1, 1, 7);

  EXPECT_FALSE(a == b);
}

TEST(MatrixCsvTests, TransposeInPlaceSwapsRowsAndColumns) {
  Matrix<int> matrix = makeMatrix<int>(2, 3, {1, 2, 3, 4, 5, 6});

  matrix.transposeInPlace();

  EXPECT_EQ(matrix.size()[0], 3);
  EXPECT_EQ(matrix.size()[1], 2);
  EXPECT_EQ(matrix.get(0, 0), 1);
  EXPECT_EQ(matrix.get(0, 1), 4);
  EXPECT_EQ(matrix.get(1, 0), 2);
  EXPECT_EQ(matrix.get(1, 1), 5);
  EXPECT_EQ(matrix.get(2, 0), 3);
  EXPECT_EQ(matrix.get(2, 1), 6);
}

TEST(MatrixCsvTests, HadamardInPlaceCalculatesElementwiseProduct) {
  Matrix<int> a = makeMatrix<int>(2, 2, {1, 2, 3, 4});
  Matrix<int> b = makeMatrix<int>(2, 2, {5, 6, 7, 8});

  a.hadamardInPlace(b);

  EXPECT_EQ(a.get(0, 0), 5);
  EXPECT_EQ(a.get(0, 1), 12);
  EXPECT_EQ(a.get(1, 0), 21);
  EXPECT_EQ(a.get(1, 1), 32);
}

TEST(MatrixCsvTests, ConvertToCopiesValuesToDifferentType) {
  Matrix<int> source = makeMatrix<int>(2, 2, {1, 2, 3, 4});
  Matrix<float> target;

  source.convertTo(target);
  target.multiplyInPlace(1.1f);

  EXPECT_NEAR(target.get(0, 0), 1.1f, 1e-5f);
  EXPECT_NEAR(target.get(0, 1), 2.2f, 1e-5f);
  EXPECT_NEAR(target.get(1, 0), 3.3f, 1e-5f);
  EXPECT_NEAR(target.get(1, 1), 4.4f, 1e-5f);
}