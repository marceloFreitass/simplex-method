#include <iostream>
#include <vector>
#include "Simplex.h"


struct slackSurplus{
    int row;
    int value;
};

Simplex::Simplex(Model& initialModel){
    Model teste;

    double optValue = 0;
    vector<double> optSolution;
    vector<double> solution;

    cout << "Carregando o modelo..\n\n";
    bool need = initialModel.checkNeedTwoPhase();
    this->cGE = 0;
    initialModel.setNeedTwoPhase(need);
    if(!initialModel.getInTwoPhase()){
        cout << "Colocando em forma padrao\n\n";
        formaPadrao(initialModel);
    }
    // else{
        
    //     this->initialModel = initialModel;
    //      //Checar pq o initialModelo ta diferente
    // }

    teste = initialModel;

    //Duas fases
}

void Simplex::twoPhase(Model& model){

    cout << "\nPrimeira fase\n";
    Model newModel('-');
    newModel.setNeedTwoPhase(0);
    newModel.setInTwoPhase(1);
    vector<vector<double>> A = this->model.getA();
    vector<double> B = this->model.getB();

    for(int i = 0; i < A.size(); i++){ 
        A[i].resize(A[i].size() + cGE);
    }

    for(int i = 0; i < this->cGE; i++){
        A[i][A[i].size() -cGE + i] = 1;
        newModel.addConstraint(A[i], "=", B[i]);
    }
    vector<double> c;
    for(int i = 0; i < A[0].size(); i++){
        if(i >= A[0].size() - cGE){
            c.push_back(1);
            continue;
        }
        c.push_back(0);
    }

    newModel.setObjFunc(c);
    solve(newModel); //Modelo original pegar os nao basicos e basicos
    
    vector<int> notBasics = newModel.getNotBasics();
    int notBasicsSize = notBasics.size();
    int size = newModel.getSize();
    c = newModel.getC();
    
    vector<int> indexes; //Saber onde estao as variaveis artificiais na funcao de custo
    int costCount = 0;
    for(int i = 0; i < size; i++){
        if(c[i] == 1){
            costCount++;
            indexes.push_back(i);
        }
    }

    int found = 0;
    for(int i = 0; i < costCount; i++){
        for(int j = 0; j < notBasicsSize - found; j++){
            if(indexes[i] + 1 == notBasics[j]){
                found++;
                notBasics.erase(notBasics.begin() + j);
            }
        }
    }



    A = model.getA();

    
    
    model.setNeedTwoPhase(0);
    model.setInTwoPhase(0);
    model.setOutTwoPhase(1);
    model.setBasics(newModel.getBasics());
    model.setNotBasics(notBasics);

    cout << "Segunda fase\n\n";

    //solve(model);
    //return;

    
    //Executar de novo, e acompanhar onde ta o erro (depois de setar o Y)

}
Model Simplex::formaPadrao(Model* model){
    
    char type = model->getType();

    Model newModel(type);
    newModel.setNeedTwoPhase(model->getNeedTwoPhase());


    vector<double> c = model->getC();
    int Csize = c.size();

    if(type == '+'){
        for(int i = 0; i < Csize; i++){
            c[i] *= -1;
        }
    }

    vector<double> b = model->getB();
    vector<vector<double>> A = model->getA();
    vector<string> relations = model->getRelations();

    int constrSize = b.size();
    int numVariables = 0;

    vector<slackSurplus> newVariables;

    for(int i = 0; i < constrSize; i++){
        if(relations[i] == "<="){
            newVariables.push_back({i, 1});
            numVariables++;
        }
        else if(relations[i] == ">="){
            newVariables.push_back({i, -1});
            numVariables++;
            this->cGE++;
        }
        relations[i] = "=";
    }
    numVariables += Csize;
    cout << A.size() << endl;
    A.resize(constrSize);
    cout << A.size() << endl;
    for(int i = 0; i < numVariables; i++){ 
        A[i].resize(numVariables);
    }
    cout << "Matrix " << constrSize << "x" << numVariables << endl;
    for(int i = 0; i < A.size(); i++){
        for(int j = 0; j < A[i].size(); j++){
            cout << A[i][j] << " ";
        }
        cout << endl;
    }
    newModel.setA(A);

    int numNewVariables = newVariables.size();

    for(int i = 0; i < numNewVariables; i++){
        
        A[newVariables[i].row][Csize+i] = newVariables[i].value;
        //newModel.addConstraint(A[i], relations[i], b[i]);
        c.push_back(0); //Adding on the obj func
    }
    for(int i = 0; i < A.size(); i++){
        for(int j = 0; j < A[i].size(); j++){
            cout << A[i][j] << " ";
        } 
        cout << endl;
    }

    newModel.setObjFunc(c);
    newModel.setA(A);
    cout << "Travou aqui\n";
    A = newModel.getA();

    for(int i = 0; i < A.size(); i++){
        cout << "I: " << i << endl;
        for(int j = 0; j < A[i].size(); j++){
            cout << A[i][j] << " ";
        }
        cout << endl;
    }
    cout << "N passou\n";
    cout << "Passou\n";

    return newModel;
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

void Simplex::solve(Model& model){

    vector<int> basics;
    vector<int> notBasics;  
    vector<double> c = model.getC();
    int first = 1;
    bool flag = 1;
    int size = model.getSize();
    if(model.getNeedTwoPhase()){
        twoPhase(model);
    }
    else if(model.getInTwoPhase()){
        for(int i = 0; i < size; i++){
            if(c[i] == 0){
                notBasics.push_back(i + 1);
            }
            else{
                basics.push_back(i+1);
            }
        }
        model.setBasics(basics);
        model.setNotBasics(notBasics);
    }
    else if(!model.getOutTwoPhase()){
        cout << "C" << endl;
        for(int i = 0; i < size; i++){
            if(c[i] != 0){
                notBasics.push_back(i+1);
            }
            else{
                basics.push_back(i+1);
            }
        }
        model.setBasics(basics);
        model.setNotBasics(notBasics);
    }
    
    vector<vector<double>> A = model.getA();
    
    cout << endl;
    vector<double> y;
    int i;

    vector<int> indexes; //Saber onde estao as variaveis artificiais na funcao de custo
    int costCount = 0;
    for(int i = 0; i < size; i++){
        if(c[i] == 1){
            costCount++;
            indexes.push_back(i);
        }
    }
    
    while(flag){
        
        if(!flag){
            break;
        }
        
        model.setReversedMatrix();
        
        model.setxB();
        model.setP();
        model.calcRelativeCosts();
        model.smallerRelativeCost();
        flag = (model.checkRelativeCost() == 0);
        if(!flag){
            break;
        }
        model.setY();
        if(testeNegatividade(model.getY())){
            cout << "Problema ilimitado." << endl;
            break;
        }
        
        model.ratioTest();
        model.basisChange();
        
        if(model.getInTwoPhase()){

            vector<int> notBasics = model.getNotBasics();
            int notBasicsSize = notBasics.size();
            int found = 0;
            for(int i = 0; i < costCount; i++){
                for(int j = 0; j < notBasicsSize; j++){
                    if(indexes[i] + 1 == notBasics[j]){
                        found++;
                    }
                }
            }
            if(found == costCount){
                return;
            }            
        }
    }
    this->model = model;
    //return;
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
    if(this->model.getType() == '+'){
        this->optValue *= -1;
    }
    cout << "Opt.Value: " << this->optValue << endl;
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
