#include<core/schwi.h>
#include<image/image.h>
#include<math/matrix.h>

using namespace schwi;
using namespace std;

int main() {
	std::cout << "Test" << std::endl;
	Matrix2d A({1,2,3,4});
	A.info();
	cout << A;
	Matrix2d B({ 3,4,8,9 });
	cout << A .dot( B);
	Matrix<double,2,3> C({ 1,2,3,4,5,6});
}