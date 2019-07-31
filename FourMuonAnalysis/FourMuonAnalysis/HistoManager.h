#ifndef CxAOD__HistoManager_H
#define CxAOD__HistoManager_H

#include <map>
#include <vector>

#include "TH1.h"
#include "TH1F.h"
#include "TH2.h"
#include "TH2F.h"
#include "TTree.h"
#include "TMacro.h"

using namespace std;

//<struct for variables>
struct VarConfig{
  int NBins;
  float Xmin;
  float Xmax;
  float Option;
  map<string, int> CutStep;
  int Vector;
  int Dim;
  int Type;

  VarConfig() {
    Option=0;
    CutStep.clear();
    Vector=0;
    Dim=0;
    Type=0;
  }
};

//<Map type STRING:DOUBLE...>
typedef map<string, int> MapType_Int;
typedef map<string, float> MapType_Float;
typedef map<string, double> MapType_Double;
typedef map<string, vector<string> > MapType_VString;
typedef map<string, vector<double> > MapType_VDouble;
typedef map<string, vector<double>* > MapType_VDoubleStar;
typedef map<string, VarConfig >  MapType_VarConfig;
typedef map<string, map<string, VarConfig > > MapType2_VarConfig;
typedef map<string, map<string, int > > MapType2_Int;
typedef map<string, map<string, float > > MapType2_Float;

typedef map<string, TH1F*> MapType_TH1F;
typedef map<string, map<string,TH1F*> > MapType2_TH1F;
typedef map<string, map<string,map<string,TH1F*> > > MapType3_TH1F;

typedef map<string, TH2F*> MapType_TH2F;
typedef map<string, map<string,TH2F*> > MapType2_TH2F;
typedef map<string, map<string,map<string,TH2F*> > > MapType3_TH2F;

typedef map<string, string> MapType_String;
typedef map<string, vector<string> > MapType_VString;
typedef map<string, vector<string>* > MapType_VStringStar;

//<Options for Variables to determine using which event weight to fill into histograms>
//<Both means both histograms and trees>
enum {InBoth,InTreeOnly,InHistoOnly};

class HistoManager {
    public:
        HistoManager();
        virtual ~HistoManager();

        void InitStrVec(vector<string>& out, string in, string de);
        void InitVar(string varlist, int nbin, double xmin, double xmax, int vec=0, int option=InBoth, int type=0);
        void InitVar(string varlist, int nbin, double xmin, double xmax, string catlist, string cutstep, int vec, int option, int type=0);
        void Init2DHelper(string var1, int nbinx, double xmin, double xmax, string var2, int nbiny, double ymin, double ymax, int vec, int option);
        void Init2DHelper(string var1, int nbinx, double xmin, double xmax, string var2, int nbiny, double ymin, double ymax, string catlist, string cutstep,  int vec, int option);

        void InitCutflow(string name, string cutlist);
        void ClearVariables();
        void ClearVariables(MapType_Double& map);
        void ClearVariables(MapType_VDoubleStar& map);
        void ClearVariables(MapType_VStringStar& map);
        void ClearVariables(MapType_String& map);
        void ClearCutflowFlag();
        void ClearCutflowFlag(std::string name);
        void ClearCutflowCount();
        void CreateHistoMap();
        void FillCutflow();
        void FillCutflow(std::string name);
        void FillCutflowFlag(std::string name);
        void FillHistograms();
        void FillTree();
        void PrintCutflow();

        void AddVarIntoTree();

        std::vector<TH1*> getHistos();
        TTree* getTree();
        TMacro* getCutflow() {return log_cutflow;}

        //Event weight
        float Evt_weight;//!

        //MapType2_Double VarName; 
        MapType_VarConfig VarName;//!
        //"Var1_Var2" : "Var1"{1,xbins,xmin,xmax},"Var2"{2,ybins,ymin,ymax}
        MapType_Double Var;//!
        MapType_VDoubleStar VVar;//!
        MapType_String VStr; //!
        MapType_VStringStar VVStr;//!
        MapType2_VarConfig helper_2D; //!

        //Cutflow
        MapType2_Int Cutflow_flag_tmp, Cutflow_flag; //!
        MapType2_Float Cutflow_count; //!
        std::vector<std::string> Cutflow_names; //!
        MapType_VString Cutflow_cuts; //!

        MapType_TH1F histoMap;//!
        MapType_TH2F histoMap2D;//!
        MapType3_TH1F histoMap_CutStep;//!
        MapType3_TH2F histoMap2D_CutStep;//!

        TTree *myTree;//!
        TMacro *log_cutflow;
};

#endif
