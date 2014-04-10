#!/usr/bin/env perl

use strict;
use warnings;

my $pid1 = fork;
if ($pid1 == 0) {
    exec './dgemm';
} else {
    sleep 2;
    my $pid2 = fork;
    if ($pid2 == 0) {
        exec './dgemm';
    } else {
        sleep 15;
    }
}

