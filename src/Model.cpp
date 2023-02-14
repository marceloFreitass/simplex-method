#include "Model.h"
#include <iostream>
#include <vector>

using namespace std;




Model::Model(){

    vector<double> c; //Fazer com uma estrutura, especificando se é minimo ou maximo
    vector<double> b;
    vector<vector<double>> A;
    double optValue = 0;

    vector<int> basics;
    vector<int> notBasics;
    vector<vector<double>> reversedB;
    vector<double> xB;
    vector<double> y;
    vector<double> p;
    vector<double> relativeCosts;

};

void Model::setObjFunc(vector<double> c){this->c = c;}
void Model::addConstraint(vector<double> a, double b1){

    this->A.push_back(a);
    this->b.push_back(b1);
}

void Model::setReversedMatrix(){

    int qConstraints = this->b.size();
    
    vector<vector<double>> reversedB(qConstraints, vector<double>(qConstraints*2));
    vector<vector<double>> reversedBcopy(qConstraints, vector<double>(qConstraints*2));
    
    
    for(int i = 0; i < this->basics.size(); i++){

        for(int j = 0; j < qConstraints; j++){
            reversedBcopy[i][j] = A[j][basics[i] - 1];
        }
        
    }
    
    
    for(int i = 0; i < qConstraints; i++){ //Transposing
        for(int j = 0; j < qConstraints; j++){
            reversedB[i][j] = reversedBcopy[j][i];
            
        }

    }

    for(int i = 0; i < qConstraints; i++){ //Putting the identity matrix of qConstraints order
        for(int j = 0; j < qConstraints; j++){
            if(i == j){
                reversedB[i][j + qConstraints] = 1;
            }
            else{

                reversedB[i][j + qConstraints] = 0;
            }
        }
    }

    //Gauss Jourdan Elimination
    for(int i = 0; i < qConstraints; i++){

        if(reversedB[i][i] == 0.0){

            cout << "Erro" << endl;
            exit(0);
        }

        for(int j = 0; j < qConstraints; j++){

            if(i != j){

                double ratio = reversedB[j][i]/reversedB[i][i];

                for(int k = 0; k < qConstraints*2; k++){

                    reversedB[j][k] -= ratio*reversedB[i][k];
                }
            }
        }
    }
    //Principal diagonal to 1
    for(int i = 0; i < qConstraints; i++){

        for(int j = qConstraints; j < qConstraints*2; j++){

            reversedB[i][j] = reversedB[i][j]/reversedB[i][i];
        }
    }

    for(int i = 0; i < qConstraints; i++){
        for(int j = 0; j < qConstraints; j++){

            reversedB[i][j] = reversedB[i][j + qConstraints];
        }
    }
    

    
    reversedB.resize(qConstraints, vector<double>(qConstraints));
    
    this->reversedB = reversedB;
    
    
    
}

void Model::showReverseMatrix(){
    int qConstraints = this->b.size();
    for(int i = 0; i < qConstraints; i++){
        for(int j = 0; j < qConstraints; j++){
            cout << this->reversedB[i][j] << " ";
        }
        cout << endl;
    }
}

void Model::setBasics(vector<int> basics){this->basics = basics;}
void Model::setNotBasics(vector<int> notBasics){this->notBasics = notBasics;}

void Model::setxB(){

    int qConstraints = this->b.size();
    vector<double> xB;

    for(int i = 0; i < qConstraints; i++){

        double sum = 0;

        for(int j = 0; j < qConstraints; j++){
            
            sum += this->reversedB[i][j]*this->b[j];
        }
        xB.push_back(sum);
    }

    for(int i = 0; i < xB.size(); i++){
        cout << xB[i] << " ";
    }
    this->xB = xB;
}

void Model::setP(){

    int qConstraints = this->b.size();
    this->p.clear();
    for(int i = 0; i < qConstraints; i++){ 

        this->p.push_back(this->c[basics[i] - 1]);
        
    }

    cout << endl;
    vector<double> pt;
    for(int i = 0; i < qConstraints; i++){
        double sum = 0;

        for(int j = 0; j < qConstraints; j++){

            sum += this->reversedB[j][i]*this->p[j];
        }
         pt.push_back(sum);
    }

    this->p = pt;
}

void Model::calcRelativeCosts(){
    
    this->relativeCosts.clear();
    int notBasicsSize = this->notBasics.size();
    int qConstraints = this->b.size();
    for(int i = 0; i < notBasicsSize; i++){
        double relativeCost = this->c[this->notBasics[i] - 1];
        for(int j = 0; j < qConstraints; j++){

            relativeCost -= this->p[j] * this->A[j][this->notBasics[i] - 1];
        }
        cout << "Relative cost: " << relativeCost << endl; 
        this->relativeCosts.push_back(relativeCost);
    }

}

void Model::smallerRelativeCost(){

    int notBasicsSize = this->notBasics.size();
    double smaller;
    int smallerI;
    for(int i = 0; i < notBasicsSize; i++){
        if(i == 0){
            smaller = this->relativeCosts[i];
            smallerI = i;
            continue;
        }
        if(this->relativeCosts[i] < smaller){

            smaller = this->relativeCosts[i];
            smallerI = i;
        }
    }
    this->sK = smallerI;
};

int Model::checkRelativeCost(){

    if(this->relativeCosts[this->sK] < 0)
        return 0;
    if(this->relativeCosts[this->sK] == 0)
        return 1;
    
    return 2;
}

void Model::setY(){

    int notBasicsSize = this->notBasics.size();
    int qConstraints = this->b.size();
    
    this->y.clear();
    vector<double> y1;
    for(int i = 0; i < qConstraints; i++){
        y.push_back(this->A[i][this->sK]);
    }

    for(int i = 0; i < qConstraints; i++){
        double sum = 0;

        for(int j = 0; j < qConstraints; j++){
            sum += y[j]*this->reversedB[i][j];
        }
        y1.push_back(sum);
    }
    this->y = y1;

    cout << "Y: ";
    for(int i = 0; i < y.size(); i++){
        cout << y[i] << " ";
    }
    cout << endl;
}

void Model::ratioTest(){

    double smaller;
    int smallerI;
    int qConstraints = this->b.size();
    double ratio;
    for(int i = 0; i < qConstraints; i++){

        ratio = this->xB[i]/this->y[i];
        cout << ratio << endl;
        if(ratio < 0){
            ratio = 999999999;
            
        }
        if(i == 0){  
            smaller = ratio;
            smallerI = i;
            continue;
        }
        if(ratio < smaller){
            smaller = ratio;
            smallerI = i;
        }
        
    }
    this->xL = smallerI;

}

void Model::basisChange(){
    swap(this->notBasics[this->sK], this->basics[this->xL]);
    
    for(int i = 0; i < this->basics.size(); i++){
        cout << basics[i] << " ";
    }
    cout << endl;

    for(int i = 0; i < this->notBasics.size(); i++){
        cout << notBasics[i] << " ";
    }
    cout << endl;
}

int Model::getSize(){return this->c.size();}
vector<double> Model::getC(){return this->c;}
vector<double> Model::getY(){return this->y;}
vector<int> Model::getBasics(){return this->basics;}
vector<int> Model::getNotBasics(){return this->notBasics;}
vector<double> Model::getXB(){return this->xB;}
void Model::showObjFunc(){

    cout << "OF: -min "; //Ajeitar
    for(int i = 0; i < this->c.size(); i++){
        
        if(c[i] != 0){

            cout << c[i] << "X" << i + 1 << " ";
        }
    }
    cout << endl;
}