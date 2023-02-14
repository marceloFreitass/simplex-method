#include <iostream>
#include <vector>
#include "Simplex.h"

Simplex::Simplex(Model model){
    this->model = model;
}

bool Simplex::testeNegatividade(vector<double> y){

    int n = y.size();
    int i;
    for(i = 0; i < n; i++){
        if(y[i] > 0){
            break;
        }

    }
    return i == n;
}

void Simplex::solve(){

    vector<int> basics;
    vector<int> notBasics;
    vector<double> c = this->model.getC();
    int first = 1;
    bool flag = 1;
    int size = this->model.getSize();
    for(int i = 0; i < size; i++){
        if(c[i] != 0){
            notBasics.push_back(i+1);
        }
        else{
            basics.push_back(i+1);
        }
    }

    this->model.setBasics(basics);
    this->model.setNotBasics(notBasics);
    vector<double> y;
    int i;
    while(flag){

        this->model.setReversedMatrix();
        this->model.setxB();
        this->model.setP();
        this->model.calcRelativeCosts();
        this->model.smallerRelativeCost();
        flag = (this->model.checkRelativeCost() == 0);
        if(!flag){
            break;
        }
        this->model.setY();
        if(testeNegatividade(model.getY())){
            cout << "Problema ilimitado." << endl;
            break;
        }
        
        this->model.ratioTest();
        this->model.basisChange();
        
    }
    setoptSolution();   
}

void Simplex::setoptSolution(){

    vector<int> basics = this->model.getBasics();
    vector<int> notBasics = this->model.getNotBasics();
    vector<double> xB = this->model.getXB();
    bool flag = 1;
    int qVariables = this->model.getC().size();
    this->optSolution.clear();
    for(int i = 0; i < qVariables; i++){
        flag = 0;
        for(int j = 0; j < qVariables; j++){

            if(i+1 == basics[j]){
                this->optSolution.push_back(xB[j]);
                flag = 1;      
            }
        }
        if(!flag){
            this->optSolution.push_back(0);
        }
    }
    setOptValue();
}

void Simplex::setOptValue(){

    vector<double> c = this->model.getC();
    int variables = c.size();
    this->optValue = 0;
    for(int i = 0; i < variables; i++){
        this->optValue+= c[i]*this->optSolution[i];
    }
    cout << this->optValue << endl;
}

void Simplex::showOptSolution(){

    int size = this->optSolution.size();

    cout << "Optimal Solution: [ ";
    for(int i = 0; i < size; i++){
        cout << optSolution[i] << " ";
    }
    cout << "]\n";
}

void Simplex::showOptValue(){cout << "Optimal value: " << this->optValue << endl;}
