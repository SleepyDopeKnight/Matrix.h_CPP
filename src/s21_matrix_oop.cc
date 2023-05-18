#include "s21_matrix_oop.h"

// Constructors

S21Matrix::S21Matrix() : rows_(0), cols_(0), matrix_(0) {}

S21Matrix::S21Matrix(int rows, int cols) {
  if (rows > 0 && cols > 0) {
    rows_ = rows;
    cols_ = cols;
    matrix_ = MemoryAllocating(rows_, cols_);
  }
}

S21Matrix::S21Matrix(const S21Matrix& other) {
  if (other.ExistMatrix()) {
    this->rows_ = other.rows_;
    this->cols_ = other.cols_;
    this->matrix_ = MemoryAllocating(other.rows_, other.cols_);
    CopyMatrix(other);
  }
}

S21Matrix::S21Matrix(S21Matrix&& other) { MoveMatrix(other); }

S21Matrix::~S21Matrix() { MemoryDeallocating(); }

// Operations

bool S21Matrix::EqMatrix(const S21Matrix& other) {
  bool status_of_equality = false;
  if (other.ExistMatrix() && this->ExistMatrix() && this->EqSizeMatrix(other)) {
    status_of_equality = true;
    for (int i = 0; i < rows_ && status_of_equality; ++i) {
      for (int j = 0; j < cols_ && status_of_equality; ++j) {
        if (fabs(this->matrix_[i][j] - other.matrix_[i][j]) > 1e-07) {
          status_of_equality = false;
        }
      }
    }
  }
  return status_of_equality;
}

void S21Matrix::SumMatrix(const S21Matrix& other) {
  if (this->EqSizeMatrix(other)) {
    if (other.ExistMatrix() && this->ExistMatrix()) {
      for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
          matrix_[i][j] = matrix_[i][j] + other.matrix_[i][j];
        }
      }
    }
  } else {
    throw std::out_of_range("Different size of matrix");
  }
}

void S21Matrix::SubMatrix(const S21Matrix& other) {
  if (this->EqSizeMatrix(other)) {
    if (other.ExistMatrix() && this->ExistMatrix()) {
      for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
          matrix_[i][j] = matrix_[i][j] - other.matrix_[i][j];
        }
      }
    }
  } else {
    throw std::out_of_range("Different size of matrix");
  }
}

void S21Matrix::MulNumber(double number) {
  if (this->ExistMatrix()) {
    for (int i = 0; i < rows_; ++i) {
      for (int j = 0; j < cols_; ++j) {
        matrix_[i][j] = matrix_[i][j] * number;
      }
    }
  }
}

void S21Matrix::MulMatrix(const S21Matrix& other) {
  S21Matrix multiplied_matrix(rows_, other.cols_);
  if (cols_ == other.rows_) {
    if (other.ExistMatrix() && this->ExistMatrix()) {
      for (int i = 0; i < this->rows_; ++i) {
        for (int j = 0; j < other.cols_; ++j) {
          for (int k = 0; k < this->cols_; ++k) {
            multiplied_matrix.matrix_[i][j] +=
                matrix_[i][k] * other.matrix_[k][j];
          }
        }
      }
      *this = multiplied_matrix;
    }
  } else {
    throw std::out_of_range(
        "The number of columns of the first matrix does not equal the number "
        "of rows of the second matrix");
  }
}

S21Matrix S21Matrix::Transpose() {
  S21Matrix result(cols_, rows_);
  if (this->ExistMatrix()) {
    for (int i = 0; i < rows_; ++i) {
      for (int j = 0; j < cols_; ++j) {
        result.matrix_[j][i] = matrix_[i][j];
      }
    }
  }
  return result;
}

S21Matrix S21Matrix::CalcComplements() {
  S21Matrix result(rows_, cols_);
  S21Matrix HelpMatrix(rows_ - 1, cols_ - 1);
  double temp_result = 0;
  if (this->rows_ == this->cols_) {
    if (this->ExistMatrix()) {
      for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
          HelpMatrix.ShortCopy(*this, i, j);
          temp_result = HelpMatrix.Determinant();
          result.matrix_[i][j] += pow(-1, i + j) * temp_result;
        }
      }
    }
  } else {
    throw std::out_of_range("The matrix isn't square");
  }
  return result;
}

double S21Matrix::Determinant() {
  double det = 0;
  double help_det = 0;
  if (this->rows_ == this->cols_) {
    if (this->ExistMatrix()) {
      if (rows_ == 1) {
        help_det = this->matrix_[0][0];
      } else if (rows_ == 2) {
        help_det = (this->matrix_[0][0] * this->matrix_[1][1] -
                    this->matrix_[1][0] * this->matrix_[0][1]);
      } else {
        S21Matrix submat(this->rows_ - 1, this->cols_ - 1);
        for (int i = 0; i < cols_; ++i) {
          submat.ShortCopy(*this, 0, i);
          det = submat.Determinant();
          help_det += this->matrix_[0][i] * pow(-1, i) * det;
        }
      }
      det = help_det;
    }
  } else {
    throw std::out_of_range("The matrix isn't square");
  }
  return det;
}

S21Matrix S21Matrix::InverseMatrix() {
  S21Matrix inversed_matrix(this->rows_, this->cols_);
  double determinant = this->Determinant();
  if (determinant != 0) {
    if (this->ExistMatrix() && (this->rows_ == this->cols_)) {
      inversed_matrix = this->CalcComplements();
      inversed_matrix = inversed_matrix.Transpose();
      for (int i = 0; i < this->rows_; ++i) {
        for (int j = 0; j < this->cols_; ++j) {
          inversed_matrix.matrix_[i][j] =
              1 / determinant * inversed_matrix.matrix_[i][j];
        }
      }
    }
  } else {
    throw std::invalid_argument("the Determinant of the matrix is 0");
  }
  return inversed_matrix;
}

// Overloadings opertators

S21Matrix S21Matrix::operator+(const S21Matrix& other) {
  S21Matrix new_matrix(*this);
  new_matrix.SumMatrix(other);
  return new_matrix;
}

S21Matrix S21Matrix::operator-(const S21Matrix& other) {
  S21Matrix new_matrix(*this);
  new_matrix.SubMatrix(other);
  return new_matrix;
}

S21Matrix S21Matrix::operator*(const S21Matrix& other) {
  S21Matrix new_matrix(*this);
  new_matrix.MulMatrix(other);
  return new_matrix;
}

S21Matrix S21Matrix::operator*(double number) {
  S21Matrix new_matrix(*this);
  new_matrix.MulNumber(number);
  return new_matrix;
}

bool S21Matrix::operator==(const S21Matrix& other) {
  return this->EqMatrix(other);
}

S21Matrix S21Matrix::operator=(const S21Matrix& other) {
  if (this != &other) {
    MemoryDeallocating();
    rows_ = other.rows_;
    cols_ = other.cols_;
    matrix_ = MemoryAllocating(rows_, cols_);
    CopyMatrix(other);
  }
  return *this;
}

S21Matrix S21Matrix::operator=(S21Matrix&& other) {
  this->MemoryDeallocating();
  MoveMatrix(other);
  return *this;
}

S21Matrix S21Matrix::operator+=(const S21Matrix& other) {
  this->SumMatrix(other);
  return *this;
}

S21Matrix S21Matrix::operator-=(const S21Matrix& other) {
  this->SubMatrix(other);
  return *this;
}

S21Matrix S21Matrix::operator*=(const S21Matrix& other) {
  this->MulMatrix(other);
  return *this;
}

S21Matrix S21Matrix::operator*=(double number) {
  this->MulNumber(number);
  return *this;
}

double& S21Matrix::operator()(int i, int j) {
  if (rows_ <= i || i < 0 || cols_ <= j || j < 0) {
    throw std::out_of_range("The index out of matrix limit");
  }
  return matrix_[i][j];
}

// Accessors & mutators

int S21Matrix::GetCols() const { return cols_; }

int S21Matrix::GetRows() const { return rows_; }

void S21Matrix::SetCols(int cols) {
  if (this->matrix_) {
    S21Matrix result(rows_, cols);
    for (int i = 0; i < rows_; ++i) {
      for (int j = 0; j < cols_ && j < result.cols_; ++j) {
        result.matrix_[i][j] = matrix_[i][j];
      }
    }
    *this = result;
  }
}

void S21Matrix::SetRows(int rows) {
  if (this->matrix_) {
    S21Matrix result(rows, cols_);
    for (int i = 0; i < rows_ && i < result.rows_; ++i) {
      for (int j = 0; j < cols_; ++j) {
        result.matrix_[i][j] = matrix_[i][j];
      }
    }
    *this = result;
  }
}

// Additional

double** S21Matrix::MemoryAllocating(int rows, int cols) {
  double** allocated_matrix = new double*[rows];
  for (int i = 0; i < rows; ++i) {
    allocated_matrix[i] = new double[cols];
  }
  for (int j = 0; j < rows; ++j) {
    for (int k = 0; k < cols; ++k) {
      allocated_matrix[j][k] = 0;
    }
  }
  return allocated_matrix;
}

void S21Matrix::MemoryDeallocating() {
  if (this->matrix_) {
    for (int i = 0; i < rows_; ++i) {
      delete[] matrix_[i];
    }
    delete[] matrix_;
  }
}

void S21Matrix::CopyMatrix(const S21Matrix& other) {
  for (int i = 0; i < other.rows_; ++i) {
    for (int j = 0; j < other.cols_; ++j) {
      matrix_[i][j] = other.matrix_[i][j];
    }
  }
}

bool S21Matrix::ExistMatrix() const {
  return (rows_ > 0 && cols_ > 0 && matrix_);
}

bool S21Matrix::EqSizeMatrix(const S21Matrix& other) const {
  return (rows_ == other.rows_ && cols_ == other.cols_);
}

void S21Matrix::ShortCopy(const S21Matrix& other, int rows, int cols) {
  for (int i = 0, k = 0; i < other.rows_; ++i) {
    if (i != rows) {
      for (int j = 0, t = 0; j < other.cols_; ++j) {
        if (j != cols) {
          this->matrix_[k][t] = other.matrix_[i][j];
          ++t;
        }
      }
      ++k;
    }
  }
}

void S21Matrix::FillingMatrix() {
  double count = 0;
  for (int i = 0; i < this->GetRows(); ++i) {
    for (int j = 0; j < this->GetCols(); ++j) {
      this->matrix_[i][j] = count;
      count++;
    }
  }
}

void S21Matrix::MoveMatrix(S21Matrix& other) {
  this->rows_ = other.rows_;
  this->cols_ = other.cols_;
  this->matrix_ = other.matrix_;
  other.rows_ = 0;
  other.cols_ = 0;
  other.matrix_ = 0;
}
