// Copyright (c) 2015 The Guldencoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GULDENCOIN_DIFF_H
#define GULDENCOIN_DIFF_H

#include "../consensus/params.h"
#include "../arith_uint256.h"
#include "util.h"

#include "diff_kimoto.h"
#include "diff_DGW.h"
#include "diff_DIGI.h"
#include "diff_delta.h"



//Forward declare
unsigned int GetNextWorkRequired_original(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params&);

unsigned int static GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    static int nDeltaSwitchoverBlock = GetBoolArg("-testnet", false) ? 500 : (GetBoolArg("-testnetaccel", false) ? 1 : 213500) ;
    
    int DiffMode = 1;
    if (pindexLast->nHeight+1 >= nDeltaSwitchoverBlock)
            DiffMode = 5;
    else
    {    
        if (GetBoolArg("-testnet", false))
        {
            if (pindexLast->nHeight+1 >= 30)
                DiffMode = 4;
            else if (pindexLast->nHeight+1 >= 15)
                DiffMode = 3;
            else if (pindexLast->nHeight+1 >= 5)
                DiffMode = 2;
            else
                return GetNextWorkRequired_original(pindexLast, pblock, params);
        }
        else
        {
            if (pindexLast->nHeight+1 >= 194300)
                DiffMode = 4;
            else if (pindexLast->nHeight+1 >= 123793)
                DiffMode = 3;
            else if (pindexLast->nHeight+1 >= 10)
                DiffMode = 2; // KGW will kick in at block 10, so pretty much direct from start.
        }
    }

    switch(DiffMode)
    {
        // Original 'bitcoin' targetting algo - because diff never changed before this was switched out we can just return a hardcoded diff instead.
        case 1: return 504365040;
        case 2: return GetNextWorkRequired_KGW(pindexLast, pblock, params);
        case 3: return GetNextWorkRequired_DGW3(pindexLast, pblock, params);
        case 4: return GetNextWorkRequired_DIGI(pindexLast, pblock, params);
        case 5: return GetNextWorkRequired_DELTA(pindexLast, pblock, params, nDeltaSwitchoverBlock);
        default: return GetNextWorkRequired_DELTA(pindexLast, pblock, params, nDeltaSwitchoverBlock);
    }
}


#endif

