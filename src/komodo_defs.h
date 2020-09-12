// Copyright (c) 2019-2020      The Hush developers
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

#ifndef KOMODO_DEFS_H
#define KOMODO_DEFS_H
#include "arith_uint256.h"
#include "chain.h"
#include "komodo_nk.h"

#define KOMODO_EARLYTXID_HEIGHT 100
//#define ADAPTIVEPOW_CHANGETO_DEFAULTON 1572480000
#define ASSETCHAINS_MINHEIGHT 128
#define ASSETCHAINS_MAX_ERAS 7
#define KOMODO_ELECTION_GAP 2000
#define ROUNDROBIN_DELAY 61
#define KOMODO_ASSETCHAIN_MAXLEN 65
#define KOMODO_LIMITED_NETWORKSIZE 4
#define IGUANA_MAXSCRIPTSIZE 10001
#define KOMODO_MAXMEMPOOLTIME 3600 // affects consensus
#define CRYPTO777_PUBSECPSTR "020e46e79a2a8d12b9b5d12c7a91adb4e454edfae43c0a0cb805427d2ac7613fd9"
#define KOMODO_FIRSTFUNGIBLEID 100
#define KOMODO_SAPLING_ACTIVATION 1544832000 // Dec 15th, 2018
#define KOMODO_SAPLING_DEADLINE 1550188800 // Feb 15th, 2019
#define _COINBASE_MATURITY 100

// KMD Notary Seasons 
// 1: May 1st 2018 1530921600
// 2: July 15th 2019 1563148800 -> estimated height 1444000
// 3: 3rd season
    // 1751328000 = dummy timestamp, 1 July 2025!
    // 7113400 = 5x current KMD blockheight. 
// to add seasons, change NUM_KMD_SEASONS, and add timestamp and height of activation to these arrays. 

#define NUM_KMD_SEASONS 6
#define NUM_KMD_NOTARIES 64

// $ ./contrib/block_time.pl 166250
// Hush Block 166250 will happen at roughly:
// Wed Jan 29 08:14:12 2020 Eastern # 1580303652
// Wed Jan 29 13:14:12 2020 GMT     # 1580303652
const uint32_t nHushHardforkHeight  = 166250;
// $ ./contrib/block_time.pl 245555
// Hush Block 245555 will happen at roughly... now
const uint32_t nHushHardforkHeight2 = 245055;

// No coins/code are currently using timestamp activated fork
const uint32_t nHushHardforkTimestamp  = 1580303652; // Jan 29nd 1pm GMT
const uint32_t nHushHardforkTimestamp2 = 1594425600; // Jul 11th 12a GMT

static const uint32_t KMD_SEASON_TIMESTAMPS[NUM_KMD_SEASONS] = {1525132800, 1563148800, nHushHardforkTimestamp, nHushHardforkTimestamp2, nHushHardforkTimestamp2*5, nHushHardforkTimestamp2*6};
static const int32_t  KMD_SEASON_HEIGHTS[NUM_KMD_SEASONS]    = {1,2,nHushHardforkHeight, nHushHardforkHeight2, (int)340000, 5*nHushHardforkHeight2};

// Era array of pubkeys. Add extra seasons to bottom as requried, after adding appropriate info above. 
static const char *notaries_elected[NUM_KMD_SEASONS][NUM_KMD_NOTARIES][2] =
{
    {
        { "0_jl777_testA", "03b7621b44118017a16043f19b30cc8a4cfe068ac4e42417bae16ba460c80f3828" },
        { "0_jl777_testB", "02ebfc784a4ba768aad88d44d1045d240d47b26e248cafaf1c5169a42d7a61d344" },
        { "0_kolo_testA", "0287aa4b73988ba26cf6565d815786caf0d2c4af704d7883d163ee89cd9977edec" },
        { "artik_AR", "029acf1dcd9f5ff9c455f8bb717d4ae0c703e089d16cf8424619c491dff5994c90" },
        { "artik_EU", "03f54b2c24f82632e3cdebe4568ba0acf487a80f8a89779173cdb78f74514847ce" },
        { "artik_NA", "0224e31f93eff0cc30eaf0b2389fbc591085c0e122c4d11862c1729d090106c842" },
        { "artik_SH", "02bdd8840a34486f38305f311c0e2ae73e84046f6e9c3dd3571e32e58339d20937" },
        { "badass_EU", "0209d48554768dd8dada988b98aca23405057ac4b5b46838a9378b95c3e79b9b9e" },
        { "badass_NA", "02afa1a9f948e1634a29dc718d218e9d150c531cfa852843a1643a02184a63c1a7" },
        { "badass_SH", "026b49dd3923b78a592c1b475f208e23698d3f085c4c3b4906a59faf659fd9530b" },
        { "crackers_EU", "03bc819982d3c6feb801ec3b720425b017d9b6ee9a40746b84422cbbf929dc73c3" }, // 10
        { "crackers_NA", "03205049103113d48c7c7af811b4c8f194dafc43a50d5313e61a22900fc1805b45" },
        { "crackers_SH", "02be28310e6312d1dd44651fd96f6a44ccc269a321f907502aae81d246fabdb03e" },
        { "durerus_EU", "02bcbd287670bdca2c31e5d50130adb5dea1b53198f18abeec7211825f47485d57" },
        { "etszombi_AR", "031c79168d15edabf17d9ec99531ea9baa20039d0cdc14d9525863b83341b210e9" },
        { "etszombi_EU", "0281b1ad28d238a2b217e0af123ce020b79e91b9b10ad65a7917216eda6fe64bf7" }, // 15
        { "etszombi_SH", "025d7a193c0757f7437fad3431f027e7b5ed6c925b77daba52a8755d24bf682dde" },
        { "farl4web_EU", "0300ecf9121cccf14cf9423e2adb5d98ce0c4e251721fa345dec2e03abeffbab3f" },
        { "farl4web_SH", "0396bb5ed3c57aa1221d7775ae0ff751e4c7dc9be220d0917fa8bbdf670586c030" },
        { "fullmoon_AR", "0254b1d64840ce9ff6bec9dd10e33beb92af5f7cee628f999cb6bc0fea833347cc" },
        { "fullmoon_NA", "031fb362323b06e165231c887836a8faadb96eda88a79ca434e28b3520b47d235b" }, // 20
        { "fullmoon_SH", "030e12b42ec33a80e12e570b6c8274ce664565b5c3da106859e96a7208b93afd0d" },
        { "grewal_NA", "03adc0834c203d172bce814df7c7a5e13dc603105e6b0adabc942d0421aefd2132" },
        { "grewal_SH", "03212a73f5d38a675ee3cdc6e82542a96c38c3d1c79d25a1ed2e42fcf6a8be4e68" },
        { "indenodes_AR", "02ec0fa5a40f47fd4a38ea5c89e375ad0b6ddf4807c99733c9c3dc15fb978ee147" },
        { "indenodes_EU", "0221387ff95c44cb52b86552e3ec118a3c311ca65b75bf807c6c07eaeb1be8303c" },
        { "indenodes_NA", "02698c6f1c9e43b66e82dbb163e8df0e5a2f62f3a7a882ca387d82f86e0b3fa988" },
        { "indenodes_SH", "0334e6e1ec8285c4b85bd6dae67e17d67d1f20e7328efad17ce6fd24ae97cdd65e" },
        { "jeezy_EU", "023cb3e593fb85c5659688528e9a4f1c4c7f19206edc7e517d20f794ba686fd6d6" },
        { "jsgalt_NA", "027b3fb6fede798cd17c30dbfb7baf9332b3f8b1c7c513f443070874c410232446" },
        { "karasugoi_NA", "02a348b03b9c1a8eac1b56f85c402b041c9bce918833f2ea16d13452309052a982" }, // 30
        { "kashifali_EU", "033777c52a0190f261c6f66bd0e2bb299d30f012dcb8bfff384103211edb8bb207" },
        { "kolo_AR", "03016d19344c45341e023b72f9fb6e6152fdcfe105f3b4f50b82a4790ff54e9dc6" },
        { "kolo_SH", "02aa24064500756d9b0959b44d5325f2391d8e95c6127e109184937152c384e185" },
        { "metaphilibert_AR", "02adad675fae12b25fdd0f57250b0caf7f795c43f346153a31fe3e72e7db1d6ac6" },
        { "movecrypto_AR", "022783d94518e4dc77cbdf1a97915b29f427d7bc15ea867900a76665d3112be6f3" },
        { "movecrypto_EU", "021ab53bc6cf2c46b8a5456759f9d608966eff87384c2b52c0ac4cc8dd51e9cc42" },
        { "movecrypto_NA", "02efb12f4d78f44b0542d1c60146738e4d5506d27ec98a469142c5c84b29de0a80" },
        { "movecrypto_SH", "031f9739a3ebd6037a967ce1582cde66e79ea9a0551c54731c59c6b80f635bc859" },
        { "muros_AR", "022d77402fd7179335da39479c829be73428b0ef33fb360a4de6890f37c2aa005e" },
        { "noashh_AR", "029d93ef78197dc93892d2a30e5a54865f41e0ca3ab7eb8e3dcbc59c8756b6e355" }, // 40
        { "noashh_EU", "02061c6278b91fd4ac5cab4401100ffa3b2d5a277e8f71db23401cc071b3665546" },
        { "noashh_NA", "033c073366152b6b01535e15dd966a3a8039169584d06e27d92a69889b720d44e1" },
        { "nxtswe_EU", "032fb104e5eaa704a38a52c126af8f67e870d70f82977e5b2f093d5c1c21ae5899" },
        { "polycryptoblog_NA", "02708dcda7c45fb54b78469673c2587bfdd126e381654819c4c23df0e00b679622" },
        { "pondsea_AR", "032e1c213787312099158f2d74a89e8240a991d162d4ce8017d8504d1d7004f735" },
        { "pondsea_EU", "0225aa6f6f19e543180b31153d9e6d55d41bc7ec2ba191fd29f19a2f973544e29d" },
        { "pondsea_NA", "031bcfdbb62268e2ff8dfffeb9ddff7fe95fca46778c77eebff9c3829dfa1bb411" },
        { "pondsea_SH", "02209073bc0943451498de57f802650311b1f12aa6deffcd893da198a544c04f36" },
        { "popcornbag_AR", "02761f106fb34fbfc5ddcc0c0aa831ed98e462a908550b280a1f7bd32c060c6fa3" },
        { "popcornbag_NA", "03c6085c7fdfff70988fda9b197371f1caf8397f1729a844790e421ee07b3a93e8" }, // 50
        { "ptytrader_NA", "0328c61467148b207400b23875234f8a825cce65b9c4c9b664f47410b8b8e3c222" },
        { "ptytrader_SH", "0250c93c492d8d5a6b565b90c22bee07c2d8701d6118c6267e99a4efd3c7748fa4" },
        { "rnr_AR", "029bdb08f931c0e98c2c4ba4ef45c8e33a34168cb2e6bf953cef335c359d77bfcd" },
        { "rnr_EU", "03f5c08dadffa0ffcafb8dd7ffc38c22887bd02702a6c9ac3440deddcf2837692b" },
        { "rnr_NA", "02e17c5f8c3c80f584ed343b8dcfa6d710dfef0889ec1e7728ce45ce559347c58c" },
        { "rnr_SH", "037536fb9bdfed10251f71543fb42679e7c52308bcd12146b2568b9a818d8b8377" },
        { "titomane_AR", "03cda6ca5c2d02db201488a54a548dbfc10533bdc275d5ea11928e8d6ab33c2185" },
        { "titomane_EU", "02e41feded94f0cc59f55f82f3c2c005d41da024e9a805b41105207ef89aa4bfbd" },
        { "titomane_SH", "035f49d7a308dd9a209e894321f010d21b7793461b0c89d6d9231a3fe5f68d9960" },
        { "vanbreuk_EU", "024f3cad7601d2399c131fd070e797d9cd8533868685ddbe515daa53c2e26004c3" }, // 60
        { "xrobesx_NA", "03f0cc6d142d14a40937f12dbd99dbd9021328f45759e26f1877f2a838876709e1" },
        { "xxspot1_XX", "02ef445a392fcaf3ad4176a5da7f43580e8056594e003eba6559a713711a27f955" },
        { "xxspot2_XX", "03d85b221ea72ebcd25373e7961f4983d12add66a92f899deaf07bab1d8b6f5573" }
    },
    {
        {"0dev1_jl777", "03b7621b44118017a16043f19b30cc8a4cfe068ac4e42417bae16ba460c80f3828" },
        {"0dev2_kolo", "030f34af4b908fb8eb2099accb56b8d157d49f6cfb691baa80fdd34f385efed961" },
        {"0dev3_kolo", "025af9d2b2a05338478159e9ac84543968fd18c45fd9307866b56f33898653b014" },
        {"0dev4_decker", "028eea44a09674dda00d88ffd199a09c9b75ba9782382cc8f1e97c0fd565fe5707" },
        {"a-team_SH", "03b59ad322b17cb94080dc8e6dc10a0a865de6d47c16fb5b1a0b5f77f9507f3cce" },
        {"artik_AR", "029acf1dcd9f5ff9c455f8bb717d4ae0c703e089d16cf8424619c491dff5994c90" },
        {"artik_EU", "03f54b2c24f82632e3cdebe4568ba0acf487a80f8a89779173cdb78f74514847ce" },
        {"artik_NA", "0224e31f93eff0cc30eaf0b2389fbc591085c0e122c4d11862c1729d090106c842" },
        {"artik_SH", "02bdd8840a34486f38305f311c0e2ae73e84046f6e9c3dd3571e32e58339d20937" },
        {"badass_EU", "0209d48554768dd8dada988b98aca23405057ac4b5b46838a9378b95c3e79b9b9e" },
        {"badass_NA", "02afa1a9f948e1634a29dc718d218e9d150c531cfa852843a1643a02184a63c1a7" }, // 10
        {"batman_AR", "033ecb640ec5852f42be24c3bf33ca123fb32ced134bed6aa2ba249cf31b0f2563" },
        {"batman_SH", "02ca5898931181d0b8aafc75ef56fce9c43656c0b6c9f64306e7c8542f6207018c" },
        {"ca333_EU", "03fc87b8c804f12a6bd18efd43b0ba2828e4e38834f6b44c0bfee19f966a12ba99" },
        {"chainmakers_EU", "02f3b08938a7f8d2609d567aebc4989eeded6e2e880c058fdf092c5da82c3bc5ee" },
        {"chainmakers_NA", "0276c6d1c65abc64c8559710b8aff4b9e33787072d3dda4ec9a47b30da0725f57a" },
        {"chainstrike_SH", "0370bcf10575d8fb0291afad7bf3a76929734f888228bc49e35c5c49b336002153" },
        {"cipi_AR", "02c4f89a5b382750836cb787880d30e23502265054e1c327a5bfce67116d757ce8" },
        {"cipi_NA", "02858904a2a1a0b44df4c937b65ee1f5b66186ab87a751858cf270dee1d5031f18" },
        {"crackers_EU", "03bc819982d3c6feb801ec3b720425b017d9b6ee9a40746b84422cbbf929dc73c3" },
        {"crackers_NA", "03205049103113d48c7c7af811b4c8f194dafc43a50d5313e61a22900fc1805b45" }, // 20
        {"dwy_EU", "0259c646288580221fdf0e92dbeecaee214504fdc8bbdf4a3019d6ec18b7540424" },
        {"emmanux_SH", "033f316114d950497fc1d9348f03770cd420f14f662ab2db6172df44c389a2667a" },
        {"etszombi_EU", "0281b1ad28d238a2b217e0af123ce020b79e91b9b10ad65a7917216eda6fe64bf7" },
        {"fullmoon_AR", "03380314c4f42fa854df8c471618751879f9e8f0ff5dbabda2bd77d0f96cb35676" },
        {"fullmoon_NA", "030216211d8e2a48bae9e5d7eb3a42ca2b7aae8770979a791f883869aea2fa6eef" },
        {"fullmoon_SH", "03f34282fa57ecc7aba8afaf66c30099b5601e98dcbfd0d8a58c86c20d8b692c64" },
        {"goldenman_EU", "02d6f13a8f745921cdb811e32237bb98950af1a5952be7b3d429abd9152f8e388d" },
        {"indenodes_AR", "02ec0fa5a40f47fd4a38ea5c89e375ad0b6ddf4807c99733c9c3dc15fb978ee147" },
        {"indenodes_EU", "0221387ff95c44cb52b86552e3ec118a3c311ca65b75bf807c6c07eaeb1be8303c" },
        {"indenodes_NA", "02698c6f1c9e43b66e82dbb163e8df0e5a2f62f3a7a882ca387d82f86e0b3fa988" }, // 30
        {"indenodes_SH", "0334e6e1ec8285c4b85bd6dae67e17d67d1f20e7328efad17ce6fd24ae97cdd65e" },
        {"jackson_AR", "038ff7cfe34cb13b524e0941d5cf710beca2ffb7e05ddf15ced7d4f14fbb0a6f69" },
        {"jeezy_EU", "023cb3e593fb85c5659688528e9a4f1c4c7f19206edc7e517d20f794ba686fd6d6" },
        {"karasugoi_NA", "02a348b03b9c1a8eac1b56f85c402b041c9bce918833f2ea16d13452309052a982" },
        {"komodoninja_EU", "038e567b99806b200b267b27bbca2abf6a3e8576406df5f872e3b38d30843cd5ba" },
        {"komodoninja_SH", "033178586896915e8456ebf407b1915351a617f46984001790f0cce3d6f3ada5c2" },
        {"komodopioneers_SH", "033ace50aedf8df70035b962a805431363a61cc4e69d99d90726a2d48fb195f68c" },
        {"libscott_SH", "03301a8248d41bc5dc926088a8cf31b65e2daf49eed7eb26af4fb03aae19682b95" },
        {"lukechilds_AR", "031aa66313ee024bbee8c17915cf7d105656d0ace5b4a43a3ab5eae1e14ec02696" },
        {"madmax_AR", "03891555b4a4393d655bf76f0ad0fb74e5159a615b6925907678edc2aac5e06a75" }, // 40
        {"meshbits_AR", "02957fd48ae6cb361b8a28cdb1b8ccf5067ff68eb1f90cba7df5f7934ed8eb4b2c" },
        {"meshbits_SH", "025c6e94877515dfd7b05682b9cc2fe4a49e076efe291e54fcec3add78183c1edb" },
        {"metaphilibert_AR", "02adad675fae12b25fdd0f57250b0caf7f795c43f346153a31fe3e72e7db1d6ac6" },
        {"metaphilibert_SH", "0284af1a5ef01503e6316a2ca4abf8423a794e9fc17ac6846f042b6f4adedc3309" },
        {"patchkez_SH", "0296270f394140640f8fa15684fc11255371abb6b9f253416ea2734e34607799c4" },
        {"pbca26_NA", "0276aca53a058556c485bbb60bdc54b600efe402a8b97f0341a7c04803ce204cb5" },
        {"peer2cloud_AR", "034e5563cb885999ae1530bd66fab728e580016629e8377579493b386bf6cebb15" },
        {"peer2cloud_SH", "03396ac453b3f23e20f30d4793c5b8ab6ded6993242df4f09fd91eb9a4f8aede84" },
        {"polycryptoblog_NA", "02708dcda7c45fb54b78469673c2587bfdd126e381654819c4c23df0e00b679622" },
        {"hyper_AR", "020f2f984d522051bd5247b61b080b4374a7ab389d959408313e8062acad3266b4" }, // 50
        {"hyper_EU", "03d00cf9ceace209c59fb013e112a786ad583d7de5ca45b1e0df3b4023bb14bf51" },
        {"hyper_SH", "0383d0b37f59f4ee5e3e98a47e461c861d49d0d90c80e9e16f7e63686a2dc071f3" },
        {"hyper_NA", "03d91c43230336c0d4b769c9c940145a8c53168bf62e34d1bccd7f6cfc7e5592de" },
        {"popcornbag_AR", "02761f106fb34fbfc5ddcc0c0aa831ed98e462a908550b280a1f7bd32c060c6fa3" },
        {"popcornbag_NA", "03c6085c7fdfff70988fda9b197371f1caf8397f1729a844790e421ee07b3a93e8" },
        {"alien_AR", "0348d9b1fc6acf81290405580f525ee49b4749ed4637b51a28b18caa26543b20f0" },
        {"alien_EU", "020aab8308d4df375a846a9e3b1c7e99597b90497efa021d50bcf1bbba23246527" },
        {"thegaltmines_NA", "031bea28bec98b6380958a493a703ddc3353d7b05eb452109a773eefd15a32e421" },
        {"titomane_AR", "029d19215440d8cb9cc6c6b7a4744ae7fb9fb18d986e371b06aeb34b64845f9325" },
        {"titomane_EU", "0360b4805d885ff596f94312eed3e4e17cb56aa8077c6dd78d905f8de89da9499f" }, // 60
        {"titomane_SH", "03573713c5b20c1e682a2e8c0f8437625b3530f278e705af9b6614de29277a435b" },
        {"webworker01_NA", "03bb7d005e052779b1586f071834c5facbb83470094cff5112f0072b64989f97d7" },
        {"xrobesx_NA", "03f0cc6d142d14a40937f12dbd99dbd9021328f45759e26f1877f2a838876709e1" },
    },
    {
        {"madmax_NA", "0237e0d3268cebfa235958808db1efc20cc43b31100813b1f3e15cc5aa647ad2c3" }, // 0
        {"alright_AR", "020566fe2fb3874258b2d3cf1809a5d650e0edc7ba746fa5eec72750c5188c9cc9" },
        {"strob_NA", "0206f7a2e972d9dfef1c424c731503a0a27de1ba7a15a91a362dc7ec0d0fb47685" },
        {"dwy_EU", "021c7cf1f10c4dc39d13451123707ab780a741feedab6ac449766affe37515a29e" },
        {"phm87_SH", "021773a38db1bc3ede7f28142f901a161c7b7737875edbb40082a201c55dcf0add" },
        {"chainmakers_NA", "02285d813c30c0bf7eefdab1ff0a8ad08a07a0d26d8b95b3943ce814ac8e24d885" },
        {"indenodes_EU", "0221387ff95c44cb52b86552e3ec118a3c311ca65b75bf807c6c07eaeb1be8303c" },
        {"blackjok3r_SH", "021eac26dbad256cbb6f74d41b10763183ee07fb609dbd03480dd50634170547cc" },
        {"chainmakers_EU", "03fdf5a3fce8db7dee89724e706059c32e5aa3f233a6b6cc256fea337f05e3dbf7" },
        {"titomane_AR", "023e3aa9834c46971ff3e7cb86a200ec9c8074a9566a3ea85d400d5739662ee989" },
        {"fullmoon_SH", "023b7252968ea8a955cd63b9e57dee45a74f2d7ba23b4e0595572138ad1fb42d21" }, // 10
        {"indenodes_NA", "02698c6f1c9e43b66e82dbb163e8df0e5a2f62f3a7a882ca387d82f86e0b3fa988" },
        {"chmex_EU", "0281304ebbcc39e4f09fda85f4232dd8dacd668e20e5fc11fba6b985186c90086e" },
        {"metaphilibert_SH", "0284af1a5ef01503e6316a2ca4abf8423a794e9fc17ac6846f042b6f4adedc3309" },
        {"ca333_DEV", "02856843af2d9457b5b1c907068bef6077ea0904cc8bd4df1ced013f64bf267958" },
        {"cipi_NA", "02858904a2a1a0b44df4c937b65ee1f5b66186ab87a751858cf270dee1d5031f18" },
        {"pungocloud_SH", "024dfc76fa1f19b892be9d06e985d0c411e60dbbeb36bd100af9892a39555018f6" },
        {"voskcoin_EU", "034190b1c062a04124ad15b0fa56dfdf34aa06c164c7163b6aec0d654e5f118afb" },
        {"decker_DEV", "028eea44a09674dda00d88ffd199a09c9b75ba9782382cc8f1e97c0fd565fe5707" },
        {"cryptoeconomy_EU", "0290ab4937e85246e048552df3e9a66cba2c1602db76e03763e16c671e750145d1" },
        {"etszombi_EU", "0293ea48d8841af7a419a24d9da11c34b39127ef041f847651bae6ab14dcd1f6b4" },  // 20
        {"karasugoi_NA", "02a348b03b9c1a8eac1b56f85c402b041c9bce918833f2ea16d13452309052a982" },
        {"pirate_AR", "03e29c90354815a750db8ea9cb3c1b9550911bb205f83d0355a061ac47c4cf2fde" },
        {"metaphilibert_AR", "02adad675fae12b25fdd0f57250b0caf7f795c43f346153a31fe3e72e7db1d6ac6" },
        {"zatjum_SH", "02d6b0c89cacd58a0af038139a9a90c9e02cd1e33803a1f15fceabea1f7e9c263a" },
        {"madmax_AR", "03c5941fe49d673c094bc8e9bb1a95766b4670c88be76d576e915daf2c30a454d3" },
        {"lukechilds_NA", "03f1051e62c2d280212481c62fe52aab0a5b23c95de5b8e9ad5f80d8af4277a64b" },
        {"cipi_AR", "02c4f89a5b382750836cb787880d30e23502265054e1c327a5bfce67116d757ce8" },
        {"tonyl_AR", "02cc8bc862f2b65ad4f99d5f68d3011c138bf517acdc8d4261166b0be8f64189e1" },
        {"infotech_DEV", "0345ad4ab5254782479f6322c369cec77a7535d2f2162d103d666917d5e4f30c4c" },
        {"fullmoon_NA", "032c716701fe3a6a3f90a97b9d874a9d6eedb066419209eed7060b0cc6b710c60b" },  // 30
        {"etszombi_AR", "02e55e104aa94f70cde68165d7df3e162d4410c76afd4643b161dea044aa6d06ce" },
        {"node-9_EU", "0372e5b51e86e2392bb15039bac0c8f975b852b45028a5e43b324c294e9f12e411" },
        {"phba2061_EU", "03f6bd15dba7e986f0c976ea19d8a9093cb7c989d499f1708a0386c5c5659e6c4e" },
        {"indenodes_AR", "02ec0fa5a40f47fd4a38ea5c89e375ad0b6ddf4807c99733c9c3dc15fb978ee147" },
        {"and1-89_EU", "02736cbf8d7b50835afd50a319f162dd4beffe65f2b1dc6b90e64b32c8e7849ddd" },
        {"komodopioneers_SH", "032a238a5747777da7e819cfa3c859f3677a2daf14e4dce50916fc65d00ad9c52a" },
        {"komodopioneers_EU", "036d02425916444fff8cc7203fcbfc155c956dda5ceb647505836bef59885b6866" },
        {"d0ct0r_NA", "0303725d8525b6f969122faf04152653eb4bf34e10de92182263321769c334bf58" },
        {"kolo_DEV", "02849e12199dcc27ba09c3902686d2ad0adcbfcee9d67520e9abbdda045ba83227" },
        {"peer2cloud_AR", "02acc001fe1fe8fd68685ba26c0bc245924cb592e10cec71e9917df98b0e9d7c37" }, // 40
        {"webworker01_SH", "031e50ba6de3c16f99d414bb89866e578d963a54bde7916c810608966fb5700776" },
        {"webworker01_NA", "032735e9cad1bb00eaababfa6d27864fa4c1db0300c85e01e52176be2ca6a243ce" },
        {"pbca26_NA", "03a97606153d52338bcffd1bf19bb69ef8ce5a7cbdc2dbc3ff4f89d91ea6bbb4dc" },
        {"indenodes_SH", "0334e6e1ec8285c4b85bd6dae67e17d67d1f20e7328efad17ce6fd24ae97cdd65e" },
        {"pirate_NA", "0255e32d8a56671dee8aa7f717debb00efa7f0086ee802de0692f2d67ee3ee06ee" },
        {"lukechilds_AR", "025c6a73ff6d750b9ddf6755b390948cffdd00f344a639472d398dd5c6b4735d23" },
        {"dragonhound_NA", "0224a9d951d3a06d8e941cc7362b788bb1237bb0d56cc313e797eb027f37c2d375" },
        {"fullmoon_AR", "03da64dd7cd0db4c123c2f79d548a96095a5a103e5b9d956e9832865818ffa7872" },
        {"chainzilla_SH", "0360804b8817fd25ded6e9c0b50e3b0782ac666545b5416644198e18bc3903d9f9" },
        {"titomane_EU", "03772ac0aad6b0e9feec5e591bff5de6775d6132e888633e73d3ba896bdd8e0afb" }, // 50
        {"jeezy_EU", "037f182facbad35684a6e960699f5da4ba89e99f0d0d62a87e8400dd086c8e5dd7" },
        {"titomane_SH", "03850fdddf2413b51790daf51dd30823addb37313c8854b508ea6228205047ef9b" },
        {"alien_AR", "03911a60395801082194b6834244fa78a3c30ff3e888667498e157b4aa80b0a65f" },
        {"pirate_EU", "03fff24efd5648870a23badf46e26510e96d9e79ce281b27cfe963993039dd1351" },
        {"thegaltmines_NA", "02db1a16c7043f45d6033ccfbd0a51c2d789b32db428902f98b9e155cf0d7910ed" },
        {"computergenie_NA", "03a78ae070a5e9e935112cf7ea8293f18950f1011694ea0260799e8762c8a6f0a4" },
        {"nutellalicka_SH", "02f7d90d0510c598ce45915e6372a9cd0ba72664cb65ce231f25d526fc3c5479fc" },
        {"chainstrike_SH", "03b806be3bf7a1f2f6290ec5c1ea7d3ea57774dcfcf2129a82b2569e585100e1cb" },
        {"dwy_SH", "036536d2d52d85f630b68b050f29ea1d7f90f3b42c10f8c5cdf3dbe1359af80aff" },
        {"alien_EU", "03bb749e337b9074465fa28e757b5aa92cb1f0fea1a39589bca91a602834d443cd" }, // 60
        {"gt_AR", "0348430538a4944d3162bb4749d8c5ed51299c2434f3ee69c11a1f7815b3f46135" },
        {"patchkez_SH", "03f45e9beb5c4cd46525db8195eb05c1db84ae7ef3603566b3d775770eba3b96ee" },
        {"decker_AR", "03ffdf1a116300a78729608d9930742cd349f11a9d64fcc336b8f18592dd9c91bc" }, // 63
    },
    {
        // Season 3.5 third party NN pubkeys from https://github.com/KomodoPlatform/dPoW/blob/master/iguana/3rd_party
        {"madmax_NA", "02ef81a360411adf71184ff04d0c5793fc41fd1d7155a28dd909f21f35f4883ac1" },
        {"alright_AR", "036a6bca1c2a8166f79fa8a979662892742346cc972b432f8e61950a358d705517" },
        {"strob_NA", "02049202f3872877e81035549f6f3a0f868d0ad1c9b0e0d2b48b1f30324255d26d" },
        {"hunter_EU", "0378224b4e9d8a0083ce36f2963ec0a4e231ec06b0c780de108e37f41181a89f6a" },
        {"phm87_SH", "03889a10f9df2caef57220628515693cf25316fe1b0693b0241419e75d0d0e66ed" },
        {"chainmakers_NA", "030e4822bddba10eb50d52d7da13106486651e4436962078ee8d681bc13f4993e9" },
        {"indenodes_EU", "03a416533cace0814455a1bb1cd7861ce825a543c6f6284a432c4c8d8875b7ace9" },
        {"blackjok3r_SH", "03d23bb5aad3c20414078472220cc5c26bc5aeb41e43d72c99158d450f714d743a" },
        {"chainmakers_EU", "034f8c0a504856fb3d80a94c3aa78828c1152daf8ccc45a17c450f32a1e242bb0c" },
        {"titomane_AR", "0358cd6d7460654a0ddd5125dd6fa0402d0719999444c6cc3888689a0b4446136a" },
        {"fullmoon_SH", "0275031fa79846c5d667b1f7c4219c487d439cd367dd294f73b5ecd55b4e673254" },
        {"indenodes_NA", "02b3908eda4078f0e9b6704451cdc24d418e899c0f515fab338d7494da6f0a647b" },
        {"chmex_EU", "03e5b7ab96b7271ecd585d6f22807fa87da374210a843ec3a90134cbf4a62c3db1" },
        {"metaphilibert_SH", "03b21ff042bf1730b28bde43f44c064578b41996117ac7634b567c3773089e3be3" },
        {"ca333_DEV", "029c0342ce2a4f9146c7d1ee012b26f5c2df78b507fb4461bf48df71b4e3031b56" },
        {"cipi_NA", "034406ac4cf94e84561c5d3f25384dd59145e92fefc5972a037dc6a44bfa286688" },
        {"pungocloud_SH", "0203064e291045187927cc35ed350e046bba604e324bb0e3b214ea83c74c4713b1" },
        {"voskcoin_EU", "037bfd946f1dd3736ddd2cb1a0731f8b83de51be5d1be417496fbc419e203bc1fe" },
        {"decker_DEV", "02fca8ee50e49f480de275745618db7b0b3680b0bdcce7dcae7d2e0fd5c3345744" },
        {"cryptoeconomy_EU", "037d04b7d16de61a44a3fc766bea4b7791023a36675d6cee862fe53defd04dd8f2" },
        {"etszombi_EU", "02f65da26061d1b9f1756a274918a37e83086dbfe9a43d2f0b35b9d2f593b31907" },
        {"karasugoi_NA", "024ba10f7f5325fd6ec6cab50c5242d142d00fab3537c0002097c0e98f72014177" },
        {"pirate_AR", "0353e2747f89968741c24f254caec24f9f49a894a0039ee9ba09234fcbad75c77d" },
        {"metaphilibert_AR", "0239e34ad22957bbf4c8df824401f237b2afe8d40f7a645ecd43e8f27dde1ab0da" },
        {"zatjum_SH", "03643c3b0a07a34f6ae7b048832e0216b935408dfc48b0c7d3d4faceb38841f3f3" },
        {"madmax_AR", "038735b4f6881925e5a9b14275af80fa2b712c8bd57eef26b97e5b153218890e38" },
        {"lukechilds_NA", "024607d85ea648511fa50b13f29d16beb2c3a248c586b449707d7be50a6060cf50" },
        {"cipi_AR", "025b7655826f5fd3a807cbb4918ef9f02fe64661153ca170db981e9b0861f8c5ad" },
        {"tonyl_AR", "03a8db38075c80348889871b4318b0a79a1fd7e9e21daefb4ca6e4f05e5963569c" },
        {"infotech_DEV", "0399ff59b0244103486a94acf1e4a928235cb002b20e26a6f3163b4a0d5e62db91" },
        {"fullmoon_NA", "02adf6e3cb8a3c94d769102aec9faf2cb073b7f2979ce64efb1161a596a8d16312" },
        {"etszombi_AR", "03c786702b81e0122157739c8e2377cf945998d36c0d187ec5c5ff95855848dfdd" },
        {"node-9_EU", "024f2402daddee0c8169ccd643e5536c2cf95b9690391c370a65c9dd0169fc3dc6" },
        {"phba2061_EU", "02dc98f064e3bf26a251a269893b280323c83f1a4d4e6ccd5e84986cc3244cb7c9" },
        {"indenodes_AR", "0242778789986d614f75bcf629081651b851a12ab1cc10c73995b27b90febb75a2" },
        {"and1-89_EU", "029f5a4c6046de880cc95eb448d20c80918339daff7d71b73dd3921895559d7ca3" },
        {"komodopioneers_SH", "02ae196a1e93444b9fcac2b0ccee428a4d9232a00b3a508484b5bccaedc9bac55e" },
        {"komodopioneers_EU", "03c7fef345ca6b5326de9d5a38498638801eee81bfea4ca8ffc3dacac43c27b14d" },
        {"d0ct0r_NA", "0235b211469d7c1881d30ab647e0d6ddb4daf9466f60e85e6a33a92e39dedde3a7" },
        {"kolo_DEV", "03dc7c71a5ef7104f81e62928446c4216d6e9f68d944c893a21d7a0eba74b1cb7c" },
        {"peer2cloud_AR", "0351c784d966dbb79e1bab4fad7c096c1637c98304854dcdb7d5b5aeceb94919b4" },
        {"webworker01_SH", "0221365d89a6f6373b15daa4a50d56d34ad1b4b8a48a7fd090163b6b5a5ecd7a0a" },
        {"webworker01_NA", "03bfc36a60194b495c075b89995f307bec68c1bcbe381b6b29db47af23494430f9" },
        {"pbca26_NA", "038319dcf74916486dbd506ac866d184c17c3202105df68c8335a1a1079ef0dfcc" },
        {"indenodes_SH", "031d1584cf0eb4a2d314465e49e2677226b1615c3718013b8d6b4854c15676a58c" },
        {"pirate_NA", "034899e6e884b3cb1f491d296673ab22a6590d9f62020bea83d721f5c68b9d7aa7" },
        {"lukechilds_AR", "031ee242e67a8166e489c0c4ed1e5f7fa32dff19b4c1749de35f8da18befa20811" },
        {"dragonhound_NA", "022405dbc2ea320131e9f0c4115442c797bf0f2677860d46679ac4522300ce8c0a" },
        {"fullmoon_AR", "03cd152ae20adcc389e77acad25953fc2371961631b35dc92cf5c96c7729c2e8d9" },
        {"chainzilla_SH", "03fe36ff13cb224217898682ce8b87ba6e3cdd4a98941bb7060c04508b57a6b014" },
        {"titomane_EU", "03d691cd0914a711f651082e2b7b27bee778c1309a38840e40a6cf650682d17bb5" },
        {"jeezy_EU", "022bca828b572cb2b3daff713ed2eb0bbc7378df20f799191eebecf3ef319509cd" },
        {"titomane_SH", "038c2a64f7647633c0e74eec93f9a668d4bf80214a43ed7cd08e4e30d3f2f7acfb" },
        {"alien_AR", "024f20c096b085308e21893383f44b4faf1cdedea9ad53cc7d7e7fbfa0c30c1e71" },
        {"pirate_EU", "0371f348b4ac848cdfb732758f59b9fdd64285e7adf769198771e8e203638db7e6" },
        {"thegaltmines_NA", "03e1d4cec2be4c11e368ff0c11e80cd1b09da8026db971b643daee100056b110fa" },
        {"computergenie_NA", "02f945d87b7cd6e9f2173a110399d36b369edb1f10bdf5a4ba6fd4923e2986e137" },
        {"nutellalicka_SH", "035ec5b9e88734e5bd0f3bd6533e52f917d51a0e31f83b2297aabb75f9798d01ef" },
        {"chainstrike_SH", "0221f9dee04b7da1f3833c6ea7f7325652c951b1c239052b0dadb57209084ca6a8" },
        {"hunter_SH", "02407db70ad30ce4dfaee8b4ae35fae88390cad2b0ba0373fdd6231967537ccfdf" },
        {"alien_EU", "022b85908191788f409506ebcf96a892f3274f352864c3ed566c5a16de63953236" },
        {"gt_AR", "0307c1cf89bd8ed4db1b09a0a98cf5f746fc77df3803ecc8611cf9455ec0ce6960" },
        {"patchkez_SH", "03d7c187689bf829ca076a30bbf36d2e67bb74e16a3290d8a55df21d6cb15c80c1" },
        {"decker_AR", "02a85540db8d41c7e60bf0d33d1364b4151cad883dd032878ea4c037f67b769635" }
    },
    {
        // Season 4 https://github.com/KomodoPlatform/dPoW/blob/s4/iguana/3rd_party
        {"alien_AR", "024f20c096b085308e21893383f44b4faf1cdedea9ad53cc7d7e7fbfa0c30c1e71" },
        {"alien_EU", "022b85908191788f409506ebcf96a892f3274f352864c3ed566c5a16de63953236" },
        {"alien_NA", "022f62b56ddfd07c9860921c701285ac39bb3ac8f6f083d1b59c8f4943be3de162" },
        {"alright_DEV", "03b6f9493658bdd102503585a08ae642b49d6a68fb69ac3626f9737cd7581abdfa" },
        {"artemii235_DEV", "037a20916d2e9ea575300ac9d729507c23a606b9a200c8e913d7c9832f912a1fa7" },
        {"chainmakers_NA", "028803e07bcc521fde264b7191a944f9b3612e8ee4e24a99bcd903f6976240839a" },
        {"chainzilla_SH", "0311dde03c2dd654ce78323b718ed3ad73a464d1bde97820f3395f54788b5420dd" },
        {"chmex_AR", "030cd487e10fbf142e0e8d582e702ecb775f378569c3cb5acd0ff97b6b12803588" },
        {"chmex_EU", "030bf7bd7ad0515c33b5d5d9a91e0729baf801b9002f80495ae535ea1cebb352cb" },
        {"cipi_EU", "026f4f66385daaf8313ef30ffe4988e7db497132682dca185a70763d93e1417d9d" },
        {"cipi_NA", "03f4e69edcb4fa3b2095cb8cb1ca010f4ec4972eac5d8822397e5c8d87aa21a739" },
        {"daemonfox_NA", "023c7584b1006d4a62a4b4c9c1ede390a3789316547897d5ed49ff9385a3acb411" },
        {"dappvader_SH", "025199bc04bcb8a17976d9fe8bc87763a6150c2727321aa59bf34a2b49f2f3a0ce" },
        {"decker_AR", "02a85540db8d41c7e60bf0d33d1364b4151cad883dd032878ea4c037f67b769635" },
        {"decker_DEV", "02fca8ee50e49f480de275745618db7b0b3680b0bdcce7dcae7d2e0fd5c3345744" },
        {"decker_EU", "027777775b89ff548c3be54fb0c9455437d87f38bfce83bdef113899881b219c9e" },
        {"dragonhound_NA", "029912212d370ee0fb4d38eefd8bfcd8ab04e2c3b0354020789c29ddf2a35c72d6" },
        {"dudezmobi_AR", "033c121d3f8d450174674a73f3b7f140b2717a7d51ea19ee597e2e8e8f9d5ed87f" },
        {"etszombi_AR", "03bfcbca83f11e622fa4eed9a1fa25dba377981ea3b22e3d0a4015f9a932af9272" },
        {"etszombi_EU", "03a5c083c78ba397970f20b544a01c13e7ed36ca8a5ae26d5fe7bd38b92b6a0c94" },
        {"fullmoon_AR", "03639bc56d3fecf856f17759a441c5893668e7c2d460f3d216798a413cd6766bb2" },
        {"fullmoon_NA", "03e388bcc579ac2675f8fadfa921eec186dcea8d2b43de1eed6caba23d5a962b74" },
        {"fullmoon_SH", "03a5cfda2b097c808834ccdd805828c811b519611feabdfe6b3644312e53f6748f" },
        {"gcharang_SH", "02a654037d12cdd609f4fad48e15ec54538e03f61fdae1acb855f16ebacac6bd73" },
        {"greer_NA", "0262da6aaa0b295b8e2f120035924758a4a630f899316dc63ee15ef03e9b7b2b23" },
        {"indenodes_AR", "0242778789986d614f75bcf629081651b851a12ab1cc10c73995b27b90febb75a2" },
        {"indenodes_EU", "03a416533cace0814455a1bb1cd7861ce825a543c6f6284a432c4c8d8875b7ace9" },
        {"indenodes_NA", "02b3908eda4078f0e9b6704451cdc24d418e899c0f515fab338d7494da6f0a647b" },
        {"indenodes_SH", "031d1584cf0eb4a2d314465e49e2677226b1615c3718013b8d6b4854c15676a58c" },
        {"karasugoi_NA", "02f803e6f159824a181cc5d709f3d1e7ff65f19e1899920724aeb4e3d2d869f911" },
        {"madmax_AR", "027afddbcf690230dd8d435ec16a7bfb0083e6b77030f763437f291dfc40a579d0" },
        {"madmax_EU", "0397ec3a4ad84b3009566d260c89f1c4404e86e5d044964747c9371277e38f5995" },
        {"madmax_NA", "036d3afebe1eab09f4c38c3ee6a4659ad390f3df92787c11437a58c59a29e408e6" },
        {"marmarachain_AR", "028690ca1e3afdf8a38b421f6a41f5ff407afc96d5a7a6a488330aae26c8b086bb" },
        {"mcrypt_SH", "027a4ca7b11d3456ff558c08bb04483a89c7f383448461fd0b6b3b07424aabe9a4" },
        {"metaphilibert_AR", "0239e34ad22957bbf4c8df824401f237b2afe8d40f7a645ecd43e8f27dde1ab0da" },
        {"metaphilibert_SH", "03b21ff042bf1730b28bde43f44c064578b41996117ac7634b567c3773089e3be3" },
        {"mihailo_EU", "036494e7c9467c8c7ff3bf29e841907fb0fa24241866569944ea422479ec0e6252" },
        {"mrlynch_AR", "03e67440141f53a08684c329ebc852b018e41f905da88e52aa4a6dc5aa4b12447a" },
        {"mylo_SH", "026d5f29d09ff3f33e14db4811606249b2438c6bcf964876714f81d1f2d952acde" },
        {"node9_EU", "0392e4c9400e69f28c6b9e89d586da69d5a6af7702f1045eaa6ebc1996f0496e1f" },
        {"nodeone_NA", "0310a249c6c2dcc29f2135715138a9ddb8e01c0eab701cbd0b96d9cec660dbdc58" },
        {"nutellalicka_SH", "0284c4d3cb97dd8a32d10fb32b1855ae18cf845dad542e3b8937ca0e998fb54ecc" },
        {"oszy_EU", "03c53bd421de4a29ce68c8cc83f802e1181e77c08f8f16684490d61452ea8d023a" },
        {"patchkez_SH", "028c08db6e7242681f50db6c234fe3d6e12fb1a915350311be26373bac0d457d49" },
        {"pbca26_NA", "03c18431bb6bc95672f640f19998a196becd2851d5dcba4795fe8d85b7d77eab81" },
        {"peer2cloud_AR", "0243958faf9ae4d43b598b859ddc595c170c4cf50f8e4517d660ae5bc72aeb821b" },
        {"phba2061_EU", "03369187ce134bd7793ee34af7756fe1ab27202e09306491cdd5d8ad2c71697937" },
        {"phm87_SH", "03889a10f9df2caef57220628515693cf25316fe1b0693b0241419e75d0d0e66ed" },
        {"pirate_EU", "0240011b95cde819f298fe0f507b2260c9fecdab784924076d4d1e54c522103cb1" },
        {"pirate_NA", "02ad7ef25d2dd461e361120cd3efe7cbce5e9512c361e9185aac33dd303d758613" },
        {"pungocloud_SH", "02641c36ae6747b88150a463a1fe65cf7a9d1c00a64387c73f296f0b64e77c7d3f" },
        {"smdmitry_AR", "0397b7584cb29717b721c0c587d4462477efc1f36a56921f133c9d17b0cd7f278a" },
        {"starfleet_EU", "03c6e047218f34644ccba67e317b9da5d28e68bbbb6b9973aef1281d2bafa46496" },
        {"strob_NA", "02285bf2f9e96068ecac14bc6f770e394927b4da9f5ba833eaa9468b5d47f203a3" },
        {"strob_SH", "0213751a1c59d3489ca85b3d62a3d606dcef7f0428aa021c1978ea16fb38a2fad6" },
        {"swisscertifiers_EU", "02e7722ebba9f8b5ebfb4e87d4fa58cc75aef677535b9cfc060c7d9471aacd9c9e" },
        {"titomane_AR", "03958bd8d13fe6946b8d0d0fbbc3861c72542560d0276e80a4c6b5fe55bc758b81" },
        {"titomane_EU", "02276090e483db1a01a802456b10831b3b6e0a6ad3ece9b2a01f4aad0e480c8edc" },
        {"titomane_SH", "02abf206bafc8048dbdc042b8eb6b1e356ea5dbe149eae3532b4811d4905e5cf01" },
        {"tonyl_AR", "0229e499e3f2e065ced402ceb8aaf3d5ab8bd3793aa074305e9fa30772ce604908" },
        {"tonyl_DEV", "0258b77d7dcfc6c2628b0b6b438951a6e74201fb2cd180a795e4c37fcf8e78a678" },
        {"webworker01_NA", "02de90c720c007229374772505a43917a84ed129d5fbcfa4949cc2e9b563351124" },
        {"zatjum_SH", "0241c5660ca540780be66603b1791127a1261d56abbcb7562c297eec8e4fc078fb" }
   }
};

#define SETBIT(bits,bitoffset) (((uint8_t *)bits)[(bitoffset) >> 3] |= (1 << ((bitoffset) & 7)))
#define GETBIT(bits,bitoffset) (((uint8_t *)bits)[(bitoffset) >> 3] & (1 << ((bitoffset) & 7)))
#define CLEARBIT(bits,bitoffset) (((uint8_t *)bits)[(bitoffset) >> 3] &= ~(1 << ((bitoffset) & 7)))

#define KOMODO_MAXNVALUE (((uint64_t)1 << 63) - 1)
#define KOMODO_BIT63SET(x) ((x) & ((uint64_t)1 << 63))
#define KOMODO_VALUETOOBIG(x) ((x) > (uint64_t)10000000001*COIN)

//#ifndef TESTMODE
#define PRICES_DAYWINDOW ((3600*24/ASSETCHAINS_BLOCKTIME) + 1)
//#else
//#define PRICES_DAYWINDOW (7)
//#endif

extern uint8_t ASSETCHAINS_TXPOW,ASSETCHAINS_PUBLIC;
extern int8_t ASSETCHAINS_ADAPTIVEPOW;
int32_t MAX_BLOCK_SIZE(int32_t height);
extern char ASSETCHAINS_SYMBOL[KOMODO_ASSETCHAIN_MAXLEN];
extern uint16_t ASSETCHAINS_P2PPORT,ASSETCHAINS_RPCPORT;
extern uint32_t ASSETCHAIN_INIT, ASSETCHAINS_MAGIC;
extern int32_t ASSETCHAINS_LWMAPOS, ASSETCHAINS_SAPLING, ASSETCHAINS_OVERWINTER,ASSETCHAINS_BLOCKTIME;
extern uint64_t ASSETCHAINS_SUPPLY, ASSETCHAINS_FOUNDERS_REWARD;

extern int32_t ASSETCHAINS_LWMAPOS, ASSETCHAINS_SAPLING, ASSETCHAINS_OVERWINTER,ASSETCHAINS_BLOCKTIME;
extern int32_t KOMODO_BLOCK_POSUNITS, KOMODO_CONSECUTIVE_POS_THRESHOLD, KOMODO_NOPOS_THRESHHOLD;

extern uint64_t ASSETCHAINS_TIMELOCKGTE;
extern uint32_t ASSETCHAINS_ALGO,ASSETCHAINS_EQUIHASH,KOMODO_INITDONE;

extern int32_t KOMODO_MININGTHREADS,KOMODO_LONGESTCHAIN,ASSETCHAINS_SEED,IS_KOMODO_NOTARY,USE_EXTERNAL_PUBKEY,KOMODO_CHOSEN_ONE,KOMODO_ON_DEMAND,KOMODO_PASSPORT_INITDONE,ASSETCHAINS_STAKED,KOMODO_NSPV;
extern uint64_t ASSETCHAINS_COMMISSION, ASSETCHAINS_LASTERA,ASSETCHAINS_CBOPRET;
extern uint64_t ASSETCHAINS_REWARD[ASSETCHAINS_MAX_ERAS+1], ASSETCHAINS_NOTARY_PAY[ASSETCHAINS_MAX_ERAS+1], ASSETCHAINS_TIMELOCKGTE, ASSETCHAINS_NONCEMASK[],ASSETCHAINS_NK[2];
extern const char *ASSETCHAINS_ALGORITHMS[];
extern uint32_t ASSETCHAINS_NONCESHIFT[], ASSETCHAINS_HASHESPERROUND[];
extern std::string NOTARY_PUBKEY,ASSETCHAINS_OVERRIDE_PUBKEY,ASSETCHAINS_SCRIPTPUB;
extern uint8_t NOTARY_PUBKEY33[33],ASSETCHAINS_OVERRIDE_PUBKEY33[33],ASSETCHAINS_MARMARA;
extern std::vector<std::string> ASSETCHAINS_PRICES,ASSETCHAINS_STOCKS;

extern uint256 KOMODO_EARLYTXID;

extern int32_t KOMODO_CONNECTING,KOMODO_CCACTIVATE,KOMODO_DEALERNODE;
extern uint32_t ASSETCHAINS_CC;
extern std::string CCerror,ASSETCHAINS_CCLIB;
extern uint8_t ASSETCHAINS_CCDISABLES[256];

extern int32_t USE_EXTERNAL_PUBKEY;
extern std::string NOTARY_PUBKEY,NOTARY_ADDRESS;
extern int32_t KOMODO_EXCHANGEWALLET;
extern std::string DONATION_PUBKEY;
extern uint8_t ASSETCHAINS_PRIVATE;
extern int32_t USE_EXTERNAL_PUBKEY;
extern char NOTARYADDRS[64][64];
extern char NOTARY_ADDRESSES[NUM_KMD_SEASONS][64][64];
extern int32_t KOMODO_TESTNODE, KOMODO_SNAPSHOT_INTERVAL,IS_STAKED_NOTARY,STAKED_ERA;
extern int32_t ASSETCHAINS_EARLYTXIDCONTRACT;
extern int32_t ASSETCHAINS_STAKED_SPLIT_PERCENTAGE;
int tx_height( const uint256 &hash );
extern std::vector<std::string> vWhiteListAddress;
extern std::map <std::int8_t, int32_t> mapHeightEvalActivate;
void komodo_netevent(std::vector<uint8_t> payload);
int32_t getacseason(uint32_t timestamp);
int32_t getkmdseason(int32_t height);

#define IGUANA_MAXSCRIPTSIZE 10001
#define KOMODO_KVDURATION 1440
#define KOMODO_KVBINARY 2
#define PRICES_SMOOTHWIDTH 1
#define PRICES_MAXDATAPOINTS 8
uint64_t komodo_paxprice(uint64_t *seedp,int32_t height,char *base,char *rel,uint64_t basevolume);
int32_t komodo_paxprices(int32_t *heights,uint64_t *prices,int32_t max,char *base,char *rel);
int32_t komodo_notaries(uint8_t pubkeys[64][33],int32_t height,uint32_t timestamp);
char *bitcoin_address(char *coinaddr,uint8_t addrtype,uint8_t *pubkey_or_rmd160,int32_t len);
int32_t komodo_minerids(uint8_t *minerids,int32_t height,int32_t width);
int32_t komodo_kvsearch(uint256 *refpubkeyp,int32_t current_height,uint32_t *flagsp,int32_t *heightp,uint8_t value[IGUANA_MAXSCRIPTSIZE],uint8_t *key,int32_t keylen);

uint32_t komodo_blocktime(uint256 hash);
int32_t komodo_longestchain();
int32_t komodo_dpowconfs(int32_t height,int32_t numconfs);
int8_t komodo_segid(int32_t nocache,int32_t height);
int32_t komodo_heightpricebits(uint64_t *seedp,uint32_t *heightbits,int32_t nHeight);
char *komodo_pricename(char *name,int32_t ind);
int32_t komodo_priceind(const char *symbol);
int32_t komodo_pricesinit();
int64_t komodo_priceave(int64_t *tmpbuf,int64_t *correlated,int32_t cskip);
int64_t komodo_pricecorrelated(uint64_t seed,int32_t ind,uint32_t *rawprices,int32_t rawskip,uint32_t *nonzprices,int32_t smoothwidth);
int32_t komodo_nextheight();
uint32_t komodo_heightstamp(int32_t height);
int64_t komodo_pricemult(int32_t ind);
int32_t komodo_priceget(int64_t *buf64,int32_t ind,int32_t height,int32_t numblocks);
uint64_t komodo_accrued_interest(int32_t *txheightp,uint32_t *locktimep,uint256 hash,int32_t n,int32_t checkheight,uint64_t checkvalue,int32_t tipheight);
int32_t komodo_currentheight();
int32_t komodo_notarized_bracket(struct notarized_checkpoint *nps[2],int32_t height);
arith_uint256 komodo_adaptivepow_target(int32_t height,arith_uint256 bnTarget,uint32_t nTime);
bool hush_hardfork_active(uint32_t time);

uint256 Parseuint256(const char *hexstr);
void komodo_sendmessage(int32_t minpeers, int32_t maxpeers, const char *message, std::vector<uint8_t> payload);
CBlockIndex *komodo_getblockindex(uint256 hash);
int32_t komodo_nextheight();
CBlockIndex *komodo_blockindex(uint256 hash);
CBlockIndex *komodo_chainactive(int32_t height);
int32_t komodo_blockheight(uint256 hash);
bool komodo_txnotarizedconfirmed(uint256 txid);
int32_t komodo_blockload(CBlock& block, CBlockIndex *pindex);
uint32_t komodo_chainactive_timestamp();
uint32_t GetLatestTimestamp(int32_t height);

#ifndef KOMODO_NSPV_FULLNODE
#define KOMODO_NSPV_FULLNODE (KOMODO_NSPV <= 0)
#endif // !KOMODO_NSPV_FULLNODE
#ifndef KOMODO_NSPV_SUPERLITE
#define KOMODO_NSPV_SUPERLITE (KOMODO_NSPV > 0)
#endif // !KOMODO_NSPV_SUPERLITE

#endif
