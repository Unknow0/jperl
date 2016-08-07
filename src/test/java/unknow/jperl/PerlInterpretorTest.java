package unknow.jperl;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;

import java.util.*;

import javax.script.*;

import org.junit.*;

@SuppressWarnings({"unchecked", "restriction"})
public class PerlInterpretorTest
	{
	PerlInterpretor p;

	public PerlInterpretorTest() throws Throwable
		{
		try
			{
			p=new PerlInterpretor();
			}
		catch (Throwable e)
			{
			e.printStackTrace();
			throw e;
			}
		}

	@Test
	public void test_Scalar() throws ScriptException
		{
		Object[] o=p.eval("'42'");
		PerlScalar eval=(PerlScalar)o[0];
		assertEquals("SCALAR", eval.ref());
		assertEquals("42", eval.toString());

		o=p.eval("('un','deux','trois')");
		assertEquals(3, o.length);
		assertEquals("un", o[0].toString());
		assertEquals("deux", o[1].toString());
		assertEquals("trois", o[2].toString());
		}

	@Test
	public void test_ref() throws ScriptException
		{
		Object[] o=p.eval("42");
		PerlScalar e=(PerlScalar)o[0];
		assertEquals("SCALAR", e.ref());
		assertNull(e.deref());

		o=p.eval("'test'");
		e=(PerlScalar)o[0];
		assertEquals("SCALAR", e.ref());
		assertNull(e.deref());

		o=p.eval("@a=(1, 'test'); \\@a");
		e=(PerlScalar)o[0];
		assertEquals("REF", e.ref());
		e=e.deref();
		assertNotNull(e);
		assertEquals("ARRAY", e.ref());

		o=p.eval("{1=>'test', foo=>'bar'}");
		e=(PerlScalar)o[0];
		assertEquals("REF", e.ref());
		e=e.deref();
		assertNotNull(e);
		assertEquals("HASH", e.ref());

		}

	@Test
	public void test_conv() throws ScriptException
		{
		Object[] e=p.eval("42");
		PerlScalar sv=(PerlScalar)e[0];
		assertFalse(sv.isUndefined());
		assertEquals(42, (int)sv.toInt());
		assertEquals(42, (double)sv.toDouble(), 0.0001);
		assertEquals("42", sv.toString());
		assertTrue(sv.toBoolean());

		e=p.eval("0");
		sv=(PerlScalar)e[0];
		assertFalse(sv.isUndefined());
		assertFalse(sv.toBoolean());

		e=p.eval("42.23");
		sv=(PerlScalar)e[0];
		assertFalse(sv.isUndefined());
		assertEquals(42, (int)sv.toInt());
		assertEquals(42.23, (double)sv.toDouble(), 0.0001);
		assertEquals("42.23", sv.toString());
		assertTrue(sv.toBoolean());

		e=p.eval("$t=undef");
		sv=(PerlScalar)e[0];
		assertTrue(sv.isUndefined());
		assertFalse(sv.toBoolean());

		e=p.eval("test");
		sv=(PerlScalar)e[0];
		assertFalse(sv.isUndefined());
		assertTrue(sv.toBoolean());

		e=p.eval("$t='';");
		sv=(PerlScalar)e[0];
		assertFalse(sv.isUndefined());
		assertFalse(sv.toBoolean());
		}

	@Test
	public void test_array() throws ScriptException
		{
		PerlScalar perl=p.toPerl(Arrays.asList(42, "Test", "23"));

		assertEquals(PerlArray.class, perl.getClass());
		PerlArray pa=(PerlArray)perl;

		assertEquals(3, pa.size());

		assertEquals(42, (long)pa.get(0).toLong());
		assertEquals("Test", pa.get(1).toString());
		assertEquals("23", pa.get(2).toString());

		PerlScalar ps=PerlScalar.createPV(p.perl, "Test");
		pa.push(ps);
		assertEquals(4, pa.size());
		assertEquals(ps.toString(), pa.pop().toString());
		assertEquals(3, pa.size());

		ps=pa.shift();
		assertEquals(2, pa.size());
		assertEquals("42", ps.toString());

		pa.clear();
		assertTrue(pa.isEmpty());
		assertEquals(0, pa.size());
		}

	@Test
	public void test_hash() throws ScriptException
		{
		Map<String,String> map=new HashMap<String,String>();
		map.put("k1", "v1");
		map.put("k2", "v2");
		map.put("k3", "v3");

		PerlScalar perl=p.toPerl(map);

		assertEquals(PerlHash.class, perl.getClass());
		PerlHash ph=(PerlHash)perl;

		assertEquals(3, ph.size());

		for(String k:map.keySet())
			{
			PerlScalar v=ph.get(k);
			assertNotNull(v);
			assertEquals(map.get(k), v.toString());
			}

		PerlScalar ps=PerlScalar.createPV(p.perl, "Test");
		ph.put("t", ps);
		assertEquals(4, ph.size());
		assertTrue(ph.containsKey("t"));
		assertEquals(ph.get("t").toString(), ps.toString());

		ph.remove("t");
		assertEquals(3, ph.size());
		assertFalse(ph.containsKey("t"));
		assertNull(ph.get("t"));

		ph.clear();
		assertTrue(ph.isEmpty());
		assertEquals(0, ph.size());
		assertNull(ph.get("truc"));
		}

	@Test
	public void test_die()
		{
		try
			{
			p.eval("die 'DIEDIEDIEDIEDIE';");
			}
		catch (ScriptException e)
			{
			assertEquals("DIEDIEDIEDIEDIE at (eval 2) line 2.", e.getMessage());
			}
		}

	@Test
	public void test_bind() throws ScriptException
		{
		PerlBinding b=new PerlBinding("test", p);
		assertEquals(0, b.size());
		b.put("v0", "test");
		assertEquals(1, b.size());
		assertEquals("test", b.get("v0").asGlob().scalar().toString());
		}

	@Test
	public void test_bless() throws ScriptException
		{
		Object[] e=p.eval("my $v=42; bless(\\$v, 'test');");
		assertEquals("test", ((PerlScalar)e[0]).deref().ref());
		}

	@Test
	public void test_import() throws ScriptException
		{
//		Object[] e=p.eval("jperl::import 'unknow.jperl.PerlInterpretorTest$TestClass'");
//		assertEquals(1, (int)((PerlScalar)e[0]).toInt());
//		p.eval("print \"keys\\n\"; print \"	$_\\n\" for (keys %{'unknow::jperl::PerlInterpretorTest$TestClass::class'}{'method'});");
		Object[] e=p.eval("jperl::import 'java.lang.String'");

//		e=p.eval("$v='unknow::jperl::PerlInterpretorTest$TestClass'->new(42, 42);");
//		assertEquals("42: '42'", e[0].toString());
//		e=p.eval("$v->test()");
//		assertEquals("test", e[0]);
//		e=p.eval("$v->toString()");
//		assertEquals("42: '42'", e[0]);
		}

	@Test
	public void test_context() throws ScriptException
		{
		PerlContext ctx1=p.getContext();
		PerlContext ctx2=new PerlContext(p, "test1");
		ctx1.getBindings(ScriptContext.ENGINE_SCOPE).put("test", "context1");
		ctx2.getBindings(ScriptContext.ENGINE_SCOPE).put("test", "context2");
		assertEquals("context1", p.eval("$test;", ctx1)[0].toString());
		assertEquals("context2", p.eval("$test;", ctx2)[0].toString());
		}

	public static class TestClass
		{
		public int i;
		public String s;

		public TestClass(int i)
			{
			this.i=i;
			}

		public TestClass(int i, String s)
			{
			this.i=i;
			this.s=s;
			}

		public TestClass(String s)
			{
			this.s=s;
			}

		public TestClass(long i)
			{
			this.i=(int)i;
			}

		public String test()
			{
			return "test";
			}

		public String toString()
			{
			System.out.println("toString: "+i+": "+(s==null?"null":"'"+s+"'"));
			return i+": "+(s==null?"null":"'"+s+"'");
			}
		}
	}
