import random
import os

#This script produces a series of G4 macros that vary a specified set of simulation parameters that are of relevance to the MOLLER detector design.
#This script will also produce a bash script at the end, called "StartRuns" that can be used to run all of the generated macros at the same time.
#Care should be taken with respect to the number of variations. Each simulation run takes about 1GB of memory. On my own machine with 20 cores and 15 GB Memory + 15 GB Swap
#I can comfortably run about 20 such processes at a time.

runscript = "#!/bin/bash\n\n"
datadir =  "../data/February2022/Ring5LGAngleParamScan/" # with respect to the sim executable directory - change this to preferred dir
OutputFilePrefix = "MOLLEROpt_Scan"                      # String that starts all the output files from this script (all macro files and root output files)
RunID = 1  #Set this to distinguish scans - the same ID is assigned to each run in this scan - see below for additional run identifiers within the scan

#Fixed values
QWidth = 80  #mm
QHeight = 140 #mm
QLGOInterfOpening = QWidth + 8  #mm   This is the opening where the quartz and light guide meet. The extra space is dictated by CAD design
LightGuideLength = 329  #mm           This is the total guide length, fixed for each module by the detector tiling (location) and the cathode position
LGPMTInterfaceOpening = 70 #mm        This is fixed by the PMT size, which is 3 inches for the main detector 
QuartzBevelSize = 0.5 #mm             This should be as small as possible - 0 is desirable but not peferred by manufacturer
QuartzRotX = -3  #degrees             This is the ralative rotation between the quartz and the light guide. It is optimized to minimize background in the LG
PolarRotation = 3  #degress           This counter rotates the detector module so that the electrons are nomarlly incident on the quartz. Opposite to former
QuartzRadDamage = 170 #MRad           This should be the peak radiation damage in a tile. This is normally turned off and currently the wrong damage level until we get new data from Dustin

RndSeed1 = random.randrange(300000, 600000)
RndSeed2 = random.randrange(600001, 900000)

NumEvents = [2000,100000,100000]   #Number of events on region 1,2, and 3 which is the quartz, lower guide funnel, and upper guide funnel - vary to taste ...
                                   #The LG funnel regions can run many more events than the quartz, because there not many photons generated there


#If new parameters are to be varied, implement a new nested loop below.
#Presently we have
#ba = LightGuideLowerConeBackAngle   - downstream angle 
#fa = LightGuideLowerConeFrontAngle  - upstream angle
#li = LightGuideLowerInterface       - length of the lower guide cone
#qt = QuartzSizeZ                    - quartz thickness in beam direction
#of = LightGuideQuartzToPMTOffset    - offset of PMT wrt. quartz in the beam direction
#hr = electron hit region            - quartz, lower cone/funnel, or upper cone/funnel

#Refer to the detector limits presentation for details on these parameters.
#The correspondence with paraemters specified there is:
#ba = a2
#fa = a1
#li = d3
#qt = d4 - 7 mmm
#of = d6


#Loop over values (to fix a variable set the start and stop both to the desired value)
ba_start = 20 #degress
ba_stop = 24  #degress
ba_step = 1  

fa_start = 17 #degress 
fa_stop = 22  #degress
fa_step = 1

li_start = 75 #mm
li_stop = 75  #mm
li_step = 2

qt_start = 17 #mm
qt_stop = 17 #mm
qt_step = 1

of_start = 0 #mm
of_stop = 0  #mm
of_step = 2

hr_start = 1
hr_stop = 1
hr_step = 1

for ba in range(ba_start,ba_stop+ba_step,ba_step):
    for fa in range(fa_start,fa_stop+fa_step,fa_step):
        for li in range(li_start,li_stop+li_step,li_step):
            for qt in range(qt_start,qt_stop+qt_step,qt_step):
                for of in range(of_start,of_stop+of_step,of_step):
                    for hr in range(hr_start,hr_stop+hr_step,hr_step):
                        Text = ""
                        FileIDString = "_fA"+str(fa)+"_bA"+str(ba)+"_hR"+str(hr)+"_lI"+str(li)+"_qT"+str(qt) + "_oF"+str(of)
                        Text += "/Det/LightGuideLowerConeBackAngle " + str(ba) + " deg" + "\n"
                        Text += "/Det/LightGuideLowerConeFrontAngle " + str(fa) + " deg" + "\n"
                        Text += "/Det/LightGuideLowerInterface "+ str(li) + " mm" + "\n"
                        Text += "/Det/LightGuideUpperInterface " + str(LightGuideLength) + " mm" + "\n" 
                        Text += "/Det/LightGuidePMTInterfaceOpeningX " + str(LGPMTInterfaceOpening) + " mm" + "\n" 
                        Text += "/Det/LightGuidePMTInterfaceOpeningZ " + str(LGPMTInterfaceOpening) + " mm" + "\n" 
                        Text += "/Det/LightGuideQuartzInterfaceOpeningX " + str(QLGOInterfOpening) + " mm" + "\n" 
                        Text += "/Det/LightGuideQuartzInterfaceOpeningZ " + str(qt+7) + " mm" + "\n" 
                        Text += "/Det/QuartzSizeX " + str(QWidth) + " mm" + "\n"
                        Text += "/Det/QuartzSizeY " + str(QHeight) + " mm" + "\n"
                        Text += "/Det/QuartzSizeZ " + str(qt) + " mm" + "\n"
                        Text += "/Det/QuartzBevelSize " + str(QuartzBevelSize) + " mm" + "\n"
                        Text += "/Det/QuartzRotX " + str(QuartzRotX) + " deg" + "\n"   
                        Text += "/Det/PolarRotation " + str(PolarRotation) + " deg" + "\n" 
                        Text += "/Det/LightGuideQuartzToPMTOffset " + str(of) + " mm" + "\n"
                        Text += "#/Det/SetSegmentedRadDamageFlag" + "\n"  #currently turned off 
                        Text += "#/MOLLEROpt/QuartzRadDamage " + str(QuartzRadDamage) + "\n"  #currently turned off
                        Text += "/Det/UpdateGeometry" + "\n"
                        Text += "/Generator/EventHitRegion "+str(hr) + "\n"
                        Text += "/RunAction/SetID " + str(RunID) + "\n"
                        Text += "/RunAction/SetOutputName " + FileIDString + "\n"
                        Text += "/random/setSeeds " + str(RndSeed1) + " " + str(RndSeed2) + "\n"
                        Text += "/run/beamOn " + str(NumEvents[hr-1]) + "\n"

                        FileName = OutputFilePrefix + FileIDString+".mac"
                        fout = open(datadir+FileName, "w")
                        fout.write(Text)
                        fout.close()
                        runscript += "./MOLLEROpt " + datadir + FileName + " > " + datadir + OutputFilePrefix + FileIDString + ".out" + " & \n"


scfile = open("StartRuns","w")
scfile.write(runscript)
scfile.close()

        
