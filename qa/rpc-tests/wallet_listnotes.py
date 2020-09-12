#!/usr/bin/env python2
# Copyright (c) 2019-2020 The Hush developers
# Copyright (c) 2018 The Zcash developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

from test_framework.test_framework import BitcoinTestFramework
from test_framework.util import assert_equal, start_nodes, wait_and_assert_operationid_status

from decimal import Decimal

# Test wallet z_listunspent behaviour across network upgrades
class WalletListNotes(BitcoinTestFramework):

    def setup_nodes(self):
        return start_nodes(4, self.options.tmpdir, [[
            '-nuparams=5ba81b19:202', # Overwinter
            '-nuparams=76b809bb:204', # Sapling
        ]] * 4)

    def run_test(self):
        # Current height = 200 -> Sprout
        assert_equal(200, self.nodes[0].getblockcount())

        # test that we can create a sapling zaddr before sapling activates
        saplingzaddr = self.nodes[0].z_getnewaddress('sapling')
        
        # Set current height to 204 -> Sapling
        self.nodes[0].generate(2)
        self.sync_all()
        assert_equal(204, self.nodes[0].getblockcount())
        
        # No funds in saplingzaddr yet
        assert_equal(0, len(self.nodes[0].z_listunspent(0, 9999, False, [saplingzaddr])))

        # Send 0.9999 to our sapling zaddr
        # (sending from a sprout zaddr to a sapling zaddr is disallowed,
        # so send from coin base)
        receive_amount_2 = Decimal('2.0') - Decimal('0.0001')
        recipients = [{"address": saplingzaddr, "amount":receive_amount_2}]
        myopid = self.nodes[0].z_sendmany(mining_addr, recipients)
        txid_3 = wait_and_assert_operationid_status(self.nodes[0], myopid)
        self.sync_all()
        unspent_tx = self.nodes[0].z_listunspent(0)
        assert_equal(3, len(unspent_tx))

        # low-to-high in amount
        unspent_tx = sorted(unspent_tx, key=lambda k: k['amount'])

        assert_equal(False,             unspent_tx[0]['change'])
        assert_equal(txid_2,            unspent_tx[0]['txid'])
        assert_equal(True,              unspent_tx[0]['spendable'])
        assert_equal(sproutzaddr2,      unspent_tx[0]['address'])
        assert_equal(receive_amount_1,  unspent_tx[0]['amount'])

        assert_equal(False,             unspent_tx[1]['change'])
        assert_equal(txid_3,            unspent_tx[1]['txid'])
        assert_equal(True,              unspent_tx[1]['spendable'])
        assert_equal(saplingzaddr,      unspent_tx[1]['address'])
        assert_equal(receive_amount_2,  unspent_tx[1]['amount'])

        assert_equal(True,              unspent_tx[2]['change'])
        assert_equal(txid_2,            unspent_tx[2]['txid'])
        assert_equal(True,              unspent_tx[2]['spendable'])
        assert_equal(sproutzaddr,       unspent_tx[2]['address'])
        assert_equal(change_amount_9,   unspent_tx[2]['amount'])

        unspent_tx_filter = self.nodes[0].z_listunspent(0, 9999, False, [saplingzaddr])
        assert_equal(1, len(unspent_tx_filter))
        assert_equal(unspent_tx[1], unspent_tx_filter[0])

        # test that pre- and post-sapling can be filtered in a single call
        unspent_tx_filter = self.nodes[0].z_listunspent(0, 9999, False,
            [sproutzaddr, saplingzaddr])
        assert_equal(2, len(unspent_tx_filter))
        unspent_tx_filter = sorted(unspent_tx_filter, key=lambda k: k['amount'])
        assert_equal(unspent_tx[1], unspent_tx_filter[0])
        assert_equal(unspent_tx[2], unspent_tx_filter[1])

        # so far, this node has no watchonly addresses, so results are the same
        unspent_tx_watchonly = self.nodes[0].z_listunspent(0, 9999, True)
        unspent_tx_watchonly = sorted(unspent_tx_watchonly, key=lambda k: k['amount'])
        assert_equal(unspent_tx, unspent_tx_watchonly)

        # TODO: use z_exportviewingkey, z_importviewingkey to test includeWatchonly
        # but this requires Sapling support for those RPCs

if __name__ == '__main__':
    WalletListNotes().main()
