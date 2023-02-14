#include <iostream>
#include <vector>
#include "Simplex.h"

using namespace std;

int main(){

	Model modelo;
	vector<double> c{-1, -2, 0, 0, 0};
	modelo.setObjFunc(c);
	vector<double> a{1, 1, 1, 0, 0};
	modelo.addConstraint(a, 6);
	a = {1, -1, 0, 1, 0};
	modelo.addConstraint(a, 4);
	a = {-1, 1, 0, 0, 1};
	modelo.addConstraint(a, 4);
	Simplex simplex1(modelo);
	simplex1.solve();
	simplex1.showOptSolution();
	simplex1.show

	//Fazer todas as modificaçoes necessarias para a forma padrao	
	
	return 0;

}
