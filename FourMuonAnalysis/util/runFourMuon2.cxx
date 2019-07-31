#include "xAODRootAccess/Init.h"
#include "SampleHandler/Sample.h"
#include "SampleHandler/SampleHandler.h"
#include "SampleHandler/ToolsDiscovery.h"
#include "SampleHandler/DiskListLocal.h"
#include "SampleHandler/DiskListEOS.h"
#include "SampleHandler/ScanDir.h"
#include "SampleHandler/ToolsSplit.h"
#include "EventLoop/Job.h"
#include "EventLoop/DirectDriver.h"
#include "EventLoop/LSFDriver.h"
#include "EventLoop/CondorDriver.h"
#include "EventLoop/Driver.h"

#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>

#include "TChain.h"

#include "FourMuonAnalysis/XFourMuonAnalysis.h"

using namespace std;

bool cmdline(int argc, char** argv, map<string,string> &opts);
void usage();

int main (int argc, char **argv) {
    map<string, string> opts;
    if (!cmdline(argc, argv, opts)) return 0;

    string outDir_in = opts["outdir"];
    if (outDir_in == "") {
        cout << "Name of output directory required!" << endl;
        return 1;
    }
    //string outDir = string(getenv("WORK")) + "/workarea/outData/" + outDir_in; // Laser was here
    string outDir = outDir_in;

    xAOD::Init().ignore();

    SH::SampleHandler sh;

    if (opts["in"] == "") {
        cout << "Name of input file(s) required!" << endl;
        return 2;
    }

    vector<string> ins;

    //<Get input list>
    std::string inlist=opts["in"];

    ifstream filelist;
    filelist.open(inlist);
    if(!filelist.good()) {
        cout<<"ERROR: Cannot find the input filelist, now quit!"<<endl;
        return 2;
    }

    /*
    string file;
    while(!filelist.eof()) {
        getline(filelist,file);        
        if(file.size()==0) continue; //remove the blank lines
        cout<<"Add file \""<<file<<"\""<<endl;
        //if dcache, use dcap://dcap.aglt2.org as prefix
        if(file.find("/pnfs")!=string::npos)
            file =  "dcache:" + file;
        if(file.find("/eos")!=string::npos)
            file =  "root://eosatlas/" + file;
        ins.push_back(file);
    } 
   
    TChain chain("CollectionTree");
    for (auto &f : ins) {
        chain.Add(f.c_str());
    }
    sh.add(SH::makeFromTChain("xAOD", chain));
    */

    SH::readFileList(sh, "xAOD", inlist);

    sh.setMetaString("nc_tree", "CollectionTree");

    sh.print();

    EL::Job job;

    //job.options()->setDouble(EL::Job::optMaxEvents, 10000);
    job.options()->setString (EL::Job::optXaodAccessMode, EL::Job::optXaodAccessMode_class);
    //job.options()->setString (EL::Job::optXaodAccessMode, EL::Job::optXaodAccessMode_branch);

    job.sampleHandler(sh);

    XFourMuonAnalysis* analysis = new XFourMuonAnalysis();
    analysis->msg().setName("XFourMuonAnalysis");
    job.algsAdd(analysis);

    //EL::DirectDriver driver;
    //driver.submit(job, outDir);

    int nFilesPerJob=1.;
    int nEventsPerJob=10000;
    bool splitByEvents=true;
    if (splitByEvents) {
      SH::scanNEvents(sh);
      sh.setMetaDouble(EL::Job::optEventsPerWorker, nEventsPerJob);
    }

    EL::CondorDriver* eldriver = new EL::CondorDriver;
    eldriver->shellInit = "export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase && source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh && export LD_LIBRARY_PATH=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/Gcc/gcc493_x86_64_slc6/slc6/x86_64-slc6-gcc49-opt/lib64:${LD_LIBRARY_PATH} && export LD_LIBRARY_PATH=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/Gcc/gcc493_x86_64_slc6/slc6/x86_64-slc6-gcc49-opt/lib:${LD_LIBRARY_PATH}";
    if (!splitByEvents) job.options()->setDouble (EL::Job::optFilesPerWorker, nFilesPerJob);
    job.options()->setString(EL::Job::optCondorConf, "getenv = true\n +Experiment = atlas\n accounting_group = group_atlas.bnl\n Universe = vanilla\n Notification = Error");
    eldriver->submitOnly(job, outDir);

    return 0;
}

bool cmdline(int argc, char** argv, map<string,string> &opts) {
    opts.clear();

    // defaults
    opts["outdir"] = "";
    opts["in"] = "";
    opts["debug"] = "0";

    for (int i=1;i<argc;i++) {

        string opt=argv[i];

        if (opt=="--help") {usage(); return false;}

        if (0 != opt.find("-")) {
            cout<<"ERROR: options start with '-'!"<<endl;
            return false;
        }
        opt.erase(0,1);
        if (opts.find(opt) == opts.end()) {
            cout<<"ERROR: invalid option '"<<opt<<"'!"<<endl;
            return false;
        }
        string nxtopt=argv[i+1];
        if (0 == nxtopt.find("-") || i+1 >= argc) {
            cout<<"ERROR: option '"<<opt<<"' requires value!"<<endl;
            return false;
        }

        opts[opt] = nxtopt;
        i++;
    }

    return true;
}

void usage()
{
    cout<<"USAGE: run [-option value]\n\n"
        <<"options [default]:\n\n"
        <<"-outdir (required!)\n"
        <<"-in (required!)\n"
        <<"-debug <0/1> [0]\n"
        <<endl;

    return;
}
