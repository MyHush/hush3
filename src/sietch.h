/******************************************************************************
 * Copyright © 2019-2020 The Hush developers                                       *
 *                                                                            *
 * See the AUTHORS and LICENSE files at                                       *
 * the top-level directory of this distribution for the individual copyright  *
 * holder information and the developer policies on copyright and licensing.  *
 *                                                                            *
 * Unless otherwise agreed in a custom licensing agreement, no part of the    *
 * this software, including this file may be copied, modified, propagated     *
 * or distributed except according to the terms contained in the GPLv3        *
 *                                                                            *
 * Removal or modification of this copyright notice is prohibited.            *
 *                                                                            *
 ******************************************************************************/

#ifndef SIETCH_H
#define SIETCH_H

string newSietchZaddr() {
    bool addToWallet = false;
    auto zaddr = EncodePaymentAddress(pwalletMain->GenerateNewSaplingZKey(addToWallet));
    return zaddr;
}

SendManyRecipient newSietchRecipient(string zaddr) {
    int nAmount = 0;
    // Sietch zouts have random data in their memos so they are indistinguishable from
    // encrypted data being stored in the memo field
    char hex[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    // memo field is 512 bytes or 1024 hex chars
    char str[1024];
    for(int i=0;i<1024;i++) {
      str[i] = hex[GetRandInt(16)];
    }
    str[1024] = 0;
    return SendManyRecipient( zaddr, nAmount, string(str) );
}


// The network essentially DoS's these addresses and reduces their privacy slightly
// by making them public, but in return, the rest of the shielded pool benefits
// and so it's a large benefit for a small cost.
string randomSietchZaddr() {
    std::vector<string> SietchShieldedPool1 = {
  "zs1qqj4aw35ku9yn72g3tha588mxk66mhl5smnn99as40887xvdg49d4yqf43hhqnj9rpsq7qaqqhx",
  "zs1qywzq2cutvg6rkynjljtssz246easagumg3hlwjluz4g7qttg9kqrld7s43ewutpph56jmn2zu6",
  "zs1qx7swmw28dj992f6zs0aqucds9kag88mnca5u73dddeqek4m97pg7h9qsaxxwwkgqxht6zujaxc",
  "zs1q82pkqu58uemrm2749x6g2ta5shnsra9p5jgk0qqzxd3e08ke6vyeezz7lhldj32jxtsuemskk7",
  "zs1qvah5w05qq4yhrsqrt73ckgntkmwdv9mntxep8clnxqfph8xggqad96a5nvwg4evfr9pc5ruvc8",
  "zs1qwrkjcmnrwrqqkz3dyfjvdvdppe0ndnm8fhhpsz8stje4qcfc54jtuygz2jfwc3aag69wsjcm8h",
  "zs1q5pd7h4x7dtnpa4ace7tytye5sd0j4043t4f3jdntyxvg9ex258nu6pw9tthn6t5gmjq5gv0lhc",
  "zs1q6vjrpsuf468an545q7fh9cx0xlkwh75a7qjpemjh3ymuqqzxz5ts2n2dcth3yfnlv6vqpjyglr",
  "zs1qmsvwtxkwlh3tkh0lqtwqv2mxy94jt778f7j74a8067nejkt4j3m2rkmypccju7cfuw7xyg0gg8",
  "zs1qu3jxrw5qwuvl7zfvnsdvywr4a9cn4h00me65te29platx5agek072f5rvxgt8kdt630qs4tgtr",
  "zs1qamxwddwrl5xn56alffruk69p304cqf7uf5nhqpwfc374l9ph00m78xv2kzwymyz0fhxcku7v5k",
  "zs1q7gv479e6q7d8fxc35v5s27em66mmm5gz50excudf95rzjgnwdy5pnwdfytvm7gt8kt6wpkfphq",
  "zs1pqvxd9a2zcsh2v8gaswh3jp2qkzz5umrucl5k4gx0rkvmq68krpx3jesavxftd8t0z56v8whllj",
  "zs1ppn2mdznaa2pd9mavfnxrcjtv0c9un8pg6jaa9ww4wy6wdfs8xxwquqk5umtcjwm6vr4zrqy5x7",
  "zs1pz9c9ydyrm7e876cvae9ha45ww77ru5fhys2yz33kjy8ej9230wjy8yupfxkynwqr6nfupgmf94",
  "zs1p83g95avpsgr847eu3rm3xcmgurt9hc77wtndnmpypa046n529aqdc9ptz04ugsuhvum2ztzwe3",
  "zs1p83jle2p8awu4peevq389y5kyrs5tqxxyrk32zy0t98d4cfepmme0myxp68nrq60xwzc5teulvg",
  "zs1pg5ggzwx4yaa7g83yuhay8kzh78nahxfe7cgavn88f7rxlnuckhl2vznv0f33yuqhhs3sh62vl6",
  "zs1p2nrefmqfhnwrxqfsz4ruvu8wl7742j5rv2fmwlpmpudwqpdn2yrha9rwsu5gc0fdv2j73tfk6a",
  "zs1pw29hkrvszxpe2e4tjpj5h9pszjhql2p4tzcj2c5lf67m7d8jtgc22vehftxdnqze33mgtjc25k",
  "zs1p0ggg024alu2l5x8df8ucu4lz8r453su56w2mmshru49uh9a0p6ufy3qfj8a9n7xeu8dxxjrk4p",
  "zs1psaydszvszu4mar7ef9gk8atmwpnfjjsh8plajn6ttlgdk44jfud5zv8l3uyq73qk3eskec96ut",
  "zs1pjf3kczvrwduuy4x99t03wfrgwstueyw0ypvwa44fz2tukux8kqqqs48uag4y39ed4rux8etvv0",
  "zs1pnwzzh4mhehkvf4ys9x70q6meq9mgqj6mgl9ddzwvf8excswrj54lfgu4m9slmc90s37q8e63du",
  "zs1pnndevupuakjcqyqzu4sfcgdmdzrhutp8ygxwsl5wvq5hgu20u55auu8p08wphvz9mu3k8ynyr5",
  "zs1pmy6tvt9p3xxp5edt70mkwfqk0ljgaranzdnluh5ln36g9g3v2udquleuz5974q2mamywmrxl7j",
  "zs1pau6lddk3uapms7v7rsmxvxeekqh52z795kzy9z3dv9qvzq3jwh4pr2adg5cf8fw2e3mzqmgstq",
  "zs1zpy6wuvy3jlrfmj0363tx6cuume6j0mqfakk7ydw4f6zvn4s7plewk0gtm7r34pjtppvkp8rzl0",
  "zs1zpvkccety206ww6c344ehughuyklc3v02q07j4p72pqewxl0n50zugtje6lclj3m292t6vs56fl",
  "zs1zzucdg9kalcjnnknt98gpg42qm9aqpkc6qf5ewgr29udu55r0zt862z3zt23sd4mj9t47k7k6w4",
  "zs1z9agq4vq7eswwynhmzdsy58nxq3azhn66y853yw9kvercmve8vv6d5pawpwwpwpuyedmzpcqk8q",
  "zs1zvddl2e0pve5kl0hu7fcum30lyhx4pdq64jztq2vp09lqtu5gclfg4fe9fqvnm8k2d5svydg7s4",
  "zs1zvsmkn6a4celtcg8ece6exnkvnr2u06ej8kjt6mrpm0983e86dr9al6gd5g73k24j0a0zkpjs3w",
  "zs1zv33kglx4n5572dg2pez2k3m7tgtz08a70ndpfj6x204pphlm0pzcppetsnjlat3qflswqfmu83",
  "zs1zsz5c9xua7arkvd60vsl559d4dvnjz8ejq4rlmmm9cnz942fdmjmvsgrdl7d5ddxh4y9258jk2l",
  "zs1z5n6qvch0wfymumxjflezekxa2j5t978eqzh9ldxsl39h2jjrlzdv9rf00wdrvg0t6afq7mq0us",
  "zs1z4ymm3gt22f3pcj9p9l2yg00e2m39kfexgaz99s9y4nsuxhlk6u0sl9lsx9awzywumxyuxv9vuw",
  "zs1zkjnhz96xepc97rfyven23epymd5s558yqhp488gcxcj794z6p37h5ej5m5waqxfupmc538mej3",
  "zs1zcqdekyq656yj2y86lh08u8zpetfanj5u4rhfpcphne9xw7esce8asux2rdr4yjxzds56fuda5r",
  "zs1zceru3jt9m3jqzacnffetvrg8zch6le0zactl7ssfky2vwy7pcffkg3pmauqqpkv5v7nv3uzc5a",
  "zs1zellp4tdmxdsd6hmg2c4qlx96m39c0cjlqupw085z87lvzcnx2r0gs7plc0wp4m4upk3zcs35e8",
  "zs1zm2pcg2d3hnyxufn8cyshjn742gmffwaqdc0nt5uf9gsytdjywsqaasfdxf4sysfu0sgxjwjp0g",
  "zs1za9nk7fpgnfetjulq6t8jgfyks06xg4q8rfwpgsfrkn49k34nc7xhm27dnjuzztgnwkww28ztyw",
  "zs1zaaz6j6z7z748mck4v55s3fd4htl4f478kulnwr84m8vn4m0r227juddyq0ncu3cyvc765z9gm4",
  "zs1zlz59lgwc8pqq33508v5ygk9e58f7fs4rpj3achhwnkcyn2dku44yfjghdf5l2v50nu2gjzgl2l",
  "zs1zlgenma0yuqxmqgns2avpnauclxgfvgd6rf0jt09fmfyaflwlhsscem9ypmrvewl9l356dn3jtr",
  "zs1rzu2yg2328l2wlfstu27dl024ucqsjx6g635yxmyhs0wr3lduutkc3peuhqe83uueh5n5q624rd",
  "zs1rr9jpeemn9ek30x4h65rx0ddul7xet6cc8atwrjftmq9sdrvj9f5zdc9xg7amtc6dv5fxjyhu54",
  "zs1rrgccr0yvfn5fdek39x09y2ylyf42xkydcwzq67wdrzyjj2mv874easa4h4tymz5gj852aqpffk",
  "zs1rynh7vl05weafzwkp0p5eafuzzqph04prgg74emqq0sx6xudtrs2380v3ycxgp5lzudw6tmc2zd",
  "zs1rxqz0a59zx3yduncc3azyzexwu8xl6cc0zu83hfd28mksrgahhvx8604uhf0u5mna9m4znnj4gr",
  "zs1rxr2xff2vcv0y9s6ux6c6t9y7r3zvcvrqmwkvsnxj39t0qv7qwyhncnykhzcqg0ggpu423ykzxe",
  "zs1r8chvye5uzlskv8jvt6j8nxnyz4lshy3u46yjhc8w9ekhjhneg402knft3t943t5nrhs6d0d5um",
  "zs1rgu6mz58xrqxpyl5xw7ss2szz29cg98s8xl25hu7fy4lcxw58zr93a8rgyha52vwxx7hxj2emkw",
  "zs1rveutz8ftruljrlctum42kakjqk4cm8dm2807nqm974dz0rptlxcs999ttsvwp65vc9e59vv9fe",
  "zs1rwfqhlezm5ln7kq8wp8hkl08ddn74ryr8cs4cu8qc23spdx79xgfqj4lju4u2yqdrkxkwfvk3ed",
  "zs1rwklllsk9jwhhf0fedvlzsqfqdkwer05zh8c8hwypl4tls2hdal54lexhca7kghhum5hycurvfe",
  "zs1r0ulehrcg9xz0lfdcukjm3qlvqy74t0u8raqf4zn88vdsta8mzp8t8p9ul8jragssvs9qaqpw2e",
  "zs1r3t0cve050wufwl8r2tly05vn7w79v53fe6dagjtt2ese0qm6vgjp3rrsfu4n0am840sq5thn72",
  "zs1rnd8zwan2xlrnfwylm9khkeutnzg2v9vjdnyellyuzkwmed94uvd2dq8ceumxwspz037zp2ctqa",
  "zs1r4tphfnf3zy34dqyjuld2kgtyg88hrxpv756pkkkmrfdeun0cqzpepac4ma9qrjrvdqxg2z5fuv",
  "zs1rcpywy0v4sfej85wdaslfwsp4pe9sa87xgzv24ywhps2l4c9jlrqttv0wl9zkc5e7rsekf42wvp",
  "zs1r66et0z9xw3qqhzyc2aht294y7yqyz8wx2l2t63unca4k4zh4v9t44kpmg52z9va5968y45fdkx",
  "zs1rat3szdulwxwmx7y4rdphvlpjj5phadvz0fsvj7mtg9ggzwd22sn30hzsh27h7t6cvca6a8wu2n",
  "zs1rau4jcth66jsg098ucclp9tfa5qe6jmtmfkcluaqnyj3uatkt9dsrgx44xtmxr3e9xuxv6sy208",
  "zs1ypf2v2yu4p4mnlmw0wd3mpztpjlhl39jnn6hkvf77ell0h5u6yg2pgeusvku5y5sykyy6kk6yyh",
  "zs1yzkt8catgk6slwj7azu79tvwv6tkd6agcykvvht4rxlevtsh99u036jf5503pju3h05w7x02cm8",
  "zs1yrty5fmnyaartpcyjnpal0w44g4mt2ey5tyzcmgq4g6qtsfjmxae7fvy2zsw7t0zvseuwcfwt2e",
  "zs1y9p5gazcx04kke96xudep0edzkqr06gdjnr5vm99a6qxzatqz5katput4q9nx95e8965sg7d3pl",
  "zs1y9vpfgkxwh3xm3j9d38zkeqql2lh8w3ucgerkww2asdv89p87emdavkzurnqpkrmu7e3xv5myue",
  "zs1yfa9gwmn0xe4myrg0rf8kmu36243u8773ukeev0set2yv0z9vpxm6ratee52e9zmpvvx7w2xy28",
  "zs1ytpjrujfsgs69smqerxeaf8m38jwxc4ejgxxe4pzc5qu4auqrgy8tf7zxc402pxf9uku646kc0q",
  "zs1ytvtdwmcn8tnka6w6wa8n3ghnkf7gp2qaudd4233y6m509ntm59rr0n8eudhre0md7m0zedpcsq",
  "zs1yjmeu09mzrt8rgehv2gcfhxx6ddqz7ww87ssfapndvc94hxfrfsdkkgm8f8nr36xm8p7q462qy2",
  "zs1ynqghdu0r0c20csp3ygrxdw9hk2l89j3g59q8zhht9jyxycpcc9ccvhyyn2f9j0ehp4pk5wkhqs",
  "zs1y5ny4jpjm05vp5awjd6muaqqypdv0y9tr6pz0m5t82cwtrearxwf7km4aznydpcjeuzxqvk0z9m",
  "zs1yh2vd5usfsyv4pscjrxg9wdy3gnnyuh8vky27ln3u9jspadmpqsjmyvxmvfyyq3nv4deudvygxa",
  "zs1yclvhy57hngs7d233e4x8ywfreswslz2gvn0f8epcs0wrzuqqau3hkrvf7ru6jhh0zmsyn5jkj6",
  "zs1yunkgwzf0m5suz380j7xqge6rd9e6acjc5wp22z0jhalqdpspdjey7jfjvgrckgsk9ydc9yrnq3",
  "zs19p94fnry6p88ms3yh60nl4kxlxmu7uxv9aafmf5pc4nyd64vslaqgmj30nxe3l5j7cxu5kqeqpm",
  "zs19x2df6qmd4c9whrgj6m4mssz22x9qj9x8lmcnexnhw32pey24xy9sws5ts2q7guunm7mx9wmllj",
  "zs198a984na6qt2z3uyhdkmj7sfevt794dl3mum3782kddjy4uawr2teznpuxvnzc4dvs3c6zyqxey",
  "zs192ld62azpypesveqsat6m63sqaw95ejlqfcjsal5t0fea9zjzqnurmpnl6074zdms0amw83rw0x",
  "zs19vsx09xmzlj9vr3s3vu8z4237gpcgrl7qs0vapzzawgnu7gxngeaxlgwqf0ppu0f7us9cfe3cqz",
  "zs19wfwd8zufu27zugan77wf2g790egdw7vkulf6f375ylq0arnv2nv94l84nl8lp3tpccv763wetn",
  "zs19wcqtqqjj0mnrn90ntcmyq5x8qr2wsaslqwt0fysz4xh2mmjy0z9jjh4sj86sjrgen0axx04zt4",
  "zs19jypvpjpvhv5et5wq2ny09skt72hxz9adfgk2ev7nza5jyxr6gss5qelygnxn0szmjqyke2h8a7",
  "zs195kll03d43her83a65y7z0zsetynlnft4pjxdspegvun0m7cwtx0vsxfm89mv50vxr90qhvcqpz",
  "zs195e2g52jpyly7t9vjpfcegt87g7lpa4rm74nxn0zvmtzjhvg7f5gjnskc5ax5skvwprcshenyqs",
  "zs194e84mfxc4vn4ssce7hkvgrcm3c8j7vehcetkdf78rele2lwkx9tzcfnrwhykdqa2nmwx5qcr0j",
  "zs19cxqspj63ksk6uwtynj0la72zuvh8rxfh0e0pr2y5vuuvw35sm78juzh5gxcuqa8jggv703rplf",
  "zs19e04k24qrca0sx5z47dxmtx0swcx2ywxqjt5594gu95rjaeyxrpa2vyylvzxpau5spt2v529me6",
  "zs19707gmdvc4hfwg4lgvxg55695gltx3jwye8l2gjekrx4zqz7yr6grq8s8hpfqwggrywx509ln5y",
  "zs1xrw8nwla7yrz8l3d7y3rxlhamel2ly4kdmxlc6w4cztxhd6l8wufqv2pcsvtl3d7s6awvjatyx9",
  "zs1xymrgyhle6dcvjk5f62k4xygr0vrr2wckqy7sp6wc6cttn29hra77dzhwxst7z9rxqxkz08jd7g",
  "zs1x9c8tetxgauxh474hlhnlscuhgzww8vnvxfwm0p8589x73t5yl2fph8q8r8qpl8sh0wfwx0vg62",
  "zs1xxcpzsfpyekhvvum3erxjpt34pw3h70ma8vxwecg85tze380f4srlg8zlgxty8yqhutt234nk9q",
  "zs1xx6pd3vtj78tg0zpwtc0jjkdxlfy48vegzd6cng4m9w0gtcya8ck7pqgf4l5sxf9ml5zvzru5xg",
  "zs1x8qre6x5d8e3tt2m4l9q2tujw8ajun70qelp8tfynyw390rm6vhjtmpf58dmx4hccef9xe50az0",
  "zs1x88vjduckqarz2j8tp2me08ya4dgd6pw7j4j98j5jynrze3xy2jjptzye7eftjxd6dn4sj03v7m",
    };

    std::vector<string> SietchShieldedPool2 = {
  "zs1ehrs7hetemzsmfz9xwt8xetxag33j3xzj8mhdgywdnvkk4dmtvw6eqys6gm3vr679y865jz7rth",
  "zs1ehyr6w0c4mwp0wlp6w5letgm3rjk32rxa9kkkur86e5x8lutr9mwzey0hsesnz0yaarmxra7w2z",
  "zs1ec793pjf3anee0qq9ex5u2qygjzk9llmwfygev563l89quc3u8wxvtz9kctlmv2dtjgqwn7krcg",
  "zs1eclpgnqy7qll0l5z0gy7m4ew86yjvp397yxyau9y2y43x5mqfdw2sll95l83ux6h8mppzsx3xnp",
  "zs1eexedkctuzhjysl00m0j3ekknd32635yd8rejx9ykjp6zz77fyzf5388env642ja2qlg6mrwgsc",
  "zs1e77uz8yaj998arp56d0fm4p827wchlf2w09hve6rgkmavhzeyhke8qsk3l7s5k6yh2jwjheqpup",
  "zs1elj4qvy42jpts2cu22tcwt0kmwzafpmrjgztwf2xcqaycrlr4rpfxfpswx63e6agvhz96gps9yf",
  "zs16q0fzcvf25fh70ysn38v7qkpfspakmelnljgnyrpu7rvllyew57n5cpqjqe0wmy5j57au47j6x8",
  "zs16pnkw3mucdef34jjk6q28zd7ghhatdcqn598vs3g70qev234uc5uw6xxxjnzef3pt2t567qev8v",
  "zs16rmnl4hd6c226u3v6eekk68y59u0x7v37n8pmytt9xw6drugjml7ryhd243nf3l2pvafw42rnxc",
  "zs16ruwvwmetmnnns0rxwtx2kss23da2pccares0tehzw3v3nxd483qn49c9apdfua0hpm8xdecdt4",
  "zs16ymafsjd7fp9zdl9vtyvfedecvn7q2vcs7emeglwy7hpuphve97d3j87v2evqs0xm3jrx44nwfe",
  "zs1697ggm9zqp4rh0fv4r9hnh9uy6gg4hk7r2lm33tp2we4ry5azpxq6nwuzscha0g2nx03x4sstq8",
  "zs168t8u6at9k6kt748dv749dxahnk8rv8yn32z8gcgvc9rkhkewscgu8r8vuzv0zecnq26w9p7x0c",
  "zs16gnyx6f8vql24cepamfquhr3jjt7wgmzjcvwtct4lwy9p2studln6ut9kjzf6empwqjtxqmddqy",
  "zs1622ra8snywvxufsunk97pccr7k0j32p960evl9yqjadhju22m4sk48md84q5u238gej8xnm4xm5",
  "zs162stv8m0udzy4c6ff5kqva2g7pqth2rrdn7rjjgw29dcx7lj3vs4dnj8fz0gczsat3u3q5axgva",
  "zs16t78376h9ledgt50k2dmwssuhyy4hn94wfgd8vzegvqy9k9kauuvpe8xz0f3va65l8ufqhgv4gv",
  "zs16vxrghgv7mth6k9unzdude9cyt9vl76ms5hm9ce4py92rk69j4p9u570974nh8kqh8e2stqknj5",
  "zs1654eafn589g2ujzcasa8caz8exag742tra2dd6mjkp22axh27hda6sy9exh3wkp4c6f5vztafr0",
  "zs164u3ntsnn7s3zrp6c5gsqfrwr0ywhspynvunq3fhvr9crwz5eme706j5awwvuatqwh7m2qqkqm9",
  "zs16cpqwm4yml5x8j5r3q5j0xljam68pf56xt40hylzn69w45venwdvd4h8drys5t380mspkvt7h8r",
  "zs16c6m9aqs0q9kadm4nk6hugmqw6p0lf2h6v5d9ccwszssyecq35sm6284c3uqx2u40da4s2mr2ap",
  "zs16u7rc066566j9ux73dcq0m7cq4qdmtd3gefrnhhrpjww3z00j4za7m5mcutmj6qcezkzys87mpp",
  "zs1673rm5d5z9sh2k9uc2cvgwk2e44z0sekx6ezt9n4fvgnky5yxa2tc306dw7n2dg5vwfn7ppwr5s",
  "zs167nvq5ahvu4s26447rem9j37qglgtle4fghsgpksumkz34g2q9x783pak5jgdhhzylmgs9wemg8",
  "zs16lluayez28xevxg0rawxxcd7yx5t7qaraet256sxe8ac69lj7n7ppncsx3m2tddxlzptzyxv0qr",
  "zs1mydlmczamey4ydc79n8gj5wtgs79zge4nwhcg8g640r6fvwu6wpt70970p7ptkcrzg6r6frqh0u",
  "zs1my6tgqmy3kgdlqncyd5dv9s727x9hcrmj8h06e5whn4hkn5t2x46j84276yd8pw0p6ysx53u38u",
  "zs1mte528eue8smvjwpe9cs8qz3wud9735rnk7vrtefu9lhyvh5pyeyenpaq5fa08jpwrl4x7sz069",
  "zs1mvmu20syf3u5yzd6hpdd29xfej8237x2k0pcmf7pvra46qem5g0jralrmdcvncgf5j0y5varzr2",
  "zs1mw86y6g2c8972a2ndw57648p9qcx2jppxr9g5k24df8hl3rgfzfsfe2xyesemdfmasutk88duht",
  "zs1mwca7dqjq0r2mzmn4qarw8acmt4rjk26lyyar35sssqe2fky2nmv3kmf0lfxs2a9htugsadg027",
  "zs1m0rpcrkfagzpex9mlw2htyrwpxe48v47pj8zg96l9wm5k8xkj3kev3xca6wumv5avkgsjtygush",
  "zs1mjgyju3jnyxjhekv72y448edq3j2cvtk5s4wrej790zzpv3uc20dvewt7nhjdqryuecmykfm0je",
  "zs1mjlfwyma3fv45yg50j73wcswnap2s4lsh5c9km6lcem73wky9lej7vy8wrlkr75sy6z2g0agtmw",
  "zs1mkh9s9gdfxx4dkug47dqnc98g76dhm09zty779pm6gqc5aye4gvxvduqm5j56nq4lncx5ygqu90",
  "zs1mm7tesf30r56l8xmnxjyxvgmnsny2zntsxahllrd930q8ycahz8npdxt7lqwu3k8ljs9sw7uzs8",
  "zs1mu34v9wtyhtlr22phfxkp670wt0gj4z8czumx0m4u05elg7kjzu0dzveq5jn28xg5a2wqr6cywd",
  "zs1ma3xy4fj0vm0w37a4kur3ghe72f8nstmqmmvh0u48kurtlwg50rqqlw39j6ng8lrxgam2dh0zyr",
  "zs1m7yauj7694wjz92m5sxds8udp43z6kclarqwzc22wn2q6svkkk4lpzx53kwctwfn6jqcu4yww5u",
  "zs1m77qqksfvuqmjmwdm22xr2u99z3uz4glenk02md4tq5z066gc7jkdayhf2txspqggv29quacpy4",
  "zs1uq88d69zrnksytnquejpksdvdlp9e5y3xec3eyf0rrya8zap623zpwjs5mfzadrp7twawkpennl",
  "zs1uq0zzag2cmekwn9a35vkf2q45sary5v8nt2adukhej9ydq3qpegskg5naysl3wvvxtzuyv5lu8w",
  "zs1uqs3n3j8kfgurz8tj0ea5g2ny200c69rwwpq50kkup8sxntdne6h9uhx3wc2y5jjkcggvpvcl4g",
  "zs1uqmnl793xh4cskxjherwlpt7xfnt6fy2sp2l3n58hpmd4msj2g3fjsks02069tqgzzvn77mpfg7",
  "zs1uz73f8arfahrvgtxcjwya33ql5w8mwkmyrvxvjrjx5rla07hv3ax49hkeqg3aqynxx39z4y4vtt",
  "zs1urzuwedq4qgnqjsjwjeauzyyjwxvs27mau6zmpretn37my92h3jqc0waad4r8s7zeeczq6jfas6",
  "zs1urfls60sjxpnla4hhe736qu54w7047akw2p9cksx054y8e8gdyknkhwv6spwzgstf7z2wrsdapw",
  "zs1u9jum0rl3959ay3qqxskkak5emv0fpceuhuhcj9rnkzfy6gpe2uv6ny29c399nrc68fx6fffg6c",
  "zs1u8fdxg8lu08p6s5z9qe6jf6fkvd74a99yg3n5exlm6wm8paypygy82ue2smf0qqlhdhjzy2jxq4",
  "zs1u8u44rlv4ay27248807fqwyf6lw2w76v8evn26nvwv887caqlenjgsw0nqlajzw9equn7phhs95",
  "zs1u4jhx25fghqzn2az7he7lrv7xj23xd8spcl660g9kzqa3wyykm5gucuu5wwxvum6l6spq4u4e5s",
  "zs1uh85d43vr4wwqnq6e7akxtlwkzx2y8cmdm3wrxzj6m42pzj3xs2heqey79hgxrhwhapwu8tt73g",
  "zs1uhwgwkg2y3xex7e34dvnx97d0jdnhmchevcvg54tg4dg00ay9sjucr82py4jqx0kkf7cz5dytad",
  "zs1uhkhsv3c3m5r8z7unmceg4zln6edwfr25wjauv5u8fxdqhdflj7vuc2xnkm9028y802w2pyf07g",
  "zs1uhuwuzu89j05p9y48nseklu7gfhw4mrwv68f9s66csafrkpsmpsh23zzgxm24nspmsgrvtpttzg",
  "zs1ucetuz8ysvz42uvuwndjfnphrtsfgekgef228meuhfrqtm97gql87rsvhxmxplw2xmj27p9jemj",
  "zs1ucm95fgyy72rf6x5rwl5m5pldt38lulzqf9ueepel4gvd4cqjhk8xrf4kvl2lnn5ge9fwyfrfva",
  "zs1um3pu2u0eu0zpx4czj9dufvxnud2zd0x5lygepp6maz96j572r6mh8gpmlm3w6jhmxxz2sepdfh",
  "zs1ulkw8j488xuhhlewedj9dr7atm5jatmlwsxz83lquhz42ln78utqmtqsmhagcquq6uqszk23g9d",
  "zs1az03j8f46n2600z47xnf5y28j34rpxljh3j6w5p3xgsmqt2fuklmhyd383aljxf4mx4pcfv0xm8",
  "zs1ayal4wq4crwj70u6hae82h6r4jpk7ptycfhs9v5y83lxc7u4gadegu2r5ggsgt95n0rjk284fzj",
  "zs1a984wux9sr2594lr24yal43zq6pstczyevj7yr8pn403643zuge88ekrc0cj8n2fk6z6xrxjpsz",
  "zs1a95ptq9d6nauwtvgdj2lnct9y6g4cmm258jzyuqhsk4m3gqmaz62t4klqe42eu83n27jcpgz0ed",
  "zs1a8xdhdzpjqx0alrjg43enszjnf6nn6nhf6c3xd77ecrvp4kfarvux4u4lcttv3rgyhmgzxukw2q",
  "zs1afqnaljmvzunvjhr3m8yf2g4r0pe66kkxkm6ldjhylyjfsclk3nf88u29pskdqx76szdz48tt3m",
  "zs1avx0j6mrcqrhp9903xrysptm6gefcyv6uqltta92hsgjpp2f8h8rz8suwxgjkp2f8366y6n6dj8",
  "zs1a036amk3q8azryytah2zpdhyazeruq2q7zq2c06l0845kl0v2rmg0h8cdjvta4alxj0my4kcfad",
  "zs1ahy2sja2ala03wrc79xx2ks3ujxxnsm44uh6yucmy5p5l0xcfa3g90cdgs4l9rhy975zvd4uzts",
  "zs1aeyqta7xs8edzq2c0z6e9v3vjep55e4lg9fp9ls6d55x2mel6snxud6kqcaz8nl7zgrxxj96d6a",
  "zs1aap3thj9xna07vlg3yz389c34v9mgd06g234d3htyq667286phjsf98nndm8frts7frmcyjfa90",
  "zs17qdqrdgwcafkpgchc4rvk22e4u86alhnmzne5xzantpecwrxf3c6jxqk7xgzanzjj4kmxd2tuwg",
  "zs17z3fmn6e84ypzpzn0p0j9nddptrj2nwhk9lhaw952j0lzxslrp24cj0ltuem9g5dustakcq4dsp",
  "zs17zut2398dst3hnmnslnk0jv9w4q9yn8akelymvs8ewurdytxushp92nyqv30quqhy0yju7rn7lw",
  "zs17r4fuv9ldl3kzwk5stv59exusc7jlsmtcz4t2uzjnrgrr6aj6tvnp04wc9jq2n3eh6fsyqe2ru5",
  "zs17rajftxlkcywcenl0cn3fqw4lh2un5lpfegjdz06j3vl9gjmay2d5pk7uequ02vw6tmtzz6jrst",
  "zs179rvu8endcr48sft7zg4w6uvxwu4ps94r06uwk7e606yffkgtx7epaamlfdqnc6xa4l9scqcv4v",
  "zs178dr2z7zgqrsg5ul3sxx05qky2kemfequf08dxr332n9f5fq9cj98jttssz97lzmf2k22xpn54m",
  "zs172zz77ds82urqmsa9pgyz65k04euw7uuk58k4ardcxectjc4t4yjekxm8xxmgd7gqs8k6jupypk",
  "zs172eamykp6sl4vx4tsgmg2t0p482hnn3gg4my9y4frd4tl5zgsyrsvjvlxc9zjtqpve6e6djdc4x",
  "zs17v4xqdu83fkvrjxrpnjksuanj0pung2kqn9ys533nnm8kq8ad8xv9kd48e4utrz947pejg55p46",
  "zs17dgakqvwzgh4dgfe70cjulju698cs50zvchsze2e3zvdp68wytqdcvj4suh4vq2acdg7wuvs7ar",
  "zs17snaqr6vukwp4apsdf44h6w3flgphzrnpmdjly662tgqtvkgs72lpz7m7tnkksdmt5uzjgmpg2e",
  "zs173ad7l6u8dr90e2t5jkrnw0gc9u2mppv9vjeh8l6q2jdvgnq6tq4anxxltwuxm3wssfzgg6hfcy",
  "zs17nez6jn8tnse243f5uf72d8y000ynmjnm6vsrpzpd2fj75wq4u4lu7xc8fmtn2e5v0r7uknphs3",
  "zs174c6x8u2yagjnsq2kswnd6fh8u2f3g5dkrk7r7ja5n30zwjm4ke3x84syt9qklqyk0m7vekcx9f",
  "zs174mlfm6snsmgj4usez3e6xtd5nkwwl24vgg96srpnv7ulz4de6n4lx6cmxaqszqnk7p9y6wcl8q",
  "zs17h5lnrnpprdtkjwq09ax94qetryf65qm5jqv0gpyeesw4wujytks9qljvlry863flf242arvx8f",
  "zs1lp07e40usxenrznuuf2nzn5v7tx9pzp9r6eaw6upnm4t9cer8l5fckzm7jr58j5l77tzjrprv8v",
  "zs1ly5u5sqeeax9g3uafva7fl35r3wv0nm2aka9m940graqjh0zlw7rrcgay0a7f29j3ar4wrj4uzu",
  "zs1lgqckcp2uqx5c6gdm5zklzrxz8ygva9kxtxc4u4dlzpg68m9prga5q3ur3uqutkcy4ztuhclrxw",
  "zs1lgz7ychnnhe58hk2e379zhqdxynp30e6fdh6xjxx8u9ga9rmwzdrdvqcq5kps2uetyf6gzeqdn2",
  "zs1l2ghymesqwrfw89pqnw08u346es6wn86r77a55n7d7xky2rc58jfhn7man9kjjesnegec3frxeh",
  "zs1ls3lyaqhm39zgz3528ereaa48vzsw4cw99k536524a6ruxmdqyvqnv4pl477q7rwptrzx8dhhzu",
  "zs1lsnr42d2ez0w55pxws4qn70f68vxllv92wppu24n75y7a0wrmkw6qgup0md5jhjmkwhzu742zx4",
  "zs1ljzwlum9nme83hhvkjkxl323u0ezm4sgnk84nzkyu5acum0kxf0s6g06gy78w0hl66f5263g7ha",
  "zs1l5kfev0dpl8swjlyvyms5t9yhhnvg0590jfgpxw7zxx6eh29vd7453q9d0ne75x7gsm42j65l3v",
  "zs1lhpxmvxmfpdfa5myd35wf24pmacrgdhrcpxydrcwz3qvmfvpt9x78nf2ne3kkqh40m0nvhhd3uj",
  "zs1lhkhftvpkvcuyhwgcz4gq9y9l3ly5esglk2g0sgdctrz2cd63lgss2gtn8eedsvtuh8f6shpwww",
    };
    //TODO: Assumes pools of 100
    int randIndex = GetRandInt(100); // random int between 0 and 99
    if(randIndex % 2) {
       return SietchShieldedPool1[randIndex];
    } else {
       return SietchShieldedPool2[randIndex];
    }
}

#endif
