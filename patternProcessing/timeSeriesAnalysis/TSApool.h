#ifndef TSAPOOL_H

#define TSAPOOL_H

#include "TSAhashDefs.h"
#include "TSAdataStructs.h"


class TSApool
{
public:
    
    int K;
    float blackDur;
    TSAmotifInfo_t *priorityQDisc;
    TSAmotifInfoExt_t **priorityQSear;
    TSAmotifDataStorage_t **longTermDataStorage;
    TSAIND patternID;
    int *emptySpaceInd;
    
    TSApool();
    TSApool(int K, float blackDur);
    int initPriorityQDisc();
    int initPriorityQSear(TSAIND nQueries);
    
    TSADIST managePriorityQDisc(TSAsubSeq_t *subSeqPtr, TSAIND ind1, TSAIND ind2, TSADIST dist);
    TSADIST managePriorityQSear(TSAIND queryInd, TSAsubSeq_t *subSeqPtr, TSAIND ind1, TSAIND ind2, TSADIST dist, int searchFileID);
    
    int     updatePattStorageData(TSAIND queryInd, TSAsubSeq_t *subSeqPtr, int lenMotifReal, int searchFileID);
    int     initPattStorage(TSAIND nQueries, int lenMotifReal);
    
    int     sortQSearch(TSAIND queryInd);
    
    
};


#endif  //TSAPOOL_H