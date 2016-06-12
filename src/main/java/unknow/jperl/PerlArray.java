package unknow.jperl;

import java.util.*;

/**
 * Represent a hash in perl
 */
public class PerlArray extends PerlScalar
	{
	public PerlArray(PerlInterpretor perl, Collection<?> col)
		{
		create(perl.perl);
		for(Object o:col)
			push(perl.toPerl(o));
		}

	protected PerlArray(long perl, long sv)
		{
		super(perl, sv);
		}

	private native void create(long perl);

	public native void clear();

	public native int size();

	public native void push(PerlScalar sv);

	public native PerlScalar get(int i);

	public native void set(int i, PerlScalar sv);

	public native PerlScalar pop();

	public native PerlScalar shift();

	public boolean isEmpty()
		{
		return size()==0;
		}
	}