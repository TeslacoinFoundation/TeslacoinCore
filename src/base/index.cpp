#include "index.h"
#include "block.h"
#include "main.h"

using namespace std;
using namespace boost;

CBlockIndex::CBlockIndex(unsigned int nFileIn, unsigned int nBlockPosIn, CBlock& block)
{
    phashBlock = NULL;
    pprev = NULL;
    pnext = NULL;
    nFile = nFileIn;
    nBlockPos = nBlockPosIn;
    nHeight = 0;
    nChainTrust = 0;
    nMint = 0;
    nMoneySupply = 0;
    nFlags = 0;
    nStakeModifier = 0;
    nStakeModifierChecksum = 0;
    hashProofOfStake = 0;
    if (block.IsProofOfStake())
    {
        SetProofOfStake();
        prevoutStake = block.vtx[1].vin[0].prevout;
        nStakeTime = block.vtx[1].nTime;
    }
    else
    {
        prevoutStake.SetNull();
        nStakeTime = 0;
    }

    nVersion       = block.nVersion;
    hashMerkleRoot = block.hashMerkleRoot;
    nTime          = block.nTime;
    nBits          = block.nBits;
    nNonce         = block.nNonce;
}

CBlock CBlockIndex::GetBlockHeader() const
{
    CBlock block;
    block.nVersion       = nVersion;
    if (pprev)
        block.hashPrevBlock = pprev->GetBlockHash();
    block.hashMerkleRoot = hashMerkleRoot;
    block.nTime          = nTime;
    block.nBits          = nBits;
    block.nNonce         = nNonce;
    return block;
}

uint256 CBlockIndex::GetBlockHash() const
{
    return *phashBlock;
}


int64 CBlockIndex::GetBlockTime() const
{
    return (int64)nTime;
}


uint256 CBlockIndex::GetBlockTrust() const
{
    CBigNum bnTarget;
    bnTarget.SetCompact(nBits);
    if (bnTarget <= 0)
        return 0;
    return (IsProofOfStake()? ((CBigNum(1)<<256) / (bnTarget+1)).getuint256() : 1);
}

bool CBlockIndex::IsInMainChain() const
{
    return (pnext || this == pindexBest);
}

bool CBlockIndex::CheckIndex() const
{
    return true;
}

int64 CBlockIndex::GetMedianTimePast() const
{
    int64 pmedian[nMedianTimeSpan];
    int64* pbegin = &pmedian[nMedianTimeSpan];
    int64* pend = &pmedian[nMedianTimeSpan];

    const CBlockIndex* pindex = this;
    for (int i = 0; i < nMedianTimeSpan && pindex; i++, pindex = pindex->pprev)
        *(--pbegin) = pindex->GetBlockTime();

    std::sort(pbegin, pend);
    return pbegin[(pend - pbegin)/2];
}

int64 CBlockIndex::GetMedianTime() const
{
    const CBlockIndex* pindex = this;
    for (int i = 0; i < nMedianTimeSpan/2; i++)
    {
        if (!pindex->pnext)
            return GetBlockTime();
        pindex = pindex->pnext;
    }
    return pindex->GetMedianTimePast();
}

/**
 * Returns true if there are nRequired or more blocks of minVersion or above
 * in the last nToCheck blocks, starting at pstart and going backwards.
 */
bool CBlockIndex::IsSuperMajority(int minVersion, const CBlockIndex* pstart, unsigned int nRequired, unsigned int nToCheck)
{
    unsigned int nFound = 0;
    for (unsigned int i = 0; i < nToCheck && nFound < nRequired && pstart != NULL; i++)
    {
        if (pstart->nVersion >= minVersion)
            ++nFound;
        pstart = pstart->pprev;
    }
    return (nFound >= nRequired);
}


bool CBlockIndex::IsProofOfWork() const
{
    return !(nFlags & BLOCK_PROOF_OF_STAKE);
}

bool CBlockIndex::IsProofOfStake() const
{
    return (nFlags & BLOCK_PROOF_OF_STAKE);
}

void CBlockIndex::SetProofOfStake()
{
    nFlags |= BLOCK_PROOF_OF_STAKE;
}

unsigned int CBlockIndex::GetStakeEntropyBit() const
{
    return ((nFlags & BLOCK_STAKE_ENTROPY) >> 1);
}

bool CBlockIndex::SetStakeEntropyBit(unsigned int nEntropyBit)
{
    if (nEntropyBit > 1)
        return false;
    nFlags |= (nEntropyBit? BLOCK_STAKE_ENTROPY : 0);
    return true;
}

bool CBlockIndex::GeneratedStakeModifier() const
{
    return (nFlags & BLOCK_STAKE_MODIFIER);
}

void CBlockIndex::SetStakeModifier(uint64 nModifier, bool fGeneratedStakeModifier)
{
    nStakeModifier = nModifier;
    if (fGeneratedStakeModifier)
        nFlags |= BLOCK_STAKE_MODIFIER;
}

std::string CBlockIndex::ToString() const
{
    return strprintf("CBlockIndex(nprev=%p, pnext=%p, nFile=%u, nBlockPos=%-6d nHeight=%d, nMint=%s, nMoneySupply=%s, nFlags=(%s)(%d)(%s), nStakeModifier=%016" PRI64x ", nStakeModifierChecksum=%08x, hashProofOfStake=%s, prevoutStake=(%s), nStakeTime=%d merkle=%s, hashBlock=%s)",
        pprev, pnext, nFile, nBlockPos, nHeight,
        FormatMoney(nMint).c_str(), FormatMoney(nMoneySupply).c_str(),
        GeneratedStakeModifier() ? "MOD" : "-", GetStakeEntropyBit(), IsProofOfStake()? "PoS" : "PoW",
        nStakeModifier, nStakeModifierChecksum,
        hashProofOfStake.ToString().c_str(),
        prevoutStake.ToString().c_str(),
        nStakeTime,
        hashMerkleRoot.ToString().c_str(),
        GetBlockHash().ToString().c_str());
}

void CBlockIndex::print() const
{
    printf("%s\n", ToString().c_str());
}
