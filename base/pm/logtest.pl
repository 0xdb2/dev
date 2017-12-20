#!/usr/bin/perl -w

use lib "$ENV{HOME}/rel/lib/perl/";
use My::Log;

my $log;

print "logtest::main: Log=undef\n"; 

{
    #my $log = Log -> new(-dt=>0, -debug=>0);
    my $log = Log -> new(-debug=>1);
    $log->open;
    print "logtest::main: 1 logname=", $log->{logname}, "\n"; 
    $log->line("logtest::main: logname=" . $log->{logname}); 
    $log->put("111\n222\n333\n");
    $log->close;
    $log->open(-path => './My');
    print "logtest::main: 2 logname=", $log->{logname}, "\n"; 
    $log->line("logtest::main: logname=" . $log->{logname}); 
}
