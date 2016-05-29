package unknow.jperl;

/**
 * represant a scalar in perl (or a ref)
 */
public class PerlScalar
	{
	/** interpreter in witch this scalar is defined */
	private long perl;
	/** link to the value in perl */
	private long sv;

	/**
	 * create a new PerlScalar
	 */
	private static native PerlScalar create(long sv, long perl);

	/**
	 * avoid instantiation from java
	 */
	private PerlScalar()
		{
		}

	protected void finalize()
		{
		if(sv!=0)
			destroy();
		}

	public native String toString();

	native void destroy();
	}