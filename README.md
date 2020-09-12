# HUSH

## What is HUSH?

![Logo](doc/hush/hush.png "Logo")

HUSH (originally Zdash) is a code fork of [ZCash](https://z.cash/) which has
it's own genesis block. It is not a fork of another network.  Based on
Bitcoin's code, it intends to offer a far higher standard of privacy through a
sophisticated zero-knowledge proving scheme that preserves confidentiality of
transaction metadata.

This software is the HUSH node and command-line client. It downloads and stores
the entire history of HUSH transactions; depending on the speed of your
computer and network connection, the synchronization process could take a day
or more once the blockchain has reached a significant size.

**HUSH is unfinished and highly experimental.** Use at your own risk!

## Discord

Please feel free to join us on Discord at https://myhush.org/discord
There are many channels, some you might enjoy are #general, #support and #mining.

## Claiming Funds From Old Hush Wallets

Hush migrated to a new mainnet after Block 500,000 on the old Hush blockchain.
Funds in addresses as of Block 500,000 were transported to our new chain. About
31,000 addresses with at least 0.00000001 HUSH were transported to the new Hush
mainnet.

To claim funds on the new chain, there are few options.

### Funds on exchanges

Firstly, no bueno! Not your keys, not your coins. It's best not to store coins
on exchanges. But in this case, you lucked out! There is nothing to do to claim
new coins if you have coins on an exchange that supports the new Hush chain.
The exchange will follow the instructions from the next section and you will
magically have funds on the new chain. Note that old Hush addresses started
with `t1` and now they begin with `R`.

To see what an old HUSH v2 address looks like on the new chain, this online tool
can be used: https://dexstats.info/addressconverter.php

or this command line tool: https://github.com/MyHush/hush3/blob/duke/contrib/convert_address.py


### Using an old wallet.dat

Backup your old HUSH wallet.dat, and backup any current wallet.dat that is in your

        ~/.komodo/HUSH3/

directory. There is no way to lose funds, as long as you have backups!!! Make sure
to make backups. Do not skip this step.

Make sure any/all GUI wallets are stopped! Also make sure your old Hush node
and new Hush3 node are stopped:

        cd hush3
        ./src/hush-cli stop

Do not copy wallets or move wallets while your full node is running! This could
corrupt your wallet!

Now copy your old Hush wallet.dat to

        ~/.komodo/HUSH3/

with a command like

        # DO NOT RUN THIS WITHOUT MAKING BACKUPS!
        cp ~/.hush/wallet.dat ~/.komodo/HUSH3/

The reason this works is that both old HUSH and new HUSH are still Bitcoin Protocol
coins, which both use secp256k1 public keys. Now start your HUSH3 node again,
with this special CLI argument that will clear out transactions from your wallet:

        cd hush3
        ./src/hushd -zapwallettxes

This will cause a full history rescan, which will take some time. Once it's complete,
you can see your funds with this command:

        ./src/hush-cli getwalletinfo

NOTE: Do not use this wallet except to send funds to a new wallet!

### Private Keys

You can also transport funds one address at a time via private keys.

Agama Desktop Wallet WIF-to-WIF Tool can convert between old HUSH and new HUSH3
private keys.

### Web Wallet Seed Phrase

Nothing needs to be done, and if you use the Hush web wallet with a seed phrase,
you can unlock your new funds on the new Hush mainnet with the same seedphrase.

This web wallet is hosted on a best-effort basis to give newcomers an easy way
to make addresses for mining and other uses. Please heed this advice to keep
your funds safe:

 * DO NOT USE FOR LARGE AMOUNTS, use a full node or light wallet
 * ALWAYS ACCESS VIA https://
 * DO NOT USE FROM PUBLIC WIFI
 * DO NOT USE ON A COMPUTER OTHER PEOPLE USE
 * BACK UP YOUR SEED PHRASE (multiple paper copies)

Even if you follow all those rules, due to web wallets relying on DNS and IP
addresss, there are still potential attacks. You have been warned: https://wallet.myhush.org

The source code for the Hush web wallet is here: https://github.com/MyHush/myhushwallet


Installing
----------

See [INSTALL.md](https://github.com/MyHush/hush3/blob/master/INSTALL.md).


License
-------

For license information see the file [COPYING](COPYING).
