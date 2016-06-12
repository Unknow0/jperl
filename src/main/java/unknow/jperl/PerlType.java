package unknow.jperl;

public class PerlType
	{
	/** null type */
	public static final int SVt_NULL=0;
	/** integer or ref type */
	public static final int SVt_IV=1;
	/** SVt_IV or double */
	public static final int SVt_NV=2;
	/** alias SVt_IV */
	public static final int SVt_RV=3;
	/** string */
	public static final int SVt_PV=4;
	/** PV or IV */
	public static final int SVt_PVIV=5;
	/** PV or NV */
	public static final int SVt_PVNV=6;
	/** All scalar */
	public static final int SVt_PVMG=7;
	/** magic type */
	public static final int SVt_PVBM=8;
	/** LVALUE */
	public static final int SVt_PVLV=9;
	/** array type */
	public static final int SVt_PVAV=10;
	/** hash type */
	public static final int SVt_PVHV=11;
	/** sub type */
	public static final int SVt_PVCV=12;
	/** glob type */
	public static final int SVt_PVGV=13;
	/** format type */
	public static final int SVt_PVFM=14;
	/** IO type */
	public static final int SVt_PVIO=15;
	}
