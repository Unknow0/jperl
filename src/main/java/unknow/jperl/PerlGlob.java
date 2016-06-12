package unknow.jperl;

/**
 * Represent a hash in perl
 */
public class PerlGlob extends PerlScalar
	{
	protected PerlGlob(long perl, long gv)
		{
		super(perl, gv);
		}

	public native PerlScalar scalar();

	public native PerlArray array();

	public native PerlHash hash();

	public native void setScalar(PerlScalar scalar);

	public native void setArray(PerlArray array);

	public native void setHash(PerlHash hash);
	}