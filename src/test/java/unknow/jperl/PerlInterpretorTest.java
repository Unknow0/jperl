package unknow.jperl;

import static org.junit.Assert.*;

import javax.script.*;

import org.junit.*;

public class PerlInterpretorTest
	{
	@Test
	public void test_Scalar() throws ScriptException
		{
		PerlInterpretor p=new PerlInterpretor();
		PerlScalar eval=p.eval("42");
		assertEquals("42", eval.toString());
		p.finalize();
		}

	@Test
	public void test_bind() throws ScriptException
		{
		PerlInterpretor p=new PerlInterpretor();
		p.put("test", "value");
		assertEquals("value", p.eval("$test;").toString());
		p.finalize();
		}

	@Test
	public void test_context() throws ScriptException
		{
		PerlInterpretor p=new PerlInterpretor();
		PerlContext ctx1=p.getContext();
		PerlContext ctx2=new PerlContext(p, "test1");
		ctx1.getBindings(ScriptContext.ENGINE_SCOPE).put("test", "context1");
		ctx2.getBindings(ScriptContext.ENGINE_SCOPE).put("test", "context2");
		assertEquals("context1", p.eval("$test;", ctx1).toString());
		assertEquals("context2", p.eval("$test;", ctx2).toString());
		p.finalize();
		}
	}
