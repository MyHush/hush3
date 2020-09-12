// Copyright (c) 2019-2020 The Hush developers
// Copyright (c) 2019 Cryptoforge
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "init.h"
#include "key_io.h"
#include "rpc/server.h"
#include "wallet.h"
#include "rpchushwallet.h"
#include "utilmoneystr.h"
#include <utf8.h>

using namespace std;
using namespace libzcash;

bool EnsureWalletIsAvailable(bool avoidException);

void zsTxSpendsToJSON(const CWalletTx& wtx, UniValue& spends, CAmount& totalSpends, CAmount& filteredSpends, const std::string& strAddress, bool filterByAddress) {

  LOCK2(cs_main, pwalletMain->cs_wallet);

  //Used to identify incomplete key sets
  int vinCount = 0;
  int vinSpendCount = 0;
  int shieldedSpendCount = 0;
  int shieldedSpendsSpentCount = 0;

  //Check address
  bool isTAddress = false;
  bool isZsAddress = false;

  CTxDestination tAddress = DecodeDestination(strAddress);
  auto zAddress = DecodePaymentAddress(strAddress);
  SaplingPaymentAddress zsAddress;

  if (filterByAddress) {

    if (IsValidDestination(tAddress))
      isTAddress = true;

    if (IsValidPaymentAddress(zAddress)) {
      if (boost::get<libzcash::SaplingPaymentAddress>(&zAddress) != nullptr) {
          zsAddress = boost::get<libzcash::SaplingPaymentAddress>(zAddress);
          isZsAddress = true;
      }
    }
  }


  // Transparent Inputs belonging to the wallet
  UniValue tSpends(UniValue::VARR);
  if (isTAddress || !filterByAddress) {
    for (int i = 0; i < wtx.vin.size(); i++) {
      vinCount++;
      const CTxIn& txin = wtx.vin[i];
      UniValue obj(UniValue::VOBJ);
      CTxDestination address;
      const CWalletTx* parent = pwalletMain->GetWalletTx(txin.prevout.hash);
      if (parent != NULL) {
        const CTxOut& parentOut = parent->vout[txin.prevout.n];
        ExtractDestination(parentOut.scriptPubKey, address);
        if(IsMine(*pwalletMain, address)){
          vinSpendCount++;
          totalSpends += CAmount(-parentOut.nValue);
          obj.push_back(Pair("address",EncodeDestination(address)));
          obj.push_back(Pair("scriptPubKey",HexStr(parentOut.scriptPubKey.begin(), parentOut.scriptPubKey.end())));
          obj.push_back(Pair("amount",ValueFromAmount(-parentOut.nValue)));
          obj.push_back(Pair("spendTxid",parent->GetHash().ToString()));
          obj.push_back(Pair("spendVout",(int)txin.prevout.n));
          CTxDestination filterAddress = DecodeDestination(strAddress);
          if (address == tAddress || !filterByAddress) {
            filteredSpends += CAmount(-parentOut.nValue);
            tSpends.push_back(obj);
          }
        }
      }
    }
  }
  spends.push_back(Pair("transparentSpends",tSpends));

  // Sapling Inputs belonging to the wallet
  UniValue zsSpends(UniValue::VARR);
  if (isZsAddress || !filterByAddress) {
    for (int i = 0; i < wtx.vShieldedSpend.size(); i++) {
      shieldedSpendCount++;
      const SpendDescription& spendDesc = wtx.vShieldedSpend[i];
      UniValue obj(UniValue::VOBJ);

      SaplingOutPoint op = pwalletMain->mapSaplingNullifiersToNotes[spendDesc.nullifier];

      if (pwalletMain->IsSaplingNullifierFromMe(spendDesc.nullifier)) {
        const CWalletTx* parent = pwalletMain->GetWalletTx(pwalletMain->mapSaplingNullifiersToNotes[spendDesc.nullifier].hash);
        const OutputDescription& output = parent->vShieldedOutput[op.n];
        auto nd = pwalletMain->mapWallet[pwalletMain->mapSaplingNullifiersToNotes[spendDesc.nullifier].hash].mapSaplingNoteData[op];
        auto pt = libzcash::SaplingNotePlaintext::decrypt(output.encCiphertext,nd.ivk,output.ephemeralKey,output.cm);

        if (pt) {
          auto note = pt.get();
          auto pa = nd.ivk.address(note.d);
          auto address = pa.get();
          shieldedSpendsSpentCount++;
          totalSpends += CAmount(-note.value());
          obj.push_back(Pair("address",EncodePaymentAddress(address)));
          obj.push_back(Pair("amount", ValueFromAmount(CAmount(-note.value()))));
          obj.push_back(Pair("spendTxid",parent->GetHash().ToString()));
          obj.push_back(Pair("spendshieldedOutputIndex",(int)op.n));
          if (address == zsAddress || !filterByAddress) {
            filteredSpends += CAmount(-note.value());
            zsSpends.push_back(obj);
          }
        }
      }
    }
  }
  spends.push_back(Pair("saplingSpends",zsSpends));

 
  spends.push_back(Pair("totalSpends",ValueFromAmount(filteredSpends)));

  if (!filterByAddress) {
    if (filteredSpends != 0 && (vinCount != vinSpendCount || shieldedSpendCount != shieldedSpendsSpentCount)) {
      spends.push_back(Pair("missingSpendingKeys", true));
      spends.push_back(Pair("vinCount", vinCount));
      spends.push_back(Pair("vinSpendCount", vinSpendCount));
      spends.push_back(Pair("shieldedSpendCount", shieldedSpendCount));
      spends.push_back(Pair("shieldedSpendsSpentCount", shieldedSpendsSpentCount));
    } else {
        spends.push_back(Pair("missingSpendingKeys", false));
    }
  }
}


void zsTxReceivedToJSON(const CWalletTx& wtx, UniValue& received, CAmount& totalReceived, const std::string& strAddress, bool filterByAddress) {

  if(fZdebug)
    fprintf(stderr,"%s: txid=%s\n", __func__, wtx.GetHash().ToString().c_str() );

  LOCK2(cs_main, pwalletMain->cs_wallet);

  //Check address
  bool isTAddress = false;
  bool isZsAddress = false;

  CTxDestination tAddress = DecodeDestination(strAddress);
  auto zAddress = DecodePaymentAddress(strAddress);
  SaplingPaymentAddress zsAddress;

  if (filterByAddress) {

    if (IsValidDestination(tAddress))
      isTAddress = true;

    if (IsValidPaymentAddress(zAddress)) {
      if (boost::get<libzcash::SaplingPaymentAddress>(&zAddress) != nullptr) {
          zsAddress = boost::get<libzcash::SaplingPaymentAddress>(zAddress);
          isZsAddress = true;
      }
    }
  }


  //Transparent Received txos belonging to the wallet
  UniValue tReceived(UniValue::VARR);
  if (isTAddress || !filterByAddress) {
    for (int i = 0; i < wtx.vout.size(); i++) {
      const CTxOut& txout = wtx.vout[i];
      UniValue obj(UniValue::VOBJ);
      CTxDestination address;
      ExtractDestination(txout.scriptPubKey, address);
      if(IsMine(*pwalletMain, address)){
        obj.push_back(Pair("address",EncodeDestination(address)));
        obj.push_back(Pair("scriptPubKey",HexStr(txout.scriptPubKey.begin(), txout.scriptPubKey.end())));
        obj.push_back(Pair("amount",ValueFromAmount(txout.nValue)));
        obj.push_back(Pair("vout", i));
        if (address == tAddress || !filterByAddress) {
          totalReceived += CAmount(txout.nValue);
          tReceived.push_back(obj);
        }
      }
    }
  }
  received.push_back(Pair("transparentReceived",tReceived));


  //Sapling Sends belonging to the wallet
  UniValue zsReceived(UniValue::VARR);
  if (isZsAddress || !filterByAddress) {
    for (int i = 0; i < wtx.vShieldedOutput.size(); i++) {
      const OutputDescription& outputDesc = wtx.vShieldedOutput[i];
      UniValue obj(UniValue::VOBJ);
      bool changeTx = false;
      //Decrypt sapling incoming commitments using IVK
      std::set<libzcash::SaplingPaymentAddress> addresses;
      pwalletMain->GetSaplingPaymentAddresses(addresses);
      for (auto addr : addresses) {
        libzcash::SaplingExtendedSpendingKey extsk;
        if (pwalletMain->GetSaplingExtendedSpendingKey(addr, extsk)) {
          auto pt = libzcash::SaplingNotePlaintext::decrypt(
                  outputDesc.encCiphertext, extsk.expsk.full_viewing_key().in_viewing_key(), outputDesc.ephemeralKey, outputDesc.cm);

          if (pt) {
            auto note = pt.get();
            obj.push_back(Pair("address",EncodePaymentAddress(addr)));
            obj.push_back(Pair("amount", ValueFromAmount(CAmount(note.value()))));
            obj.push_back(Pair("shieldedOutputIndex",i));

            //Check Change Status
            if (wtx.vShieldedSpend.size()!=0) {
              std::set<std::pair<PaymentAddress, uint256>> nullifierSet;
              nullifierSet = pwalletMain->GetNullifiersForAddresses({addr});
              BOOST_FOREACH(const SpendDescription& spendDesc, wtx.vShieldedSpend) {
                if (nullifierSet.count(std::make_pair(addr, spendDesc.nullifier))) {
                    changeTx = true;
                }
              }
            }
            obj.push_back(Pair("change",changeTx));
            if (addr == zsAddress || !filterByAddress) {
              totalReceived += CAmount(note.value());
              zsReceived.push_back(obj);
            }
          }
        }
      }
    }
  }
  received.push_back(Pair("saplingReceived",zsReceived));

  received.push_back(Pair("totalReceived",ValueFromAmount(totalReceived)));
}


void zsTxSendsToJSON(const CWalletTx& wtx, UniValue& sends, CAmount& totalSends, const std::string& strAddress, bool filterByAddress) {

  LOCK2(cs_main, pwalletMain->cs_wallet);

  //Used to identify incomplete key sets
  int shieldedOutputCount = 0;
  int shieldedOutputDecryptedCount = 0;

  //Check address
  bool isTAddress = false;
  bool isZsAddress = false;

  CTxDestination tAddress = DecodeDestination(strAddress);
  auto zAddress = DecodePaymentAddress(strAddress);
  SaplingPaymentAddress zsAddress;

  if (filterByAddress) {

    if (IsValidDestination(tAddress))
      isTAddress = true;

    if (IsValidPaymentAddress(zAddress)) {
      if (boost::get<libzcash::SaplingPaymentAddress>(&zAddress) != nullptr) {
          zsAddress = boost::get<libzcash::SaplingPaymentAddress>(zAddress);
          isZsAddress = true;
      }
    }
  }

  //All Transparent Sends in the transaction

  UniValue tSends(UniValue::VARR);
  if (isTAddress || !filterByAddress) {
    for (int i = 0; i < wtx.vout.size(); i++) {
      const CTxOut& txout = wtx.vout[i];
      UniValue obj(UniValue::VOBJ);
      CTxDestination address;
      ExtractDestination(txout.scriptPubKey, address);
      obj.push_back(Pair("address",EncodeDestination(address)));
      obj.push_back(Pair("scriptPubKey",HexStr(txout.scriptPubKey.begin(), txout.scriptPubKey.end())));
      obj.push_back(Pair("amount",ValueFromAmount(-txout.nValue)));
      obj.push_back(Pair("vout", i));
      if (address == tAddress || !filterByAddress) {
        totalSends += CAmount(-txout.nValue);
        tSends.push_back(obj);
      }
    }
  }
  sends.push_back(Pair("transparentSends",tSends));

  //All Shielded Sends in the transaction
  UniValue zsSends(UniValue::VARR);
  if (isZsAddress || !filterByAddress) {
    for (int i = 0; i < wtx.vShieldedOutput.size(); i++) {
      const OutputDescription& outputDesc = wtx.vShieldedOutput[i];
      shieldedOutputCount++;
      UniValue obj(UniValue::VOBJ);
      bool changeTx = false;

      //Decrypt sapling outgoing t to z transaction using HDseed
      if (wtx.vShieldedSpend.size()==0) {
        HDSeed seed;
        if (pwalletMain->GetHDSeed(seed)) {
          auto opt = libzcash::SaplingOutgoingPlaintext::decrypt(
                  outputDesc.outCiphertext,ovkForShieldingFromTaddr(seed),outputDesc.cv,outputDesc.cm,outputDesc.ephemeralKey);

          if (opt) {
            auto opt_unwrapped = opt.get();
            auto pt = libzcash::SaplingNotePlaintext::decrypt(
                    outputDesc.encCiphertext,outputDesc.ephemeralKey,opt_unwrapped.esk,opt_unwrapped.pk_d,outputDesc.cm);

            if (pt) {
              shieldedOutputDecryptedCount++;
              auto pt_unwrapped = pt.get();
              libzcash::SaplingPaymentAddress sentAddr(pt_unwrapped.d, opt_unwrapped.pk_d);
              obj.push_back(Pair("address",EncodePaymentAddress(sentAddr)));
              obj.push_back(Pair("amount", ValueFromAmount(CAmount(pt_unwrapped.value()))));
              obj.push_back(Pair("shieldedOutputIndex",i));
              obj.push_back(Pair("change",false));
              if (sentAddr == zsAddress || !filterByAddress) {
                totalSends += CAmount(-pt_unwrapped.value());
                zsSends.push_back(obj);
              }
            }
          }
        }

      //attempt Decryption of Outgoing Sapling using wallet extended spending keys
      } else {
        std::set<libzcash::SaplingPaymentAddress> addresses;
        pwalletMain->GetSaplingPaymentAddresses(addresses);
        for (auto addr : addresses) {
          libzcash::SaplingExtendedSpendingKey extsk;
          if (pwalletMain->GetSaplingExtendedSpendingKey(addr, extsk)) {
            auto opt = libzcash::SaplingOutgoingPlaintext::decrypt(
                    outputDesc.outCiphertext,extsk.expsk.full_viewing_key().ovk,outputDesc.cv,outputDesc.cm,outputDesc.ephemeralKey);

            if (opt) {
              auto opt_unwrapped = opt.get();
              auto pt = libzcash::SaplingNotePlaintext::decrypt(
                      outputDesc.encCiphertext,outputDesc.ephemeralKey,opt_unwrapped.esk,opt_unwrapped.pk_d,outputDesc.cm);

              if (pt) {
                auto pt_unwrapped = pt.get();
                auto memo = pt_unwrapped.memo();
                shieldedOutputDecryptedCount++;
                libzcash::SaplingPaymentAddress sentAddr(pt_unwrapped.d, opt_unwrapped.pk_d);
                obj.push_back(Pair("address",EncodePaymentAddress(sentAddr)));
                obj.push_back(Pair("amount", ValueFromAmount(CAmount(-pt_unwrapped.value()))));
                obj.push_back(Pair("memo", HexStr(memo)));

              if (memo[0] <= 0xf4) {
                  auto end = std::find_if(memo.rbegin(), memo.rend(), [](unsigned char v) { return v != 0; });
                  std::string memoStr(memo.begin(), end.base());
                if (utf8::is_valid(memoStr)) {
                obj.push_back(Pair("memoStr", memoStr));
            }
        }
                obj.push_back(Pair("shieldedOutputIndex",i));

                //Check Change Status
                if (wtx.vShieldedSpend.size()!=0) {
                  std::set<std::pair<PaymentAddress, uint256>> nullifierSet;
                  nullifierSet = pwalletMain->GetNullifiersForAddresses({sentAddr});
                  BOOST_FOREACH(const SpendDescription& spendDesc, wtx.vShieldedSpend) {
                    if (nullifierSet.count(std::make_pair(sentAddr, spendDesc.nullifier))) {
                        changeTx = true;
                    }
                  }
                }
                obj.push_back(Pair("change",changeTx));
                if (sentAddr == zsAddress || !filterByAddress) {
                  totalSends += CAmount(-pt_unwrapped.value());
                  zsSends.push_back(obj);
                }
              }
            }
          }
        }
      }
    }
  }
  sends.push_back(Pair("saplingSends",zsSends));

  if (shieldedOutputCount != shieldedOutputDecryptedCount) {
    sends.push_back(Pair("missingSaplingOVK", true));
  } else {
    sends.push_back(Pair("missingSaplingOVK", false));
  }

  sends.push_back(Pair("totalSends",ValueFromAmount(totalSends)));
}


void zsWalletTxJSON(const CWalletTx& wtx, UniValue& ret, const std::string strAddress, bool fBool, const int returnType) {

  LOCK2(cs_main, pwalletMain->cs_wallet);

  //Track total wallet spend and received
  CAmount totalSpends = 0;
  CAmount filteredSpends = 0;
  CAmount totalReceived = 0;
  CAmount totalSends = 0;

  //Various Univalue to be added to the final transaction
  UniValue spends(UniValue::VOBJ);
  UniValue received(UniValue::VOBJ);
  UniValue sends(UniValue::VOBJ);
  UniValue tx(UniValue::VOBJ);

  //Begin Compiling the Decrypted Transaction
  tx.push_back(Pair("txid", wtx.GetHash().ToString()));

  if(fZdebug)
    fprintf(stderr,"%s: txid=%s\n", __func__, wtx.GetHash().ToString().c_str() );

  if (wtx.IsCoinBase())
  {
      tx.push_back(Pair("coinbase", true));
      if (wtx.GetDepthInMainChain() < 1)
          tx.push_back(Pair("category", "orphan"));
      else if (wtx.GetBlocksToMaturity() > 0)
          tx.push_back(Pair("category", "immature"));
      else
          tx.push_back(Pair("category", "generate"));
  } else {
    tx.push_back(Pair("coinbase", false));
    tx.push_back(Pair("category", "standard"));
  }

  tx.push_back(Pair("blockhash", wtx.hashBlock.GetHex()));
  tx.push_back(Pair("blockindex", wtx.nIndex));
  int confirms = wtx.GetDepthInMainChain();
  if(confirms > 0)
  {
      tx.push_back(Pair("blocktime", mapBlockIndex[wtx.hashBlock]->GetBlockTime()));
  } else {
      tx.push_back(Pair("blocktime", 0));
  }
  tx.push_back(Pair("expiryheight", (int64_t)wtx.nExpiryHeight));
  tx.push_back(Pair("confirmations", confirms));
  tx.push_back(Pair("time", wtx.GetTxTime()));
  tx.push_back(Pair("size", static_cast<uint64_t>(GetSerializeSize(static_cast<CTransaction>(wtx), SER_NETWORK, PROTOCOL_VERSION))));

  //Wallet Conflicts
  UniValue conflicts(UniValue::VARR);
  BOOST_FOREACH(const uint256& conflict, wtx.GetConflicts())
      conflicts.push_back(conflict.GetHex());
  tx.push_back(Pair("walletconflicts", conflicts));

  // Return Type used to determine what is included in the transaction
  // 0 Spends, Received and spends
  // 1 Sends
  // 2 Received
  // 3 Spends
  // TODO - Setup Enum, maybe...

  // Add Spends retrieved earlier
  if (returnType != 2) {
    zsTxSpendsToJSON(wtx, spends, totalSpends, filteredSpends, strAddress, fBool);
    if ((!fBool || filteredSpends != 0) && (returnType == 0 || returnType == 1)) {
      tx.push_back(Pair("spends",spends));
    }
  }
  // Get Received
  if (returnType == 0 || returnType == 2) {
    zsTxReceivedToJSON(wtx, received, totalReceived, strAddress, fBool);
    if (!fBool || totalReceived != 0) {
      tx.push_back(Pair("received",received));
    }
  }

  // Get Sends
  if (returnType == 0 || returnType == 3) {
    //Only include sends if there are spends that belong to the wallet.
    if (totalSpends != 0 || fBool) {
      zsTxSendsToJSON(wtx, sends, totalSends, strAddress, fBool);
    }
    if (!fBool || totalSends != 0) {
      tx.push_back(Pair("sends",sends));
    }
  }

  if ((returnType == 0 && (!fBool || filteredSpends != 0 || totalReceived != 0 || totalSends != 0))
   || (returnType == 1 && (!fBool || filteredSpends != 0))
   || (returnType == 2 && (!fBool || totalReceived != 0))
   || (returnType == 3 && (!fBool || totalSends != 0))) {
    ret.push_back(tx);
  }

}
