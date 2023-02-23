#include <iostream>
#include <vector>
#include "Model.h"

using namespace std;

class Simplex{

    private:

        Model model;
        double optValue;
        vector<double> optSolution;
        vector<double> solution;
        
        
        int cGE; //Greater equal count (-1)

    public:

        Model formaPadrao(Model* model);

        void twoPhase();
        Simplex(Model model);


        bool testeNegatividade(vector<double> y);
        void solve(Model& model);
        void setoptSolution();
        void setOptValue();

        void showOptSolution();
        void showOptValue();
        


};
