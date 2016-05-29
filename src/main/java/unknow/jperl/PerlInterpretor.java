package unknow.jperl;

import java.io.*;

import javax.script.*;

@SuppressWarnings("restriction")
public class PerlInterpretor implements ScriptEngine
	{
	static
		{
		Runtime.getRuntime().addShutdownHook(new Thread()
			{
				public void run()
					{
					deinit();
					}
			});
		NarSystem.loadLibrary();
		init();
		}

	private static native void init();

	private static native void deinit();

	/** pointer to the PerlInterpreter */
	private long perl;
	/** current perlcontext */
	private PerlContext ctx;

	/**
	 * create a new perlInterpretor with a default context
	 */
	public PerlInterpretor()
		{
		create();
		// TODO generate package
		ctx=new PerlContext(this, "unknow::jperl");
		}

	/**
	 * create and initialize native PerlInterpreter
	 */
	private native void create();

	/**
	 * destroy native PerlInterpreter
	 */
	private native void destroy();

	/**
	 * evaluate this script in this package
	 */
	private native Object eval(String script, String ns);

	/**
	 * evaluate this script in this context.
	 */
	public Object eval(String script, ScriptContext context) throws ScriptException
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

	public Object eval(Reader reader, ScriptContext context) throws ScriptException
		{
		// TODO Auto-generated method stub
		return null;
		}

	public Object eval(String script) throws ScriptException
		{
		return eval(script, getContext());
		}

	public Object eval(Reader reader) throws ScriptException
		{
		return eval(reader, getContext());
		}

	public Object eval(String script, Bindings n) throws ScriptException
		{
		// TODO Auto-generated method stub
		return null;
		}

	public Object eval(Reader reader, Bindings n) throws ScriptException
		{
		// TODO Auto-generated method stub
		return null;
		}

	public void put(String key, Object value)
		{
		// TODO Auto-generated method stub

		}

	public Object get(String key)
		{
		// TODO Auto-generated method stub
		return null;
		}

	public Bindings getBindings(int scope)
		{
		// TODO Auto-generated method stub
		return null;
		}

	public void setBindings(Bindings bindings, int scope)
		{
		// TODO Auto-generated method stub

		}

	public Bindings createBindings()
		{
		// TODO Auto-generated method stub
		return null;
		}

	public ScriptContext getContext()
		{
		return ctx;
		}

	public void setContext(ScriptContext context)
		{
		// TODO Auto-generated method stub

		}

	public ScriptEngineFactory getFactory()
		{
		// TODO Auto-generated method stub
		return null;
		}

	@Override
	protected void finalize() throws Throwable
		{
		super.finalize();
		destroy();
		}
	}
