package unknow.jperl;

import java.util.*;

/**
 * Represent a hash in perl
 */
public class PerlHash extends PerlScalar
	{
	/**
	 * create an empty perlHash
	 */
	public PerlHash(PerlInterpretor perl)
		{
		create(perl.perl);
		}

	/**
	 * create a perlHash that copy this map
	 */
	public PerlHash(PerlInterpretor perl, Map<String,?> o)
		{
		this(perl);
		for(Map.Entry<String,?> e:o.entrySet())
			{
			PerlScalar sv=perl.toPerl(e.getValue());
			put(e.getKey(), sv);
			}
		}

	/**
	 * create a perlHash with these pointer
	 */
	protected PerlHash(long perl, long hv)
		{
		super(perl, hv);
		}

	private native void create(long perl);

	public native void clear();

	public native int size();

	public boolean isEmpty()
		{
		return size()==0;
		}

	public native boolean containsKey(String key);

	public PerlScalar get(String key)
		{
		return get(key, false);
		}

	public native PerlScalar get(String key, boolean create);

	public native PerlScalar put(String key, PerlScalar sv);

	public native PerlScalar remove(String key);
	}