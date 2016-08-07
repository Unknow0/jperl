package jperl::Object;

use Carp;

sub new
	{
	my ($class, @arg)=@_;
	my %c=%{$class.'::class'};
	my $m=jperl::internal::find_method($c{'method'}{'<init>'}, \@arg);

	croak "failed to find a constructor" unless $m;

	my $o=jperl::internal::alloc($c{id}, $m->{m}, $m->{param}, @arg);
	return bless(\$o, $class);
	}

1;
