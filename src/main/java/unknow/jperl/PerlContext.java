package unknow.jperl;

import java.io.*;
import java.util.*;

import javax.script.*;

/**
 * contain a perl context.
 */
@SuppressWarnings("restriction")
public class PerlContext implements ScriptContext
	{
	/** interpretor linked to this context */
	private PerlInterpretor perl;

	private PerlBinding bind;

	/** the package of this context */
	private String perlPackage;

	public PerlContext(PerlInterpretor perl, String perlPackage)
		{
		this.perl=perl;
		this.perlPackage=perlPackage;
		bind=new PerlBinding(perlPackage, perl);
		}

	public PerlInterpretor getInterpretor()
		{
		return perl;
		}

	public String getPackage()
		{
		return perlPackage;
		}

	public void setBindings(Bindings bindings, int scope)
		{
		// TODO Auto-generated method stub

		}

	public Bindings getBindings(int scope)
		{
		return ScriptContext.ENGINE_SCOPE==scope?bind:null;
		}

	public void setAttribute(String name, Object value, int scope)
		{
		// TODO Auto-generated method stub

		}

	public Object getAttribute(String name, int scope)
		{
		// TODO Auto-generated method stub
		return null;
		}

	public Object removeAttribute(String name, int scope)
		{
		// TODO Auto-generated method stub
		return null;
		}

	public Object getAttribute(String name)
		{
		// TODO Auto-generated method stub
		return null;
		}

	public int getAttributesScope(String name)
		{
		// TODO Auto-generated method stub
		return 0;
		}

	public Writer getWriter()
		{
		// TODO Auto-generated method stub
		return null;
		}

	public Writer getErrorWriter()
		{
		// TODO Auto-generated method stub
		return null;
		}

	public void setWriter(Writer writer)
		{
		// TODO Auto-generated method stub

		}

	public void setErrorWriter(Writer writer)
		{
		// TODO Auto-generated method stub

		}

	public Reader getReader()
		{
		// TODO Auto-generated method stub
		return null;
		}

	public void setReader(Reader reader)
		{
		// TODO Auto-generated method stub

		}

	public List<Integer> getScopes()
		{
		// TODO Auto-generated method stub
		return null;
		}

	}
