package unknow.jperl;

import java.util.*;

import javax.script.*;

public class PerlBinding implements Bindings
	{
	private PerlContext ctx;
	private Map<String,PerlScalar> binding;

	// TODO cache declaration
//	private long len;
//	private long alloc;
//	private long decl;
//
//	private native void create();

	public PerlBinding(PerlContext ctx)
		{
		this.ctx=ctx;
		this.binding=new HashMap<String,PerlScalar>();
//		create();
		}

	public int size()
		{
		return binding.size();
		}

	public boolean isEmpty()
		{
		return binding.isEmpty();
		}

	public boolean containsValue(Object value)
		{
		return binding.containsValue(value);
		}

	public void clear()
		{
		for(PerlScalar p:binding.values())
			p.destroy();
		binding.clear();
		}

	public Set<String> keySet()
		{
		return binding.keySet();
		}

	@SuppressWarnings({"unchecked", "rawtypes"})
	public Collection<Object> values()
		{
		return (Collection)binding.values();
		}

	@SuppressWarnings({"unchecked", "rawtypes"})
	public Set<java.util.Map.Entry<String,Object>> entrySet()
		{
		return (Set)binding.entrySet();
		}

	public Object put(String name, Object value)
		{
		String v;
		if(value instanceof CharSequence)
			v="'"+value.toString().replaceAll("'", "\\'")+"'";
		else
			v=String.valueOf(value);
		// TODO map/list/array
		PerlScalar ps=define(ctx.getInterpretor().perl, ctx.getPackage(), name, v);
		if(ps!=null)
			ps=binding.put(name, ps);
		if(ps!=null)
			ps.destroy();
		return null;
		}

	public void putAll(Map<? extends String,? extends Object> toMerge)
		{
		for(java.util.Map.Entry<? extends String,? extends Object> e:toMerge.entrySet())
			put(e.getKey(), e.getValue());
		}

	public boolean containsKey(Object key)
		{
		return binding.containsKey(key);
		}

	public Object get(Object key)
		{
		return binding.get(key);
		}

	public Object remove(Object key)
		{
		PerlScalar ps=binding.get(key);
		if(ps!=null)
			ps.destroy();
		return null;
		}

//	private native void undef(PerlScalar pv);

	private native PerlScalar define(long perl, String pack, String name, String v);
	}
