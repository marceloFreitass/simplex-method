#include <iostream>
#include <vector>

using namespace std;

class Model{

    private:
    
        vector<double> c;
        vector<double> b;
        vector<vector<double>> A;

        vector<int> basics;
        vector<int> notBasics;
        vector<vector<double>> reversedB;
        vector<double> xB;
        vector<double> y;
        vector<double> p;
        vector<double> relativeCosts;

        int sK, xL;


    public:

        Model();

        void setObjFunc(vector<double> c);
        void addConstraint(vector<double> a, double b);

        void setBasics(vector<int> basics);
        void setNotBasics(vector<int> notBasics);
        void setReversedMatrix();
        void setxB();
        void setP();
        void calcRelativeCosts();
        void smallerRelativeCost();
        int checkRelativeCost();
        void setY();
        void ratioTest();
        void basisChange();

        int getSize();
        vector<double> getC();
        vector<double> getY();
        vector<int> getBasics();
        vector<int> getNotBasics();
        vector<double> getXB();
        //Ajeitar as funçoes de print
        void showObjFunc();
        void showConstraints();
        void showReverseMatrix();

};