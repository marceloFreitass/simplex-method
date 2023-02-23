#include <iostream>
#include <vector>
#include "Simplex.h"

using namespace std;

int main(){

	Model modelo('-');
	vector<double> c{2, 1};
	modelo.setObjFunc(c);
	vector<double> a{-2, 3};
	modelo.addConstraint(a, ">=", 9);
	a = {3, 2};
	modelo.addConstraint(a, ">=", 12);
	

	Simplex simplex1(modelo);
	simplex1.solve(modelo);
	simplex1.showOptValue();
	simplex1.showOptSolution();
	//modelo.showObjFunc(); */
	
	//

	//Fazer todas as modificaçoes necessarias para a forma padrao	
	
	return 0;

}
