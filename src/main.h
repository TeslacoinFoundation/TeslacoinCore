// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef BITCOIN_MAIN_H
#define BITCOIN_MAIN_H

#include "bignum.h"
#include "sync.h"
#include "net.h"
#include "script.h"
#include "scrypt_mine.h"


#include <list>

class CWallet;
class CWalletTx;
class CBlockIndex;
class CKeyItem;
class CReserveKey;
class COutPoint;

class CAddress;
class CInv;
class CRequestTracker;
class CNode;
class CBlock;
class CBlockLocator;

static const int CUTOFF_POW_BLOCK = 465000;

static const unsigned int MAX_BLOCK_SIZE = 1000000;
static const unsigned int MAX_BLOCK_SIZE_GEN = MAX_BLOCK_SIZE/2;
static const unsigned int MAX_BLOCK_SIGOPS = MAX_BLOCK_SIZE/50;
static const unsigned int MAX_ORPHAN_TRANSACTIONS = MAX_BLOCK_SIZE/100;
static const unsigned int MAX_INV_SZ = 50000;
static const int64 MIN_TX_FEE = 0.1 * CENT;
static const int64 MIN_RELAY_TX_FEE = 0.1 * CENT;
static const int64 MAX_MONEY = 299792458 * COIN; //Max initial coins 299,792,458
static const int64 MAX_MINT_PROOF_OF_WORK = 9000000 * COIN;
static const int64 MAX_MINT_PROOF_OF_WORK_LEGACY = 9000000 * COIN;

static const int64 MAX_MINT_PROOF_OF_STAKE = 1 * CENT;

static const int64 MIN_TXOUT_AMOUNT = MIN_TX_FEE;
static const unsigned int PROTOCOL_SWITCH_TIME = 1371686400; // 20 Jun 2013 00:00:00

static const unsigned int STAKE_SWITCH_TIME = 1404913117; // Wed, 28 May 2014 00:00:00 GMT

static const unsigned int REWARD_SWITCH_TIME = 1369432800; // 25 May 2013 00:00:00

static const unsigned int ROUND_SWITCH_TIME = 1375747200; // 6 August 2013 00:00:00

inline bool MoneyRange(int64 nValue) { return (nValue >= 0 && nValue <= MAX_MONEY); }
// Threshold for nLockTime: below this value it is interpreted as block number, otherwise as UNIX timestamp.
static const unsigned int LOCKTIME_THRESHOLD = 500000000; // Tue Nov  5 00:53:20 1985 UTC

#ifdef USE_UPNP
static const int fHaveUPnP = true;
#else
static const int fHaveUPnP = false;
#endif

static const uint256 hashGenesisBlockOfficial("0x000002d236d1bd8aa49f17e98117db81491d2127c5621464258e7542d95aba77");
static const uint256 hashGenesisBlockTestNet("0x");

static const int64 nMaxClockDrift = 2 * 60 * 60;        // two hours
extern CMedianFilter<int> cPeerBlockCounts; // Amount of blocks that other nodes claim to have


extern std::string strMintMessage;
extern std::string strMintWarning;


extern CScript COINBASE_FLAGS;

extern std::map<uint256, CDataStream*> mapOrphanTransactions;
extern std::map<uint256, std::map<uint256, CDataStream*> > mapOrphanTransactionsByPrev;

extern CCriticalSection cs_main;
extern std::map<uint256, CBlockIndex*> mapBlockIndex;
extern std::set<std::pair<COutPoint, unsigned int> > setStakeSeen;
extern uint256 hashGenesisBlock;
extern CBlockIndex* pindexGenesisBlock;
extern unsigned int nStakeMinAge;
extern int nCoinbaseMaturity;
extern int nBestHeight;
extern uint256 nBestChainTrust;
extern uint256 nBestInvalidTrust;
extern uint256 hashBestChain;
extern CBlockIndex* pindexBest;
extern unsigned int nTransactionsUpdated;
extern uint64 nLastBlockTx;
extern uint64 nLastBlockSize;
extern int64 nLastCoinStakeSearchInterval;
extern const std::string strMessageMagic;
extern double dHashesPerSec;
extern int64 nHPSTimerStart;
extern int64 nTimeBestReceived;
extern CCriticalSection cs_setpwalletRegistered;
extern std::set<CWallet*> setpwalletRegistered;
extern unsigned char pchMessageStart[4];
extern std::map<uint256, CBlock*> mapOrphanBlocks;
extern std::map<uint256, uint256> mapProofOfStake;
extern CBlockIndex* pblockindexFBBHLast;


// Settings
extern int64 nTransactionFee;

// Minimum disk space required - used in CheckDiskSpace()
static const uint64 nMinDiskSpace = 52428800;


class CReserveKey;
class CTxDB;
class CTxIndex;

void RegisterWallet(CWallet* pwalletIn);
void UnregisterWallet(CWallet* pwalletIn);
void SyncWithWallets(const CTransaction& tx, const CBlock* pblock = NULL, bool fUpdate = false, bool fConnect = true);
bool ProcessBlock(CNode* pfrom, CBlock* pblock);
bool CheckDiskSpace(uint64 nAdditionalBytes=0);
FILE* OpenBlockFile(unsigned int nFile, unsigned int nBlockPos, const char* pszMode="rb");
FILE* AppendBlockFile(unsigned int& nFileRet);
bool LoadBlockIndex(bool fAllowNew=true);
void PrintBlockTree();
CBlockIndex* FindBlockByHeight(int nHeight);
bool ProcessMessages(CNode* pfrom);
bool SendMessages(CNode* pto, bool fSendTrickle);
bool LoadExternalBlockFile(FILE* fileIn);
void GenerateBitcoins(bool fGenerate, CWallet* pwallet);
CBlock* CreateNewBlock(CWallet* pwallet, bool fProofOfStake=false);
void IncrementExtraNonce(CBlock* pblock, CBlockIndex* pindexPrev, unsigned int& nExtraNonce);
void FormatHashBuffers(CBlock* pblock, char* pmidstate, char* pdata, char* phash1);
bool CheckWork(CBlock* pblock, CWallet& wallet, CReserveKey& reservekey);
bool CheckProofOfWork(uint256 hash, unsigned int nBits);
int64 GetProofOfWorkReward(unsigned int nBits);
int64 GetProofOfStakeReward(int64 nCoinAge, unsigned int nBits, unsigned int nTime);
unsigned int ComputeMinWork(unsigned int nBase, int64 nTime);
int GetNumBlocksOfPeers();
bool IsInitialBlockDownload();
std::string GetWarnings(std::string strFor);
bool GetTransaction(const uint256 &hash, CTransaction &tx, uint256 &hashBlock);
uint256 WantedByOrphan(const CBlock* pblockOrphan);
const CBlockIndex* GetLastBlockIndex(const CBlockIndex* pindex, bool fProofOfStake);
void TeslaMiner(CWallet *pwallet, bool fProofOfStake);
void ResendWalletTransactions();

unsigned int GetNextTargetRequired(const CBlockIndex* pindexLast, bool fProofOfStake);
bool AddOrphanTx(const CDataStream& vMsg);
void EraseOrphanTx(uint256 hash);
unsigned int LimitOrphanTxSize(unsigned int nMaxOrphans);
void Inventory(const uint256& hash);
uint256 GetOrphanRoot(const CBlock* pblock);
bool GetTransaction(const uint256& hashTx, CWalletTx& wtx);


void SetBestChain(const CBlockLocator& loc);
void UpdatedTransaction(const uint256& hashTx);
void PrintWallets(const CBlock& block);
bool Reorganize(CTxDB& txdb, CBlockIndex* pindexNew);







bool GetWalletFile(CWallet* pwallet, std::string &strWalletFileOut);





#endif
