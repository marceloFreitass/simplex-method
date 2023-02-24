#include <iostream>
#include <vector>
#include "Simplex.h"

using namespace std;

int main(){

	Model modelo('+');
	vector<double> c{6, 1};
	modelo.setObjFunc(c);
	vector<double> a{-1, 3};
	modelo.addConstraint(a, "<=", 6);
	a = {1, -3};
	modelo.addConstraint(a, "=", 6);
	a = {1, 1};
	modelo.addConstraint(a, ">=", 1);
	
	Simplex simplex1(modelo);
	simplex1.solve(modelo);	
	simplex1.showOptValue();
	simplex1.showOptSolution();
	//modelo.showObjFunc(); */
	
	//

	//Fazer todas as modificaçoes necessarias para a forma padrao	
	
	return 0;

}
