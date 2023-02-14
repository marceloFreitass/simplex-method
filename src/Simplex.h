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

    public:

        Simplex(Model model);

        bool testeNegatividade(vector<double> y);
        void solve();
        void setoptSolution();
        void setOptValue();

        void showOptSolution();
        void showOptValue();
        


};
