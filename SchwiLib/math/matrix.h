#pragma once

#ifndef NDEBUG
#define NDEBUG
#endif

#include"math.h"

//TODO:修复Dynamic问题
//修复前不应使用Dynamic相关函数
namespace schwi {
	template<typename T, int R, int C>
	class Matrix {
	private:
		T* data;
	public:
		Matrix(){
			data = new T[R*C]();
		}

		Matrix(int r, int c){
			if (R != Dynamic || C != Dynamic) {
				std::cerr << "Matrix is not Dynamic" << std::endl;
				exit(1);
			}
			data = new T[R*C]();
		}

		Matrix(const Matrix<T, R, C>& mat){
			data = new T[R*C]();
			for (int i = 0; i < R*C; i++)
				data[i] = mat[i];
		}

		Matrix(const std::initializer_list<T>& _data){
			data = new T[R*C]();
			T* p = data;
			for (T x : _data)
				*(p++) = x;
		}

		~Matrix() {
			if (data != nullptr) {
				delete[] data;
			}
		}

		inline int nrow() const { return R; }
		inline int ncol() const { return C; }
		inline int size()const { return R*C; }

		inline T& operator()(int r, int c) { return data[r * C + c]; }
		inline const T& operator()(int r, int c) const { return data[r * C + c]; }
		inline T& operator[](int index) { return data[index]; }
		inline const T& operator[](int index)const { return data[index]; }

		Matrix<T, R, C> col(int c) {
			Matrix<T> ret(R, 1);
			for (int i = 0; i < R; i++) {
				ret[i] = data[i * C + c];
			}
			return ret;
		}

		Matrix<T, R, C> row(int r) {
			Matrix<T> ret(1, C);
			for (int i = 0; i < C; i++) {
				ret[i] = data[r * C + i];
			}
			return ret;
		}

		Matrix<T, Dynamic, Dynamic> submat(int t, int l, int h, int w) const {
			Matrix<T,Dynamic,Dynamic> ret(h,w);
			for (int i = 0; i < h; i++) {
				for (int j = 0; j < w; j++) {
					ret(i, j) = data[(i + t) * C + j + l];
				}
			}
			return ret;
		}

		template<typename T1>
		operator Matrix<T1, R, C>() {
			Matrix<T1, R, C> ret(R, C);
			for (int i = 0; i < R*C; i++)
				ret[i] = (T1)data[i];
			return ret;
		}

		Matrix<T, R, C>& operator= (const Matrix<T, R, C>& rhs) {
			delete[]data;
			data = new T[R*C]();
			for (int i = 0; i < R*C; i++)
				data[i] = rhs.data[i];
			return *this;
		}

		Matrix<T, R, C> operator+ (const Matrix<T, R, C>& rhs) {
			Matrix<T, R, C> ret;
			for (int i = 0; i < R * C; i++) {
				ret[i] = data[i] + rhs[i];
			}
			return ret;
		}

		Matrix<T, R, C> operator- (const Matrix<T, R, C>& rhs) {
			Matrix<T, R, C> ret;
			for (int i = 0; i < R * C; i++) {
				ret[i] = data[i] - rhs[i];
			}
			return ret;
		}

		Matrix<T, R, C> operator* (const Matrix<T, R, C>& rhs) {
			Matrix<T, R, C> ret;
			for (int i = 0; i < R * C; i++) {
				ret[i] = data[i] * rhs[i];
			}
			return ret;
		}

		Matrix<T, R, C> operator/ (const Matrix<T, R, C>& rhs) {
			int r = rhs.nrow();
			int c = rhs.ncol();
			Matrix<T, R, C> ret;
			for (int i = 0; i < R * C; i++) {
				ret[i] = data[i] / rhs[i];
			}
			return ret;
		}

		Matrix<T, R, C>& operator+= (const Matrix<T, R, C>& rhs) {
			for (int i = 0; i < R * C; i++) {
				data[i] += rhs[i];
			}
			return *this;
		}

		Matrix<T, R, C>& operator-= (const Matrix<T, R, C>& rhs) {
			for (int i = 0; i < R * C; i++) {
				data[i] -= rhs[i];
			}
			return *this;
		}

		Matrix<T, R, C>& operator*= (const Matrix<T, R, C>& rhs) {
			for (int i = 0; i < R * C; i++) {
				data[i] *= rhs[i];
			}
			return *this;
		}

		Matrix<T, R, C>& operator/= (const Matrix<T, R, C>& rhs) {
			for (int i = 0; i < R * C; i++) {
				data[i] /= rhs[i];
			}
			return *this;
		}

		template <typename T1>
		Matrix<T, R, C> operator+ (T1 v) {
			Matrix<T> ret(R, C);
			for (int i = 0; i < R * C; i++) {
				ret[i] = data[i] + (T)v;
			}
			return ret;
		}

		template <typename T1>
		Matrix<T, R, C> operator- (T1 v) {
			Matrix<T, R, C> ret(R, C);
			for (int i = 0; i < R * C; i++) {
				ret[i] = data[i] - (T)v;
			}
			return ret;
		}

		template <typename T1>
		Matrix<T, R, C> operator* (T1 v) {
			Matrix<T, R, C> ret;
			for (int i = 0; i < R * C; i++) {
				ret[i] = data[i] * (T)v;
			}
			return ret;
		}

		template <typename T1>
		Matrix<T, R, C> operator/ (T1 v) {
			Matrix<T, R, C> ret;
			for (int i = 0; i < R * C; i++) {
				ret[i] = data[i] / (T)v;
			}
			return ret;
		}

		template <typename T1>
		Matrix<T, R, C>& operator+= (T1 v) {
			for (int i = 0; i < R * C; i++) {
				data[i] += (T)v;
			}
			return *this;
		}

		template <typename T1>
		Matrix<T, R, C>& operator-= (T1 v) {
			for (int i = 0; i < R * C; i++) {
				data[i] -= (T)v;
			}
			return *this;
		}

		template <typename T1>
		Matrix<T, R, C>& operator*= (T1 v) {
			for (int i = 0; i < R * C; i++) {
				data[i] *= (T)v;
			}
			return *this;
		}

		template <typename T1>
		Matrix<T, R, C>& operator/= (T1 v) {
			for (int i = 0; i < R * C; i++) {
				data[i] /= (T)v;
			}
			return *this;
		}

		template<int R1,int C1>
		Matrix<T, R, C1> dot(const Matrix<T, R1, C1>& mat) {
			if (C != R1) {
				std::cerr << "Matrix dimensions do not match" << std::endl;
				exit(1);
			}
			Matrix<T, R, C1> ret;
			for (int i = 0; i < R; i++) {
				for (int j = 0; j < C1; j++) {
					for (int k = 0; k < C; k++) {
						ret(i, j) += data[i * R + k] * mat(k, j);
					}
				}
			}
			return ret;
		}

		Matrix<T, C, R>& transposed() {
			Matrix<T, R, C> tmp(*this);
			for (int i = 0; i < R; i++) {
				for (int j = 0; j < C; j++) {
					this->data[i * C + j] = tmp(j, i);
				}
			}
			return *this;
		}

		Matrix<T, C, R>& transpose() const{
			Matrix<T, C, R> tmp;
			for (int i = 0; i < R; i++) {
				for (int j = 0; j < C; j++) {
					tmp(j, i) = this->data[i * C + j];
				}
			}
			return tmp;
		}

		double cofactor(int r, int c) {
			Matrix<T, Dynamic, Dynamic> temp(R - 1, C - 1);
			int count = 0;
			for (int i = 0; i < R; i++) {
				for (int j = 0; j < C; j++) {
					if (i == r || j == c)continue;
					temp[count++] = data[i * C + j];
				}
			}
			return ((r + c) % 2 == 0 ? 1 : -1) * temp.det();
		}

		double det() {
			if (R != C)exit(1);
			if (R == 1)return data[0];
			if (R == 2)return data[0] * data[3] - data[1] * data[2];
			int sum = 0;
			for (int i = 0; i < C; i++) {
				sum += data[i] * cofactor(0, i);
			}
			return sum;
		}

		Matrix<double, R, C> adjoint() {
			Matrix<double, R, C> temp;
			for (int i = 0; i < R; i++) {
				for (int j = 0; j < C; j++) {
					temp(i, j) = cofactor(i, j);
				}
			}
			return temp.transpose();
		}

		Matrix<double, R, C> inverse() const{
			if (R != C)return *this;
			double d = det();
			if (d == 0)return *this;
			return adjoint() / d;
		}

		void info() {
			std::cout <<typeid(T).name()<<" " << R << "*" << C << std::endl;
		}
	};

	template<typename T, int R, int C>
	std::ostream& operator<<(std::ostream& os, const Matrix<T, R, C>& mat) {
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
		for (int i = 0; i < R*C; i++) {
			if (m1[i] != m2[i])return false;
		}
		return true;
	}

	template<typename T, int R, int C>
	bool operator!=(const Matrix<T, R, C>& m1, const Matrix<T, R, C>& m2) {
		return !m1 == m2;
	}
}