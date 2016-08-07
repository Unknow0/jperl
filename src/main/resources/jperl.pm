package jperl;

require jperl::internal;
require jperl::Object;

sub new
	{
	my ($class, @arg)=@_;
	
	my $pack= $class =~ s/\./::/rg;
	import $class unless(%{$pack.'::'});
	return jperl::internal::new($pack, @arg);
	}

return 1;
