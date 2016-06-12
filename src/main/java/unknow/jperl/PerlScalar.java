package unknow.jperl;

/**
 * Represent a scalar in perl (or a ref)
 */
public class PerlScalar
	{
	/** interpreter in witch this scalar is defined */
	protected long perl;
	/** link to the value in perl */
	protected long sv;

	protected static native PerlScalar createUndef(long p);

	protected static native PerlScalar createIV(long p, long l);

	protected static native PerlScalar createNV(long p, double d);

	protected static native PerlScalar createPV(long p, String s);

	protected PerlScalar()
		{
		}

	protected PerlScalar(long perl, long sv)
		{
		this.perl=perl;
		this.sv=sv;
		}

	protected void finalize()
		{
		if(sv!=0)
			destroy();
		}

	/**
	 * convert this value as string like perl would do
	 */
	public native boolean isUndefined();

	/**
	 * convert this value as boolean like perl would do
	 */
	public native boolean toBoolean();

	/**
	 * convert this value as integer like perl would do
	 */
	public native Integer toInt();

	/**
	 * convert this value as integer like perl would do
	 */
	public native Long toLong();

	/**
	 * convert this value as double like perl would do
	 */
	public native Double toDouble();

	/**
	 * convert this value as string like perl would do
	 */
	public native String toString();

	public PerlArray asArray()
		{
		if(this instanceof PerlArray)
			return (PerlArray)this;
		refInc();
		return new PerlArray(perl, sv);
		}

	public PerlHash asHash()
		{
		if(this instanceof PerlHash)
			return (PerlHash)this;
		refInc();
		return new PerlHash(perl, sv);
		}

	public PerlGlob asGlob()
		{
		if(this instanceof PerlGlob)
			return (PerlGlob)this;
		refInc();
		return new PerlGlob(perl, sv);
		}

	private native void refInc();

	/**
	 * same as `ref` function in perl
	 * @return the reference type 
	 */
	public native String ref();

	/**
	 * dereference this scalar
	 * @return null if this scalar isn't a REF
	 */
	public native PerlScalar deref();

	/**
	 * return the raw perl type
	 * @see PerlType
	 */
	public native int type();

	protected native void destroy();
	}