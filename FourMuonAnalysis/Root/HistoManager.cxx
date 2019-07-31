#include <iostream>
#include <sstream>
#include <stdlib.h>

#include "FourMuonAnalysis/HistoManager.h"

using namespace std;

HistoManager::HistoManager():
    myTree(nullptr),
    log_cutflow(nullptr), 
    Evt_weight(1.)
{
    
}

void HistoManager::InitStrVec(vector<string>& out, string in, string de) {
    int pos=0, pos_pre=0;
    while(true) {
        pos=in.find(de,pos_pre);
        if(pos==-1) {out.push_back(in.substr(pos_pre,in.size()-pos_pre)); break;}
        else  out.push_back(in.substr(pos_pre,pos-pos_pre));
        pos_pre=pos+1;
    }
}

void HistoManager::InitCutflow(string name, string cutlist) {

  // initialize cutflow
  vector<string> cuts;
  InitStrVec(cuts, cutlist, ",");
  
  Cutflow_names.push_back(name);
  for(int i=0; i<(int)cuts.size(); i++) {
    string cut=cuts[i];
    Cutflow_cuts[name].push_back(cut);
    Cutflow_flag_tmp[name][cut]=0;
    Cutflow_flag[name][cut]=0;
    Cutflow_count[name][cut]=0;
  }

  string filename = "log_Cutflow";
  if(!log_cutflow) log_cutflow = new TMacro(filename.c_str(), filename.c_str());
}

void HistoManager::ClearCutflowFlag() {
  
  // clear cutflow map
  for(int in=0; in < (int)Cutflow_names.size(); in ++) {
    string name=Cutflow_names[in];
    for(int ic=0; ic < (int)Cutflow_cuts[name].size(); ic++) {
      string cut=Cutflow_cuts[name][ic];
      Cutflow_flag_tmp[name][cut]=0;
      Cutflow_flag[name][cut]=0;
    }
  }
}

void HistoManager::ClearCutflowFlag(std::string name) {
  
  // clear cutflow map
  if(Cutflow_cuts.find(name) != Cutflow_cuts.end()) {
    for(int ic=0; ic < (int)Cutflow_cuts[name].size(); ic++) {
      string cut=Cutflow_cuts[name][ic];
      Cutflow_flag_tmp[name][cut]=0;
      Cutflow_flag[name][cut]=0;
    }
  }
}

void HistoManager::ClearCutflowCount() {
  
  // clear cutflow map
  for(int in=0; in < (int)Cutflow_names.size(); in ++) {
    string name=Cutflow_names[in];
    for(int ic=0; ic < (int)Cutflow_cuts[name].size(); ic++) {
      string cut=Cutflow_cuts[name][ic];
      Cutflow_count[name][cut]=0;
    }
  }
}

void HistoManager::FillCutflow() {
  
  double wt_all=1.0;

  for(int in=0; in < (int)Cutflow_names.size(); in ++) {
    string name=Cutflow_names[in];
    int pass=1;
    for(int ic=0; ic < (int)Cutflow_cuts[name].size(); ic++) {
      string cut=Cutflow_cuts[name][ic];
      //if(pass==0) continue;
      pass = pass && Cutflow_flag_tmp[name][cut] || Cutflow_flag[name][cut]; // allow the possibility to overwrite cutflow, by
      // setting Cutflow_flag directly, bypassing Cutflow_flag_tmp
      // check if it passes all previous cuts
      if(pass==1) {
        Cutflow_flag[name][cut]=1;
        Cutflow_count[name][cut] += wt_all;
      }
    }
  }
}

void HistoManager::FillCutflowFlag(std::string name) {
  
  double wt_all=1.0;

  if(Cutflow_cuts.find(name) != Cutflow_cuts.end()) {
    int pass=1;
    for(int ic=0; ic < (int)Cutflow_cuts[name].size(); ic++) {
      string cut=Cutflow_cuts[name][ic];
      //if(pass==0) continue;
      pass = pass && Cutflow_flag_tmp[name][cut] || Cutflow_flag[name][cut];
      // check if it passes all previous cuts
      if(pass==1) {
        Cutflow_flag[name][cut]=1;
      }
    }
  }
}

void HistoManager::FillCutflow(std::string name) {
  
  double wt_all=1.0;

  if(Cutflow_cuts.find(name) != Cutflow_cuts.end()) {
    int pass=1;
    for(int ic=0; ic < (int)Cutflow_cuts[name].size(); ic++) {
      string cut=Cutflow_cuts[name][ic];
      //if(pass==0) continue;
      pass = pass && Cutflow_flag_tmp[name][cut] || Cutflow_flag[name][cut];
      // check if it passes all previous cuts
      if(pass==1) {
        Cutflow_flag[name][cut]=1;
        Cutflow_count[name][cut] += wt_all;
      }
    }

    ClearCutflowFlag(name); // !! to avoid double counting
  }
}

void HistoManager::PrintCutflow() {

  string line;
  stringstream ss_line;

  for(int in=0; in < (int)Cutflow_names.size(); in ++) {
    string name=Cutflow_names[in];
    ss_line << name << endl;
    ss_line >> line;   
    log_cutflow->AddLine(line.c_str());
    for(int ic=0; ic < (int)Cutflow_cuts[name].size(); ic++) {
      string cut=Cutflow_cuts[name][ic];
    
      //ss_line<<cut<<"="<<Cutflow_count[name][cut] << endl;
      // add cat name to avoid confusion
      ss_line<<name<<"_"<<cut<<"="<<Cutflow_count[name][cut] << endl;
      ss_line>>line;
      log_cutflow->AddLine(line.c_str());
    }
  }
}

void HistoManager::InitVar(string varlist, int nbin, double xmin, double xmax, int vec, int option, int type) {

  vector<string> variables;
  InitStrVec(variables, varlist, ",");

  for(int i=0; i<(int)variables.size(); i++) {
    VarName[variables[i]].NBins=nbin;
    VarName[variables[i]].Xmin=xmin;
    VarName[variables[i]].Xmax=xmax;
    VarName[variables[i]].Option=(float)option;
    //VarName[variables[i]].CutStep[cat]=nstep;
    VarName[variables[i]].Type=(float)type;
    VarName[variables[i]].Vector=(float)vec;
  }
}

// fancy InitVar
void HistoManager::InitVar(string varlist, int nbin, double xmin, double xmax, string catlist, string cutstep, int vec, int option, int type) {

  vector<string> variables;
  InitStrVec(variables, varlist, ",");
  
  for(int i=0; i<(int)variables.size(); i++) {
    VarName[variables[i]].NBins=nbin;
    VarName[variables[i]].Xmin=xmin;
    VarName[variables[i]].Xmax=xmax;
    VarName[variables[i]].Option=(float)option;
    VarName[variables[i]].Type=(float)type;
    VarName[variables[i]].Vector=(float)vec;
  }

  vector<string> cats;
  InitStrVec(cats, catlist, ",");
  double nstep;
  double steponly = 1;
  //if cutstep starts with "-", then fill histograms only at cutstep
  if(cutstep.compare(0,1,"-") == 0) { steponly = -1; cutstep = cutstep.erase(0,1); }
  for(int c=0; c<(int)cats.size(); c++) {
    string cat=cats[c];
    if(Cutflow_cuts.find(cat) == Cutflow_cuts.end()) {
      std::cout<<"HistoManager::InitVar() Error in InitVar: can not find category "<< cat << std::endl;
      continue;
    }
    vector<string>::iterator p_cutstep = find( Cutflow_cuts[cat].begin(), Cutflow_cuts[cat].end(), cutstep);
    if( p_cutstep == Cutflow_cuts[cat].end() ) nstep = (double)Cutflow_cuts[cat].size();
    else nstep = (double)distance( Cutflow_cuts[cat].begin(), p_cutstep );
    nstep += 1;
    nstep *= steponly;

    for(int i=0; i<(int)variables.size(); i++) {
      VarName[variables[i]].CutStep[cat]=nstep;
    }
  }
}

// 2D histo
void HistoManager::Init2DHelper(string var1, int nbinx, double xmin, double xmax, string var2, int nbiny, double ymin, double ymax, int vec, int option) {

  vector<string> variables1, variables2;
  InitStrVec(variables1, var1, ",");
  InitStrVec(variables2, var2, ",");

  VarConfig setting_var1, setting_var2;

  for(int i=0; i<(int)variables1.size(); i++) {
    VarName[variables1[i]].NBins=nbinx;
    VarName[variables1[i]].Xmin=xmin;
    VarName[variables1[i]].Xmax=xmax;
    VarName[variables1[i]].Option=(float)option;
    VarName[variables1[i]].Vector=(float)vec;
    VarName[variables1[i]].Dim=0;
  }

  for(int i=0; i<(int)variables2.size(); i++) {
    VarName[variables2[i]].NBins=nbiny;
    VarName[variables2[i]].Xmin=ymin;
    VarName[variables2[i]].Xmax=ymax;
    VarName[variables2[i]].Option=(float)option;
    VarName[variables2[i]].Vector=(float)vec;
    VarName[variables2[i]].Dim=1;
  }

  for(int i=0; i<(int)variables1.size(); i++) {
    for(int j=0; j<(int)variables2.size(); j++) {
      string vname1=variables1[i];
      string vname2=variables2[j];
      string histo_name=vname1+"_"+vname2;
      setting_var1.NBins=nbinx;
      setting_var1.Xmin=xmin;
      setting_var1.Xmax=xmax;
      setting_var1.Option=(float)option;
      setting_var1.Vector=(float)vec;
      setting_var1.Dim=0;
      setting_var2.NBins=nbiny;
      setting_var2.Xmin=ymin;
      setting_var2.Xmax=ymax;
      setting_var2.Option=(float)option;
      setting_var2.Vector=(float)vec;
      setting_var2.Dim=1;
      helper_2D[histo_name][vname1]=setting_var1;
      helper_2D[histo_name][vname2]=setting_var2;
    }
  }
}

// fancy 2D
void HistoManager::Init2DHelper(string var1, int nbinx, double xmin, double xmax, string var2, int nbiny, double ymin, double ymax, string catlist, string cutstep,  int vec, int option) {

  vector<string> variables1, variables2, cats;
  InitStrVec(variables1, var1, ",");
  InitStrVec(variables2, var2, ",");
  InitStrVec(cats, catlist, ",");
  
  VarConfig setting_var1, setting_var2;

  for(int i=0; i<(int)variables1.size(); i++) {
    VarName[variables1[i]].NBins=nbinx;
    VarName[variables1[i]].Xmin=xmin;
    VarName[variables1[i]].Xmax=xmax;
    VarName[variables1[i]].Option=(float)option;
    VarName[variables1[i]].Vector=(float)vec;
    VarName[variables1[i]].Dim=0;
  }

  for(int i=0; i<(int)variables2.size(); i++) {
    VarName[variables2[i]].NBins=nbiny;
    VarName[variables2[i]].Xmin=ymin;
    VarName[variables2[i]].Xmax=ymax;
    VarName[variables2[i]].Option=(float)option;
    VarName[variables2[i]].Vector=(float)vec;
    VarName[variables2[i]].Dim=1;
  }
  for(int i=0; i<(int)variables1.size(); i++) {
    for(int j=0; j<(int)variables2.size(); j++) {
      string vname1=variables1[i];
      string vname2=variables2[j];
      string histo_name=vname1+"_"+vname2;
      setting_var1.NBins=nbinx;
      setting_var1.Xmin=xmin;
      setting_var1.Xmax=xmax;
      setting_var1.Option=(float)option;
      setting_var1.Vector=(float)vec;
      setting_var1.Dim=0;
      setting_var2.NBins=nbiny;
      setting_var2.Xmin=ymin;
      setting_var2.Xmax=ymax;
      setting_var2.Option=(float)option;
      setting_var2.Vector=(float)vec;
      setting_var2.Dim=1;
      helper_2D[histo_name][vname1]=setting_var1;
      helper_2D[histo_name][vname2]=setting_var2;
    }
  }

  double nstep;
  double steponly = 1;
  //if cutstep starts with "-", then fill histograms only at cutstep
  if(cutstep.compare(0,1,"-") == 0) { steponly = -1; cutstep = cutstep.erase(0,1); }
  for(int c=0; c<(int)cats.size(); c++) {
    string cat=cats[c];
    if(Cutflow_cuts.find(cat) == Cutflow_cuts.end()) {
      std::cout<<"HistoManager::InitVar() Error in InitVar: can not find category "<< cat << std::endl;
      continue;
    }
    vector<string>::iterator p_cutstep = find( Cutflow_cuts[cat].begin(), Cutflow_cuts[cat].end(), cutstep);
    if( p_cutstep == Cutflow_cuts[cat].end() ) nstep = (double)Cutflow_cuts[cat].size();
    else nstep = (double)distance( Cutflow_cuts[cat].begin(), p_cutstep );
    nstep += 1;
    nstep *= steponly;

    for(int i=0; i<(int)variables1.size(); i++) {
      VarName[variables1[i]].CutStep[cat]=nstep;
    }
    for(int i=0; i<(int)variables2.size(); i++) {
      VarName[variables2[i]].CutStep[cat]=nstep;
    }
    for(int i=0; i<(int)variables1.size(); i++) {
      for(int j=0; j<(int)variables2.size(); j++) {
        string vname1=variables1[i];
        string vname2=variables2[j];
        string histo_name=vname1+"_"+vname2;
        helper_2D[histo_name][vname1].CutStep[cat]=nstep;
        helper_2D[histo_name][vname2].CutStep[cat]=nstep;
      }
    }
  }
}

void HistoManager::ClearVariables() {
  ClearVariables(Var);
  ClearVariables(VVar);
  ClearVariables(VStr);
  ClearVariables(VVStr);
}

void HistoManager::ClearVariables(MapType_Double& map) {

    MapType_Double::iterator it;
    for(it=map.begin(); it!=map.end(); it++)
        map[(*it).first]=-9999.;
}

void HistoManager::ClearVariables(MapType_VDoubleStar& map) {

    MapType_VDoubleStar::iterator it;
    for(it=map.begin(); it!=map.end(); it++)
        map[(*it).first]->clear();
}

void HistoManager::ClearVariables(MapType_VStringStar& map) {

    MapType_VStringStar::iterator it;
    for(it=map.begin(); it!=map.end(); it++)
        map[(*it).first]->clear();
}

void HistoManager::ClearVariables(MapType_String& map) {

    MapType_String::iterator it;
    for(it=map.begin(); it!=map.end(); it++)
        map[(*it).first]="";
}

void HistoManager::CreateHistoMap() {
  MapType_VarConfig::iterator it;
  for(it=VarName.begin(); it!=VarName.end(); it++) {
      //<Prepare Variables>
      string varname=(*it).first;
      //<Create Tree Variable First>
      int nbin=0, option=0, vec=0, type=-1;
      float xlow=0., xhigh=0.;
      nbin = int((*it).second.NBins);
      xlow = double((*it).second.Xmin);
      xhigh = double((*it).second.Xmax);
      option = int((*it).second.Option);
      vec = int((*it).second.Vector);
      type = int((*it).second.Type);
      //<Initialize variable>
      if(type==0) {
        if(vec==1) {
            vector<double>* tmp = new vector<double>();
            VVar[varname] = tmp;
        }
        else Var[varname]=0.;
      }
      else if(type==1) {
        // vector<string>, for tree only
        if(vec==1) {
            vector<string>* tmp = new vector<string>();
            VVStr[varname] = tmp;
        }
        else VStr[varname] = "";
      }
      if(option==InTreeOnly) continue;
      //<Create Histograms>
      map<string, int> cutstep=(*it).second.CutStep;
      if(cutstep.size()>0) {
        map<string, int>::iterator itcs;
        for(itcs=cutstep.begin(); itcs!=cutstep.end(); itcs++) {
          string cat=(*itcs).first;
          //loop on cuts
          for(int jc=0; jc<(int)Cutflow_cuts[cat].size(); jc++) {
            string cut = Cutflow_cuts[cat][jc];
            int nstep = (int) cutstep[cat];
            if(jc<nstep-1 || (jc!=-1*nstep-1 && nstep<0)) continue;

            string histo_name= varname + "_" + cat + "_" + cut;
            TH1F *histo_pointer = new TH1F(histo_name.c_str(),histo_name.c_str(),nbin,xlow,xhigh);
            histo_pointer->Sumw2();
            histoMap_CutStep[cat][cut][varname]=histo_pointer;
          }// cut
        }// cat
      } else {
        string histo_name= varname;
        TH1F *histo_pointer = new TH1F(histo_name.c_str(),histo_name.c_str(),nbin,xlow,xhigh);
        histo_pointer->Sumw2();
        histoMap[varname]=histo_pointer;
      }
  }

  //<init 2D histogram>
  MapType2_VarConfig::iterator it2;
  for(it2=helper_2D.begin(); it2!=helper_2D.end(); it2++) {
    string base_name = (*it2).first;
    if((*it2).second.size()<2) {
      std::cout<<"HistoManager::FillHistograms() Error => not enough settings for 2D "<< base_name <<endl;
      exit(-1);
    }
  
    VarConfig setting1, setting2;
    string var1,var2;
    //get the x,y component varname
    MapType_VarConfig::iterator it3;
    for(it3=(*it2).second.begin(); it3!=(*it2).second.end(); it3++) {
        string name = (*it3).first;
        VarConfig setting = (*it3).second;
        if(setting.Dim==0) {var1=name; setting1=setting;}
        if(setting.Dim==1) {var2=name; setting2=setting;}
    }
    vector<double> var1_range, var2_range;
    var1_range.push_back(setting1.NBins);
    var1_range.push_back(setting1.Xmin);
    var1_range.push_back(setting1.Xmax);
    var2_range.push_back(setting2.NBins);
    var2_range.push_back(setting2.Xmin);
    var2_range.push_back(setting2.Xmax);

    map<string, int> cutstep=setting1.CutStep;
    if(cutstep.size()>0) { 
        map<string, int>::iterator itcs;
        for(itcs=cutstep.begin(); itcs!=cutstep.end(); itcs++) {
          string cat=(*itcs).first;
          //loop on cuts
          for(int jc=0; jc<(int)Cutflow_cuts[cat].size(); jc++) {
            string cut = Cutflow_cuts[cat][jc];
            int nstep = (int) cutstep[cat];
            if(jc<nstep-1 || (jc!=-1*nstep-1 && nstep<0)) continue;

            string histo2D_name= base_name + "_" + cat + "_" + cut;
            TH2F *histo2D_pointer = new TH2F(histo2D_name.c_str(),histo2D_name.c_str(),\
                                                        (int)var1_range[0],var1_range[1],var1_range[2],\
                                                        (int)var2_range[0],var2_range[1],var2_range[2]);
            histo2D_pointer->Sumw2();
            histoMap2D_CutStep[cat][cut][base_name]=histo2D_pointer;
       
          }// cut
        }// cat
    } else {
      string histo2D_name= base_name;
      TH2F *histo2D_pointer = new TH2F(histo2D_name.c_str(),histo2D_name.c_str(),\
                                                  (int)var1_range[0],var1_range[1],var1_range[2],\
                                                  (int)var2_range[0],var2_range[1],var2_range[2]);
      histo2D_pointer->Sumw2();
      histoMap2D[base_name]=histo2D_pointer;
    }
  }//it2

}

void HistoManager::AddVarIntoTree() {

    if(!myTree) myTree = new TTree("DiOnia", "DiOnia");

    //<loop the "treeVar" map to automatically add branches to tree>
    //<To make life easier, all variables are set to be "double">
    MapType_VarConfig::iterator it;
    for(it=VarName.begin(); it!=VarName.end(); it++) {
        string varname = (*it).first;
        if(int(VarName[varname].Type)==0) {
          if(VarName[varname].Vector)
              myTree->Branch(varname.c_str(),"vector<double>",&VVar[varname]);
          else
              myTree->Branch(varname.c_str(),&Var[varname]);
        } else if(int(VarName[varname].Type)==1) {
          if(int(VarName[varname].Vector)==1)
              myTree->Branch(varname.c_str(),"vector<string>",&VVStr[varname]);
          else
              myTree->Branch(varname.c_str(),&VStr[varname]);
        }
    }

    //<Also add event selection flags into trees for further look>
    for(int in=0; in < (int)Cutflow_names.size(); in ++) {
      string name=Cutflow_names[in];
      for(int ic=0; ic < (int)Cutflow_cuts[name].size(); ic++) {
        string cut=Cutflow_cuts[name][ic];
        string varname="FLAG_"+name+"_"+cut;
        string varname2="FLAG_"+name+"_"+cut+"/I";
        myTree->Branch(varname.c_str(),&Cutflow_flag[name][cut],varname2.c_str());
      }
    }
}

void HistoManager::FillHistograms() {
  //<fill 1D histogram>
  MapType_VarConfig::iterator it;
  for(it=VarName.begin(); it!=VarName.end(); it++) {
      string varname = (*it).first;
      int option = int((*it).second.Option);
      if(option==InTreeOnly) continue;

      // prepare variables to fill
      //handle if variable is single or vector
      vector<double> tmp;
      if(VarName[varname].Vector) {
          for(int k=0; k<(int)VVar[varname]->size(); k++)
              tmp.push_back(VVar[varname]->at(k));
      }
      else tmp.push_back(Var[varname]);
      //event weight
      //double wt_histo=1.0;
      //fill histogram
      if (tmp.size() == 0) continue;

      map<string, int> cutstep=(*it).second.CutStep;
      if(cutstep.size()>0) {
        map<string, int>::iterator itcs;
        for(itcs=cutstep.begin(); itcs!=cutstep.end(); itcs++) {
          string cat=(*itcs).first;
          //loop on cuts
          for(int jc=0; jc<(int)Cutflow_cuts[cat].size(); jc++) {
            string cut = Cutflow_cuts[cat][jc];
            int nstep = (int) cutstep[cat];
            if(jc<nstep-1 || (jc!=-1*nstep-1 && nstep<0)) continue;
            // fill if passing cutflow
            if(Cutflow_flag[cat][cut]) {
              for(int k=0; k<(int)tmp.size(); k++)
              {
                // -9999. is the default value
                // if a variable is -9999, then it means it has not been assigened
                if(tmp[k]!=-9999.) histoMap_CutStep[cat][cut][varname]->Fill(tmp[k], Evt_weight);
              }
            }
          }
        }
      } else {
        for(int k=0; k<(int)tmp.size(); k++)
        {
          // -9999. is the default value
          // if a variable is -9999, then it means it has not been assigened
          if(tmp[k]!=-9999.) histoMap[varname]->Fill(tmp[k], Evt_weight);
        }
      }
  } //it

  //<fill 2D histogram>
  MapType2_VarConfig::iterator it2;
  for(it2=helper_2D.begin(); it2!=helper_2D.end(); it2++) {
    string base_name = (*it2).first;
    if((*it2).second.size()<2) {
      std::cout<<"HistoManager::FillHistograms() Error => not enough settings for 2D "<< base_name <<endl;
      exit(-1);
    }
  
    VarConfig setting1, setting2;
    string var1,var2;
    //get the x,y component varname
    MapType_VarConfig::iterator it3;
    for(it3=(*it2).second.begin(); it3!=(*it2).second.end(); it3++) {
        string name = (*it3).first;
        VarConfig setting = (*it3).second;
        if(setting.Dim==0) {var1=name; setting1=setting;}
        if(setting.Dim==1) {var2=name; setting2=setting;}
    }
    map<string, int> cutstep=setting1.CutStep;

    //handle if variable is single or vector
    vector<double> tmpx,tmpy;
    string varname = var1;
    if(int(VarName[varname].Vector)==1) {
      for(int k=0; k<(int)VVar[varname]->size(); k++)
        tmpx.push_back(VVar[varname]->at(k));
    }
    else tmpx.push_back(Var[varname]);
    varname = var2;
    if(int(VarName[varname].Vector)==1) {
      for(int k=0; k<(int)VVar[varname]->size(); k++)
        tmpy.push_back(VVar[varname]->at(k));
    }
    else tmpy.push_back(Var[varname]);
    //fill histogram
    if (tmpx.size() == 0||tmpy.size()==0||tmpx.size()!=tmpy.size()) continue;
    
    if(cutstep.size()>0) { 
        map<string, int>::iterator itcs;
        for(itcs=cutstep.begin(); itcs!=cutstep.end(); itcs++) {
          string cat=(*itcs).first;
          //loop on cuts
          for(int jc=0; jc<(int)Cutflow_cuts[cat].size(); jc++) {
            string cut = Cutflow_cuts[cat][jc];
            int nstep = (int) cutstep[cat];
            if(jc<nstep-1 || (jc!=-1*nstep-1 && nstep<0)) continue;
            // fill if passing cutflow
            if(Cutflow_flag[cat][cut]) {
              for(int k=0; k<(int)tmpx.size(); k++)
                if(tmpx[k]!=-9999. && tmpy[k]!=-9999.) histoMap2D_CutStep[cat][cut][base_name]->Fill(tmpx[k],tmpy[k],Evt_weight);
            }
          }//jc
        }//cat
    } else {
      for(int k=0; k<(int)tmpx.size(); k++)
        if(tmpx[k]!=-9999. && tmpy[k]!=-9999.) histoMap2D[base_name]->Fill(tmpx[k],tmpy[k],Evt_weight);
    }
  } // it2

}

void HistoManager::FillTree() {

  myTree->Fill();

}

std::vector<TH1*> HistoManager::getHistos() {

  std::vector<TH1*> histos;
  MapType_TH1F::iterator it;
  for(it=histoMap.begin(); it!=histoMap.end(); it++) {
    histos.push_back(it->second);
  } 
  MapType3_TH1F::iterator it2;
  // cat
  for(it2=histoMap_CutStep.begin(); it2!=histoMap_CutStep.end(); it2++) {
    MapType2_TH1F::iterator it3;
    // cut
    for(it3=(*it2).second.begin(); it3!=(*it2).second.end(); it3++) {
      MapType_TH1F::iterator it4;
      // var
      for(it4=(*it3).second.begin(); it4!=(*it3).second.end(); it4++) {
        histos.push_back(it4->second);
      }//it4
    }//it3
  }//it2

  MapType_TH2F::iterator itt;
  for(itt=histoMap2D.begin(); itt!=histoMap2D.end(); itt++) {
    histos.push_back(itt->second);
  } 
  MapType3_TH2F::iterator itt2;
  // cat
  for(itt2=histoMap2D_CutStep.begin(); itt2!=histoMap2D_CutStep.end(); itt2++) {
    MapType2_TH2F::iterator itt3;
    // cut
    for(itt3=(*itt2).second.begin(); itt3!=(*itt2).second.end(); itt3++) {
      MapType_TH2F::iterator itt4;
      // var
      for(itt4=(*itt3).second.begin(); itt4!=(*itt3).second.end(); itt4++) {
        histos.push_back(itt4->second);
      }//itt4
    }//itt3
  }//itt2

  return histos;

}

TTree* HistoManager::getTree() {

  return myTree;

}

HistoManager::~HistoManager() {
  //auto histos=getHistos();
  //for(auto& hist: histos) if(hist) delete hist;
}
