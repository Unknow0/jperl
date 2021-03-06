package unknow.jperl;

import java.io.*;
import java.util.*;

import javax.script.*;

@SuppressWarnings("restriction")
public class PerlInterpretor implements ScriptEngine
	{
	/** pointer to the PerlInterpreter */
	long perl;

	/** current perlcontext */
	private PerlContext ctx;

	/**
	 * create a new perlInterpretor with a default context
	 */
	public PerlInterpretor()
		{
		create(PerlBootstrap.bootstrap);
		// TODO generate package
		ctx=new PerlContext(this, "unknow::jperl");
		}

	/**
	 * create and initialize native PerlInterpreter
	 * @param bootstrapfile
	 */
	private native void create(String bootstrapfile);

	/**
	 * destroy native PerlInterpreter
	 */
	private native void destroy();

	/**
	 * evaluate this script in this package
	 */
	private native Object[] eval(String script, String ns);

	/**
	 * evaluate this script in this context.
	 */
	public Object[] eval(String script, ScriptContext context) throws ScriptException
		{
		if(context instanceof PerlContext)
			{
			PerlContext c=(PerlContext)context;
			if(c.getInterpretor()!=this)
				throw new ScriptException("Context not linked to this interpretor.");
			return eval(script, c.getPackage());
			}
		else
			throw new ScriptException("Invalid context, must be a PerlContext.");
		}

	public Object[] eval(Reader reader, ScriptContext context) throws ScriptException
		{
		throw new ScriptException("unsuported");
		}

	public Object[] eval(String script) throws ScriptException
		{
		return eval(script, getContext());
		}

	public Object[] eval(Reader reader) throws ScriptException
		{
		return eval(reader, getContext());
		}

	public Object[] eval(String script, Bindings n) throws ScriptException
		{
		throw new ScriptException("unsuported");
		}

	public Object[] eval(Reader reader, Bindings n) throws ScriptException
		{
		throw new ScriptException("unsuported");
		}

	public void put(String key, Object value)
		{
		getBindings(ScriptContext.ENGINE_SCOPE).put(key, value);
		}

	public Object get(String key)
		{
		return getBindings(ScriptContext.ENGINE_SCOPE).get(key);
		}

	public Bindings getBindings(int scope)
		{
		return ctx.getBindings(scope);
		}

	public void setBindings(Bindings bindings, int scope)
		{
		throw new RuntimeException("unsuported");
		}

	public Bindings createBindings()
		{
		throw new RuntimeException("unsuported");
		}

	public PerlContext getContext()
		{
		return ctx;
		}

	public void setContext(ScriptContext context)
		{
		if(context instanceof PerlContext)
			this.ctx=(PerlContext)context;
		else
			throw new IllegalArgumentException("Invalid context");
		}

	public ScriptEngineFactory getFactory()
		{
		// TODO Auto-generated method stub
		return null;
		}

	@Override
	protected void finalize()
		{
		destroy();
		}

	@SuppressWarnings("unchecked")
	public PerlScalar toPerl(Object o)
		{
		if(o instanceof Map)
			return new PerlHash(this, (Map<String,?>)o);
		if(o instanceof Collection)
			return new PerlArray(this, (Collection<?>)o);
		if(o.getClass().isArray())
			return new PerlArray(this, (Object[])o);
		else if(o instanceof Long)
			return PerlScalar.createIV(perl, (Long)o);
		else if(o instanceof Integer)
			return PerlScalar.createIV(perl, (Integer)o);
		else if(o instanceof Short)
			return PerlScalar.createIV(perl, (Short)o);
		else if(o instanceof Byte)
			return PerlScalar.createIV(perl, (Byte)o);
		else if(o instanceof Float)
			return PerlScalar.createNV(perl, (Float)o);
		else if(o instanceof Double)
			return PerlScalar.createNV(perl, (Float)o);
		else
			return PerlScalar.createPV(perl, o.toString());
		}
	}
