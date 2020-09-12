#!/usr/bin/perl
# Copyright 2019-2020 The Hush developers
# Released under the GPLv3
use warnings;
use strict;
# Generate checkpoint data for use in src/main.cpp

# TODO: update when blocktime changes to 75s
my $perday  = 576;
my $hush    = "./src/hush-cli";
my $gethash = "$hush getblockhash";
my $stride  = shift || 5000;
my $count   = 0;
my $blocks  = qx{$hush getblockcount};
if($?) {
    print "ERROR, exiting...\n";
    exit 1;
}
my $prev    = $blocks - $perday;
my $last    = 0;
my $now     = time();
chomp($blocks);

print "// Generated at $now via hush3 contrib/checkpoints.pl by Duke Leto\n";

while (1) {
	$count++;
	my $block = $stride*$count;
	if ($block > $blocks) {
		$last = $stride*($count-1);
        #print "last checkpointed block=$last\n";
        last;
    }
	my $blockhash = qx{$gethash $block};
	chomp $blockhash;
	print qq{($block,     uint256S("0x$blockhash"))\n};
}
my $time    = qx{$hush getblock $last |grep time|cut -d: -f2| sed 's/,//g'};
chomp($time);
# TODO: This is Linux-only
my $line1       = qx{grep --text height=$prev   ~/.komodo/HUSH3/debug.log};
my $line2       = qx{grep --text height=$blocks ~/.komodo/HUSH3/debug.log};
my $txs_per_day = 2 * $perday; # default estimate is 2 txs per block, on average
my $total_txs   = 0;
#print "line1: $line1\n";
#print "line2: $line2\n";

# This will calculate the number of txs in the previous day to the last checkpointed block
if ($line1 =~ m/tx=(\d+)/) {
    my $tx1 = $1; # number of txs in the block 1 day ago
    #print "prevblock has $tx1 txs\n";
    if ($line2 =~ m/tx=(\d+)/) {
        $total_txs = $1;
        # TODO: average of last N days might be better
        $txs_per_day  = $total_txs - $tx1;
    }
}
print "(int64_t) $time, // time of last checkpointed block\n";
print "(int64_t) $total_txs,      // total txs\n";
print "(double)  $txs_per_day,        // txs in the last day before block $blocks\n";
