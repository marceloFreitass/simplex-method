#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Model{

    private:

        char type;

        vector<double> c;
        vector<double> b;
        vector<vector<double>> A;
        vector<string> relations;

        vector<int> basics;
        vector<int> notBasics;
        vector<vector<double>> reversedB;
        vector<double> xB;
        vector<double> y;
        vector<double> p;
        vector<double> relativeCosts;

        int sK, xL;
        bool needTwoPhase;
        bool inTwoPhase;
        bool outTwoPhase;


    public:

        Model();
        Model(char type);
        bool checkNeedTwoPhase();

        void setNeedTwoPhase(bool need); //Set if needs bigM or not
        void setInTwoPhase(bool in);
        void setOutTwoPhase(bool out);
        void setObjFunc(vector<double> c);
        void addConstraint(vector<double> a, string relation, double b);

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

        bool getNeedTwoPhase();
        bool getInTwoPhase();
        bool getOutTwoPhase();
        vector<string> getRelations();
        vector<vector<double>> getA();
        vector<double> getB();
        vector<double> getC();
        vector<double> getY();
        vector<int> getBasics();
        vector<int> getNotBasics();
        vector<double> getXB();
        char getType();

        //Ajeitar as funçoes de print
        void showObjFunc();
        void showConstraints();
        void showReverseMatrix();

};