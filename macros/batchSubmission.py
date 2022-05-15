#!/usr/bin/env python

import sys
import os
import subprocess
import math
import time
import argparse
import numpy as np

parser = argparse.ArgumentParser(description="Submit array jobs to ifarm.")
parser.add_argument("-c", dest="cluster", action="store", required=True, help="Enter the Server Name. Example: ifarm,grex,beluga")
parser.add_argument("-a", dest="account", action="store", required=True, help="Enter the Jefferson Lab account. Example: halla")  
parser.add_argument("-s", dest="src", action="store", required=False, default="/w/halla-scshelf2102/moller12gev/rahmans/mollerOpticalSim", help="source folder where simulation directory exists")
parser.add_argument("-v", dest="version", action="store", required=False, default="MOLLER-IntDetectorOpticalSim", help= "choose the version of simulation to use. Should just be the name of the repository unless you rename local folder")
parser.add_argument("-e", dest="eic_shell", action="store", required=False, default="/w/halla-scshelf2102/moller12gev/rahmans/mollerOpticalSim/eic-shell", help= "provide the location of eic-shell")
parser.add_argument("-j", dest="jsub_dir", action="store", required=True, help="choose directory to write the slurm submission scripts")
parser.add_argument("-t", dest="tmp_dir", action="store", required=True, help="choose directory to write the slurm output logs")
parser.add_argument("-o", dest="out_dir", action="store", required=True, help="choose where to write the output root files")
parser.add_argument("-r", dest="run_range", action = "store", required=False, default="1-2", help="provide run range. Example: \"2-5\"")
parser.add_argument("-n", dest="n_events", action= "store", required=False, default=1000, help= "provide number of events per job in the array")
parser.add_argument("--hr", dest="hit_region", action= "store", required=False, default=1, help= "provide the hit region. 1 for quartz, 2 for lower lg, 3 for upper lg.")
parser.add_argument("--time", dest="time", action= "store", required= False, default= "00:25:00", help= "provide the estimated run time. Ex: \"00:25:00\". Usually it is 10 minutes for 1000 moller events.")
parser.add_argument("--run", dest="run_id", action= "store", required=True, help = "provide an integer run id")
parser.add_argument("--ring", dest="ring_id", action = "store", required= True, help = "provide an integer ring id")



args=parser.parse_args()

args.eic_shell=os.path.realpath(args.eic_shell)
args.src=os.path.realpath(args.src)

HitRegion = ["Quartz", "LowerLg", "UpperLg"]
jsub_dir=os.path.realpath(args.jsub_dir+"/jsub/Run"+args.run_id+"/Ring"+args.ring_id+"/"+str(HitRegion[int(args.hit_region)-1]))
tmp_dir= os.path.realpath(args.tmp_dir+"/tmp/Run"+args.run_id+"/Ring"+args.ring_id+"/"+str(HitRegion[int(args.hit_region)-1]))
out_dir= os.path.realpath(args.out_dir+"/out/Run"+args.run_id+"/Ring"+args.ring_id+"/"+str(HitRegion[int(args.hit_region)-1]))

if not os.path.exists(jsub_dir):
        os.system("mkdir -p "+jsub_dir)
if not os.path.exists(tmp_dir):
        os.system("mkdir -p "+tmp_dir)
if not os.path.exists(out_dir):
        os.system("mkdir -p "+out_dir)

LightGuideLowerConeBackAngle = np.arange(14,30,1)
LightGuideLowerConeFrontAngle = np.arange(14,25,1)
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

jsubf=open(jsub_dir+"/run.sh", "w")
jsubf.write("#!/bin/bash\n")
jsubf.write("#SBATCH --account="+args.account+"\n")
if (args.cluster == "ifarm"):
  jsubf.write("#SBATCH --partition=production\n")
if (args.cluster == "grex"):
  jsubf.write("#SBATCH --partition=compute\n")
jsubf.write("#SBATCH --job-name=molleropt\n")
jsubf.write("#SBATCH --time="+args.time+" \n")
jsubf.write("#SBATCH --nodes=1\n")
jsubf.write("#SBATCH --ntasks=1\n")
jsubf.write("#SBATCH --cpus-per-task=1\n")
jsubf.write("#SBATCH --mem=4G\n")
jsubf.write("#SBATCH --output="+tmp_dir+"/run_%A_%a.out\n")

c=0
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
                              jsubf.write("LightGuideLowerConeBackAngle["+str(c)+"]="+str(ba)+"\n")
                              jsubf.write("LightGuideLowerConeFrontAngle["+str(c)+"]="+str(fa)+"\n")
                              jsubf.write("LightGuideLowerInterface["+str(c)+"]="+str(li)+"\n")
                              jsubf.write("LightGuideUpperInterface["+str(c)+"]="+str(ui)+"\n")
                              jsubf.write("LightGuidePMTInterfaceOpeningX["+str(c)+"]="+str(pmtx)+"\n")
                              jsubf.write("LightGuidePMTInterfaceOpeningZ["+str(c)+"]="+str(pmtz)+"\n")
                              jsubf.write("LightGuideQuartzInterfaceOpeningX["+str(c)+"]="+str(lgqx)+"\n")
                              jsubf.write("LightGuideQuartzInterfaceOpeningZ["+str(c)+"]="+str(lgqz)+"\n")
                              jsubf.write("QuartzSizeZ["+str(c)+"]="+str(qz)+"\n")
                              jsubf.write("QuartzSizeX["+str(c)+"]="+str(qx)+"\n")
                              jsubf.write("QuartzSizeY["+str(c)+"]="+str(qy)+"\n")
                              jsubf.write("QuartzBevelSize["+str(c)+"]="+str(qv)+"\n")
                              jsubf.write("QuartzRotX["+str(c)+"]="+str(qr)+"\n")
                              jsubf.write("PolarRotation["+str(c)+"]="+str(pr)+"\n")
                              jsubf.write("LightGuideQuartzToPMTOffset["+str(c)+"]="+str(off)+"\n")
                              c+=1
if (args.cluster == "ifarm"):
  jsubf.write("cd /scratch/slurm\n")
  jsubf.write("cd ${SLURM_JOB_ID}\n")
if (args.cluster == "grex"):
  jsubf.write("cd /scratch\n")
if (args.cluster == "beluga" or args.cluster == "narval"):
  jsubf.write("cd $TMPDIR\n")
jsubf.write("mkdir ${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}\n")
jsubf.write("cd ${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}\n")
macro="run.mac"
jsubf.write("touch "+macro+"\n")
jsubf.write("echo /Det/LightGuideLowerConeBackAngle ${LightGuideLowerConeBackAngle[${SLURM_ARRAY_TASK_ID}-1]} deg >>"+macro+"\n")
jsubf.write("echo /Det/LightGuideLowerConeFrontAngle ${LightGuideLowerConeFrontAngle[${SLURM_ARRAY_TASK_ID}-1]} deg>>"+macro+"\n")
jsubf.write("echo /Det/LightGuideLowerInterface ${LightGuideLowerInterface[${SLURM_ARRAY_TASK_ID}-1]} mm >>"+macro+"\n")
jsubf.write("echo /Det/LightGuideUpperInterface ${LightGuideUpperInterface[${SLURM_ARRAY_TASK_ID}-1]} mm >>"+macro+"\n")
jsubf.write("echo /Det/LightGuidePMTInterfaceOpeningX ${LightGuidePMTInterfaceOpeningX[${SLURM_ARRAY_TASK_ID}-1]} mm >>"+macro+"\n")
jsubf.write("echo /Det/LightGuidePMTInterfaceOpeningZ ${LightGuidePMTInterfaceOpeningZ[${SLURM_ARRAY_TASK_ID}-1]} mm >>"+macro+"\n")
jsubf.write("echo /Det/LightGuideQuartzInterfaceOpeningX ${LightGuideQuartzInterfaceOpeningX[${SLURM_ARRAY_TASK_ID}-1]} mm >>"+macro+"\n")
jsubf.write("echo /Det/LightGuideQuartzInterfaceOpeningZ ${LightGuideQuartzInterfaceOpeningZ[${SLURM_ARRAY_TASK_ID}-1]} mm >>"+macro+"\n")
jsubf.write("echo /Det/QuartzSizeZ ${QuartzSizeZ[${SLURM_ARRAY_TASK_ID}-1]} mm >>"+macro+"\n")
jsubf.write("echo /Det/QuartzSizeX ${QuartzSizeX[${SLURM_ARRAY_TASK_ID}-1]} mm >>"+macro+"\n")
jsubf.write("echo /Det/QuartzSizeY ${QuartzSizeY[${SLURM_ARRAY_TASK_ID}-1]} mm >>"+macro+"\n")
jsubf.write("echo /Det/QuartzBevelSize ${QuartzBevelSize[${SLURM_ARRAY_TASK_ID}-1]} mm >>"+macro+"\n")
jsubf.write("echo /Det/QuartzRotX ${QuartzRotX[${SLURM_ARRAY_TASK_ID}-1]} deg >>"+macro+"\n")
jsubf.write("echo /Det/PolarRotation ${PolarRotation[${SLURM_ARRAY_TASK_ID}-1]} deg >>"+macro+"\n")
jsubf.write("echo /Det/LightGuideQuartzToPMTOffset ${LightGuideQuartzToPMTOffset[${SLURM_ARRAY_TASK_ID}-1]} mm >>"+macro+"\n")
jsubf.write("echo /Det/UpdateGeometry >>"+macro+"\n")
jsubf.write("echo /Generator/EventHitRegion "+args.hit_region+" >>"+macro+"\n")
jsubf.write("echo /RunAction/SetOutputName Config >>"+macro+"\n")
jsubf.write("echo /RunAction/SetID ${SLURM_ARRAY_TASK_ID} >>"+macro+"\n")
jsubf.write("echo /random/setSeeds ${SLURM_ARRAY_JOB_ID}${SLURM_ARRAY_TASK_ID} ${SLURM_ARRAY_TASK_ID}${SLURM_ARRAY_JOB_ID} >>"+macro+"\n")
jsubf.write("echo /run/beamOn "+str(args.n_events)+" >>"+macro+"\n")
jsubf.write("cat "+macro+"\n")
jsubf.write("cp -r "+args.src+"/"+args.version+" .\n")
jsubf.write("cd "+args.version+"/build \n")
jsubf.write("echo \"Current working directory is `pwd`\"\n")
jsubf.write("cat << EOF | "+args.eic_shell+"\n")
if (args.cluster == "ifarm"):
  jsubf.write("./MOLLEROpt /scratch/slurm/${SLURM_JOB_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}/run.mac\n")
if (args.cluster == "grex"):
  jsubf.write("./MOLLEROpt /scratch/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}/run.mac\n")
if (args.cluster == "beluga" or args.cluster == "narval"):
  jsubf.write("./MOLLEROpt $TMPDIR/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}/run.mac\n")
jsubf.write("EOF\n")
jsubf.write("echo \"Program remoll finished with exit code $? at: `date`\"\n")
jsubf.write("cp *.root "+out_dir+"\n")
if (args.cluster == "ifarm"):
  jsubf.write("rm -rf /scratch/slurm/${SLURM_JOB_ID}/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}\n")
if (args.cluster == "grex"):
  jsubf.write("rm -rf /scratch/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}\n")
if (args.cluster == "beluga" or args.cluster == "narval"):
  jsubf.write("rm -rf $TMPDIR/${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}\n")
jsubf.close()
	        
                
subprocess.call("sbatch --array="+args.run_range+" "+jsub_dir+"/run.sh",shell=True)
