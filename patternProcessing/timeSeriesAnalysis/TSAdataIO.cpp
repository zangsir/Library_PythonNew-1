
#include "TSAdataIO.h"

using namespace std;

TSAparamHandle::TSAparamHandle()
{
    
    memset(fileExts.tsFileExt, '\0', sizeof(char)*MAX_FEXT_CHARS);
    memset(fileExts.tonicExt, '\0', sizeof(char)*MAX_FEXT_CHARS);
    memset(fileExts.blackTimeExt, '\0', sizeof(char)*MAX_FEXT_CHARS);
    memset(fileExts.logFileExt, '\0', sizeof(char)*MAX_FEXT_CHARS);
    memset(fileExts.paramsDumpExt, '\0', sizeof(char)*MAX_FEXT_CHARS);
    memset(fileExts.outFileExt, '\0', sizeof(char)*MAX_FEXT_CHARS);
    memset(fileExts.mappFileExt, '\0', sizeof(char)*MAX_FEXT_CHARS);
    memset(fileExts.searchListExt, '\0', sizeof(char)*MAX_FEXT_CHARS);
    memset(fileExts.queryFileExt, '\0', sizeof(char)*MAX_FEXT_CHARS);

}

TSAparamHandle::~TSAparamHandle()
{
    free(procParams.simMeasureRankRefinement);
    for(int ii=0; ii< procParams.nInterpFac; ii++)
    {
        free(procParams.combMTX[ii]);
    }
    free(procParams.combMTX);
}
 
int TSAparamHandle::readParamsFromFile(char *paramFile)
{
    FILE *fp;
    char tempFilename[400]={'\0'};
    char field[100]={'\0'};
    char value[100]={'\0'};
    
    fp = fopen(paramFile, "r");
    if (fp == NULL){printf("Error opening file %s\n", paramFile);exit(0);}
    while(fgets(tempFilename, 400, fp))
    {
        sscanf(tempFilename, "%s %s\n", field, value);
        if (strcmp(field, "durMotif:")==0){procParams.durMotif=atof(value);}
        if (strcmp(field, "blackDurFactor:")==0){procParams.blackDur=atof(value)*procParams.durMotif;}
        if (strcmp(field, "dsFactor:")==0){procParams.dsFactor=atoi(value);}
        if (strcmp(field, "binsPOct:")==0){procParams.binsPOct=atoi(value);}
        if (strcmp(field, "minPossiblePitch:")==0){procParams.minPossiblePitch=atof(value);}
        if (strcmp(field, "varDur:")==0){procParams.varDur=atof(value);}
        if (strcmp(field, "threshold:")==0){procParams.threshold=atof(value);}
        if (strcmp(field, "flatThreshold:")==0){procParams.flatThreshold=atof(value);}
        if (strcmp(field, "maxPauseDur:")==0){procParams.maxPauseDur=atof(value);}
        if (strcmp(field, "DTWBand:")==0){procParams.DTWBand=atof(value);}
        if (strcmp(field, "nInterpFac:")==0){procParams.nInterpFac=atoi(value);}
        if (strcmp(field, "SimMeasuresUsed:")==0){procParams.SimMeasuresUsed=atoi(value);}
        if (strcmp(field, "removeTaniSegs:")==0){procParams.removeTaniSegs=atoi(value);}
        if (strcmp(field, "dumpLogs:")==0){procParams.dumpLogs=atoi(value);}
        if (strcmp(field, "maxNMotifsPairs:")==0){procParams.maxNMotifsPairs=atoi(value);}
    }
    fclose(fp);
    
    if (procParams.SimMeasuresUsed>0)
    {
        procParams.simMeasureRankRefinement = (int*)malloc(sizeof(int)*1);
        procParams.simMeasureRankRefinement[0] = procParams.SimMeasuresUsed;
        procParams.nSimMeasuresUsed =1;
    }
    else    //In such case use 4 different similarity measures needed for experimentation
    {
        procParams.simMeasureRankRefinement = (int*)malloc(sizeof(int)*4);
        procParams.simMeasureRankRefinement[0] = SqEuclidean;
        procParams.simMeasureRankRefinement[1] = CityBlock;
        procParams.simMeasureRankRefinement[2] = ShiftCityBlock;
        procParams.simMeasureRankRefinement[3] = ShiftLinExp;
        procParams.nSimMeasuresUsed =4;
    }
    
    int ii, jj;
    
    if (procParams.nInterpFac==1)
    {
        procParams.interpFac[0]=1.0;
        
        procParams.combMTX = (int **)malloc(sizeof(int*)*procParams.nInterpFac);
        for(ii=0;ii<procParams.nInterpFac;ii++)
        {
            procParams.combMTX[ii] =  (int *)malloc(sizeof(int)*procParams.nInterpFac);
            for(jj=0;jj<procParams.nInterpFac;jj++)
            {
                procParams.combMTX[ii][jj] = 1;
            }
        }
    }
    else if (procParams.nInterpFac==3)
    {
        procParams.interpFac[0]=0.9;
        procParams.interpFac[1]=1.0;
        procParams.interpFac[2]=1.1;
        
        procParams.combMTX = (int **)malloc(sizeof(int*)*procParams.nInterpFac);
        for(ii=0;ii<procParams.nInterpFac;ii++)
        {
            procParams.combMTX[ii] =  (int *)malloc(sizeof(int)*procParams.nInterpFac);
            for(jj=0;jj<procParams.nInterpFac;jj++)
            {
                procParams.combMTX[ii][jj] = combAllwd_3[ii][jj];
            }
        }
        
        
    }
    else if (procParams.nInterpFac==5)
    {
        procParams.interpFac[0]=0.9;
        procParams.interpFac[1]=0.95;
        procParams.interpFac[2]=1.0;
        procParams.interpFac[3]=1.05;
        procParams.interpFac[4]=1.1;
        
        procParams.combMTX = (int **)malloc(sizeof(int*)*procParams.nInterpFac);
        for(ii=0;ii<procParams.nInterpFac;ii++)
        {
            procParams.combMTX[ii] =  (int *)malloc(sizeof(int)*procParams.nInterpFac);
            for(jj=0;jj<procParams.nInterpFac;jj++)
            {
                procParams.combMTX[ii][jj] = combAllwd_5[ii][jj];
            }
        }
    }
    
    return 1;
}

int TSAparamHandle::readFileExtsInfoFile(char *fileExtsFile)
{
    FILE *fp;
    char tempFilename[400]={'\0'};
    char field[100]={'\0'};
    char value[100]={'\0'};
    
    fp = fopen(fileExtsFile, "r");
    if (fp == NULL){printf("Error opening file %s\n", fileExtsFile);exit(0);}
    while(fgets(tempFilename, 400, fp))
    {
        sscanf(tempFilename, "%s %s\n", field, value);
        if (strcmp(field, "tsFileExt:")==0){strcat(fileExts.tsFileExt, value);}
        if (strcmp(field, "tonicExt:")==0){strcat(fileExts.tonicExt, value);}
        if (strcmp(field, "blackTimeExt:")==0){strcat(fileExts.blackTimeExt, value);}
        if (strcmp(field, "logFileExt:")==0){strcat(fileExts.logFileExt, value);}
        if (strcmp(field, "paramsDumpExt:")==0){strcat(fileExts.paramsDumpExt, value);}
        if (strcmp(field, "outFileExt:")==0){strcat(fileExts.outFileExt, value);}
        if (strcmp(field, "mappFileExt:")==0){strcat(fileExts.mappFileExt, value);}
        if (strcmp(field, "searchListExt:")==0){strcat(fileExts.searchListExt, value);}
        if (strcmp(field, "queryFileExt:")==0){strcat(fileExts.queryFileExt, value);}
    }
    fclose(fp);
    
    return 1;
}
procParams_t* TSAparamHandle::getParamPtr()
{
    return &procParams;
};
fileExts_t* TSAparamHandle::getExtPtr()
{
    return &fileExts;
};

TSAdataHandler::TSAdataHandler(char *bName, procLogs_t *procLogs, fileExts_t *fileExts, procParams_t *pParams)
{
    
    procLogPtr = procLogs;
    fileExtPtr = fileExts;
    procParams = *pParams;
    baseName = bName;
    
    fHandle.initialize(baseName, fileExtPtr);
    
    isBlackListAlloc=-1;
    nQueries=-1;
	
}
TSAdataHandler::~TSAdataHandler()
{
    free(samPtr);
    for(TSAIND ii=0; ii< nSubSeqs; ii++)
    {
        free(subSeqPtr[ii].pData);
        free(subSeqPtr[ii].pTStamps);
    }
    free(subSeqPtr);
    if (isBlackListAlloc==1)
    {free(blacklist);}
    if(nQueries>0)
    {free(queryTStamps);}
    
}

int TSAdataHandler::freeSubSeqsMem()
{
    for(TSAIND ii=0; ii< nSubSeqs; ii++)
    {
        free(subSeqPtr[ii].pData);
        free(subSeqPtr[ii].pTStamps);
    }
    free(subSeqPtr);
}


int TSAdataHandler::loadMotifDataTemplate1()
{
     //read the time series data    
    readTSData(fHandle.getTSFileName());
    readHopSizeTS(fHandle.getTSFileName());
    
    //downsample
    downSampleTS();
    
    //remove silence pitch regions
    filterSamplesTS();
    
    convertHz2Cents(fHandle.getTonicFileName());
    
    //calculate different motif lengths before doing sliding window candidate generation
    calculateDiffMotifLengths();
    
    readQueryTimeStamps(fHandle.getQueryFileName(), MOTIFPAIR_DUMP_FORMAT);
    
    genSubSeqsWithTStarts(queryTStamps, nQueries);
    
    genUniScaledSubSeqs();
    
    
}

int TSAdataHandler::genSubSeqsWithTStamps(TSAseg_t *qTStamps, TSAIND nQueries)
{
    TSAIND *seedMotifStrInd = (TSAIND*)malloc(sizeof(TSAIND)*nQueries);
    TSAIND *seedMotifEndInd = (TSAIND*)malloc(sizeof(TSAIND)*nQueries);
    
    for(TSAIND ii=0;ii<nQueries;ii++)
    {
        TSADIST min_valS = INF;
        TSADIST min_valE = INF;
        for(TSAIND jj=0;jj<lenTS;jj++)
        {
            if (fabs(samPtr[jj].tStamp-qTStamps[ii].sTime) < min_valS)
            {
                min_valS = fabs(samPtr[jj].tStamp-qTStamps[ii].sTime);
                seedMotifStrInd[ii] = jj;
            }
            if (fabs(samPtr[jj].tStamp-qTStamps[ii].eTime) < min_valE)
            {
                min_valE = fabs(samPtr[jj].tStamp-qTStamps[ii].eTime);
                seedMotifEndInd[ii] = jj;
            }
        }
    }
    subSeqPtr = (TSAsubSeq_t *)malloc(sizeof(TSAsubSeq_t)*nQueries);
    
    for(TSAIND ii=0;ii<nQueries;ii++)
    {
                
        //whats the length of this motif
        procParams.durMotif = samPtr[seedMotifEndInd[ii]].tStamp - samPtr[seedMotifStrInd[ii]].tStamp;
        calculateDiffMotifLengths();
        int lenRawMotifData = procParams.motifLengths[procParams.indexMotifLenLongest];
        
        subSeqPtr[ii].pData = (TSADATA *)malloc(sizeof(TSADATA)*lenRawMotifData);
        subSeqPtr[ii].pTStamps = (float *)malloc(sizeof(float)*lenRawMotifData);
        
        //at this point in time just store the original length start end without uniform scaling
        subSeqPtr[ii].sTime = samPtr[seedMotifStrInd[ii]].tStamp;
        subSeqPtr[ii].eTime = samPtr[seedMotifEndInd[ii]].tStamp;
        
        for(TSAIND jj=0;jj<lenRawMotifData;jj++)
        {
            subSeqPtr[ii].pData[jj]=samPtr[seedMotifStrInd[ii]+jj].value;
            subSeqPtr[ii].pTStamps[jj]=samPtr[seedMotifStrInd[ii]+jj].tStamp;
        }
    }
    nSubSeqs = nQueries;
    free(seedMotifStrInd);
    free(seedMotifEndInd);
    
    return 1;
    
    
}

int TSAdataHandler::genSubSeqsWithTStarts(TSAseg_t *qTStamps, TSAIND nQueries)
{
    int lenRawMotifData = procParams.motifLengths[procParams.indexMotifLenLongest];
    
    TSAIND *seedMotifInd = (TSAIND*)malloc(sizeof(TSAIND)*nQueries);
    
    for(TSAIND ii=0;ii<nQueries;ii++)
    {
        TSADIST min_val = INF;
        for(TSAIND jj=0;jj<lenTS;jj++)
        {
            if (fabs(samPtr[jj].tStamp-qTStamps[ii].sTime)<min_val)
            {
                min_val = fabs(samPtr[jj].tStamp-qTStamps[ii].sTime);
                seedMotifInd[ii] = jj;
            }
        }
    }
    
    subSeqPtr = (TSAsubSeq_t *)malloc(sizeof(TSAsubSeq_t)*nQueries);
    
    for(TSAIND ii=0;ii<nQueries;ii++)
    {
        
        subSeqPtr[ii].pData = (TSADATA *)malloc(sizeof(TSADATA)*lenRawMotifData);
        subSeqPtr[ii].pTStamps = (float *)malloc(sizeof(float)*lenRawMotifData);
        
        subSeqPtr[ii].sTime = samPtr[seedMotifInd[ii]].tStamp;
        subSeqPtr[ii].eTime = samPtr[seedMotifInd[ii]+lenRawMotifData-1].tStamp;
        
        for(TSAIND jj=0;jj<lenRawMotifData;jj++)
        {
            subSeqPtr[ii].pData[jj]=samPtr[seedMotifInd[ii]+jj].value;
            subSeqPtr[ii].pTStamps[jj]=samPtr[seedMotifInd[ii]+jj].tStamp;
        }
    }
    nSubSeqs = nQueries;
    free(seedMotifInd);
    
    return 1;
    
    
}

int TSAdataHandler::readQueryTimeStamps(char *queryFileName, int format)
{
    FILE *fp;
    TSAseg_t *qTStamps;
    
    if (format == MOTIFPAIR_DUMP_FORMAT)
    {
        float temp[10]={0};
        TSAIND ii=0, jj=0;
        TSADIST temp4;
        fp = fopen(queryFileName,"r");
        if (fp==NULL)
        {
            printf("Error opening file %s\n", queryFileName);
            return 0;
        }
        //reading number of lines in the file
        int nLines = getNumLines(queryFileName);
        qTStamps = (TSAseg_t*)malloc(sizeof(TSAseg_t)*nLines*2);
        
        while(fscanf(fp,"%f\t%f\t%f\t%f\t%lf\t%f\t%f\n", &temp[0], &temp[1], &temp[2], &temp[3], &temp4, &temp[5], &temp[6])!=EOF)
        {
            if(ii>=nLines)
                break;
            
            if(temp4<INF)
            {
                qTStamps[jj].sTime=temp[0];
                qTStamps[jj].eTime=temp[1];
                jj++;
                
                qTStamps[jj].sTime=temp[2];
                qTStamps[jj].eTime=temp[3];
                jj++;
            }
            ii++;
        }
        fclose(fp);
        nQueries=jj;
        queryTStamps = qTStamps;
    }
    else if (format == VIGNESH_MOTIF_ANNOT_FORMAT)
    {
        float temp[10]={0};
        TSAIND ii=0, jj=0;
        char temp4[10]={'\0'};
        fp = fopen(queryFileName,"r");
        if (fp==NULL)
        {
            printf("Error opening file %s\n", queryFileName);
            return 0;
        }
        //reading number of lines in the file
        int nLines = getNumLines(queryFileName);
        qTStamps = (TSAseg_t*)malloc(sizeof(TSAseg_t)*nLines*1);
        
        while(fscanf(fp,"%f %f %s\n", &temp[0], &temp[1], temp4)!=EOF)
        {
            qTStamps[jj].sTime=temp[0];
            qTStamps[jj].eTime=temp[1]+qTStamps[jj].sTime;
            jj++;
        }
        fclose(fp);
        nQueries=jj;
        queryTStamps = qTStamps;
    }    
    
    
    return 1;
    
}

int TSAdataHandler::genTemplate1SubSeqs()
{
    //read the time series data    
    readTSData(fHandle.getTSFileName());
    readHopSizeTS(fHandle.getTSFileName());
    
    //downsample
    downSampleTS();
    
    //remove silence pitch regions
    filterSamplesTS();
    
    convertHz2Cents(fHandle.getTonicFileName());
	
    //calculate different motif lengths before doing sliding window candidate generation
	calculateDiffMotifLengths();
    
    genSlidingWindowSubSeqs();
    
    initializeBlackList();
    
    updateBLDurThsld();
    
    updateBLStdThsld();
    
    updateBLInvalidSegment(fHandle.getBlackListSegFileName());
    
    filterBlackListedSubSeqs();
    
    genUniScaledSubSeqs();

}

int TSAdataHandler::genUniScaledSubSeqs()
{
    int nInterpFac = procParams.nInterpFac;
    int lenMotifReal = procParams.motifLengths[procParams.indexMotifLenReal];
    
    TSAIND nSubSeqs_new = nSubSeqs*nInterpFac;
    
    TSAsubSeq_t *subSeqPtr_new = (TSAsubSeq_t *)malloc(sizeof(TSAsubSeq_t)*nSubSeqs_new);
    
    
    //we do interpolation as well
    float **indInterp = (float**)malloc(sizeof(float*)*nInterpFac);
    for (int jj=0;jj<nInterpFac;jj++)
    {
        indInterp[jj] = (float *)malloc(sizeof(float)*lenMotifReal);
        for (int ii=0;ii<lenMotifReal; ii++)
        {
            indInterp[jj][ii] = procParams.interpFac[jj]*ii ;
        }
    }
    TSAIND ind=0;
    for (TSAIND ii=0;ii<nSubSeqs;ii++)
    {
        for (int jj=0;jj<nInterpFac;jj++)
        {
            if (procParams.interpFac[jj]==1)
            {
                subSeqPtr_new[ind] = subSeqPtr[ii];
                subSeqPtr_new[ind].len = lenMotifReal;
                subSeqPtr_new[ind].sTime  = subSeqPtr[ii].pTStamps[0];
                subSeqPtr_new[ind].eTime  = subSeqPtr[ii].pTStamps[lenMotifReal-1];
                ind++;
            }
            else
            {
                subSeqPtr_new[ind].pData = (TSADATA *)malloc(sizeof(TSADATA)*lenMotifReal);
                subSeqPtr_new[ind].pTStamps = (float *)malloc(sizeof(float)*1); //just a dummy allocation
                subSeqPtr_new[ind].len = lenMotifReal;
                cubicInterpolate(subSeqPtr[ii].pData, subSeqPtr_new[ind].pData, indInterp[jj], lenMotifReal);
                subSeqPtr_new[ind].sTime  = subSeqPtr[ii].pTStamps[0];
                subSeqPtr_new[ind].eTime  = subSeqPtr[ii].pTStamps[procParams.motifLengths[jj]-1];
                ind++;
            }
        }
    }
    
    for (int jj=0;jj<nInterpFac;jj++)
    {
        free(indInterp[jj]);
    }
    free(indInterp);
    
    free(subSeqPtr);
    subSeqPtr = subSeqPtr_new;
    nSubSeqs = nSubSeqs_new;
    
}


int TSAdataHandler::filterBlackListedSubSeqs()
{
    
    //counting number of valid segments
    TSAIND cnt=0;
    for (TSAIND ii=0; ii<nSubSeqs; ii++)
    {
        if(blacklist[ii]==0)
        {
            cnt++;
        }
    }
    
    TSAIND nSubSeqs_new = cnt;
    
    TSAsubSeq_t *subSeqPtr_new = (TSAsubSeq_t *)malloc(sizeof(TSAsubSeq_t)*nSubSeqs_new);
    cnt=0;
    for(TSAIND ii=0;ii<nSubSeqs;ii++)
    {
        if(blacklist[ii]==0)
        {
            subSeqPtr_new[cnt] = subSeqPtr[ii];
            cnt++;
        }
        else
        {
            free(subSeqPtr[ii].pData);
            free(subSeqPtr[ii].pTStamps);
        }
    }
    free(subSeqPtr);
    subSeqPtr = subSeqPtr_new;
    nSubSeqs = nSubSeqs_new;
    
}

int TSAdataHandler::updateBLInvalidSegment(char *segmentFile)
{
    TSAIND ii,jj,nLines;
    FILE *fp;
    float temp[4] = {0};
    TSAPattern_t *invalidSegs;
    
    nLines = getNumLines(segmentFile);
    invalidSegs = (TSAPattern_t *)malloc(sizeof(TSAPattern_t)*nLines);
    fp =fopen(segmentFile,"r");
    if (fp==NULL)
    {
        printf("Error opening file %s\n", segmentFile);
        return 0;
    }
    //reading segments of the tani sections
    ii=0;
    while (fscanf(fp, "%f %f\n",&temp[0],&temp[1])!=EOF)
    {
        invalidSegs[ii].sTime = temp[0];
        invalidSegs[ii].eTime = temp[1];
        ii++;
    }
    fclose(fp);
    //blacklisting the indexes whose time stamps lie between these segments
    for(ii=0;ii<nSubSeqs;ii++)
    {
        for(jj=0;jj<nLines;jj++)
        {
            if ((subSeqPtr[ii].eTime >= invalidSegs[jj].sTime) && (subSeqPtr[ii].sTime <= invalidSegs[jj].eTime))
                blacklist[ii]=1;
        }
        
    }
    
    free(invalidSegs);
    
    return 1;
}

/*
 * This function computes a running variance of the input data
 */
int TSAdataHandler::computeStdTSLocal(float **std, int varSam)
{
    float *mean, *stdVec, temp[2]={0};
    TSAIND ii;
    int N;
    
    
    mean = (float *)malloc(sizeof(float)*lenTS);
    memset(mean,0,sizeof(float)*lenTS);
    stdVec = (float *)malloc(sizeof(float)*lenTS);
    memset(stdVec,0,sizeof(float)*lenTS);
    
    N = 2*varSam+1 ;    
    for(ii=0;ii<N;ii++)
    {
        mean[varSam] +=  samPtr[ii].value ;
    }    
    for(ii=varSam+1;ii<lenTS-varSam;ii++)
    {
        mean[ii] =  mean[ii-1] - samPtr[ii-(varSam+1)].value + samPtr[ii+varSam].value ;  //running mean
    }
    for(ii=0;ii<lenTS;ii++)
    {
        mean[ii] =  mean[ii]/N;  //running mean
    }
    
    //computing running variance
    for(ii=0;ii<N;ii++)
    {
        temp[0] = (samPtr[ii].value- mean[ii]);
        stdVec[varSam] += temp[0]*temp[0];
    } 
    for(ii=varSam+1;ii<lenTS-varSam;ii++)
    {
        temp[0] = samPtr[ii-(varSam+1)].value -mean[ii-(varSam+1)];
        stdVec[ii] =  stdVec[ii-1] - temp[0]*temp[0] ; 
        temp[1] = (samPtr[ii+varSam].value -mean[ii+varSam]);
        stdVec[ii] += temp[1]*temp[1] ;
    }
    for(ii=0;ii<lenTS;ii++)
    {
        stdVec[ii] =  sqrt(stdVec[ii]/N);
    }    
    
    *std = stdVec;
    free(mean);
    
}


int TSAdataHandler::updateBLStdThsld()
{
    //computing standard deviation
    float *stdVec;
    int varSam = (int)round(procParams.varDur/pHop);
    computeStdTSLocal(&stdVec, varSam);
    
    // Assigning weather a point belongs to a flat region or non flar region based on a threhsold
    for(int ii=varSam;ii<lenTS-varSam;ii++)
    {
        if (stdVec[ii]>procParams.threshold)
        {
            stdVec[ii] = 1;
        }
        else
        {
            stdVec[ii] = 0;
        }            
            
    }
    
     //blackCnt=lenTS;
    TSAIND ind=0;
    float ex=0;
    int lenRawMotifData = procParams.motifLengths[procParams.indexMotifLenLongest];
    int lenRawMotifDataM1 = lenRawMotifData-1;
    
    //############################## generating subsequences ##########################################
    while(ind<lenTS)
    {
        ex+=stdVec[ind];
        if (ind >= lenRawMotifDataM1)
        {
            if (blacklist[ind-lenRawMotifDataM1]==0)
            {
                if(ex < procParams.flatThreshold*(float)lenRawMotifData)
                {
                    
                    blacklist[ind-lenRawMotifDataM1]=1;
                }
                
            }
            ex -= stdVec[ind-lenRawMotifDataM1];
        }
       ind++;        
    }
     
    free(stdVec);
    return 1;
}

int TSAdataHandler::updateBLDurThsld()
{
    float start, end, maxMotifDur;
    
    maxMotifDur = (procParams.durMotif*procParams.interpFac[procParams.indexMotifLenLongest]) + procParams.maxPauseDur ; 
    
    for(int ind=0; ind<nSubSeqs; ind++)
    {
        start = samPtr[ind].tStamp;
        end = samPtr[ind+procParams.motifLengths[procParams.indexMotifLenLongest]-1].tStamp;
        
        if (fabs(start-end) > maxMotifDur)       //allow 200 ms pauses in total not more than that
        {
            blacklist[ind]=1;
        }
    }
    
    return 1;
}



int TSAdataHandler::initializeBlackList()
{
    blacklist = (int *)malloc(sizeof(int)*nSubSeqs);
    for(TSAIND ii=0; ii<nSubSeqs; ii++)
    {
        blacklist[ii]=0;
    }
    isBlackListAlloc=1;
    return 1;
}

int TSAdataHandler::genSlidingWindowSubSeqs()
{
    
    
    int lenRawMotifData = procParams.motifLengths[procParams.indexMotifLenLongest];
    int lenRawMotifDataM1 = lenRawMotifData-1;
    
    subSeqPtr = (TSAsubSeq_t *)malloc(sizeof(TSAsubSeq_t)*lenTS);
    
    TSAIND ind=0;
    float ex=0;
    
    //############################## generating subsequences ##########################################
    while(ind<lenTS)
    {
    
        if (ind < lenTS - lenRawMotifDataM1)
        {
            subSeqPtr[ind].sTime  = samPtr[ind].tStamp;
            subSeqPtr[ind].eTime  = samPtr[ind+procParams.motifLengths[procParams.indexMotifLenReal]-1].tStamp;

            subSeqPtr[ind].pData = (TSADATA *)malloc(sizeof(TSADATA)*lenRawMotifData);
            subSeqPtr[ind].len = lenRawMotifData;
            subSeqPtr[ind].pTStamps = (float *)malloc(sizeof(float)*lenRawMotifData);
        }
        
        for(TSAIND ll = min(ind, lenTS - lenRawMotifDataM1-1) ; ll >= max(0,ind-lenRawMotifDataM1) ; ll--)
        {
            subSeqPtr[ll].pData[ind-ll] = samPtr[ind].value; 
            subSeqPtr[ll].pTStamps[ind-ll] = samPtr[ind].tStamp;
        }
        
       ind++;        
    }
    nSubSeqs = lenTS - lenRawMotifDataM1;
    
    return 1;

}

int TSAdataHandler::calculateDiffMotifLengths()
{
    int ii,jj;
    //######### computing all the motif lengths for several interpolation factors ##############
    int max_factor=-1;
    for (ii=0;ii<procParams.nInterpFac; ii++)
    {
        procParams.motifLengths[ii] = (int)ceil((procParams.durMotif*procParams.interpFac[ii])/pHop);
        if (procParams.interpFac[ii]==1)
        {
            procParams.indexMotifLenReal = ii;
        }
        if (procParams.interpFac[ii]>max_factor)
        {
            max_factor = procParams.interpFac[ii];
            procParams.indexMotifLenLongest = ii;
        }
    }
    
    //CRUCIAL POINT !!! since cubic interpolation needs 4 points (2 ahead) just store
    if (procParams.nInterpFac >1)
    {
        procParams.motifLengths[procParams.indexMotifLenLongest]+=1;
    }
    
}

int TSAdataHandler::convertHz2Cents(char *tonicFileName)
{
    float tonic;
    
    //Opening tonic file
    FILE *fp =fopen(tonicFileName,"r");
    if (fp==NULL)
    {
        printf("Error opening file %s\n", tonicFileName);
        return 0;
    }
    int nRead = fscanf(fp, "%f\n",&tonic);
    fclose(fp);
    
    float temp1 = ((float)procParams.binsPOct)/LOG2;
    
    for(int ii=0;ii<lenTS;ii++)
    {
        samPtr[ii].value = (TSADATA)(temp1*log((samPtr[ii].value+EPS)/tonic));
    }
    
    return 1;
}


int TSAdataHandler::filterSamplesTS()
{
    
    // There can be many filtering criterion. Currently what is implemented is to filter out silence regions of pitch sequence. 
    
    TSAIND nValidSamples = 0;
    TSAIND *validSampleInd = (TSAIND *)malloc(sizeof(TSAIND)*lenTS);
    //store all locations which have valid samples
    for(TSAIND ii=0;ii<lenTS; ii++)
    {
        if(samPtr[ii].value > procParams.minPossiblePitch)
        {
            validSampleInd[nValidSamples]=ii;
            nValidSamples++;
        }
    }
    // just copy valid samples to another location
    TSAIND lenTS_new = nValidSamples;
    TSAsam_t * samPtr_new = (TSAsam_t *)malloc(sizeof(TSAsam_t)*lenTS_new);
    for (TSAIND ii=0;ii<lenTS_new; ii++)
    {
        samPtr_new[ii] = samPtr[validSampleInd[ii]];
    }
    
    //free old memory
    free(samPtr);
    free(validSampleInd);
    samPtr = samPtr_new;
    lenTS = lenTS_new;
    
    return 1;
}

int TSAdataHandler::readHopSizeTS(char *fileName)
{
    FILE *fp;
    int nRead;
    float tsData;
    float tsTime1, tsTime2;
    
    // Opening time series file (JUST TO OBTAIN HOP SIZE)
    fp =fopen(fileName,"r");
    if (fp==NULL)
    {
        printf("Error opening file %s\n", fileName);
        return 0;
    }
    //reading just first two lines, in order to obtain hopsize//
    nRead = fscanf(fp, "%f\t%f\n",&tsTime1, &tsData);
    nRead = fscanf(fp, "%f\t%f\n",&tsTime2, &tsData);
    
    pHop = (tsTime2-tsTime1);
    fclose(fp);
    return 1;
}
int TSAdataHandler::downSampleTS()
{
    if (procParams.dsFactor <=0)
    {
        return 1;
    }
    
    TSAIND lenTS_new = ceil(lenTS/procParams.dsFactor);
    TSAsam_t * samPtr_new = (TSAsam_t *)malloc(sizeof(TSAsam_t)*lenTS_new);
    for (int ii=0;ii<lenTS_new; ii++)
    {
        samPtr_new[ii] = samPtr[ii*procParams.dsFactor];
    }
    
    //correcting hop size as well
    pHop = pHop*procParams.dsFactor;
    
    //free old memory
    free(samPtr);
    samPtr = samPtr_new;
    lenTS = lenTS_new;
    
    return 1;
}

int TSAdataHandler::readTSData(char *fileName)
{
    FILE *fp;
    float tsData;
    float tsTime;
    TSAIND cnt;
    
    
    //count number of lines in the file specified
    nLinesFile = getNumLines(fHandle.getTSFileName());
    procLogPtr->lenTS += nLinesFile;
    lenTS = nLinesFile;
    
    //allocate memory to store the time series data 
    samPtr = (TSAsam_t *)malloc(sizeof(TSAsam_t)*lenTS);
    fp =fopen(fileName,"r");
    if (fp==NULL)
    {
        printf("Error opening file %s\n", fileName);
        return 0;
    }
    cnt = 0;
    while (fscanf(fp, "%f\t%f\n",&tsTime, &tsData)!=EOF)    //read till the end of the file
    {
        samPtr[cnt].value  = tsData;
        samPtr[cnt].tStamp  = tsTime;
        cnt++;
    }
    fclose(fp);
    
    return 1;
}
    
    

/*
 * This function quickly returns number of lines in a text file
 * It uses memory mapp methods to perform this task. Its super fast as compared to reading and obtaining number of lines. 
 * By reading number of lines beforehand we can know how many lines are there in pitch file and we can then pre-allocate the memory to store pitch samples. Otherwise adaptively its really slow to read data (because that requires rellocation of buffers).
 */
TSAIND TSAdataHandler::getNumLines(const char *file)
{
    int fp;
    struct stat fs;
    char *buf;
    TSAIND line=0, ii;
    
    fp = open(file,O_RDONLY);
    if (fp == -1) 
    {
                printf("Error opening file1 %s\n",file);
                return 0;
    }
    if (fstat(fp, &fs) == -1)
    {
                printf("Error opening file2 %s\n",file);
                return 0;
    }
    buf = (char*)mmap(0, fs.st_size, PROT_READ, MAP_SHARED, fp, 0);
    
    for (ii=0;ii<fs.st_size;ii++)
    {
        if (buf[ii]=='\n')
            line++;
    }
    
    munmap(buf, fs.st_size);
    close(fp);
    
    return line;
    
}

int TSAdataHandler::dumpDiscMotifInfo(char *motifFile, TSAmotifInfo_t *priorityQDisc, int K, int verbos)
{
    FILE *fp;
    int ii;
    fp =fopen(motifFile,"w");
    for(ii=0;ii<K;ii++)
    {
        fprintf(fp, "%f\t%f\t%f\t%f\t%lf\t%lld\t%lld\n", subSeqPtr[priorityQDisc[ii].ind1].sTime, subSeqPtr[priorityQDisc[ii].ind1].eTime, subSeqPtr[priorityQDisc[ii].ind2].sTime, subSeqPtr[priorityQDisc[ii].ind2].eTime, priorityQDisc[ii].dist, priorityQDisc[ii].ind1, priorityQDisc[ii].ind2);
        if (verbos)
        {
            printf("motif pair is %f\t%f\t%f\t%lld\t%lld\n", subSeqPtr[priorityQDisc[ii].ind1].sTime,subSeqPtr[priorityQDisc[ii].ind2].sTime, priorityQDisc[ii].dist, priorityQDisc[ii].ind1%3, priorityQDisc[ii].ind2%3);
        }
    }
    fclose(fp);
    
    return 1;
}

int TSAdataHandler::dumpSearMotifInfo(char *motifFile, TSAmotifInfoExt_t **priorityQSear, TSAIND nQueries, int K, int verbos)
{
    FILE *fp;
    fp = fopen(motifFile, "w");
    
    for(TSAIND ii=0;ii<K;ii++)
    {
        for(TSAIND jj=0;jj < nQueries;jj++)
        {
            if ( priorityQSear[jj][ii].dist < INF)
            {
                fprintf(fp, "%f\t%f\t%f\t%f\t%f\t%d\t", subSeqPtr[priorityQSear[jj][ii].ind1].sTime, subSeqPtr[priorityQSear[jj][ii].ind1].eTime, priorityQSear[jj][ii].sTime, priorityQSear[jj][ii].eTime, priorityQSear[jj][ii].dist, priorityQSear[jj][ii].searchFileID);
            }
            else
            {
                fprintf(fp, "%f\t%f\t%f\t%f\t%f\t%d\t", -1.0,-1.0,-1.0,-1.0,-1.0,-1);
            }
            
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    
    return 1;
}


fileNameHandler::fileNameHandler()
{
    nSearchFiles=-1;
}
fileNameHandler::~fileNameHandler()
{
    if(nSearchFiles>0)
    {
        for(int ii=0; ii< nSearchFiles; ii++)
        {
            free(searchFileNames[ii]);
        }
        free(searchFileNames);
    }
}

int fileNameHandler::initialize(char *bName, fileExts_t *fExtPtr)
{
    baseName = bName;
    fileExtPtr = fExtPtr;
    
    return 1;
}

char* fileNameHandler::getTSFileName()
{
    memset(fileName, '\0', sizeof(char)*MAX_FNAME_CHARS);
    strcat(fileName, baseName);
    strcat(fileName, fileExtPtr->tsFileExt);
    
    return fileName;
    
}
char* fileNameHandler::getTonicFileName()
{
    memset(fileName, '\0', sizeof(char)*MAX_FNAME_CHARS);
    strcat(fileName, baseName);
    strcat(fileName, fileExtPtr->tonicExt);
    
    return fileName;
    
}
char* fileNameHandler::getBlackListSegFileName()
{
    memset(fileName, '\0', sizeof(char)*MAX_FNAME_CHARS);
    strcat(fileName, baseName);
    strcat(fileName, fileExtPtr->blackTimeExt);
    
    return fileName;
    
}
char* fileNameHandler::getLogFileName()
{
    memset(fileName, '\0', sizeof(char)*MAX_FNAME_CHARS);
    strcat(fileName, baseName);
    strcat(fileName, fileExtPtr->logFileExt);
    
    return fileName;
    
}
char* fileNameHandler::getParamDumpFileName()
{
    memset(fileName, '\0', sizeof(char)*MAX_FNAME_CHARS);
    strcat(fileName, baseName);
    strcat(fileName, fileExtPtr->paramsDumpExt);
    
    return fileName;
    
}
char* fileNameHandler::getOutFileName()
{
    memset(fileName, '\0', sizeof(char)*MAX_FNAME_CHARS);
    strcat(fileName, baseName);
    strcat(fileName, fileExtPtr->outFileExt);
    
    return fileName;
    
}
char* fileNameHandler::getOutFileNamePostRR(int similarityMeasure)
{
    memset(fileName, '\0', sizeof(char)*MAX_FNAME_CHARS);
    strcat(fileName, baseName);
    strcat(fileName, fileExtPtr->outFileExt);
    strcat(fileName, SimMeasureNames[similarityMeasure]);
    
    return fileName;
    
}

char* fileNameHandler::getMappFileName()
{
    memset(fileName, '\0', sizeof(char)*MAX_FNAME_CHARS);
    strcat(fileName, baseName);
    strcat(fileName, fileExtPtr->mappFileExt);
    
    return fileName;
    
}

char* fileNameHandler::getSearchListFileName()
{
    memset(fileName, '\0', sizeof(char)*MAX_FNAME_CHARS);
    strcat(fileName, baseName);
    strcat(fileName, fileExtPtr->searchListExt);
    
    return fileName;
}
char* fileNameHandler::getQueryFileName()
{
    memset(fileName, '\0', sizeof(char)*MAX_FNAME_CHARS);
    strcat(fileName, baseName);
    strcat(fileName, fileExtPtr->queryFileExt);
    
    return fileName;
}



int fileNameHandler::loadSearchFileList()
{
    char tempFilename[MAX_FNAME_CHARS];
    
    FILE *fp1 = fopen(getSearchListFileName(), "r");
    if (fp1==NULL)
    {
        printf("Error opening file %s\n", getSearchListFileName());
        return 0;
    }
    int ii=0;
    searchFileNames = (char **)malloc(sizeof(char *)*MAX_NUM_SEARCHFILES);
    while(fgets(tempFilename, MAX_FNAME_CHARS, fp1))
    {
        searchFileNames[ii] = (char *)malloc(sizeof(char)*MAX_FNAME_CHARS);
        sscanf(tempFilename, "%[^\n]s\n", searchFileNames[ii]);
        ii++;
    }
    nSearchFiles  = ii;
    fclose(fp1);
    return 1;
}

  
  
