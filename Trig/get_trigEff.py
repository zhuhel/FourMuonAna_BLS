#!/usr/bin/env python

import sys
import operator

from ROOT import gStyle,gDirectory,TCanvas,TPad,TFile,TH1F,TF1,TLegend,TPaveText,TTree,TChain, TBranch, AddressOf, Double
from math import sqrt, pow


def get_trigEff(treeList="", trigList="", treename="tree1", ch=''):

  list_trig=[]
  dict_trig={}
  
  ## read trig list
  with open(trigList) as f:
    for line in f.readlines():
      trig=line.rstrip()
      if trig[0]=='#': continue
      if trig not in dict_trig:
       list_trig.append(trig)
       dict_trig[trig]=0
  
  dict_trig['All']=0
  
  ## read input tree
  tchin=TChain(treename)
  with open(treeList, 'r') as fin: 
    for line in fin.readlines():
      file=line.rstrip()
      if len(file)==0: continue
      tchin.Add(file)

  pass_trig=dict(dict_trig)
  for trig in pass_trig.keys(): pass_trig[trig]=0

  ## get trigger count
  nTot=0
  nEntries = tchin.GetEntries()
  print "Info=> Total number of events %d" % nEntries
  ## loop events
  for i in range(0, nEntries):
    tchin.GetEntry(i)

    if i%1000 ==0 :
      print "Info=> %d events processed" % i

    if i>100000: break
    ## pass event selection first
    if not (tchin.FLAG_Event_Cand==1) : continue
    if ch=='2m2e' and tchin.cand_pX_nEl[tchin.ind_cand]!=2: continue
    if ch=='3me' and tchin.cand_pX_nEl[tchin.ind_cand]!=1: continue
    nTot +=1
    passTrig=0
    ## loop all triggers
    for i, trig in enumerate(tchin.trigList):
      if trig in dict_trig.keys() and tchin.trigPass[i]==1:
        passTrig=1 
        dict_trig[trig] += 1
      if tchin.trigPass[i]==1:
        for otrig in dict_trig.keys():
          if otrig[-4:]=='_All' and (trig.replace('_L2StarA', '')==otrig.replace('_All', '') or trig.replace('_L2StarB', '')==otrig.replace('_All', '')):
            pass_trig[otrig] = 1
            print 'Check=>', trig, otrig, passTrig

    for trig in dict_trig.keys():
      if trig[-4:]=='_All' and pass_trig[trig]==1: 
        dict_trig[trig] += 1
    if passTrig: dict_trig['All'] += 1

  ## get trigger efficiency
  dict_trigEff=dict(dict_trig)
  for trig in dict_trigEff.keys():
    if nTot!=0: dict_trigEff[trig]=dict_trigEff[trig]/float(nTot)

  #sorted_dict=sorted(dict_trigEff.items(), key=operator.itemgetter(1))  ## sort on values. to use sort on keys: itemgetter(0)
  sorted_dict=sorted(dict_trigEff.items(), key=operator.itemgetter(1), reverse=True)  ## sort on values. to use sort on keys: itemgetter(0)
  #print sorted_dict
  for itrig in range(len(sorted_dict)):
    print sorted_dict[itrig]


if __name__ == '__main__':

  listTree="listTree"
  #listTree="listTree_data"
  #listTree="listTree_emu"
  #ch='2m2e'
  #ch='3me'
  ch='4m'
  listTrig="trigList"
  #listTrig="trigList_data"
  treeName="DiOnia"
  get_trigEff(listTree, listTrig, treename=treeName, ch=ch)
