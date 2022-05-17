#!/usr/bin/env python

import argparse
import numpy as np
import time
import os

parser = argparse.ArgumentParser(description="Create input macro files for submitting jobs to open science grid")

parser.add_argument("-j", dest="jsub_dir", action="store", required=True, help = "choose directory to write the slurm submission scripts")
parser.add_argument("-n", dest="n_events", action= "store", required=True, help = "provide number of events per job in the array")
parser.add_argument("--hr", dest="hit_region", action= "store", required=True, help = "provide the hit region. 1 for quartz, 2 for lower lg, 3 for upper lg.")
parser.add_argument("-r", dest="run_id", action= "store", required=True, help = "provide an integer run id")
parser.add_argument("-R", dest="ring_id", action = "store", required= True, help = "provide an integer ring id")

args=parser.parse_args()

LightGuideLowerConeBackAngle = np.arange(12,25,1)
LightGuideLowerConeFrontAngle = np.arange(12,25,1)
LightGuideLowerInterface = np.arange(75,76,1)
LightGuideUpperInterface = np.arange(611,612,1)
LightGuidePMTInterfaceOpeningX = np.arange(70,71,1)
LightGuidePMTInterfaceOpeningZ = np.arange(70,71,1)
QuartzSizeZ = np.arange(20,21,1)
QuartzSizeX = np.arange(177,178,1)
QuartzSizeY = np.arange(60,61,1)
LightGuideQuartzInterfaceOpeningX = QuartzSizeX+8
LightGuideQuartzInterfaceOpeningZ = QuartzSizeZ+7
QuartzBevelSize = 0.5*np.arange(1,2,1)
QuartzRotX = np.arange(-3,-2,1)
PolarRotation = np.arange(3,4,1)
LightGuideQuartzToPMTOffset = np.arange(-5,6,1)
HitRegion = ["Quartz", "LowerLg", "UpperLg"] 		

jsub_dir=os.path.realpath(args.jsub_dir+"/input/Run"+args.run_id+"/Ring"+args.ring_id+"/"+str(HitRegion[int(args.hit_region)-1]))
log_dir= os.path.realpath(args.jsub_dir+"/logs/Run"+args.run_id+"/Ring"+args.ring_id+"/"+str(HitRegion[int(args.hit_region)-1]))
#ut_dir= os.path.realpath("/public/rahmans1/output/"+args.run_id+"/"+args.ring_id+"/"+str(HitRegion[int(args.hit_region)-1]))

if not os.path.exists(jsub_dir):
        os.system("mkdir -p "+jsub_dir)
if not os.path.exists(log_dir):
        os.system("mkdir -p "+log_dir)
#if not os.path.exists(out_dir):
#        os.system("mkdir -p "+out_dir)

count=1 
for ba in LightGuideLowerConeBackAngle:
  for fa in LightGuideLowerConeFrontAngle:
    for li in LightGuideLowerInterface:
      for ui in LightGuideUpperInterface:
        for pmtx in LightGuidePMTInterfaceOpeningX:
          for pmtz in LightGuidePMTInterfaceOpeningZ:
            for qz in QuartzSizeZ:
              for qx in QuartzSizeX:
                for qy in QuartzSizeY:
                  for lgqx in LightGuideQuartzInterfaceOpeningX:
                    for lgqz in LightGuideQuartzInterfaceOpeningZ:
                      for qv in QuartzBevelSize:
                        for qr in QuartzRotX:
                          for pr in PolarRotation:
                            for off in LightGuideQuartzToPMTOffset:
                              config= "config"+str(count)
                              seed = args.run_id+str(count)+" "+str(int(time.time()))
                              jsubf=open(jsub_dir+"/"+config, "w")
                              jsubf.write("/Det/LightGuideLowerConeBackAngle "+str(ba)+" deg\n")
                              jsubf.write("/Det/LightGuideLowerConeFrontAngle "+str(fa)+" deg\n")
                              jsubf.write("/Det/LightGuideLowerInterface "+str(li)+" mm\n")
                              jsubf.write("/Det/LightGuideUpperInterface "+str(ui)+" mm\n")
                              jsubf.write("/Det/LightGuidePMTInterfaceOpeningX "+str(pmtx)+" mm\n")
                              jsubf.write("/Det/LightGuidePMTInterfaceOpeningZ "+str(pmtz)+" mm\n")
                              jsubf.write("/Det/LightGuideQuartzInterfaceOpeningX "+str(lgqx)+" mm\n")
                              jsubf.write("/Det/LightGuideQuartzInterfaceOpeningZ "+str(lgqz)+" mm\n")
                              jsubf.write("/Det/QuartzSizeZ "+str(qz)+" mm\n")
                              jsubf.write("/Det/QuartzSizeX "+str(qx)+" mm\n")
                              jsubf.write("/Det/QuartzSizeY "+str(qy)+" mm\n")
                              jsubf.write("/Det/QuartzBevelSize "+str(qv)+" mm\n")
                              jsubf.write("/Det/QuartzRotX "+str(qr)+" deg\n")
                              jsubf.write("/Det/PolarRotation "+str(pr)+" deg\n")
                              jsubf.write("/Det/LightGuideQuartzToPMTOffset "+str(off)+" mm\n")
                              jsubf.write("/Det/UpdateGeometry\n")
                              jsubf.write("/Generator/EventHitRegion "+args.hit_region+"\n")
                              jsubf.write("/RunAction/SetOutputName Run"+args.run_id+"_Ring"+args.ring_id+"_"+str(HitRegion[int(args.hit_region)-1])+"_Config \n")                 
                              jsubf.write("/RunAction/SetID "+args.run_id+"\n")
                              jsubf.write("/random/setSeeds "+seed+"\n")
                              jsubf.write("/run/beamOn "+str(args.n_events)+"\n")
                              jsubf.close()
                              count += 1

