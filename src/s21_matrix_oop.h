#include <cmath>
#include <cstdlib>
#include <iostream>

class S21Matrix {
  int rows_;
  int cols_;
  double** matrix_;

  double** MemoryAllocating(int rows, int cols);
  void MemoryDeallocating();
  void CopyMatrix(const S21Matrix& other);
  bool ExistMatrix() const;
  bool EqSizeMatrix(const S21Matrix& other) const;
  void ShortCopy(const S21Matrix& other, int rows, int cols);

 public:
  S21Matrix();
  S21Matrix(int rows, int cols);
  S21Matrix(const S21Matrix& other);
  S21Matrix(S21Matrix&& other);

  bool EqMatrix(const S21Matrix& other);
  void SumMatrix(const S21Matrix& other);
  void SubMatrix(const S21Matrix& other);
  void MulNumber(double number);
  void MulMatrix(const S21Matrix& other);
  S21Matrix Transpose();
  S21Matrix CalcComplements();
  double Determinant();
  S21Matrix InverseMatrix();

  S21Matrix operator+(const S21Matrix& other);
  S21Matrix operator-(const S21Matrix& other);
  S21Matrix operator*(const S21Matrix& other);
  S21Matrix operator*(double number);
  bool operator==(const S21Matrix& other);
  S21Matrix operator=(const S21Matrix& other);
  S21Matrix operator+=(const S21Matrix& other);
  S21Matrix operator-=(const S21Matrix& other);
  S21Matrix operator*=(const S21Matrix& other);
  S21Matrix operator*=(double number);
  double& operator()(int i, int j);
  int GetCols() const;
  int GetRows() const;
  void FillingMatrix();
  void SetCols(int cols);
  void SetRows(int rows);

  ~S21Matrix();
};
