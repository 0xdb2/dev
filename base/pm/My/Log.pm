package Log;

#use ;
use FileHandle;

use POSIX qw(strftime);

my $startdt;
my $logfile;
my $myname = qx(basename main::$0 .pl);
chomp($myname);

my $filehandle = FileHandle->new;
my $debug = 0;
#print "Log::BODY: filehandle=$filehandle\n"; 
#die "Cannot open Log File $LogFileName: $!\n" unless $Log_file->open("> $LogFileName");

BEGIN
{   
    #print "Log::BEGIN\n";
    $startdt = time;
}

sub new 
{
    my $class = shift;
    my %args = (
        -path => $ENV{PWD},
        -logname => $myname,
        -dt => 1,
        -dtformat => "%H:%M:%S",
        -debug => $debug,
        @_,);
    $debug = $args{-debug};
    print "$class\::new: args=", join('|', %args), "\n" if $debug;
    my $path = $args{-path};
    my $logname = $args{-logname};
    my $self = bless {
        class => $class,
        path => $path,
        logname => $logname,
        dt =>$args{-dt},
        dtformat => $args{-dtformat}, 
    }, $class;

    $self->init();
 
    return $self;
}

sub init() 
{   
    my $self = shift;
    print "Log::init: class=$self->{class},path=$self->{path},logname=$self->{logname},startdt=$startdt,dt=$self->{dt},debug=$debug\n" if $debug;
};

sub open
{
    my $self = shift;
    my %args = (
        -path => $self->{path},
        -logname => $self->{logname}, 
        @_,);
    $self->{path} = $args{-path};
    $self{logname} = $args{-logname};

    my $suffix = $self->{dt} ? '_' . strftime("%y%m%d_%H%M%S", localtime($startdt)) : '';
    $logfile = $self->{path} . '/' .  $self->{logname} . $suffix . '.log'; 
    #print "Log::open: path=$self->{path}\n";
    print "Log::open: logfile=$logfile\n" if $debug;
    $filehandle->open(">> $logfile") or die "cannot open $logfile ($!)";
    print $filehandle  " *** $myname *** starting on " . strftime ("%d.%m.%y %H:%M:%S", localtime) . "\n"
}

sub close
{
    my $self = shift;
    if (fileno $filehandle)
    {
        $self->line("*** $myname *** closing file $logfile");
        print "$self->{class}::close: closing $logfile\n" if $debug;
        $filehandle->close;
        #undef $filehandle;
    }
    else
    {
        print "$self->{class}::close: alrady closed" if $debug;
    }
}

# $log->print
sub line()
{
    my $self = shift;
    my $line = $_[0];
    my $date = strftime($self->{dtformat}, localtime);
    $line = $date . ' ' . $line;
    if (fileno $filehandle)
    {
        print $filehandle "$line\n";
    }
    else
    {
        print $filehandle "$line\n";
    }
}

sub put()
{
    my $self = shift;
    my $line = $_[0];
    if (fileno $filehandle)
    {
        print $filehandle "$line";
    }
    else
    {
        print $filehandle "$line";
    }
}

sub DESTROY 
{
    my $self = shift;
    print("Log::DESTROY ", fileno $filehandle ? "closing"  : "closed", " $logfile \n") if $debug;
    $self->close();
}

END 
{
    #my $self =shift; print "Log::END: self=undef\n" unless defined $self; # undef ??? 
    print("Log::END ", fileno $filehandle ? "opened"  : "closed", " $logfile \n") if $debug;
}

return(1);
__END__

#TODO {...};
