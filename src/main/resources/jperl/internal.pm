package jperl::internal;

use Carp;

#our $jni;
our @scalar=qw(Z B S C I J F D Ljava/lang/String; Ljava/lang/Boolean; Ljava/lang/Byte; Ljava/lang/Short; Ljava/lang/Character; Ljava/lang/Integer; Ljava/lang/Long; Ljava/lang/Float; Ljava/lang/Double;);

sub is_scalar
	{
	my ($e)=@_;

	for(@scalar)
		{
		return 1 if $_ eq $e;
		}
	return;
	}
sub can_convert
	{
	my ($e, $g)=@_; # $e expected type java internal, $g actual value
	my $r=ref($g);
	
	if(!$r)
		{
		return unless is_scalar $e;

		# check max / min value if it's expecting a number
		return if ($e eq 'B' or $e eq 'L/java/lang/Byte;')and ($g>2**7-1 || $g<-2**7);
		return if ($e eq 'S' or $e eq 'C' or $e eq 'Ljava/lang/Short;' or $e eq 'L/java/.lang/Character;') and ($g>2**15-1 || $g<-2**15);
		return if ($e eq 'I' or $e eq 'Ljava/lang/Integer;') and ($g>2**31-1 || $g<-2**31);
		return if ($e eq 'J' or $e eq 'Ljava/lang/Long;') and ($g>2**63-1 || $g<-2**63);
		return if ($e eq 'F' or $e eq 'Ljava/lang/Float;') and ($g<-(2-2**-23)*2**127 || $g>(2-2**-23)*2**127);
		return if ($e eq 'D' or $e eq 'Ljava/lang/Double;') and ($g<-(2-2**-52)*2**1023 || $g>(2-2**-52)*2**1023);
		}
	elsif($r eq 'HASH')
		{
		print "hash\n";
		# Check if expecting a Map
		return unless $e =~ m/L(.*);/;

		my $cl=$1;
		$cl='s#/#.#g';
		import $cl;

		$1 =~ s/=\./::/g;
		return unless $1->isa('java::util::Map'); # check generic ?
		}
	elsif($r eq 'ARRAY')
		{
		print "array\n";
		
		if($e =~ m/^\[(.*)/)
			{ # array check content
			for(@{g})
				{
				return unless can_convert $1, $_;
				}
			}
		else
			{ # object should be a colection
			return unless $e =~ m/L(.*);/;

			my $cl=$1;
			$cl='s#/#.#g';
			import $cl;

			$1 =~ s/=\./::/g;
			return unless $1->isa('java::util::Collection'); # check generic ?
			}
		}
	else
		{ # $e internal $r package
		# make $e be a perl package
		$e=~ s/^L(.+);$/$1=~s#[\/\$]#::#gr/e;
		return unless $e->isa($r);
		}
	return 1;
	}

sub find_method
	{
	my ($methods, $arg)=@_;
	my @arg=@{$arg};
	my $argCount=$#arg+1;
	my $mm;
LOOP:	for my $m (@{$methods})
		{
		my $c=$m->{count};
		next if $c!=$argCount;
		my $p=$m->{param};
		
		for(my $i=0; $i<$c; $i++)
			{
			next LOOP unless can_convert $p->[$i], $arg[$i];
			}
		croak "ambigous call" if $mm;
		$mm=$m;
		}
	return $mm;
	}

return 1;
