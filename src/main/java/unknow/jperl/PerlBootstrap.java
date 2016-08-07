package unknow.jperl;

import java.io.*;

public class PerlBootstrap
	{
	private static final String[] resources=new String[] {"jperl.pm", "jperl/internal.pm", "jperl/Object.pm"};
	public static final String bootstrap;
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
		File dir=new File(System.getProperty("java.io.tmpdir"), "jperl_"+System.currentTimeMillis());
		dir.deleteOnExit();
		bootstrap=dir.getAbsolutePath();
		ClassLoader cl=PerlInterpretor.class.getClassLoader();
		for(String s:resources)
			extract(cl, dir, s);
		init();
		}

	private static native void init();

	private static native void deinit();

	private static File extract(ClassLoader cl, File dir, String s)
		{
		File b=new File(dir, s);
		b.deleteOnExit();
		b.getParentFile().mkdirs();
		InputStream is=cl.getResourceAsStream(s);
		try
			{
			FileOutputStream out=new FileOutputStream(b);
			byte[] buf=new byte[4096];
			int len;
			while ((len=is.read(buf))>0)
				out.write(buf, 0, len);
			is.close();
			out.close();
			}
		catch (IOException e)
			{
			throw new RuntimeException(e);
			}
		return b;
		}
	}
