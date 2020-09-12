#!/usr/bin/perl
# Copyright 2019-2020 The Hush developers
# Released under the GPLv3
use warnings;
use strict;

# Given a block time, estimate when it will happen
my $block      = shift || die "Usage: $0 123";
my $hush       = "./src/hush-cli";
my $blockcount = qx{$hush getblockcount};

unless ($blockcount = int($blockcount)) {
    print "Invalid response from hush-cli\n";
    exit 1;
}

if ($block <= $blockcount) {
	die "That block has already happened!";
} else {
	my $diff    = $block - $blockcount;
	my $minutes = $diff*2.5;
	my $seconds = $minutes*60;
	my $now     = time;
	my $then    = $now + $seconds;
	my $ldate   = localtime($then);
	my $gmdate  = gmtime($then);
	print "Hush Block $block will happen at roughly:\n";
	print "$ldate Eastern # $then\n";
	print "$gmdate GMT     # $then\n";
}
