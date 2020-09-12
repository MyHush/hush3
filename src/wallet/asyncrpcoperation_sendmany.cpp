// Copyright (c) 2016 The Zcash developers
// Copyright (c) 2019-2020 The Hush developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://www.opensource.org/licenses/mit-license.php

/******************************************************************************
 * Copyright © 2014-2019 The SuperNET Developers.                             *
 *                                                                            *
 * See the AUTHORS, DEVELOPER-AGREEMENT and LICENSE files at                  *
 * the top-level directory of this distribution for the individual copyright  *
 * holder information and the developer policies on copyright and licensing.  *
 *                                                                            *
 * Unless otherwise agreed in a custom licensing agreement, no part of the    *
 * SuperNET software, including this file may be copied, modified, propagated *
 * or distributed except according to the terms contained in the LICENSE file *
 *                                                                            *
 * Removal or modification of this copyright notice is prohibited.            *
 *                                                                            *
 ******************************************************************************/

#include "asyncrpcoperation_sendmany.h"
#include "asyncrpcqueue.h"
#include "amount.h"
#include "consensus/upgrades.h"
#include "core_io.h"
#include "init.h"
#include "key_io.h"
#include "main.h"
#include "net.h"
#include "netbase.h"
#include "rpc/protocol.h"
#include "rpc/server.h"
#include "timedata.h"
#include "util.h"
#include "utilmoneystr.h"
#include "wallet.h"
#include "walletdb.h"
#include "script/interpreter.h"
#include "utiltime.h"
#include "zcash/IncrementalMerkleTree.hpp"
#include "sodium.h"
#include "miner.h"

#include <stdint.h>

#include <array>
#include <iostream>
#include <chrono>
#include <thread>
#include <string>

#include <boost/optional/optional_io.hpp>

using namespace libzcash;

extern char ASSETCHAINS_SYMBOL[65];

int32_t komodo_dpowconfs(int32_t height,int32_t numconfs);
int32_t komodo_blockheight(uint256 hash);
int tx_height( const uint256 &hash );
bool hush_hardfork_active(uint32_t time);
extern UniValue signrawtransaction(const UniValue& params, bool fHelp, const CPubKey& mypk);
extern UniValue sendrawtransaction(const UniValue& params, bool fHelp, const CPubKey& mypk);

AsyncRPCOperation_sendmany::AsyncRPCOperation_sendmany(
        boost::optional<TransactionBuilder> builder,
        CMutableTransaction contextualTx,
        std::string fromAddress,
        std::vector<SendManyRecipient> tOutputs,
        std::vector<SendManyRecipient> zOutputs,
        int minDepth,
        CAmount fee,
        UniValue contextInfo) :
        tx_(contextualTx), fromaddress_(fromAddress), t_outputs_(tOutputs), z_outputs_(zOutputs), mindepth_(minDepth), fee_(fee), contextinfo_(contextInfo)
{
    assert(fee_ >= 0);

    if (minDepth < 0) {
        throw JSONRPCError(RPC_INVALID_PARAMETER, "Minconf cannot be negative");
    }

    if (fromAddress.size() == 0) {
        throw JSONRPCError(RPC_INVALID_PARAMETER, "From address parameter missing");
    }

    if (tOutputs.size() == 0 && zOutputs.size() == 0) {
        throw JSONRPCError(RPC_INVALID_PARAMETER, "No recipients");
    }

    isUsingBuilder_ = false;
    if (builder) {
        isUsingBuilder_ = true;
        builder_ = builder.get();
    }

    fromtaddr_ = DecodeDestination(fromAddress);
    isfromtaddr_ = IsValidDestination(fromtaddr_);
    isfromzaddr_ = false;

    if (!isfromtaddr_) {
        auto address = DecodePaymentAddress(fromAddress);
        if (IsValidPaymentAddress(address)) {
            // We don't need to lock on the wallet as spending key related methods are thread-safe
            if (!boost::apply_visitor(HaveSpendingKeyForPaymentAddress(pwalletMain), address)) {
                throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Invalid from address, no spending key found for zaddr");
            }

            isfromzaddr_ = true;
            frompaymentaddress_ = address;
            spendingkey_ = boost::apply_visitor(GetSpendingKeyForPaymentAddress(pwalletMain), address).get();
        } else {
            throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Invalid from address");
        }
    }

    if (isfromzaddr_ && minDepth==0) {
        throw JSONRPCError(RPC_INVALID_PARAMETER, "Minconf cannot be zero when sending from zaddr");
    }

    // Log the context info i.e. the call parameters to z_sendmany
    if (LogAcceptCategory("zrpcunsafe")) {
        LogPrint("zrpcunsafe", "%s: z_sendmany initialized (params=%s)\n", getId(), contextInfo.write());
    } else {
        LogPrint("zrpc", "%s: z_sendmany initialized\n", getId());
    }
}

AsyncRPCOperation_sendmany::~AsyncRPCOperation_sendmany() {
}

void AsyncRPCOperation_sendmany::main() {
    if (isCancelled())
        return;

    set_state(OperationStatus::EXECUTING);
    start_execution_clock();

    bool success = false;

#ifdef ENABLE_MINING
  #ifdef ENABLE_WALLET
    GenerateBitcoins(false, NULL, 0);
  #else
    GenerateBitcoins(false, 0);
  #endif
#endif

    try {
        success = main_impl();
    } catch (const UniValue& objError) {
        int code = find_value(objError, "code").get_int();
        std::string message = find_value(objError, "message").get_str();
        set_error_code(code);
        set_error_message(message);
    } catch (const runtime_error& e) {
        set_error_code(-1);
        set_error_message("runtime error: " + string(e.what()));
    } catch (const logic_error& e) {
        set_error_code(-1);
        set_error_message("logic error: " + string(e.what()));
    } catch (const exception& e) {
        set_error_code(-1);
        set_error_message("general exception: " + string(e.what()));
    } catch (...) {
        set_error_code(-2);
        set_error_message("unknown error");
    }

#ifdef ENABLE_MINING
  #ifdef ENABLE_WALLET
    GenerateBitcoins(GetBoolArg("-gen",false), pwalletMain, GetArg("-genproclimit", 1));
  #else
    GenerateBitcoins(GetBoolArg("-gen",false), GetArg("-genproclimit", 1));
  #endif
#endif

    stop_execution_clock();

    if (success) {
        set_state(OperationStatus::SUCCESS);
    } else {
        set_state(OperationStatus::FAILED);
    }

    std::string s = strprintf("%s: z_sendmany finished (status=%s", getId(), getStateAsString());
    if (success) {
        s += strprintf(", txid=%s)\n", tx_.GetHash().ToString());
    } else {
        s += strprintf(", error=%s)\n", getErrorMessage());
    }
    LogPrintf("%s",s);
}

// Notes:
// 1. #1159 Currently there is no limit set on the number of shielded spends, so size of tx could be invalid.
// 2. #1360 Note selection is not optimal
// 3. #1277 Spendable notes are not locked, so an operation running in parallel could also try to use them
bool AsyncRPCOperation_sendmany::main_impl() {

    assert(isfromtaddr_ != isfromzaddr_);

    bool isSingleZaddrOutput = (t_outputs_.size()==0 && z_outputs_.size()==1);
    bool isMultipleZaddrOutput = (t_outputs_.size()==0 && z_outputs_.size()>=1);
    bool isPureTaddrOnlyTx = (isfromtaddr_ && z_outputs_.size() == 0);
    CAmount minersFee = fee_;

    // When spending coinbase utxos, you can only specify a single zaddr as the change must go somewhere
    // and if there are multiple zaddrs, we don't know where to send it.
    if (isfromtaddr_) {
        if (isSingleZaddrOutput) {
            bool b = find_utxos(true);
            if (!b) {
                throw JSONRPCError(RPC_WALLET_INSUFFICIENT_FUNDS, "Insufficient funds, no UTXOs found for taddr from address.");
            }
        } else {
            bool b = find_utxos(false);
            if (!b) {
                if (isMultipleZaddrOutput) {
                    throw JSONRPCError(RPC_WALLET_INSUFFICIENT_FUNDS, "Could not find any non-coinbase UTXOs to spend. Coinbase UTXOs can only be sent to a single zaddr recipient.");
                } else {
                    throw JSONRPCError(RPC_WALLET_INSUFFICIENT_FUNDS, "Could not find any non-coinbase UTXOs to spend.");
                }
            }
        }
    }

    if (isfromzaddr_ && !find_unspent_notes()) {
        throw JSONRPCError(RPC_WALLET_INSUFFICIENT_FUNDS, "Insufficient funds, no unspent notes found for zaddr from address.");
    }

    CAmount t_inputs_total = 0;
    for (SendManyInputUTXO & t : t_inputs_) {
        t_inputs_total += std::get<2>(t);
    }

    CAmount z_inputs_total = 0;
    for (auto t : z_sapling_inputs_) {
        z_inputs_total += t.note.value();
    }

    CAmount t_outputs_total = 0;
    for (SendManyRecipient & t : t_outputs_) {
        t_outputs_total += std::get<1>(t);
    }

    CAmount z_outputs_total = 0;
    for (SendManyRecipient & t : z_outputs_) {
        z_outputs_total += std::get<1>(t);
    }

    CAmount sendAmount = z_outputs_total + t_outputs_total;
    CAmount targetAmount = sendAmount + minersFee;

    assert(!isfromtaddr_ || z_inputs_total == 0);
    assert(!isfromzaddr_ || t_inputs_total == 0);

    if (isfromtaddr_ && (t_inputs_total < targetAmount)) {
        throw JSONRPCError(RPC_WALLET_INSUFFICIENT_FUNDS,
            strprintf("Insufficient transparent funds, have %s, need %s",
            FormatMoney(t_inputs_total), FormatMoney(targetAmount)));
    }

    if (isfromzaddr_ && (z_inputs_total < targetAmount)) {
        throw JSONRPCError(RPC_WALLET_INSUFFICIENT_FUNDS,
            strprintf("Insufficient shielded funds, have %s, need %s",
            FormatMoney(z_inputs_total), FormatMoney(targetAmount)));
    }

    // If from address is a taddr, select UTXOs to spend
    CAmount selectedUTXOAmount = 0;
    bool selectedUTXOCoinbase = false;
    if (isfromtaddr_) {
        // Get dust threshold
        CKey secret;
        secret.MakeNewKey(true);
        CScript scriptPubKey = GetScriptForDestination(secret.GetPubKey().GetID());
        CTxOut out(CAmount(1), scriptPubKey);
        CAmount dustThreshold = out.GetDustThreshold(minRelayTxFee);
        CAmount dustChange = -1;

        std::vector<SendManyInputUTXO> selectedTInputs;
        for (SendManyInputUTXO & t : t_inputs_) {
            bool b = std::get<3>(t);
            if (b) {
                selectedUTXOCoinbase = true;
            }
            selectedUTXOAmount += std::get<2>(t);
            selectedTInputs.push_back(t);
            if (selectedUTXOAmount >= targetAmount) {
                // Select another utxo if there is change less than the dust threshold.
                dustChange = selectedUTXOAmount - targetAmount;
                if (dustChange == 0 || dustChange >= dustThreshold) {
                    break;
                }
            }
        }

        // If there is transparent change, is it valid or is it dust?
        if (dustChange < dustThreshold && dustChange != 0) {
            throw JSONRPCError(RPC_WALLET_INSUFFICIENT_FUNDS,
                strprintf("Insufficient transparent funds, have %s, need %s more to avoid creating invalid change output %s (dust threshold is %s)",
                FormatMoney(t_inputs_total), FormatMoney(dustThreshold - dustChange), FormatMoney(dustChange), FormatMoney(dustThreshold)));
        }

        t_inputs_ = selectedTInputs;
        t_inputs_total = selectedUTXOAmount;

        // Check mempooltxinputlimit to avoid creating a transaction which the local mempool rejects
        size_t limit = (size_t)GetArg("-mempooltxinputlimit", 0);
        {
            LOCK(cs_main);
            if (NetworkUpgradeActive(chainActive.Height() + 1, Params().GetConsensus(), Consensus::UPGRADE_OVERWINTER)) {
                limit = 0;
            }
        }
        if (limit > 0) {
            size_t n = t_inputs_.size();
            if (n > limit) {
                throw JSONRPCError(RPC_WALLET_ERROR, strprintf("Too many transparent inputs %zu > limit %zu", n, limit));
            }
        }

        // update the transaction with these inputs
        if (isUsingBuilder_) {
            CScript scriptPubKey;
            for (auto t : t_inputs_) {
                scriptPubKey = GetScriptForDestination(std::get<4>(t));
                //printf("Checking new script: %s\n", scriptPubKey.ToString().c_str());
                uint256 txid = std::get<0>(t);
                int vout = std::get<1>(t);
                CAmount amount = std::get<2>(t);
                builder_.AddTransparentInput(COutPoint(txid, vout), scriptPubKey, amount);
            }
            // for Komodo, set lock time to accure interest, for other chains, set
            // locktime to spend time locked coinbases
            if (ASSETCHAINS_SYMBOL[0] == 0)
            {
                //if ((uint32_t)chainActive.LastTip()->nTime < ASSETCHAINS_STAKED_HF_TIMESTAMP)
                if ( !hush_hardfork_active((uint32_t)chainActive.LastTip()->nTime) )
                    builder_.SetLockTime((uint32_t)time(NULL) - 60); // set lock time for Komodo interest
                else
                    builder_.SetLockTime((uint32_t)chainActive.Tip()->GetMedianTimePast());
            }
        } else {
            CMutableTransaction rawTx(tx_);
            for (SendManyInputUTXO & t : t_inputs_) {
                uint256 txid = std::get<0>(t);
                int vout = std::get<1>(t);
                CAmount amount = std::get<2>(t);
                CTxIn in(COutPoint(txid, vout));
                rawTx.vin.push_back(in);
            }
            if (ASSETCHAINS_SYMBOL[0] == 0)
            {
                //if ((uint32_t)chainActive.LastTip()->nTime < ASSETCHAINS_STAKED_HF_TIMESTAMP)
                if ( !hush_hardfork_active((uint32_t)chainActive.LastTip()->nTime) )
                    rawTx.nLockTime = (uint32_t)time(NULL) - 60; // jl777
                else
                    rawTx.nLockTime = (uint32_t)chainActive.Tip()->GetMedianTimePast();
            }
            tx_ = CTransaction(rawTx);
        }
    }

    LogPrint((isfromtaddr_) ? "zrpc" : "zrpcunsafe", "%s: spending %s to send %s with fee %s\n",
            getId(), FormatMoney(targetAmount), FormatMoney(sendAmount), FormatMoney(minersFee));
    LogPrint("zrpc", "%s: transparent input: %s (to choose from)\n", getId(), FormatMoney(t_inputs_total));
    LogPrint("zrpcunsafe", "%s: private input: %s (to choose from)\n", getId(), FormatMoney(z_inputs_total));
    LogPrint("zrpc", "%s: transparent output: %s\n", getId(), FormatMoney(t_outputs_total));
    LogPrint("zrpcunsafe", "%s: private output: %s\n", getId(), FormatMoney(z_outputs_total));
    LogPrint("zrpc", "%s: fee: %s\n", getId(), FormatMoney(minersFee));


    /**
     * SCENARIO #0
     *
     * Sprout not involved, so we just use the TransactionBuilder and we're done.
     * We added the transparent inputs to the builder earlier.
     */
    if (isUsingBuilder_) {
        builder_.SetFee(minersFee);

        // Get various necessary keys
        SaplingExpandedSpendingKey expsk;
        uint256 ovk;
        if (isfromzaddr_) {
            auto sk = boost::get<libzcash::SaplingExtendedSpendingKey>(spendingkey_);
            expsk = sk.expsk;
            ovk = expsk.full_viewing_key().ovk;
        } else {
            // Sending from a t-address, which we don't have an ovk for. Instead,
            // generate a common one from the HD seed. This ensures the data is
            // recoverable, while keeping it logically separate from the ZIP 32
            // Sapling key hierarchy, which the user might not be using.
            HDSeed seed;
            if (!pwalletMain->GetHDSeed(seed)) {
                throw JSONRPCError(
                    RPC_WALLET_ERROR,
                    "AsyncRPCOperation_sendmany::main_impl(): HD seed not found");
            }
            ovk = ovkForShieldingFromTaddr(seed);
        }

        // Set change address if we are using transparent funds
        // TODO: Should we just use fromtaddr_ as the change address?
        if (isfromtaddr_) {
            LOCK2(cs_main, pwalletMain->cs_wallet);

            EnsureWalletIsUnlocked();
            CReserveKey keyChange(pwalletMain);
            CPubKey vchPubKey;
            bool ret = keyChange.GetReservedKey(vchPubKey);
            if (!ret) {
                // should never fail, as we just unlocked
                throw JSONRPCError(
                    RPC_WALLET_KEYPOOL_RAN_OUT,
                    "Could not generate a taddr to use as a change address");
            }

            CTxDestination changeAddr = vchPubKey.GetID();
            assert(builder_.SendChangeTo(changeAddr));
        }

        // Select Sapling notes
        if(fZdebug)
            LogPrintf("%s: Selecting Sapling notes\n", __FUNCTION__);
        std::vector<SaplingOutPoint> ops;
        std::vector<SaplingNote> notes;
        CAmount sum = 0;
        for (auto t : z_sapling_inputs_) {
            ops.push_back(t.op);
            notes.push_back(t.note);
            sum += t.note.value();
            if (sum >= targetAmount) {
                break;
            }
        }

        // Fetch Sapling anchor and witnesses
        //LogPrintf("%s: Gathering anchors and witnesses\n", __FUNCTION__);
        uint256 anchor;
        std::vector<boost::optional<SaplingWitness>> witnesses;
        {
            LOCK2(cs_main, pwalletMain->cs_wallet);
            pwalletMain->GetSaplingNoteWitnesses(ops, witnesses, anchor);
        }

        // Add Sapling spends
        for (size_t i = 0; i < notes.size(); i++) {
            if (!witnesses[i]) {
                throw JSONRPCError(RPC_WALLET_ERROR, "Missing witness for Sapling note");
            }
            assert(builder_.AddSaplingSpend(expsk, notes[i], anchor, witnesses[i].get()));
        }

        // Add Sapling outputs
        for (auto r : z_outputs_) {
            auto address = std::get<0>(r);
            auto value   = std::get<1>(r);
            auto hexMemo = std::get<2>(r);
            auto addr    = DecodePaymentAddress(address);
            assert(boost::get<libzcash::SaplingPaymentAddress>(&addr) != nullptr);
            auto to = boost::get<libzcash::SaplingPaymentAddress>(addr);
            if(fZdebug)
                LogPrintf("%s: Adding Sapling output to address %s\n", __FUNCTION__, address.c_str());

            auto memo = get_memo_from_hex_string(hexMemo);

            builder_.AddSaplingOutput(ovk, to, value, memo);
        }

        // Add transparent outputs
        for (auto r : t_outputs_) {
            auto outputAddress = std::get<0>(r);
            auto amount = std::get<1>(r);

            auto address = DecodeDestination(outputAddress);
            if (!builder_.AddTransparentOutput(address, amount)) {
                throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Invalid output address, not a valid taddr.");
            }
        }

        // Build the transaction
        auto maybe_tx = builder_.Build();
        if (!maybe_tx) {
            throw JSONRPCError(RPC_WALLET_ERROR, "Failed to build transaction.");
        }
        tx_ = maybe_tx.get();
        if(fZdebug)
            LogPrintf("%s: Raw transaction created\n", __FUNCTION__);

        // Send the transaction
        // TODO: Use CWallet::CommitTransaction instead of sendrawtransaction
        auto signedtxn = EncodeHexTx(tx_);
        if (!testmode) {
            UniValue params = UniValue(UniValue::VARR);
            params.push_back(signedtxn);
            if(fZdebug)
                LogPrintf("%s: Sending raw xtn with txid=%s\n", __FUNCTION__, tx_.GetHash().ToString().c_str());
            UniValue sendResultValue = sendrawtransaction(params, false, CPubKey());
            if (sendResultValue.isNull()) {
                throw JSONRPCError(RPC_WALLET_ERROR, "sendrawtransaction did not return an error or a txid.");
            }

            auto txid = sendResultValue.get_str();

            UniValue o(UniValue::VOBJ);
            o.push_back(Pair("txid", txid));
            set_result(o);
        } else {
            // Test mode does not send the transaction to the network.
            UniValue o(UniValue::VOBJ);
            o.push_back(Pair("test", 1));
            o.push_back(Pair("txid", tx_.GetHash().ToString()));
            o.push_back(Pair("hex", signedtxn));
            set_result(o);
        }

        return true;
    }
    /**
     * END SCENARIO #0
     */

    return false;
}


/**
 * Sign and send a raw transaction.
 * Raw transaction as hex string should be in object field "rawtxn"
 */
void AsyncRPCOperation_sendmany::sign_send_raw_transaction(UniValue obj)
{
    // Sign the raw transaction
    UniValue rawtxnValue = find_value(obj, "rawtxn");
    if (rawtxnValue.isNull()) {
        throw JSONRPCError(RPC_WALLET_ERROR, "Missing hex data for raw transaction");
    }
    std::string rawtxn = rawtxnValue.get_str();
    if(fZdebug)
        LogPrintf("%s: Signing raw txid=%s\n", __FUNCTION__, rawtxn.c_str());

    UniValue params = UniValue(UniValue::VARR);
    params.push_back(rawtxn);
    UniValue signResultValue = signrawtransaction(params, false, CPubKey());
    UniValue signResultObject = signResultValue.get_obj();
    UniValue completeValue = find_value(signResultObject, "complete");
    bool complete = completeValue.get_bool();
    if (!complete) {
        // TODO: #1366 Maybe get "errors" and print array vErrors into a string
        throw JSONRPCError(RPC_WALLET_ENCRYPTION_FAILED, "Failed to sign transaction");
    }
    if(fZdebug)
        LogPrintf("%s: Signed raw txid correctly %s\n", __FUNCTION__);

    UniValue hexValue = find_value(signResultObject, "hex");
    if (hexValue.isNull()) {
        throw JSONRPCError(RPC_WALLET_ERROR, "Missing hex data for signed transaction");
    }
    std::string signedtxn = hexValue.get_str();
    if(fZdebug)
        LogPrintf("%s: Found hex data\n", __FUNCTION__, rawtxn.c_str());

    // Send the signed transaction
    if (!testmode) {
        params.clear();
        params.setArray();
        params.push_back(signedtxn);
        UniValue sendResultValue = sendrawtransaction(params, false, CPubKey());
        if (sendResultValue.isNull()) {
            throw JSONRPCError(RPC_WALLET_ERROR, "Send raw transaction did not return an error or a txid.");
        }

        std::string txid = sendResultValue.get_str();
        if(fZdebug)
            LogPrintf("%s: sendrawtransction on txid=%s completed\n", __FUNCTION__, txid.c_str());

        UniValue o(UniValue::VOBJ);
        o.push_back(Pair("txid", txid));
        set_result(o);
    } else {
        // Test mode does not send the transaction to the network.

        CDataStream stream(ParseHex(signedtxn), SER_NETWORK, PROTOCOL_VERSION);
        CTransaction tx;
        stream >> tx;

        UniValue o(UniValue::VOBJ);
        o.push_back(Pair("test", 1));
        o.push_back(Pair("txid", tx.GetHash().ToString()));
        o.push_back(Pair("hex", signedtxn));
        set_result(o);
    }

    // Keep the signed transaction so we can hash to the same txid
    CDataStream stream(ParseHex(signedtxn), SER_NETWORK, PROTOCOL_VERSION);
    CTransaction tx;
    stream >> tx;
    tx_ = tx;
}

bool AsyncRPCOperation_sendmany::find_utxos(bool fAcceptCoinbase=false) {
    std::set<CTxDestination> destinations;
    destinations.insert(fromtaddr_);

    if(fZdebug)
        LogPrintf("%s: Looking for %s\n", boost::apply_visitor(AddressVisitorString(), fromtaddr_).c_str());

    vector<COutput> vecOutputs;

    LOCK2(cs_main, pwalletMain->cs_wallet);
    pwalletMain->AvailableCoins(vecOutputs, false, NULL, true, fAcceptCoinbase);

    BOOST_FOREACH(const COutput& out, vecOutputs) {
        CTxDestination dest;

        if (!out.fSpendable) {
            continue;
        }

        if( mindepth_ > 1 ) {
            int nHeight    = tx_height(out.tx->GetHash());
            int dpowconfs  = komodo_dpowconfs(nHeight, out.nDepth);
            if (dpowconfs < mindepth_) {
                continue;
            }
        } else {
            if (out.nDepth < mindepth_) {
                continue;
            }
        }

        const CScript &scriptPubKey = out.tx->vout[out.i].scriptPubKey;

        if (destinations.size()) {
            if (!ExtractDestination(scriptPubKey, dest)) {
                continue;
            }

            //printf("%s\n", boost::apply_visitor(AddressVisitorString(), dest).c_str());
            if (!destinations.count(dest)) {
                continue;
            }
        }

        // By default we ignore coinbase outputs
        bool isCoinbase = out.tx->IsCoinBase();
        if (isCoinbase && fAcceptCoinbase==false) {
            continue;
        }

        if (!ExtractDestination(scriptPubKey, dest, true))
            continue;

        CAmount nValue = out.tx->vout[out.i].nValue;

        SendManyInputUTXO utxo(out.tx->GetHash(), out.i, nValue, isCoinbase, dest);
        t_inputs_.push_back(utxo);
    }

    // sort in ascending order, so smaller utxos appear first
    std::sort(t_inputs_.begin(), t_inputs_.end(), [](SendManyInputUTXO i, SendManyInputUTXO j) -> bool {
        return ( std::get<2>(i) < std::get<2>(j));
    });

    return t_inputs_.size() > 0;
}


bool AsyncRPCOperation_sendmany::find_unspent_notes() {
    if(fZdebug)
        LogPrintf("%s: For address %s depth=%d\n", __FUNCTION__, fromaddress_.c_str(), mindepth_);

    std::vector<SaplingNoteEntry> saplingEntries;
    {
        LOCK2(cs_main, pwalletMain->cs_wallet);
        pwalletMain->GetFilteredNotes(saplingEntries, fromaddress_, mindepth_);
    }

    for (auto entry : saplingEntries) {
        z_sapling_inputs_.push_back(entry);
        std::string data(entry.memo.begin(), entry.memo.end());
        LogPrint("zrpcunsafe", "%s: found unspent Sapling note (txid=%s, vShieldedSpend=%d, amount=%s, memo=%s)\n",
            getId(),
            entry.op.hash.ToString().substr(0, 10),
            entry.op.n,
            FormatMoney(entry.note.value()),
            HexStr(data).substr(0, 10));
    }

    // sort in descending order, so big notes appear first
    std::sort(z_sapling_inputs_.begin(), z_sapling_inputs_.end(),
        [](SaplingNoteEntry i, SaplingNoteEntry j) -> bool {
            return i.note.value() > j.note.value();
        });

    return true;
}

void AsyncRPCOperation_sendmany::add_taddr_outputs_to_tx() {

    CMutableTransaction rawTx(tx_);

    for (SendManyRecipient & r : t_outputs_) {
        std::string outputAddress = std::get<0>(r);
        CAmount nAmount = std::get<1>(r);

        CTxDestination address = DecodeDestination(outputAddress);
        if (!IsValidDestination(address)) {
            throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Invalid output address, not a valid taddr.");
        }

        CScript scriptPubKey = GetScriptForDestination(address);

        CTxOut out(nAmount, scriptPubKey);
        rawTx.vout.push_back(out);
    }
    //if ((uint32_t)chainActive.LastTip()->nTime < ASSETCHAINS_STAKED_HF_TIMESTAMP)
    if ( !hush_hardfork_active((uint32_t)chainActive.LastTip()->nTime) )
        rawTx.nLockTime = (uint32_t)time(NULL) - 60; // jl777
    else
        rawTx.nLockTime = (uint32_t)chainActive.Tip()->GetMedianTimePast();

    tx_ = CTransaction(rawTx);
}

void AsyncRPCOperation_sendmany::add_taddr_change_output_to_tx(CBitcoinAddress *fromaddress,CAmount amount) {

    LOCK2(cs_main, pwalletMain->cs_wallet);

    EnsureWalletIsUnlocked();
    CScript scriptPubKey;
    CReserveKey keyChange(pwalletMain);
    CPubKey vchPubKey;
    if ( fromaddress != 0 )
        scriptPubKey = GetScriptForDestination(fromaddress->Get());
    else
    {
        bool ret = keyChange.GetReservedKey(vchPubKey);
        if (!ret) {
            throw JSONRPCError(RPC_WALLET_KEYPOOL_RAN_OUT, "Could not generate a taddr to use as a change address"); // should never fail, as we just unlocked
        }
        scriptPubKey = GetScriptForDestination(vchPubKey.GetID());
    }
    CTxOut out(amount, scriptPubKey);

    CMutableTransaction rawTx(tx_);
    rawTx.vout.push_back(out);
    //if ((uint32_t)chainActive.LastTip()->nTime < ASSETCHAINS_STAKED_HF_TIMESTAMP)
    if ( !hush_hardfork_active((uint32_t)chainActive.LastTip()->nTime) )
        rawTx.nLockTime = (uint32_t)time(NULL) - 60; // jl777
    else
        rawTx.nLockTime = (uint32_t)chainActive.Tip()->GetMedianTimePast();
    tx_ = CTransaction(rawTx);
}

std::array<unsigned char, ZC_MEMO_SIZE> AsyncRPCOperation_sendmany::get_memo_from_hex_string(std::string s) {
    // initialize to default memo (no_memo), see section 5.5 of the protocol spec
    std::array<unsigned char, ZC_MEMO_SIZE> memo = {{0xF6}};

    std::vector<unsigned char> rawMemo = ParseHex(s.c_str());

    // If ParseHex comes across a non-hex char, it will stop but still return results so far.
    size_t slen = s.length();
    if (slen % 2 !=0 || (slen>0 && rawMemo.size()!=slen/2)) {
        throw JSONRPCError(RPC_INVALID_PARAMETER, "Memo must be in hexadecimal format");
    }

    if (rawMemo.size() > ZC_MEMO_SIZE) {
        throw JSONRPCError(RPC_INVALID_PARAMETER, strprintf("Memo size of %d is too big, maximum allowed is %d", rawMemo.size(), ZC_MEMO_SIZE));
    }

    // copy vector into boost array
    int lenMemo = rawMemo.size();
    for (int i = 0; i < ZC_MEMO_SIZE && i < lenMemo; i++) {
        memo[i] = rawMemo[i];
    }
    return memo;
}

/**
 * Override getStatus() to append the operation's input parameters to the default status object.
 */
UniValue AsyncRPCOperation_sendmany::getStatus() const {
    UniValue v = AsyncRPCOperation::getStatus();
    if (contextinfo_.isNull()) {
        return v;
    }

    UniValue obj = v.get_obj();
    obj.push_back(Pair("method", "z_sendmany"));
    obj.push_back(Pair("params", contextinfo_ ));
    return obj;
}
