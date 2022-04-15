#pragma once

#include<iostream>
#include<initializer_list>
namespace schwi {
	constexpr int Dynamic = -1;
	template<typename T, int R, int C>
	class Matrix {
	private:
		int rows = R, cols = C, length;
		T* data;
	public:
		Matrix() {
			length = R * C;
			data = new T[length]();
		}

		Matrix(int r, int c) {
			//if (rows != Dynamic || cols != Dynamic) {
			//	std::cerr << "Matrix is not Dynamic" << std::endl;
			//	return;
			//}
			rows = r;
			cols = c;
			length = r * c;
			data = new T[length]();
		}

		Matrix(const Matrix<T, R, C>& mat) {
			rows = mat.nrow();
			cols = mat.ncol();
			length = rows * cols;
			data = new T[length]();
			for (int i = 0; i < length; i++)
				data[i] = mat[i];
		}

		Matrix(const std::initializer_list<T>& _data) {
			rows = R;
			cols = C;
			length = rows * cols;
			data = new T[length]();
			T* p = data;
			for (T x : _data)
				*(p++) = x;
		}

		~Matrix() {
			if (data != nullptr) {
				delete[] data;
			}
		}

		inline int nrow() const { return rows; }
		inline int ncol() const { return cols; }
		inline int size()const { return length; }

		inline T& operator()(int r, int c) { return data[r * cols + c]; }
		inline const T& operator()(int r, int c) const { return data[r * cols + c]; }
		inline T& operator[](int index) { return data[index]; }
		inline const T& operator[](int index)const { return data[index]; }

		Matrix<T, R, C> col(int c) {
			Matrix<T> ret(rows, 1);
			for (int i = 0; i < rows; i++) {
				ret[i] = data[i * cols + c];
			}
			return ret;
		}

		Matrix<T, R, C> row(int r) {
			Matrix<T> ret(1, cols);
			for (int i = 0; i < cols; i++) {
				ret[i] = data[r * cols + i];
			}
			return ret;
		}

		Matrix<T, R, C> submat(int l, int t, int w, int h) const {
			Matrix<T> ret(w, h);
			for (int i = 0; i < h; i++) {
				for (int j = 0; j < w; j++) {
					ret(i, j) = data[(i + t) * cols + j + l];
				}
			}
			return ret;
		}

		template<typename T1>
		operator Matrix<T1, R, C>() {
			Matrix<T1, R, C> ret(rows, cols);
			for (int i = 0; i < length; i++)
				ret[i] = (T1)data[i];
			return ret;
		}

		Matrix<T, R, C>& operator= (const Matrix<T, R, C>& rhs) {
			delete[]data;
			rows = rhs.nrow();
			cols = rhs.ncol();
			length = rows * cols;
			data = new T[length]();
			for (int i = 0; i < length; i++)
				data[i] = rhs.data[i];
			return *this;
		}

		Matrix<T, R, C> operator+ (const Matrix<T, R, C>& rhs) {
			int r = rhs.nrow();
			int c = rhs.ncol();
			Matrix<T, R, C> ret;
			for (int i = 0; i < rows * cols; i++) {
				ret[i] = data[i] + rhs[i];
			}
			return ret;
		}

		Matrix<T, R, C> operator- (const Matrix<T, R, C>& rhs) {
			int r = rhs.nrow();
			int c = rhs.ncol();
			Matrix<T, R, C> ret;
			for (int i = 0; i < rows * cols; i++) {
				ret[i] = data[i] - rhs[i];
			}
			return ret;
		}

		Matrix<T, R, C> operator* (const Matrix<T, R, C>& rhs) {
			int r = rhs.nrow();
			int c = rhs.ncol();
			Matrix<T, R, C> ret;
			for (int i = 0; i < rows * cols; i++) {
				ret[i] = data[i] * rhs[i];
			}
			return ret;
		}

		Matrix<T, R, C> operator/ (const Matrix<T, R, C>& rhs) {
			int r = rhs.nrow();
			int c = rhs.ncol();
			Matrix<T, R, C> ret;
			for (int i = 0; i < rows * cols; i++) {
				ret[i] = data[i] / rhs[i];
			}
			return ret;
		}

		Matrix<T, R, C>& operator+= (const Matrix<T, R, C>& rhs) {
			for (int i = 0; i < rows * cols; i++) {
				data[i] += rhs[i];
			}
			return *this;
		}

		Matrix<T, R, C>& operator-= (const Matrix<T, R, C>& rhs) {
			for (int i = 0; i < rows * cols; i++) {
				data[i] -= rhs[i];
			}
			return *this;
		}

		Matrix<T, R, C>& operator*= (const Matrix<T, R, C>& rhs) {
			for (int i = 0; i < rows * cols; i++) {
				data[i] *= rhs[i];
			}
			return *this;
		}

		Matrix<T, R, C>& operator/= (const Matrix<T, R, C>& rhs) {
			for (int i = 0; i < rows * cols; i++) {
				data[i] /= rhs[i];
			}
			return *this;
		}

		template <typename T1>
		Matrix<T, R, C> operator+ (T1 v) {
			Matrix<T> ret(rows, cols);
			for (int i = 0; i < rows * cols; i++) {
				ret[i] = data[i] + (T)v;
			}
			return ret;
		}

		template <typename T1>
		Matrix<T, R, C> operator- (T1 v) {
			Matrix<T, R, C> ret(rows, cols);
			for (int i = 0; i < rows * cols; i++) {
				ret[i] = data[i] - (T)v;
			}
			return ret;
		}

		template <typename T1>
		Matrix<T, R, C> operator* (T1 v) {
			Matrix<T, R, C> ret;
			for (int i = 0; i < rows * cols; i++) {
				ret[i] = data[i] * (T)v;
			}
			return ret;
		}

		template <typename T1>
		Matrix<T, R, C> operator/ (T1 v) {
			Matrix<T, R, C> ret;
			for (int i = 0; i < rows * cols; i++) {
				ret[i] = data[i] / (T)v;
			}
			return ret;
		}

		template <typename T1>
		Matrix<T, R, C>& operator+= (T1 v) {
			for (int i = 0; i < rows * cols; i++) {
				data[i] += (T)v;
			}
			return *this;
		}

		template <typename T1>
		Matrix<T, R, C>& operator-= (T1 v) {
			for (int i = 0; i < rows * cols; i++) {
				data[i] -= (T)v;
			}
			return *this;
		}

		template <typename T1>
		Matrix<T, R, C>& operator*= (T1 v) {
			for (int i = 0; i < rows * cols; i++) {
				data[i] *= (T)v;
			}
			return *this;
		}

		template <typename T1>
		Matrix<T, R, C>& operator/= (T1 v) {
			for (int i = 0; i < rows * cols; i++) {
				data[i] /= (T)v;
			}
			return *this;
		}

		Matrix<T, R, C> dot(const Matrix<T, R, C>& mat) {
			int r = this->rows;
			int c = mat.cols;
			Matrix<T, R, C> ret(r, c);
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < cols; j++) {
					for (int k = 0; k < cols; k++) {
						ret(i, j) += data[i * rows + k] * mat(k, j);
					}
				}
			}
			return ret;
		}

		Matrix<T, R, C>& transposed() {
			Matrix<T, R, C> tmp(*this);
			rows = tmp.cols;
			cols = tmp.rows;
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < cols; j++) {
					this->data[i * cols + j] = tmp(j, i);
				}
			}
			return *this;
		}

		Matrix<T, R, C>& transpose() const{
			Matrix<T, R, C> tmp;
			tmp.cols = rows;
			tmp.rows = cols;
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < cols; j++) {
					tmp(j, i) = this->data[i * cols + j];
				}
			}
			return temp;
		}

		double cofactor(int r, int c) {
			Matrix<T, Dynamic, Dynamic> temp(rows - 1, cols - 1);
			int count = 0;
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < cols; j++) {
					if (i == r || j == c)continue;
					temp[count++] = data[i * cols + j];
				}
			}
			return ((r + c) % 2 == 0 ? 1 : -1) * temp.det();
		}

		double det() {
			if (rows != cols)return -1;
			if (rows == 1)return data[0];
			if (rows == 2)return data[0] * data[3] - data[1] * data[2];
			int sum = 0;
			for (int i = 0; i < cols; i++) {
				sum += data[i] * cofactor(0, i);
			}
			return sum;
		}

		Matrix<double, R, C> adjoint() {
			Matrix<double, R, C> temp;
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < cols; j++) {
					temp(i, j) = cofactor(i, j);
				}
			}
			return temp.transpose();
		}

		Matrix<double, R, C> inverse() const{
			if (rows != cols)return *this;
			double d = det();
			if (d == 0)return *this;
			return adjoint() / d;
		}

	};

	template <typename T, int R, int C>
	std::ostream& operator<<(std::ostream& os, const Matrix<T, R, C>& mat) {
		std::cout << mat.nrow() << "*" << mat.ncol() << std::endl;
		for (int i = 0; i < mat.nrow(); i++) {
			for (int j = 0; j < mat.ncol(); j++) {
				std::cout << mat(i, j) << " ";
			}
			std::cout << std::endl;
		}
		return os;
	}

	template<typename T, int R, int C>
	bool operator==(const Matrix<T, R, C>& m1, const Matrix<T, R, C>& m2) {
		for (int i = 0; i < length; i++) {
			if (m1[i] != m2[i])return false;
		}
		return true;
	}

	template<typename T, int R, int C>
	bool operator!=(const Matrix<T, R, C>& m1, const Matrix<T, R, C>& m2) {
		return !m1 == m2;
	}

	using Matrix3d = Matrix<double, 3, 3>;
	using Matrix4d = Matrix<double, 4, 4>;

}