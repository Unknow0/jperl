package unknow.jperl;

import java.util.*;

import javax.script.*;

@SuppressWarnings("restriction")
public class PerlBinding implements Bindings
	{
	private String pack;
	private PerlInterpretor perl;
	private PerlHash stash;

	PerlBinding(String pack, PerlInterpretor perl)
		{
		this.pack=pack;
		this.perl=perl;
		stash=create(perl.perl, pack);
		}

	private native PerlHash create(long perl, String pack);

	public int size()
		{
		return stash.size();
		}

	public boolean isEmpty()
		{
		return size()==0;
		}

	public boolean containsValue(Object value)
		{ // TODO
//		return binding.containsValue(value);
		return false;
		}

	public native void clear();

	public Set<String> keySet()
		{
		return null; // TODO
//		String[] keys=keys();
//		Set<String> set=new HashSet<String>((int)(keys.length*1.33)+1);
//		for(int i=0; i<keys.length; i++)
//			set.add(keys[i]);
//		return set;
		}

	public Collection<Object> values()
		{ // TODO
		return null;
		}

	public Set<java.util.Map.Entry<String,Object>> entrySet()
		{ // TODO
		return null;
		}

	public native void define(long perl, long hv, String name, long v);

	public Object put(String name, Object value)
		{
		PerlScalar s=perl.toPerl(value);
		PerlGlob gv=(PerlGlob)stash.get(name);
		if(gv==null)
			{
			PerlScalar g=PerlScalar.createUndef(perl.perl);
			define(perl.perl, stash.sv, name, g.sv);
			stash.put(name, g);
			gv=(PerlGlob)stash.get(name);
			}

		PerlScalar old;
		if(s instanceof PerlHash)
			{
			old=gv.hash();
			gv.setHash((PerlHash)s);
			}
		else if(s instanceof PerlArray)
			{
			old=gv.array();
			gv.setArray((PerlArray)s);
			}
		else
			{
			old=gv.scalar();
			gv.setScalar(s);
			}
		return old;
		}

	public void putAll(Map<? extends String,? extends Object> toMerge)
		{
		for(java.util.Map.Entry<? extends String,? extends Object> e:toMerge.entrySet())
			put(e.getKey(), e.getValue());
		}

	public native boolean containsKey(Object key);

	public PerlScalar get(Object key)
		{
		return stash.get((String)key);
		}

	public native Object remove(Object key);

//	private native void undef(PerlScalar pv);

	private native PerlScalar define(long perl, String name, String v);
	}
