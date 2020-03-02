// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "validationinterface.h"

#include "chainparams.h"
#include "init.h"
#include "main.h"
#include "txmempool.h"
#include "ui_interface.h"

#include <boost/thread.hpp>

#include <chrono>
#include <thread>

static CMainSignals g_signals;

CMainSignals& GetMainSignals()
{
    return g_signals;
}

void RegisterValidationInterface(CValidationInterface* pwalletIn) {
    g_signals.UpdatedBlockTip.connect(boost::bind(&CValidationInterface::UpdatedBlockTip, pwalletIn, _1));
    g_signals.SyncTransaction.connect(boost::bind(&CValidationInterface::SyncTransaction, pwalletIn, _1, _2));
    g_signals.EraseTransaction.connect(boost::bind(&CValidationInterface::EraseFromWallet, pwalletIn, _1));
    g_signals.UpdatedTransaction.connect(boost::bind(&CValidationInterface::UpdatedTransaction, pwalletIn, _1));
    g_signals.RescanWallet.connect(boost::bind(&CValidationInterface::RescanWallet, pwalletIn));
    g_signals.ChainTip.connect(boost::bind(&CValidationInterface::ChainTip, pwalletIn, _1, _2, _3));
    g_signals.SetBestChain.connect(boost::bind(&CValidationInterface::SetBestChain, pwalletIn, _1));
    g_signals.Inventory.connect(boost::bind(&CValidationInterface::Inventory, pwalletIn, _1));
    g_signals.Broadcast.connect(boost::bind(&CValidationInterface::ResendWalletTransactions, pwalletIn, _1));
    g_signals.BlockChecked.connect(boost::bind(&CValidationInterface::BlockChecked, pwalletIn, _1, _2));
}

void UnregisterValidationInterface(CValidationInterface* pwalletIn) {
    g_signals.BlockChecked.disconnect(boost::bind(&CValidationInterface::BlockChecked, pwalletIn, _1, _2));
    g_signals.Broadcast.disconnect(boost::bind(&CValidationInterface::ResendWalletTransactions, pwalletIn, _1));
    g_signals.Inventory.disconnect(boost::bind(&CValidationInterface::Inventory, pwalletIn, _1));
    g_signals.ChainTip.disconnect(boost::bind(&CValidationInterface::ChainTip, pwalletIn, _1, _2, _3));
    g_signals.SetBestChain.disconnect(boost::bind(&CValidationInterface::SetBestChain, pwalletIn, _1));
    g_signals.UpdatedTransaction.disconnect(boost::bind(&CValidationInterface::UpdatedTransaction, pwalletIn, _1));
    g_signals.EraseTransaction.disconnect(boost::bind(&CValidationInterface::EraseFromWallet, pwalletIn, _1));
    g_signals.SyncTransaction.disconnect(boost::bind(&CValidationInterface::SyncTransaction, pwalletIn, _1, _2));
    g_signals.RescanWallet.disconnect(boost::bind(&CValidationInterface::RescanWallet, pwalletIn));
    g_signals.UpdatedBlockTip.disconnect(boost::bind(&CValidationInterface::UpdatedBlockTip, pwalletIn, _1));
}

void UnregisterAllValidationInterfaces() {
    g_signals.BlockChecked.disconnect_all_slots();
    g_signals.Broadcast.disconnect_all_slots();
    g_signals.Inventory.disconnect_all_slots();
    g_signals.ChainTip.disconnect_all_slots();
    g_signals.SetBestChain.disconnect_all_slots();
    g_signals.UpdatedTransaction.disconnect_all_slots();
    g_signals.EraseTransaction.disconnect_all_slots();
    g_signals.SyncTransaction.disconnect_all_slots();
    g_signals.RescanWallet.disconnect_all_slots();
    g_signals.UpdatedBlockTip.disconnect_all_slots();
}

void SyncWithWallets(const CTransaction &tx, const CBlock *pblock) {
    g_signals.SyncTransaction(tx, pblock);
}

struct CachedBlockData {
    CBlockIndex *pindex;
    std::pair<SproutMerkleTree, SaplingMerkleTree> oldTrees;
    std::list<CTransaction> txConflicted;

    CachedBlockData(
        CBlockIndex *pindex,
        std::pair<SproutMerkleTree, SaplingMerkleTree> oldTrees,
        std::list<CTransaction> txConflicted):
        pindex(pindex), oldTrees(oldTrees), txConflicted(txConflicted) {}
};

void ThreadNotifyWallets()
{
    CBlockIndex *pindexLastTip = nullptr;
    {
        LOCK(cs_main);
        pindexLastTip = chainActive.Tip();
    }

    while (true) {
        // Run the notifier on an integer second in the steady clock.
        auto now = std::chrono::steady_clock::now().time_since_epoch();
        auto nextFire = std::chrono::duration_cast<std::chrono::seconds>(
            now + std::chrono::seconds(1));
        std::this_thread::sleep_until(
            std::chrono::time_point<std::chrono::steady_clock>(nextFire));

        boost::this_thread::interruption_point();

        auto chainParams = Params();

        //
        // Collect all the state we require
        //

        // The common ancestor between the last chain tip we notified and the
        // current chain tip.
        const CBlockIndex *pindexFork;
        // The stack of blocks we will notify as having been connected.
        // Pushed in reverse, popped in order.
        std::vector<CachedBlockData> blockStack;
        // Transactions that have been recently conflicted out of the mempool.
        std::pair<std::map<CBlockIndex*, std::list<CTransaction>>, uint64_t> recentlyConflicted;
        // Transactions that have been recently added to the mempool.
        std::pair<std::vector<CTransaction>, uint64_t> recentlyAdded;

        {
            LOCK(cs_main);

            // Figure out the path from the last block we notified to the
            // current chain tip.
            CBlockIndex *pindex = chainActive.Tip();
            pindexFork = chainActive.FindFork(pindexLastTip);

            // Fetch recently-conflicted transactions. These will include any
            // block that has been connected since the last cycle, but we only
            // notify for the conflicts created by the current active chain.
        }
    }
}
void EraseFromWallets(const uint256 &hash) {
    g_signals.EraseTransaction(hash);
}

void RescanWallets() {
    g_signals.RescanWallet();
}